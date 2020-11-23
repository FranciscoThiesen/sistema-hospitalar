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
#include <ctime>
#include <chrono>

using namespace std;
// auto start = std::chrono::system_clock::now();
/*
Caso 6 (C6) - Paciente tem alta

Ator Primário - Médico

1. Médico registra que a ocupação do quarto por parte do paciente chegou ao fim 
2. Sistema atualiza o status paciente no sistema e sua estadia no hospital é adicionada a ficha médica 
*/

void troca_de_quarto(persistencia& DAO, int id_paciente, int novo_quarto)
{
    DAO.carrega_pacientes();
    DAO.carrega_quartos();
    DAO.carrega_ocupacoes();
    DAO.carrega_medicos();

    auto& P = DAO.tabela_pacientes[id_paciente];
    auto O = P.ocupacao_paciente.value();
    P.ocupacao_paciente = nullopt;

    int id_ocupacao = O.id;
    int id_quarto_ocupado = O.quarto_ocupado.id;
    auto& OC = DAO.tabela_ocupacoes[id_ocupacao];
    OC.data_fim = (int) time(0) + rand();
    // agora, temos que liberar o quarto
    
    auto& Q = DAO.tabela_quartos[id_quarto_ocupado];
    Q.ocupado = false;

    OC.quarto_ocupado = Q;

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

    DAO.persiste_quartos();
    DAO.persiste_ocupacoes();
    DAO.persiste_pacientes();
    DAO.persiste_medicos();
    DAO.cadastra_ocupacao(id_paciente, DAO.tabela_quartos[novo_quarto]);
    
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

    int id_paciente = DAO.cadastra_paciente(id_medico, string(), "Evérton Ribeiro");
    
    int id_quarto = DAO.cadastra_quarto(); // O id é atribuído automaticamente, id sempre >= 0!
    int id_quarto2 = DAO.cadastra_quarto();

    int id_ocupacao = DAO.cadastra_ocupacao(id_paciente, DAO.tabela_quartos[id_quarto]);
    cout << "ANTES DE MUDAR DE QUARTOS!!" << endl; 
    DAO.mostra_json("pacientes.json");
    DAO.mostra_json("ocupacoes.json");
    troca_de_quarto(DAO, id_paciente, id_quarto2);
    cout << "DEPOIS DE TROCAR DE QUARTO!!" << endl << endl; 
    DAO.mostra_json("pacientes.json");
    DAO.mostra_json("ocupacoes.json");

    return 0;

}
