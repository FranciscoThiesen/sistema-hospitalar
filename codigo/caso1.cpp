#include "persistencia.hpp"
#include <string>
#include <vector>
#include <cassert>
/*
Caso 1 (C1) -  Emissão de nova receita medicamentosa

Ator primário: Médico

1. Médico avalia paciente X.
2. Médico decide pela necessidade de emitir uma nova receita medicamentosa para o paciente X.
3. Sistema solicita os detalhes necessários para emitir nova receita.
4. Médico define todos os detalhes necessários, Medicamento, Forma de ingestão, Dose
5. Sistema registra dados informados pelo Médico
6. Sistema atribui cada consumo medicamentoso a um determinado turno.
*/


int main()
{
    // Populando entidades necessárias! 
    // Inicialmente todos os vetores estão vazios!    
    auto DAO = persistencia();
    DAO.clear_all(); // limpando todos os json

    // vamos exibir os jsons até o momento (vão estar vazios)!
    DAO.mostra_json("pacientes.json");
    DAO.mostra_json("medicos.json");
    DAO.mostra_json("quartos.json");
    

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
    int preescricao1 = DAO.cadastra_prescricao_medicamento(id_receita, 100.0, medicamento1, "via venosa", vector<int>{0, 1});

    // Já essa prescrição diz para tomar tylenol  todos os dias da semana, por via oral 200mg
    int preescricao2 = DAO.cadastra_prescricao_medicamento(id_receita, 200.0, medicamento2, "via oral", vector<int>{0, 1, 2, 3, 4, 5, 6});

    DAO.mostra_json("pacientes.json");
    DAO.mostra_json("medicos.json");
    DAO.mostra_json("quartos.json");
    DAO.mostra_json("medicamentos.json");
    DAO.mostra_json("consumos_medicamento.json");
    DAO.mostra_json("receitas.json");
    DAO.mostra_json("prescricoes.json"); 

    return 0;

}
