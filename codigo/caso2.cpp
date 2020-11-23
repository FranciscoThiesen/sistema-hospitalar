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
Caso 2 (C2) - Informa procedimentos do turno

Ator: Tempo - início de cada turno

1. Sistema atribui os consumos medicamentosos aos enfermeiros do turno.	
2. Sistema atribui as refeições dos pacientes às enfermeiros do turno.
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

void atribui_consumo_dieta(persistencia& DAO, int id_consumo)
{
    DAO.carrega_consumos_dieta();
    DAO.carrega_enfermeiros();
    
    consumo_dieta& CD = DAO.tabela_consumos_dieta[id_consumo];

    int turno = CD.id_turno;
    
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
    DAO.atribui_consumo_dieta_a_enfermeiro(CD.id, id_enfermeiros_disponiveis[indice_sorteado]);
}

// Atribui todos os consumos medicamentosos e de dieta de um determinado turno!
void atribui_consumos_do_turno(persistencia& DAO, int turno) {
    // vamos atribuir todos os consumos medicamentosos de um determinado turno!
    DAO.carrega_consumos_dieta();
    DAO.carrega_consumos_medicamento();
     
    auto cpy_consumos_dieta = DAO.tabela_consumos_dieta;   
    for(auto& entrada : cpy_consumos_dieta) 
    {
        auto& CD = entrada.second;
        if(CD.id_turno == turno && CD.efetivacao_consumo == false && !CD.id_enfermeiro) 
        {
            atribui_consumo_dieta(DAO, CD.id);
        }
    }
    
    auto cpy_consumos_medicamento = DAO.tabela_consumos_medicamento;
    for(auto& entrada : cpy_consumos_medicamento) 
    {
        auto& consumo = entrada.second;
        if(consumo.id_turno == turno && consumo.efetivacao_consumo == false && !consumo.id_enfermeiro)
        {
            atribui_consumo_medicamento(DAO, consumo.id);
        }
    }
}


int main()
{
    // Populando entidades necessárias! 
    // Inicialmente todos os vetores estão vazios!    
    auto DAO = persistencia();
    DAO.clear_all(); // limpando todos os json

    // vamos exibir os jsons até o momento (vão estar vazios)!
    DAO.mostra_json("enfermeiros.json");
    DAO.mostra_json("medicos.json");
    DAO.mostra_json("pacientes.json");

    /* 
        Para exibir esse caso, precisamos de;
        1 - Paciente
        2 - Médico
        3 - Receita
        4 - Prescricao adicionada à receita
        5 - Nutricionista
        6 - Dieta
        7 - Enfermeiros
    */
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
    
    // Vamos criar um nutricionista, para poder passar uma dieta
    int id_nutricionista = DAO.cadastra_nutricionista();
    
    int id_dieta = DAO.cadastra_dieta("vegetais", "nao come cenoura", id_paciente, id_nutricionista);

    int consumo_dieta_0 = DAO.cadastra_consumo_dieta(id_dieta, 0, "via oral");
    int consumo_dieta_1 = DAO.cadastra_consumo_dieta(id_dieta, 1, "via oral");
    
    // vamos agora criar enfermeiros, apenas um deles vai trabalhar no dia 0 e outro no dia 1
    int id_enfermeiro = DAO.cadastra_enfermeiro({0, 2});
    int id_enfermeiro2 = DAO.cadastra_enfermeiro({1, 3});
    
    cout << "Consumos antes da virada de turno!" << endl;
    // Vamos mostrar os consumos antes de atribuir aos enfermeiros
    DAO.mostra_json("consumos_medicamento.json");
    DAO.mostra_json("consumos_dieta.json");
    //DAO.mostra_json("consumos_dieta.json");
    
    // vamos agora atribuir os consumos do turno 0 a algum enfermeiro responsável

    atribui_consumos_do_turno(DAO, 0);
    cout << "CONSUMOS após a atribuição de enfermeiros, relativos ao turno 0" << endl;
    // Agora vamos chamar a função que atribui esses consumos a enfermeiros do turno
    DAO.mostra_json("consumos_medicamento.json");
    DAO.mostra_json("consumos_dieta.json");

    return 0;

}
