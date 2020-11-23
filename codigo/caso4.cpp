#include "persistencia.hpp"
#include <string>
#include <vector>
#include <cassert>
/*
Caso 4 (C4) - Chegada de um novo paciente

Ator primário: Paciente

1. Recepcionista registra dados do paciente a ser atendido
2. Sistema atribui um dos médicos disponíveis ao novo paciente.
*/

int main()
{
    // Populando entidades necessárias! 
    // Inicialmente todos os vetores estão vazios!    
    auto DAO = persistencia();
    DAO.clear_all(); // limpando todos os json

    // vamos exibir os jsons até o momento
    DAO.mostra_json("pacientes.json");
    DAO.mostra_json("medicos.json");
    

    vector< string > especialidades, especialidades2, especialidades3;
    especialidades.push_back(string("cardiologista"));
    especialidades2.push_back(string("cirurgião"));
    especialidades3.push_back(string("anestesista")); 
    DAO.cadastra_medico(1, especialidades);
    DAO.cadastra_medico(2, especialidades2);
    DAO.cadastra_medico(3, especialidades3);

    auto medico_disponivel = DAO.tabela_medicos.begin()->first; // Esse médico será aleatório, porque hash_table não guarda médicos em ordem
    int id_paciente = DAO.cadastra_paciente(medico_disponivel, string(), "Bernardo da Silva");

    // É necessário agora obter algum dos médicos disponíveis
    // Para fins de simplicidade, basta pegar o primeiro da tabela de médico
    DAO.mostra_json("pacientes.json");
    DAO.mostra_json("medicos.json");

    return 0;

}
