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
Caso 10 (C10) - Demissão de um enfermeiro

Ator primário - Gerente do hospital

1. Gerente remove o enfermeiro do quadro de enfermeiros do hospital
2. Consumos que não foram efetivados e que estão designados para esse enfermeiro 
   ser remanejados para outro enfermeiro que trabalhe no mesmo horário
*/

void atribui_consumo_medicamento(persistencia& DAO, int id_consumo)
{
    DAO.carrega_consumos_medicamento();
    DAO.carrega_enfermeiros();
    
    consumo_medicamento& CM = DAO.tabela_consumos_medicamento[id_consumo];

    int turno = CM.id_turno;
    
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
    DAO.atribui_consumo_medicamentoso_a_enfermeiro(CM.id, id_enfermeiros_disponiveis[indice_sorteado]);
}

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

// Atribui todos os consumos medicamentosos e de dieta de um determinado turno!
void atribui_consumos_do_turno(persistencia& DAO, int turno) 
{
    // vamos atribuir todos os consumos medicamentosos de um determinado turno!
    DAO.carrega_consumos_dieta();
    DAO.carrega_consumos_medicamento();
     
    auto cpy_consumos_dieta = DAO.tabela_consumos_dieta;   
    for(auto& entrada : cpy_consumos_dieta) 
    {
        auto& CD = entrada.second;
        if(CD.id_turno == turno && CD.efetivacao_consumo == false && !CD.id_enfermeiro) 
        {
            atribui_consumo_dieta(DAO, CD.id);
        }
    }
    
    auto cpy_consumos_medicamento = DAO.tabela_consumos_medicamento;
    for(auto& entrada : cpy_consumos_medicamento) 
    {
        auto& consumo = entrada.second;
        if(consumo.id_turno == turno && consumo.efetivacao_consumo == false && !consumo.id_enfermeiro)
        {
            atribui_consumo_medicamento(DAO, consumo.id);
        }
    }
}

void muda_enfermeiro_consumo_medicamento(persistencia& DAO, int id_consumo, int novo_enfermeiro)
{
    DAO.carrega_medicos();
    DAO.carrega_pacientes();
    DAO.carrega_receitas();
    DAO.carrega_prescricoes();
    DAO.carrega_consumos_medicamento();
    DAO.carrega_enfermeiros();

    auto& C = DAO.tabela_consumos_medicamento[id_consumo];

    C.id_enfermeiro = make_optional(novo_enfermeiro);
    
    /* 
    auto& E = DAO.tabela_enfermeiros[velho_enfermeiro];
    for(auto& CM : E.lista_consumos_medicamento) {
        if(CM.id == C.id) {
            CM = C;
        }
    }
    */
    auto& E2 = DAO.tabela_enfermeiros[novo_enfermeiro];
    E2.lista_consumos_medicamento.push_back(C);


    // Essa mudança precisa se refletir na prescricao
    int id_prescricao = C.id_prescricao;
    auto& P = DAO.tabela_prescricoes[id_prescricao];

    for(int i = 0; i < static_cast<int>(P.consumos.size()); ++i)
    {
        if(P.consumos[i].id == id_consumo)
        {
            P.consumos[i] = C;
            break;
        }
    }

    // Agora, temos que mudar o valor dessa prescricao na receita correspondente
    int id_receita = P.id_receita;
    auto& R = DAO.tabela_receitas[id_receita];

    for(int i = 0; i < static_cast<int>(R.prescricoes.size()); ++i)
    {
        if(R.prescricoes[i].id == id_prescricao)
        {
            R.prescricoes[i] = P;
            break;
        }
    }

    // Agora, temos que mudar o valor dessa receita onde ela é armazenada!
    auto& M = DAO.tabela_medicos[R.id_medico_responsavel];
    for(int i = 0; i < static_cast<int>(M.receitas_medicas.size()); ++i)
    {
        if(M.receitas_medicas[i].id == id_receita)
        {
            M.receitas_medicas[i] = R;
            break;
        }
    }

    auto& PC = DAO.tabela_pacientes[R.id_paciente];
    for(int i = 0; i < static_cast<int>(PC.receitas_medicas.size()); ++i)
    {
        if(PC.receitas_medicas[i].id == id_receita)
        {
            PC.receitas_medicas[i] = R;
            break;
        }
    }

    DAO.persiste_medicos();
    DAO.persiste_pacientes();
    DAO.persiste_receitas();
    DAO.persiste_prescricoes();
    DAO.persiste_consumos_medicamento();
    DAO.persiste_enfermeiros();
}

void muda_enfermeiro_consumo_dieta(persistencia& DAO, int id_consumo, int novo_enfermeiro)
{
    DAO.carrega_enfermeiros();
    DAO.carrega_consumos_dieta();
    DAO.carrega_nutricionistas();
    DAO.carrega_dietas();
    DAO.carrega_pacientes();

    auto& C = DAO.tabela_consumos_dieta[id_consumo];
    C.id_enfermeiro = make_optional(novo_enfermeiro);

    auto& E = DAO.tabela_enfermeiros[novo_enfermeiro];
    E.lista_consumos_dieta.push_back(C);

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

void substitui_enfermeiro(persistencia& DAO, int id_enfermeiro)
{
    // 1 - Remover enfermeiro da lista de enfermeiros
    DAO.carrega_enfermeiros();
    DAO.carrega_consumos_medicamento();
    DAO.carrega_consumos_dieta();

    DAO.tabela_enfermeiros.erase(id_enfermeiro);
    DAO.persiste_enfermeiros(); 

    auto CPY = DAO.tabela_consumos_medicamento;
    for(auto CM : CPY)
    {
        if(CM.second.id_enfermeiro && CM.second.efetivacao_consumo == false)
        {
            if(CM.second.id_enfermeiro.value() == id_enfermeiro)
            {
                int id = CM.second.id;
                int turno = CM.second.id_turno;
                auto candidatos = ids_dos_enfermeiros_que_trabalham_em_um_turno(DAO, turno);
                assert(candidatos.size() > 0);
                int id_substituto = candidatos.back(); 
                muda_enfermeiro_consumo_medicamento(DAO, id, id_substituto); 
            }
        }
    }
    auto CPY2 = DAO.tabela_consumos_dieta;
    for(auto CD : CPY2)
    {
        if(CD.second.id_enfermeiro && CD.second.efetivacao_consumo == false)
        {
            if(CD.second.id_enfermeiro.value() == id_enfermeiro)
            {
                int id = CD.second.id;
                int turno = CD.second.id_turno;
                auto candidatos = ids_dos_enfermeiros_que_trabalham_em_um_turno(DAO, turno);
                assert(candidatos.size() > 0);
                int id_substituto = candidatos.back();
                muda_enfermeiro_consumo_dieta(DAO, id, id_substituto);
            }
        }
    }
}

int main()
{
    // Populando entidades necessárias! 
    // Inicialmente todos os vetores estão vazios!    
    auto DAO = persistencia();
    DAO.clear_all(); // limpando todos os json

    // vamos exibir os jsons até o momento (vão estar vazios)!

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
    // Essa prescricao irá criar 2 consumos medicamentosos, com índices 1 e 2 
    int preescricao1 = DAO.cadastra_prescricao_medicamento(id_receita, 100.0, medicamento1, "via venosa", vector<int>{0, 1});
    
    // Vamos criar um nutricionista, para poder passar uma dieta
    int id_nutricionista = DAO.cadastra_nutricionista();
    
    int id_dieta = DAO.cadastra_dieta("vegetais", "nao come cenoura", id_paciente, id_nutricionista);

    int consumo_dieta_0 = DAO.cadastra_consumo_dieta(id_dieta, 0, "via oral");
    int consumo_dieta_1 = DAO.cadastra_consumo_dieta(id_dieta, 1, "via oral");
    int consumo_dieta_2 = DAO.cadastra_consumo_dieta(id_dieta, 2, "via oral");
    // vamos agora criar enfermeiros, apenas um deles vai trabalhar no dia 0 e outro no dia 1
    int id_enfermeiro = DAO.cadastra_enfermeiro({0, 1, 2});
    int id_enfermeiro2 = DAO.cadastra_enfermeiro({0, 1, 2});
    int id_enfermeiro3 = DAO.cadastra_enfermeiro({0, 1, 2});
    // Vamos mostrar os consumos antes de atribuir aos enfermeiros
    atribui_consumos_do_turno(DAO, 0); 

    cout << "#### Consumos antes de demitir os enfermeiros 1 e 2" << endl; 
    DAO.mostra_json("consumos_medicamento.json");
    DAO.mostra_json("consumos_dieta.json");
    // DAO.mostra_json("consumos_dieta.json");
    //DAO.mostra_json("consumos_dieta.json");
    substitui_enfermeiro(DAO, id_enfermeiro);
    substitui_enfermeiro(DAO, id_enfermeiro3);
    cout << "#### Consumos após demitir os enfermeiros 1 e 2" << endl;
    // Agora vamos chamar a função que atribui esses consumos a enfermeiros do turno
    DAO.mostra_json("consumos_medicamento.json");
    DAO.mostra_json("consumos_dieta.json"); 
    // DAO.mostra_json("consumos_dieta.json");

    return 0;

}
