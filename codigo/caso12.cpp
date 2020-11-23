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
Caso 12 (C12) - Paciente sofre uma intercorrência

Ator primário - Enfermeiro

1. Médico responsável é acionado
2. Algum dos enfermeiros do turno é acionado
*/

vector<int> ids_dos_enfermeiros_que_trabalham_em_um_turno(persistencia& DAO, int turno) 
{
    DAO.carrega_enfermeiros();
    vector< int > lista_presente;
    for(const auto& entrada : DAO.tabela_enfermeiros)
    {
        vector<int> dias_trabalhados = entrada.second.dias_trabalhados;
        bool ok = false;
        for(const auto& dia : dias_trabalhados) if(dia == turno) ok = true;
        if(ok) lista_presente.push_back(entrada.second.id);
    }
    return lista_presente;
}

int main()
{
    // Populando entidades necessárias! 
    // Inicialmente todos os vetores estão vazios!    
    auto DAO = persistencia();
    DAO.clear_all(); // limpando todos os json

    vector< string > especialidades;
    especialidades.push_back(string("cardiologista"));
    
    int id_medico = DAO.cadastra_medico(1, especialidades);
    int id_paciente = DAO.cadastra_paciente(id_medico, string(), "Bernardo da Silva");
    int id_quarto = DAO.cadastra_quarto(); // O id é atribuído automaticamente, id sempre >= 0!
    int id_ocupacao = DAO.cadastra_ocupacao(id_paciente, DAO.tabela_quartos[id_quarto]);
    // vamos agora criar enfermeiros, apenas um deles vai trabalhar no dia 0 e outro no dia 1
    int id_enfermeiro = DAO.cadastra_enfermeiro({0, 1, 2});
    int id_enfermeiro2 = DAO.cadastra_enfermeiro({0, 2});
    int id_enfermeiro3 = DAO.cadastra_enfermeiro({0, 1, 2});
    // Vamos mostrar os consumos antes de atribuir aos enfermeiros
    
    // paciente sofre a intercorrência, no turno = 2
    cout << "paciente = " << DAO.tabela_pacientes[id_paciente].nome << " sofreu intercorrência (Turno atual = 1)" << endl;

    auto lista_enf = ids_dos_enfermeiros_que_trabalham_em_um_turno(DAO, 1);
    cout << "Enfermeiro de id = " << lista_enf.back() << " é notificado para ir urgentemente ver o paciente que sofreu a intercorrência" << endl;

    return 0;

}
