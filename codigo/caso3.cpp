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
Caso 3 (C3) - Emissão de uma nova dieta

Ator primário: Nutricionista

1. Nutricionista avalia o paciente X.
2. Nutricionista decide pela necessidade de emitir uma nova dieta para o paciente X.
3. Sistema solicita os detalhes necessários para emitir nova dieta.
4. Nutricionista define todos os detalhes necessários: Tipo de comida, forma de ingestao, etc
5. Sistema registra dados informados pelo Nutricionista
6. Sistema atribui cada refeição da dieta a um determinado turno.
 */

int main()
{
    // Populando entidades necessárias! 
    // Inicialmente todos os vetores estão vazios!    
    auto DAO = persistencia();
    DAO.clear_all(); // limpando todos os json

    // vamos exibir os jsons até o momento (vão estar vazios)!
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

    int id_paciente = DAO.cadastra_paciente(id_medico, string(), "Augusto da Silva");
    
    int id_quarto = DAO.cadastra_quarto(); // O id é atribuído automaticamente, id sempre >= 0!
    
    int id_ocupacao = DAO.cadastra_ocupacao(id_paciente, DAO.tabela_quartos[id_quarto]);
    
    // Vamos criar um nutricionista, para poder passar uma dieta
    int id_nutricionista = DAO.cadastra_nutricionista();
    
    int id_dieta = DAO.cadastra_dieta("bife e batata frita", "nao come glúten", id_paciente, id_nutricionista);

    int consumo_dieta_0 = DAO.cadastra_consumo_dieta(id_dieta, 0, "via oral");
    int consumo_dieta_1 = DAO.cadastra_consumo_dieta(id_dieta, 1, "via oral");
    int consumo_dieta_2 = DAO.cadastra_consumo_dieta(id_dieta, 2, "via oral");
    
    DAO.mostra_json("pacientes.json");

    return 0;

}
