#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <bits/stdc++.h>

#include "json.hpp"
#include "turno.hpp"
#include "medico.hpp"
#include "paciente.hpp"
#include "quarto.hpp"
#include "medicamento.hpp"
#include "prescricao_medicamento.hpp"
#include "ocupacao_quarto.hpp"
#include "receita.hpp"
#include "dieta.hpp"
#include "consumo_dieta.hpp"
#include "consumo_medicamento.hpp"
#include "nutricionista.hpp"
#include "enfermeiro.hpp"

using namespace std;
using json = nlohmann::json;

// DAO
// Essa classe é responsável pela persistência de dados do sistema

// O que tem que ser armazenado?
// 1 - Médicos
// 2 - Pacientes
// 3 - Receita
// 4 - Preescrição medicamentosa
// 5 - Nutricionista
// 6 - Quartos
// 7 - Ocupação quartos
// 8 - Dieta
// 9 - Consumo dieta
// 10 - Consumo medicamento
// 11 - Nutricionista

/*
   Os métodos abaixo fazer overload dos métodos de persistência em json, para que a biblioteca json.hpp
   saiba como serializar e deserializar instâncias das classes. Quando for possível, usarei a definição via
   macros. Para saber mais, leia https://nlohmann.github.io/json/features/arbitrary_types/ 
*/
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(medicamento, id, nome, bula);

void to_json(json&j, const consumo_dieta &c) {
    j = json{{"id", c.id}, {"id_dieta", c.id_dieta}, {"id_turno", c.id_turno},
             {"forma_ingestao", c.forma_ingestao}, {"efetivacao_consumo", c.efetivacao_consumo}};
    if(c.id_enfermeiro) {
        j["id_enfermeiro"] = c.id_enfermeiro.value();
    }
}

void from_json(const json& j, consumo_dieta& c) 
{
    j.at("id").get_to(c.id);
    j.at("id_dieta").get_to(c.id_dieta);
    j.at("id_turno").get_to(c.id_turno);
    j.at("forma_ingestao").get_to(c.forma_ingestao);
    j.at("efetivacao_consumo").get_to(c.efetivacao_consumo);
    if(j.contains("id_enfermeiro")) 
    {
        c.id_enfermeiro = make_optional(j["id_enfermeiro"]);
    }    
}

void to_json(json& j, const consumo_medicamento& c) {
    j = json{{"id", c.id}, {"id_prescricao", c.id_prescricao}, {"forma_ingestao", c.forma_ingestao},
    {"efetivacao_consumo", c.efetivacao_consumo}, {"id_turno", c.id_turno}};
    if(c.id_enfermeiro)
    {
        j["id_enfermeiro"] = c.id_enfermeiro.value(); 
    }
}

void from_json(const json& j, consumo_medicamento& c) {
    j.at("id").get_to(c.id);
    j.at("id_prescricao").get_to(c.id_prescricao);
    j.at("forma_ingestao").get_to(c.forma_ingestao);
    j.at("efetivacao_consumo").get_to(c.efetivacao_consumo);
    j.at("id_turno").get_to(c.id_turno);
    if(j.contains("id_enfermeiro")) {
        c.id_enfermeiro = make_optional(j["id_enfermeiro"]);
    }
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(prescricao_medicamento, id, id_receita, dose, med, forma_ingestao, consumos);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(receita, id, id_medico_responsavel, id_paciente, prescricoes);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(dieta, tipo_comida, restricoes_alimentares, consumos, id, id_paciente, id_nutricionista);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(nutricionista, id, dietas);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(enfermeiro, id, dias_trabalhados, lista_consumos_dieta, lista_consumos_medicamento);
// NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(turno, hora_inicio, hora_fim);

// Para as classes que tem algum atributo opcional, não foi possível usar os macros acima e foi necessário
// sobrescrever os métodos to_json e from_json para poder interagir com o objeto dessa classe

void to_json(json &j, const quarto &q)
{
    j = json{{"id", q.id}, {"ocupado", q.ocupado}};
    if (q.id_ocupacao)
        j["id_ocupacao"] = q.id_ocupacao.value();
}

void to_json(json &j, const ocupacao_quarto &o)
{
    j = json{{"id", o.id}, {"id_paciente", o.id_paciente}, {"data_inicio", o.data_inicio}, {"data_fim", o.data_fim}};
    j["quarto_ocupado"] = o.quarto_ocupado;
}

void to_json(json &j, const paciente &p)
{
    j = json{{"id", p.id}, {"id_medico_responsavel", p.id_medico_responsavel}, {"ficha_medica", p.ficha_medica}, {"teve_alta", p.teve_alta}, {"nome", p.nome}, {"receitas_medicas", p.receitas_medicas}};
    if (p.ocupacao_paciente)
        j["ocupacao_paciente"] = p.ocupacao_paciente.value();
    if (p.dieta_paciente)
        j["dieta_paciente"] = p.dieta_paciente.value();
}

void from_json(const json &j, quarto &q)
{
    j.at("id").get_to(q.id);
    j.at("ocupado").get_to(q.ocupado);
    if (j.contains("id_ocupacao"))
    {
        q.id_ocupacao = make_optional(j["id_ocupacao"]);
    }
}

void from_json(const json &j, ocupacao_quarto &o)
{
    j.at("id").get_to(o.id);
    j.at("id_paciente").get_to(o.id_paciente);
    j.at("data_inicio").get_to(o.data_inicio);
    j.at("data_fim").get_to(o.data_fim);
    j.at("quarto_ocupado").get_to(o.quarto_ocupado);
}

void from_json(const json &j, paciente &p)
{
    j.at("id").get_to(p.id);
    j.at("id_medico_responsavel").get_to(p.id_medico_responsavel);
    j.at("ficha_medica").get_to(p.ficha_medica);
    j.at("nome").get_to(p.nome);
    j.at("receitas_medicas").get_to(p.receitas_medicas);
    j.at("teve_alta").get_to(p.teve_alta);
    if (j.contains("ocupacao_paciente"))
        p.ocupacao_paciente = make_optional(j["ocupacao_paciente"].get<ocupacao_quarto>());
    if (j.contains("dieta_paciente"))
        p.dieta_paciente = make_optional(j["dieta_paciente"].get<dieta>());
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(medico, CRM, especialidades, receitas_medicas, lista_pacientes);

struct persistencia
{

    unordered_map<int, medico> tabela_medicos;
    unordered_map<int, paciente> tabela_pacientes;
    unordered_map<int, quarto> tabela_quartos;
    unordered_map<int, medicamento> tabela_medicamentos;
    unordered_map<int, prescricao_medicamento> tabela_prescricoes;
    unordered_map<int, ocupacao_quarto> tabela_ocupacoes;
    unordered_map<int, receita> tabela_receitas;
    unordered_map<int, consumo_dieta> tabela_consumos_dieta;
    unordered_map<int, consumo_medicamento> tabela_consumos_medicamento;
    unordered_map<int, nutricionista> tabela_nutricionistas;
    unordered_map<int, dieta> tabela_dietas;
    unordered_map<int, enfermeiro> tabela_enfermeiros;
    // Lembrar de registrar os enfermeiros!!

    void clear_all()
    {
        tabela_medicos.clear();
        tabela_pacientes.clear();
        tabela_quartos.clear();
        tabela_medicamentos.clear();
        tabela_prescricoes.clear();
        tabela_ocupacoes.clear();
        tabela_receitas.clear();
        tabela_consumos_dieta.clear();
        tabela_consumos_medicamento.clear();
        tabela_nutricionistas.clear();
        tabela_dietas.clear();
        tabela_enfermeiros.clear();

        persiste_medicos();
        persiste_pacientes();
        persiste_quartos();
        persiste_medicamentos();
        persiste_prescricoes();
        persiste_ocupacoes();
        persiste_receitas();
        persiste_consumos_dieta();
        persiste_consumos_medicamento();
        persiste_nutricionistas();
        persiste_dietas();
        persiste_enfermeiros();
    }

    // Funções que efetuam o carregamento do sistema
    void persiste_medicos(string arquivo_saida = string("medicos.json"))
    {
        ofstream out(arquivo_saida);
        auto json_medicos = json::array();
        for (const auto &entrada : tabela_medicos)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_medicos.push_back(current);
        }
        json F;
        F["medicos"] = json_medicos;
        out << F << endl;
        out.close();
    }

    void persiste_pacientes(string arquivo_saida = string("pacientes.json"))
    {
        ofstream out(arquivo_saida);
        auto json_pacientes = json::array();
        for (const auto &entrada : tabela_pacientes)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_pacientes.push_back(current);
        }
        json F;
        F["pacientes"] = json_pacientes;
        out << F << endl;
        out.close();
    }

    void persiste_quartos(string arquivo_saida = string("quartos.json"))
    {
        ofstream out(arquivo_saida);
        auto json_quartos = json::array();
        for (const auto &entrada : tabela_quartos)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_quartos.push_back(current);
        }
        json F;
        F["quartos"] = json_quartos;
        out << F << endl;
        out.close();
    }

    void persiste_medicamentos(string arquivo_saida = string("medicamentos.json"))
    {
        ofstream out(arquivo_saida);
        auto json_medicamentos = json::array();
        for (const auto &entrada : tabela_medicamentos)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_medicamentos.push_back(current);
        }
        json F;
        F["medicamentos"] = json_medicamentos;
        out << F << endl;
        out.close();
    }

    void persiste_prescricoes(string arquivo_saida = string("prescricoes.json"))
    {
        ofstream out(arquivo_saida);
        auto json_prescricoes = json::array();
        for (const auto &entrada : tabela_prescricoes)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_prescricoes.push_back(current);
        }
        json F;
        F["prescricoes"] = json_prescricoes;
        out << F << endl;
        out.close();
    }

    void persiste_ocupacoes(string arquivo_saida = string("ocupacoes.json"))
    {
        ofstream out(arquivo_saida);
        auto json_ocupacoes = json::array();
        for (const auto &entrada : tabela_ocupacoes)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_ocupacoes.push_back(current);
        }
        json F;
        F["ocupacoes"] = json_ocupacoes;
        out << F << endl;
        out.close();
    }

    void persiste_receitas(string arquivo_saida = string("receitas.json"))
    {
        ofstream out(arquivo_saida);
        auto json_receitas = json::array();
        for (const auto &entrada : tabela_receitas)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_receitas.push_back(current);
        }
        json F;
        F["receitas"] = json_receitas;
        out << F << endl;
        out.close();
    }

    void persiste_consumos_dieta(string arquivo_saida = string("consumos_dieta.json"))
    {
        ofstream out(arquivo_saida);
        auto json_consumos_dieta = json::array();
        for (const auto &entrada : tabela_consumos_dieta)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_consumos_dieta.push_back(current);
        }
        json F;
        F["consumos_dieta"] = json_consumos_dieta;
        out << F << endl;
        out.close();
    }

    void persiste_consumos_medicamento(string arquivo_saida = string("consumos_medicamento.json"))
    {
        ofstream out(arquivo_saida);
        auto json_consumos_medicamento = json::array();
        for (const auto &entrada : tabela_consumos_medicamento)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_consumos_medicamento.push_back(current);
        }
        json F;
        F["consumos_medicamento"] = json_consumos_medicamento;
        out << F << endl;
        out.close();
    }

    void persiste_nutricionistas(string arquivo_saida = string("nutricionistas.json"))
    {
        ofstream out(arquivo_saida);
        auto json_nutricionistas = json::array();
        for (const auto &entrada : tabela_nutricionistas)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_nutricionistas.push_back(current);
        }
        json F;
        F["nutricionistas"] = json_nutricionistas;
        out << F << endl;
        out.close();
    }

    void persiste_dietas(string arquivo_saida = string("dietas.json"))
    {
        ofstream out(arquivo_saida);
        auto json_dietas = json::array();
        for (const auto &entrada : tabela_dietas)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_dietas.push_back(current);
        }
        json F;
        F["dietas"] = json_dietas;
        out << F << endl;
        out.close();
    }

    void persiste_enfermeiros(string arquivo_saida = string("enfermeiros.json"))
    {
        ofstream out(arquivo_saida);
        auto json_enfermeiros = json::array();
        for(const auto& entrada : tabela_enfermeiros)
        {
            json current;
            current["chave"] = entrada.first;
            current["valor"] = entrada.second;
            json_enfermeiros.push_back(current);
        }
        json F;
        F["enfermeiros"] = json_enfermeiros;
        out << F << endl;
        out.close();
    }

    // Fim das funções de persistência

    // Início do grupo de funções que efetua o carregamento das tabelas,
    // a partir do que foi persistido nos arquivos

    void carrega_medicos(string arquivo_entrada = string("medicos.json"))
    {
        tabela_medicos.clear();

        if (filesystem::exists(arquivo_entrada) == false)
            return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for (auto &entrada : F["medicos"])
        {
            auto chave = entrada["chave"];
            auto P = entrada["valor"].get<medico>();
            tabela_medicos[chave] = P;
        }
        in.close();
    }

    void carrega_pacientes(string arquivo_entrada = string("pacientes.json"))
    {
        tabela_pacientes.clear();

        if (filesystem::exists(arquivo_entrada) == false)
            return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for (auto &entrada : F["pacientes"])
        {
            auto chave = entrada["chave"];
            auto P = entrada["valor"].get<paciente>();
            tabela_pacientes[chave] = P;
        }
        in.close();
    }

    void carrega_quartos(string arquivo_entrada = string("quartos.json"))
    {
        tabela_quartos.clear();

        if (filesystem::exists(arquivo_entrada) == false)
            return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for (auto &entrada : F["quartos"])
        {
            auto chave = entrada["chave"];
            auto P = entrada["valor"].get<quarto>();
            tabela_quartos[chave] = P;
        }
        in.close();
    }

    void carrega_medicamentos(string arquivo_entrada = string("medicamentos.json"))
    {
        tabela_medicamentos.clear();

        if (filesystem::exists(arquivo_entrada) == false)
            return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for (auto &entrada : F["medicamentos"])
        {
            auto chave = entrada["chave"];
            auto P = entrada["valor"].get<medicamento>();
            tabela_medicamentos[chave] = P;
        }
        in.close();
    }

    void carrega_prescricoes(string arquivo_entrada = string("prescricoes.json"))
    {
        tabela_prescricoes.clear();

        if (filesystem::exists(arquivo_entrada) == false)
            return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for (auto &entrada : F["prescricoes"])
        {
            auto chave = entrada["chave"];
            auto P = entrada["valor"].get<prescricao_medicamento>();
            tabela_prescricoes[chave] = P;
        }
        in.close();
    }

    void carrega_ocupacoes(string arquivo_entrada = string("ocupacoes.json"))
    {
        tabela_ocupacoes.clear();

        if (filesystem::exists(arquivo_entrada) == false)
            return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for (auto &entrada : F["ocupacoes"])
        {
            auto chave = entrada["chave"];
            auto P = entrada["valor"].get<ocupacao_quarto>();
            tabela_ocupacoes[chave] = P;
        }
        in.close();
    }

    void carrega_receitas(string arquivo_entrada = string("receitas.json"))
    {
        tabela_receitas.clear();

        if (filesystem::exists(arquivo_entrada) == false)
            return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for (auto &entrada : F["receitas"])
        {
            auto chave = entrada["chave"];
            auto P = entrada["valor"].get<receita>();
            tabela_receitas[chave] = P;
        }
        in.close();
    }

    void carrega_consumos_dieta(string arquivo_entrada = string("consumos_dieta.json"))
    {
        tabela_consumos_dieta.clear();

        if (filesystem::exists(arquivo_entrada) == false)
            return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for (auto &entrada : F["consumos_dieta"])
        {
            auto chave = entrada["chave"];
            auto P = entrada["valor"].get<consumo_dieta>();
            tabela_consumos_dieta[chave] = P;
        }
        in.close();
    }

    void carrega_consumos_medicamento(string arquivo_entrada = string("consumos_medicamento.json"))
    {
        tabela_consumos_medicamento.clear();

        if (filesystem::exists(arquivo_entrada) == false)
            return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for (auto &entrada : F["consumos_medicamento"])
        {
            auto chave = entrada["chave"];
            auto P = entrada["valor"].get<consumo_medicamento>();
            tabela_consumos_medicamento[chave] = P;
        }
        in.close();
    }

    void carrega_nutricionistas(string arquivo_entrada = string("nutricionistas.json"))
    {
        tabela_nutricionistas.clear();

        if (filesystem::exists(arquivo_entrada) == false)
            return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for (auto &entrada : F["nutricionistas"])
        {
            auto chave = entrada["chave"];
            auto valor = entrada["valor"].get<nutricionista>();
            tabela_nutricionistas[chave] = valor;
        }
        in.close();
    }

    void carrega_dietas(string arquivo_entrada = string("dietas.json"))
    {
        tabela_dietas.clear();

        if (filesystem::exists(arquivo_entrada) == false)
            return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for (auto &entrada : F["dietas"])
        {
            auto chave = entrada["chave"];
            auto valor = entrada["valor"].get<dieta>();
            tabela_dietas[chave] = valor;
        }
        in.close();
    }

    void carrega_enfermeiros(string arquivo_entrada = string("enfermeiros.json"))
    {
        tabela_enfermeiros.clear();
        if(filesystem::exists(arquivo_entrada) == false) return;

        ifstream in(arquivo_entrada);
        json F;
        in >> F;
        for(auto &entrada : F["enfermeiros"])
        {
            auto chave = entrada["chave"];
            auto valor = entrada["valor"].get<enfermeiro>();
            tabela_enfermeiros[chave] = valor;
        }
        in.close();
    }

    // Função de visualização
    void mostra_json(string arquivo_entrada)
    {
        if (filesystem::exists(arquivo_entrada) == false)
        {
            cout << arquivo_entrada << " nao existe!" << endl;
        }
        else
        {
            cout << "Exibindo os conteúdos do arquivo: " << arquivo_entrada << endl;
            ifstream in(arquivo_entrada);
            json j;
            in >> j;
            cout << setw(4) << j << endl;
            in.close();
        }
    }

    template <typename T>
    int get_next_id(unordered_map<int, T> &vals)
    {
        int mx = 0;
        for (auto entrada : vals)
        {
            mx = max(entrada.first, mx);
        }
        return mx + 1;
    }

    // Funções que de fato alteram o estado do sistema
    int cadastra_medico(int CRM, vector<string> especialidades)
    {
        carrega_medicos();
        auto M = medico(CRM, especialidades);
        int chave = get_next_id<medico>(tabela_medicos);
        tabela_medicos[chave] = M;
        persiste_medicos();
        return chave;
    }

    int cadastra_paciente(int id_medico_responsavel, string ficha_medica, string nome)
    {
        carrega_pacientes();
        carrega_medicos();
        int id = get_next_id<paciente>(tabela_pacientes);
        int chave = id;
        auto P = paciente(id, id_medico_responsavel, ficha_medica, nome);
        tabela_pacientes[chave] = P;
        medico &M = tabela_medicos[id_medico_responsavel];
        M.lista_pacientes.push_back(P);
        persiste_pacientes();
        persiste_medicos();
        return id;
    }

    int cadastra_quarto()
    {
        carrega_quartos();
        int id = get_next_id<quarto>(tabela_quartos);
        auto Q = quarto(id);
        tabela_quartos[id] = Q;
        persiste_quartos();
        return id;
    }

    int cadastra_medicamento(string nome, string bula)
    {
        carrega_medicamentos();
        int id = get_next_id<medicamento>(tabela_medicamentos);
        auto M = medicamento(id, nome, bula);
        tabela_medicamentos[id] = M;
        persiste_medicamentos();
        return id;
    }

    int cadastra_consumo_medicamento(string forma_ingestao, int id_prescricao, int id_turno)
    {
        carrega_consumos_medicamento();
        carrega_prescricoes();
        int id = get_next_id<consumo_medicamento>(tabela_consumos_medicamento);
        auto C = consumo_medicamento(id, id_prescricao, forma_ingestao, id_turno);
        tabela_consumos_medicamento[id] = C;

        prescricao_medicamento &P = tabela_prescricoes[id_prescricao];
        P.consumos.push_back(C);

        persiste_consumos_medicamento();
        persiste_prescricoes();
        return id;
    }

    int cadastra_prescricao_medicamento(int id_receita, double dose, medicamento med, string forma_ingestao, vector<int> datas_consumo)
    {
        carrega_prescricoes();
        carrega_receitas();
        carrega_consumos_medicamento();
        carrega_medicos();
        carrega_pacientes();

        int id = get_next_id<prescricao_medicamento>(tabela_prescricoes);
        auto P = prescricao_medicamento(id, id_receita, dose, med, forma_ingestao);
        tabela_prescricoes[id] = P;
        persiste_prescricoes(); 
        // aqui, tem que ser criados os consumos de medicamento!

        for (const auto &data : datas_consumo)
        {
            // por simplificação, vou assumir aqui que data é um inteiro representando um dia do ano
            // e que o turno é um inteiro que representa o dia da semana [0, 6]
            cadastra_consumo_medicamento(forma_ingestao, id, data % 7);
        }

        receita &R = tabela_receitas[id_receita];
        R.prescricoes.push_back(P);

        // como a receita mudou, vamos mudar o valor no vetor receitas do paciente e do médico
        int id_paciente = R.id_paciente;
        int posicao_receita = 0;
        paciente &PC = tabela_pacientes[id_paciente];
        for (int i = 0; i < static_cast<int>(PC.receitas_medicas.size()); ++i)
        {
            if (PC.receitas_medicas[i].id == R.id)
            {
                posicao_receita = i;
                break;
            }
        }
        PC.receitas_medicas[posicao_receita] = R; // Receita atualizada!

        int id_medico = R.id_medico_responsavel;
        medico &M = tabela_medicos[id_medico];
        for (int i = 0; i < static_cast<int>(M.receitas_medicas.size()); ++i)
        {
            if (M.receitas_medicas[i].id == R.id)
            {
                posicao_receita = i;
                break;
            }
        }
        M.receitas_medicas[posicao_receita] = R; // Receita atualizada!

        persiste_consumos_medicamento();
        persiste_prescricoes();
        persiste_receitas();
        persiste_medicos();
        persiste_pacientes();

        return id;
    }

    int cadastra_receita(int id_medico_responsavel, int id_paciente)
    {
        carrega_receitas();
        carrega_medicos();
        carrega_pacientes();

        int id = get_next_id<receita>(tabela_receitas);
        auto R = receita(id, id_medico_responsavel, id_paciente);

        tabela_receitas[id] = R;

        // Prescricoes serão adicionadas a receita após sua criação!
        // Lembrar que essas receitas precisam ser atualizadas em medico e paciente!

        paciente &P = tabela_pacientes[id_paciente];
        P.receitas_medicas.push_back(R);

        medico &M = tabela_medicos[id_medico_responsavel];
        M.receitas_medicas.push_back(R);

        persiste_receitas();
        persiste_medicos();
        persiste_pacientes();
        return id;
    }

    int cadastra_ocupacao(int id_paciente, quarto Q)
    {
        carrega_ocupacoes();
        carrega_quartos();
        carrega_pacientes();

        int id = get_next_id<ocupacao_quarto>(tabela_ocupacoes);

        auto O = ocupacao_quarto(id, id_paciente, Q);

        tabela_ocupacoes[id] = O;

        quarto &q = tabela_quartos[Q.id];
        q.ocupado = true;
        q.id_ocupacao = id;

        paciente &p = tabela_pacientes[id_paciente];
        p.ocupacao_paciente = O;
        persiste_ocupacoes();
        persiste_quartos();
        persiste_pacientes();
        return id;
    }

    int cadastra_consumo_dieta(int id_dieta, int id_turno, string forma_ingestao)
    {
        carrega_consumos_dieta();
        carrega_dietas();
        carrega_nutricionistas();
        carrega_pacientes();

        int id = get_next_id<consumo_dieta>(tabela_consumos_dieta);
        auto C = consumo_dieta(id, id_dieta, id_turno, forma_ingestao);
        // tenho que achar a dieta correspondente, pra adicionar C
        dieta &D = tabela_dietas[id_dieta];
        D.consumos.push_back(C);
        // Tenho que adicionar esse consumo em paciente e médico tbm!
    
        paciente &P = tabela_pacientes[D.id_paciente];
        
        dieta D2 = P.dieta_paciente.value();
        D2.consumos.push_back(C);
        P.dieta_paciente = make_optional(D2);

        nutricionista &N = tabela_nutricionistas[D.id_nutricionista];
        int idx = -1;
        
        for (int i = 0; i < static_cast<int>(N.dietas.size()); ++i)
        {
            if (N.dietas[i].id == id_dieta)
            {
                idx = i;
            }
        }

        dieta &D3 = N.dietas[idx];
        D3.consumos.push_back(C);

        tabela_consumos_dieta[id] = C;

        persiste_consumos_dieta();
        persiste_dietas();
        persiste_nutricionistas();
        persiste_pacientes();
        return id;
    }

    int cadastra_dieta(string tipo_comida, string restricoes_alimentares, int id_paciente, int id_nutricionista)
    {
        carrega_dietas();
        carrega_nutricionistas();
        carrega_pacientes();

        int id = get_next_id<dieta>(tabela_dietas);
        auto D = dieta(tipo_comida, restricoes_alimentares, id, id_paciente, id_nutricionista);

        paciente &P = tabela_pacientes[id_paciente];
        nutricionista &N = tabela_nutricionistas[id_nutricionista];

        P.dieta_paciente = make_optional(D);
        N.dietas.push_back(D);

        tabela_dietas[id] = D;
        
        persiste_dietas();
        persiste_nutricionistas();
        persiste_pacientes();
        return id;
    }

    int cadastra_nutricionista()
    {
        carrega_nutricionistas();
        int id = get_next_id<nutricionista>(tabela_nutricionistas);
        auto N = nutricionista(id);
        tabela_nutricionistas[id] = N;
        persiste_nutricionistas();
        return id;
    }

    int cadastra_enfermeiro(vector< int > dias_trabalhados)
    {
        carrega_enfermeiros();
        int id = get_next_id<enfermeiro>(tabela_enfermeiros);
        auto E = enfermeiro(id, dias_trabalhados);
        tabela_enfermeiros[id] = E;
        persiste_enfermeiros();
        return id;
    }

    void atribui_consumo_dieta_a_enfermeiro(int id_consumo, int id_enfermeiro)
    {
        carrega_enfermeiros();
        carrega_consumos_dieta();
        carrega_nutricionistas();
        carrega_dietas();
        carrega_pacientes();

        auto& C = tabela_consumos_dieta[id_consumo];
        C.id_enfermeiro = make_optional(id_enfermeiro);

        auto& E = tabela_enfermeiros[id_enfermeiro];
        E.lista_consumos_dieta.push_back(C);

        // Esse consumo tem que ter seu valor persistido nos demais arquivos que o contém!
        // O consumo já vai estar presente em (dieta, nutricionista, paciente)
        auto& D = tabela_dietas[C.id_dieta]; // D vai mudar 
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
        auto& P = tabela_pacientes[D.id_paciente];
        P.dieta_paciente = make_optional(D);

        auto& N = tabela_nutricionistas[D.id_nutricionista];
        for(int i = 0; i < static_cast<int>(N.dietas.size()); ++i)
        {
            if(N.dietas[i].id == id_dieta)
            {
                N.dietas[i] = D;
                break;
            }
        }

        persiste_consumos_dieta();
        persiste_enfermeiros();
        persiste_nutricionistas();
        persiste_dietas();
        persiste_pacientes();
    }

    void atribui_consumo_medicamentoso_a_enfermeiro(int id_consumo, int id_enfermeiro)
    {
        carrega_medicos();
        carrega_pacientes();
        carrega_receitas();
        carrega_prescricoes();
        carrega_consumos_medicamento();
        carrega_enfermeiros();

        auto& C = tabela_consumos_medicamento[id_consumo];
        C.id_enfermeiro = make_optional(id_enfermeiro);

        auto& E = tabela_enfermeiros[id_enfermeiro];
        E.lista_consumos_medicamento.push_back(C);

        // Essa mudança precisa se refletir na prescricao
        int id_prescricao = C.id_prescricao;
        auto& P = tabela_prescricoes[id_prescricao];

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
        auto& R = tabela_receitas[id_receita];

        for(int i = 0; i < static_cast<int>(R.prescricoes.size()); ++i)
        {
            if(R.prescricoes[i].id == id_prescricao)
            {
                R.prescricoes[i] = P;
                break;
            }
        }

        // Agora, temos que mudar o valor dessa receita onde ela é armazenada!
        auto& M = tabela_medicos[R.id_medico_responsavel];
        for(int i = 0; i < static_cast<int>(M.receitas_medicas.size()); ++i)
        {
            if(M.receitas_medicas[i].id == id_receita)
            {
                M.receitas_medicas[i] = R;
                break;
            }
        }

        auto& PC = tabela_pacientes[R.id_paciente];
        for(int i = 0; i < static_cast<int>(PC.receitas_medicas.size()); ++i)
        {
            if(PC.receitas_medicas[i].id == id_receita)
            {
                PC.receitas_medicas[i] = R;
                break;
            }
        }

        persiste_medicos();
        persiste_pacientes();
        persiste_receitas();
        persiste_prescricoes();
        persiste_consumos_medicamento();
        persiste_enfermeiros();
    }

    void alta_a_paciente_sem_internar(int id_paciente)
    {
        carrega_pacientes();
        carrega_medicos();

        auto& P = tabela_pacientes[id_paciente];
        int id_medico = P.id_medico_responsavel;
        P.teve_alta = true;
        P.ficha_medica += "\nPaciente teve alta";

        // tem que atualizar o valor do paciente na lista do médico
        medico& M = tabela_medicos[id_medico];
        for(auto& p : M.lista_pacientes) 
        {
            if(p.id == P.id)
            {
                p = P;
                break;
            }
        }

        persiste_pacientes();
        persiste_medicos();
    }
    
};

#endif
