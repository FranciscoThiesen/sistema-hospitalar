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
Caso 11 (C11) - Demissão de um médico

Ator primário - Gerente do hospital

1. Gerente registra que médico é removido da equipe do hospital
2. Sistema retira pacientes que estavam sob responsabilidade desse médico são distribuídos para outros médicos.
*/

void remove_medico(persistencia& DAO, int id_medico)
{
    DAO.carrega_medicos();
    DAO.carrega_pacientes();
    auto valor_medico = DAO.tabela_medicos[id_medico];
    
    DAO.tabela_medicos.erase(id_medico);
    DAO.persiste_medicos();
    // todos os pacientes que ainda não tiveram alta e estão sob responsabilidade de médico devem ser
    // distribuidos para outros médicos
    for(auto& entrada : DAO.tabela_pacientes)
    {
        if(entrada.second.id_medico_responsavel == valor_medico.CRM && entrada.second.teve_alta == false)
        {
            // vamos substituir por um médico qualquer
            int id_substituto = DAO.tabela_medicos.begin()->second.CRM;
            entrada.second.id_medico_responsavel = id_substituto;
            // paciente tem que ser adicionado a lista de pacientes do médico substituto
            DAO.tabela_medicos[id_substituto].lista_pacientes.push_back(entrada.second);
        }
    }

    DAO.persiste_pacientes();
    DAO.persiste_medicos();
}


int main()
{
    auto DAO = persistencia();
    DAO.clear_all(); // limpando todos os json

    vector< string > especialidades;
    especialidades.push_back(string("cardiologista"));
    
    int id_medico = DAO.cadastra_medico(1, especialidades);
    int id_medico2 = DAO.cadastra_medico(2, especialidades);

    int id_paciente = DAO.cadastra_paciente(id_medico, string(), "Bernardo da Silva");
    int id_paciente2 = DAO.cadastra_paciente(id_medico2, string(), "Julio Batista");
    
    int id_quarto = DAO.cadastra_quarto(); // O id é atribuído automaticamente, id sempre >= 0!
    int id_ocupacao = DAO.cadastra_ocupacao(id_paciente, DAO.tabela_quartos[id_quarto]);
    
    DAO.mostra_json("pacientes.json");
    remove_medico(DAO, id_medico);
    DAO.mostra_json("pacientes.json");
    // chama a função troca_médico

    return 0;

}
