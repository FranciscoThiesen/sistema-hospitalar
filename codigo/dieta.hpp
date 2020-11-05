#ifndef DIETA_H
#define DIETA_H
#include <string>
#include <vector>
#include <ctime>
#include "nutricionista.hpp"
#include "paciente.hpp"

struct dieta {
    std::string tipo_comida, restricoes_alimentares;
    std::vector< time_t > horarios_refeicoes;
    nutricionista nutricionista_responsavel; 
    paciente paciente_atendido;

    dieta() {}
    dieta(std::string tipo_comida, std::string restricoes_alimentares, std::vector< time_t > horarios_refeicoes, 
            nutricionista nutricionista_responsavel, paciente paciente_atendido) :
    tipo_comida(tipo_comida), restricoes_alimentares(restricoes_alimentares), horarios_refeicoes(horarios_refeicoes),
    nutricionista_responsavel(nutricionista_responsavel), paciente_atendido(paciente_atendido) {}
    
};
#endif
