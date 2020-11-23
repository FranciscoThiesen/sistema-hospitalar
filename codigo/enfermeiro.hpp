#ifndef ENFERMEIRO_H
#define ENFERMEIRO_H

#include <vector>
#include "consumo_dieta.hpp"
#include "consumo_medicamento.hpp"

struct enfermeiro {
    int id;
    std::vector< int > dias_trabalhados; // inteiros no intervalo [0, 6], representando os dias da semana    
    std::vector< consumo_dieta > lista_consumos_dieta;
    std::vector< consumo_medicamento > lista_consumos_medicamento; 
    enfermeiro() {}
    enfermeiro(int id, std::vector<int> dias_trabalhados) : id(id), dias_trabalhados(dias_trabalhados) {}
};

// As 3 funções necessárias para a classe enfermeiro serão feitas pelo sistema que integra as classes
// adicionar turno!
// usar métodos estáticos para inserir e remover enfermeiros
#endif
