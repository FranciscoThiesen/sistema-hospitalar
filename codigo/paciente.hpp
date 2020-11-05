#ifndef PACIENTE_H
#define PACIENTE_H
#include <string>
#include <unordered_set>
#include <optional>
#include "quarto.hpp"
#include "receita.hpp"
struct paciente {
    int id;
    std::string ficha_medica;
    bool teve_alta;
    std::optional< quarto > quarto_ocupado;
    

    paciente() {}
    paciente(int id, std::string ficha_medica) : id(id), ficha_medica(ficha_medica) {
        teve_alta = false; 
    }
    
    void atualiza_ficha_medica(std::string& nova_ficha) {
        ficha_medica = nova_ficha;
    }
};

// Adicionar quarto
// Consertar o diagrama
// Adicionar vetor de receitas
// 

#endif
