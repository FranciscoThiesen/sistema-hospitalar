#include "persistencia.hpp"
#include "consumo_medicamento.hpp"
#include "consumo_dieta.hpp"
#include <string>
#include <vector>
#include <cassert>
#include <random>
#include <algorithm>
#include <utility>

using namespace std;

/*
Caso 9 (C9) - Enfermeiro aplica um medicamento a um paciente

Ator primário - Enfermeiro

1. Enfermeiro registra aplicação de medicamento
2. Sistema atualiza o status desse consumo medicamentoso, juntamente com a data.
*/

void atribui_consumo_medicamento(persistencia& DAO, int id_consumo)
{
    DAO.carrega_consumos_medicamento();
    DAO.carrega_enfermeiros();
    
    consumo_medicamento& CM = DAO.tabela_consumos_medicamento[id_consumo];

    int turno = CM.id_turno;
    
    vector< int > id_enfermeiros_disponiveis;
    DAO.carrega_enfermeiros();
    for(const auto& E : DAO.tabela_enfermeiros)
    {
        bool enfermeiro_pode_trabalhar = false;
        for(const auto& val : E.second.dias_trabalhados) 
        {
            if(val == turno) 
            {
                enfermeiro_pode_trabalhar = true;
                break;
            }
        }
        if(enfermeiro_pode_trabalhar) id_enfermeiros_disponiveis.push_back(E.second.id);
    }

    int enfermeiros_possiveis = static_cast<int>(id_enfermeiros_disponiveis.size());
    assert(enfermeiros_possiveis > 0);
    int indice_sorteado = rand() % enfermeiros_possiveis;
    // Agora, vamos atribuir CM ao enfemeiro de id sorteado!
    DAO.atribui_consumo_medicamentoso_a_enfermeiro(CM.id, id_enfermeiros_disponiveis[indice_sorteado]);
}

void efetiva_consumo_medicamento(persistencia& DAO, int id_consumo)
{
    DAO.carrega_medicos();
    DAO.carrega_pacientes();
    DAO.carrega_receitas();
    DAO.carrega_prescricoes();
    DAO.carrega_consumos_medicamento();
    DAO.carrega_enfermeiros();

    auto& C = DAO.tabela_consumos_medicamento[id_consumo];
    C.efetivacao_consumo = true;

    int id_enfermeiro = C.id_enfermeiro.value();
    auto& E = DAO.tabela_enfermeiros[id_enfermeiro];
    for(auto& CM : E.lista_consumos_medicamento) {
        if(CM.id == C.id) {
            CM = C;
        }
    } 

    // Essa mudança precisa se refletir na prescricao
    int id_prescricao = C.id_prescricao;
    auto& P = DAO.tabela_prescricoes[id_prescricao];

    for(int i = 0; i < static_cast<int>(P.consumos.size()); ++i)
    {
        if(P.consumos[i].id == id_consumo)
        {
            P.consumos[i] = C;
            break;
        }
    }

    // Agora, temos que mudar o valor dessa prescricao na receita correspondente
    int id_receita = P.id_receita;
    auto& R = DAO.tabela_receitas[id_receita];

    for(int i = 0; i < static_cast<int>(R.prescricoes.size()); ++i)
    {
        if(R.prescricoes[i].id == id_prescricao)
        {
            R.prescricoes[i] = P;
        }
    }

    // Agora, temos que mudar o valor dessa receita onde ela é armazenada!
    auto& M = DAO.tabela_medicos[R.id_medico_responsavel];
    for(int i = 0; i < static_cast<int>(M.receitas_medicas.size()); ++i)
    {
        if(M.receitas_medicas[i].id == id_receita)
        {
            M.receitas_medicas[i] = R;
        }
    }

    auto& PC = DAO.tabela_pacientes[R.id_paciente];
    for(int i = 0; i < static_cast<int>(PC.receitas_medicas.size()); ++i)
    {
        if(PC.receitas_medicas[i].id == id_receita)
        {
            PC.receitas_medicas[i] = R;
        }
    }

    DAO.persiste_medicos();
    DAO.persiste_pacientes();
    DAO.persiste_receitas();
    DAO.persiste_prescricoes();
    DAO.persiste_consumos_medicamento();
    DAO.persiste_enfermeiros();
}

int main()
{
    // Populando entidades necessárias! 
    // Inicialmente todos os vetores estão vazios!    
    auto DAO = persistencia();
    DAO.clear_all(); // limpando todos os json

    // vamos exibir os jsons até o momento (vão estar vazios)!
    vector< string > especialidades;
    especialidades.push_back(string("cardiologista"));
    
    int id_medico = DAO.cadastra_medico(1, especialidades);

    int id_paciente = DAO.cadastra_paciente(id_medico, string(), "Bernardo da Silva");
    
    int id_quarto = DAO.cadastra_quarto(); // O id é atribuído automaticamente, id sempre >= 0!
    
    int id_ocupacao = DAO.cadastra_ocupacao(id_paciente, DAO.tabela_quartos[id_quarto]);

    // Agora, vamos cadastrar dois medicamentos
    int id_medicamento1 = DAO.cadastra_medicamento("tramadol", "use com moderacao"); 
    int id_medicamento2 = DAO.cadastra_medicamento("tylenol", "use e abuse");
    
    // queremos gerar uma receita desses medicamentos
    int id_receita = DAO.cadastra_receita(id_medico, id_paciente);

    auto medicamento1 = DAO.tabela_medicamentos[id_medicamento1];
    auto medicamento2 = DAO.tabela_medicamentos[id_medicamento2];
    
    // A primeira prescrição diz para tomar tramadol 100ml, via venosa em duas datas (dia0 da semana e dia1)
    // Essa prescricao irá criar 2 consumos medicamentosos, com índices 1 e 2 
    int preescricao1 = DAO.cadastra_prescricao_medicamento(id_receita, 100.0, medicamento1, "via venosa", vector<int>{0, 1});
    
    // vamos agora criar enfermeiros, apenas um deles vai trabalhar no dia 0 e outro no dia 1
    int id_enfermeiro = DAO.cadastra_enfermeiro({0, 2});
    int id_enfermeiro2 = DAO.cadastra_enfermeiro({1, 3});

    DAO.mostra_json("consumos_medicamento.json");
    int id_consumo = DAO.tabela_prescricoes[preescricao1].consumos[0].id;
    // vamos agora atribuir os consumos do turno 0 a algum enfermeiro responsável

    atribui_consumo_medicamento(DAO, id_consumo); 
    efetiva_consumo_medicamento(DAO, id_consumo);

    // Agora vamos chamar a função que atribui esses consumos a enfermeiros do turno
    DAO.mostra_json("consumos_medicamento.json");

    return 0;

}
