#include "persistencia.hpp"
#include <string>
#include <vector>
#include <cassert>
/*
Caso 5 (C5) - Avaliação inicial do paciente

Ator primário: Médico

1. Médico avalia o paciente recém chegado.
2. Médico registra dados da ficha médica
3. Sistema registra dados da avaliação da ficha médica do paciente.
4. Médico finaliza atendimento

Fluxo Alternativo:

4a. Médico considera apropriado internação:
    1. Médico indica razões para internação
    2. Sistema atribui um dos quartos disponíveis para o paciente.
*/


int main()
{
    // Populando entidades necessárias! 
    // Inicialmente todos os vetores estão vazios!    
    auto DAO = persistencia();
    DAO.clear_all(); // limpando todos os json

    // vamos exibir os jsons até o momento
    DAO.mostra_json("pacientes.json");
    
    vector< string > especialidades;
    vector< string > especialidades2;
    especialidades.push_back(string("cardiologista"));
    especialidades2.push_back(string("cirurgião"));

    int medico_1 = DAO.cadastra_medico(1, especialidades);
    int medico_2 = DAO.cadastra_medico(2, especialidades2);


    string ficha_medica = "Homem. 55 Anos, com virose";
    int id_paciente = DAO.cadastra_paciente(medico_1, ficha_medica, "Bernardo da Silva");

    string ficha_medica2 = "Homem. 15 Anos, com Covid";
    int id_paciente2 = DAO.cadastra_paciente(medico_2, ficha_medica2, "Caio do Nascimento");

    // Vamos lidar com possibilidades
    // Cenário 1 -> Paciente sem gravidade.
    // Vamos dar alta ao paciente
    DAO.alta_a_paciente_sem_internar(id_paciente);

    // Cenário 2 -> Paciente com gravidade, precisa ser internado
    int id_quarto = DAO.cadastra_quarto(); // O id é atribuído automaticamente, id sempre >= 0!
    
    int id_ocupacao = DAO.cadastra_ocupacao(id_paciente2, DAO.tabela_quartos[id_quarto]);
    
    // É necessário agora obter algum dos médicos disponíveis
    // Para fins de simplicidade, basta pegar o primeiro da tabela de médico
    DAO.mostra_json("pacientes.json");

    return 0;

}
