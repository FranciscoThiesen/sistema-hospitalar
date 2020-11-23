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

void efetiva_consumo_dieta(persistencia& DAO, int id_consumo)
{
    DAO.carrega_enfermeiros();
    DAO.carrega_consumos_dieta();
    DAO.carrega_nutricionistas();
    DAO.carrega_dietas();
    DAO.carrega_pacientes();

    auto& C = DAO.tabela_consumos_dieta[id_consumo];
    C.efetivacao_consumo = true;

    int id_enfermeiro = C.id_enfermeiro.value();
    auto& E = DAO.tabela_enfermeiros[id_enfermeiro];
    for(auto& CD : E.lista_consumos_dieta) {
        if(CD.id == C.id) {
            CD = C;
        }
    } 

    // Esse consumo tem que ter seu valor persistido nos demais arquivos que o contém!
    // O consumo já vai estar presente em (dieta, nutricionista, paciente)
    auto& D = DAO.tabela_dietas[C.id_dieta]; // D vai mudar 
    // vamos procurar o consumo == C
    for(int i = 0; i < static_cast<int>(D.consumos.size()); ++i)
    {
        if(D.consumos[i].id == id_consumo)
        {
            D.consumos[i] = C;
            break; 
        }
    }
    int id_dieta = D.id;
    auto& P = DAO.tabela_pacientes[D.id_paciente];
    P.dieta_paciente = make_optional(D);

    auto& N = DAO.tabela_nutricionistas[D.id_nutricionista];
    for(int i = 0; i < static_cast<int>(N.dietas.size()); ++i)
    {
        if(N.dietas[i].id == id_dieta)
        {
            N.dietas[i] = D;
            break;
        }
    }

    DAO.persiste_consumos_dieta();
    DAO.persiste_enfermeiros();
    DAO.persiste_nutricionistas();
    DAO.persiste_dietas();
    DAO.persiste_pacientes();
}

int main()
{
    // Populando entidades necessárias! 
    // Inicialmente todos os vetores estão vazios!    
    auto DAO = persistencia();
    DAO.clear_all(); // limpando todos os json

    // vamos exibir os jsons até o momento (vão estar vazios)!
    vector< string > especialidades;
    especialidades.push_back(string("neurogista"));
    
    int id_medico = DAO.cadastra_medico(1, especialidades);

    int id_paciente = DAO.cadastra_paciente(id_medico, string(), "Bernardo da Silva");
    
    int id_quarto = DAO.cadastra_quarto(); // O id é atribuído automaticamente, id sempre >= 0!
    
    int id_ocupacao = DAO.cadastra_ocupacao(id_paciente, DAO.tabela_quartos[id_quarto]);
     
    // Vamos criar um nutricionista, para poder passar uma dieta
    int id_nutricionista = DAO.cadastra_nutricionista();
    
    int id_dieta = DAO.cadastra_dieta("vegetais", "nao come cenoura", id_paciente, id_nutricionista);

    int consumo_dieta_0 = DAO.cadastra_consumo_dieta(id_dieta, 0, "via oral");
    int consumo_dieta_1 = DAO.cadastra_consumo_dieta(id_dieta, 1, "via oral");

    // Agora, vamos cadastrar dois medicamentos
    int id_medicamento1 = DAO.cadastra_medicamento("tramadol", "use com moderacao"); 
    int id_medicamento2 = DAO.cadastra_medicamento("tylenol", "use e abuse");
    
    // queremos gerar uma receita desses medicamentos
    int id_receita = DAO.cadastra_receita(id_medico, id_paciente);
    
    int id_enfermeiro = DAO.cadastra_enfermeiro({0, 1, 2});

    DAO.mostra_json("consumos_dieta.json");
    
    atribui_consumo_dieta(DAO, consumo_dieta_0); 
    efetiva_consumo_dieta(DAO, consumo_dieta_0);

    // Agora vamos chamar a função que atribui esses consumos a enfermeiros do turno
    DAO.mostra_json("consumos_dieta.json");

    return 0;

}
