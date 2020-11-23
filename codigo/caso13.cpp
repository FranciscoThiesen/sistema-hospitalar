#include "persistencia.hpp"
#include "paciente.hpp"
#include <string>
#include <vector>
#include <cassert>
#include <optional>

using namespace std;
/*
Caso 13 (C13) - Paciente vem a óbito

Ator primário - Enfermeiro
1. Autoridades legais são notificadas
2. Familiares são chamados para os procedimentos fúnebres
3. Se ele ocupava um quarto, o quarto é liberado.
4. Qualquer consumo_medicamento ou consumo_dieta ainda pendente, será considerado concluído.
*/
void encerra_ocupacao(persistencia& DAO, int id_paciente)
{
    DAO.carrega_pacientes();
    DAO.carrega_quartos();
    DAO.carrega_ocupacoes();
    DAO.carrega_medicos();

    paciente& P = DAO.tabela_pacientes[id_paciente];
    auto O = P.ocupacao_paciente.value();
    P.ocupacao_paciente = nullopt;

    int id_ocupacao = O.id;
    int id_quarto_ocupado = O.quarto_ocupado.id;
    auto& OC = DAO.tabela_ocupacoes[id_ocupacao];
    OC.data_fim = (int) time(0) + rand();
    
    auto& Q = DAO.tabela_quartos[id_quarto_ocupado];
    Q.ocupado = false;
    Q.id_ocupacao = nullopt;
    
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
}

int main()
{
    // Populando entidades necessárias! 
    // Inicialmente todos os vetores estão vazios!    
    auto DAO = persistencia();
    DAO.clear_all(); // limpando todos os json

    // vamos exibir os jsons até o momento
    
    vector< string > especialidades;
    vector< string > especialidades2;
    especialidades.push_back(string("cardiologista"));
    especialidades2.push_back(string("cirurgião"));

    int medico_1 = DAO.cadastra_medico(1, especialidades);
    int medico_2 = DAO.cadastra_medico(2, especialidades2);


    string ficha_medica2 = "Homem. 15 Anos, com Covid";
    int id_paciente2 = DAO.cadastra_paciente(medico_2, ficha_medica2, "Caio do Nascimento");

    // Cenário 2 -> Paciente com gravidade, precisa ser internado
    int id_quarto = DAO.cadastra_quarto(); // O id é atribuído automaticamente, id sempre >= 0!
    
    int id_ocupacao = DAO.cadastra_ocupacao(id_paciente2, DAO.tabela_quartos[id_quarto]);
        
    DAO.mostra_json("pacientes.json");
    
    // Paciente vem a óbito
    
    auto& Q = DAO.tabela_pacientes[id_paciente2];
    if(Q.ocupacao_paciente) encerra_ocupacao(DAO, id_paciente2);
    
    auto& P = DAO.tabela_pacientes[id_paciente2];
    P.ficha_medica += "Paciente veio a obito.\n";
    
    DAO.persiste_pacientes();
    DAO.mostra_json("pacientes.json");
    
    return 0;

}
