#ifndef PACIENTE_H
#define PACIENTE_H
#include <string>
#include <unordered_set>
#include <optional>
#include "quarto.hpp"
#include "receita.hpp"
#include "ocupacao_quarto.hpp"
#include "dieta.hpp"
using namespace std;

struct paciente {
    int id, id_medico_responsavel;
    string ficha_medica, nome;
    bool teve_alta;
    optional< ocupacao_quarto > ocupacao_paciente;
    optional< dieta > dieta_paciente;    
    vector< receita > receitas_medicas;
    
    paciente() {}
    paciente(int id, int id_medico_responsavel, string ficha_medica, string nome) : id(id), id_medico_responsavel(id_medico_responsavel), ficha_medica(ficha_medica), nome(nome) {
        teve_alta = false; 
    }
};

#endif
