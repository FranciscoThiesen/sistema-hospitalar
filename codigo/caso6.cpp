#include "persistencia.hpp"
#include "consumo_medicamento.hpp"
#include "consumo_dieta.hpp"
#include <string>
#include <vector>
#include <cassert>
#include <random>
#include <algorithm>
#include <optional>
#include <utility>

using namespace std;

/*
Caso 6 (C6) - Paciente tem alta

Ator Primário - Médico

1. Médico registra que a ocupação do quarto por parte do paciente chegou ao fim 
2. Sistema atualiza o status paciente no sistema e sua estadia no hospital é adicionada a ficha médica 
*/

void da_alta_a_paciente_internado(persistencia& DAO, int id_paciente)
{
    DAO.carrega_pacientes();
    DAO.carrega_quartos();
    DAO.carrega_ocupacoes();
    DAO.carrega_medicos();

    auto& P = DAO.tabela_pacientes[id_paciente];
    auto O = P.ocupacao_paciente.value();
    P.ocupacao_paciente = nullopt;
    P.teve_alta = true;
    P.ficha_medica += "     Teve alta.";

    int id_ocupacao = O.id;
    int id_quarto_ocupado = O.quarto_ocupado.id;
    auto& OC = DAO.tabela_ocupacoes[id_ocupacao];
    OC.data_fim = time(0) + rand();
    // agora, temos que liberar o quarto
    
    auto& Q = DAO.tabela_quartos[id_quarto_ocupado];
    Q.id_ocupacao = nullopt;
    Q.ocupado = false;

    DAO.persiste_pacientes();

    // valor do Paciente P mudou, tem que mudar em médico!
    auto& M = DAO.tabela_medicos[P.id_medico_responsavel];
    for(auto& val : M.lista_pacientes) 
    {
        if(val.id == id_paciente) 
        {
            val = P;
            break;
        }
    }

    DAO.persiste_medicos(); 
    DAO.persiste_quartos();
    DAO.persiste_ocupacoes();
    DAO.persiste_pacientes();
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

    int id_paciente = DAO.cadastra_paciente(id_medico, string(), "Evérton Ribeiro");
    
    int id_quarto = DAO.cadastra_quarto(); // O id é atribuído automaticamente, id sempre >= 0!
    
    int id_ocupacao = DAO.cadastra_ocupacao(id_paciente, DAO.tabela_quartos[id_quarto]);
    cout << "ANTES DE DAR ALTA AO PACIENTE!!" << endl; 
    DAO.mostra_json("pacientes.json");
    DAO.mostra_json("quartos.json");
    da_alta_a_paciente_internado(DAO, id_paciente);
    cout << "DEPOIS DE DAR ALTA AO PACIENTE!!" << endl << endl; 
    DAO.mostra_json("pacientes.json");
    DAO.mostra_json("quartos.json");

    // Agora, queremos dar alta a esse paciente que foi internado
    return 0;

}
