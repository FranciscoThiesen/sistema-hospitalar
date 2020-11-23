#ifndef DIETA_H
#define DIETA_H
#include <string>
#include <vector>
#include "consumo_dieta.hpp"
struct dieta {
    std::string tipo_comida, restricoes_alimentares;
    std::vector< consumo_dieta > consumos;
    int id, id_paciente, id_nutricionista; 

    dieta() {}
    dieta(std::string tipo_comida, std::string restricoes_alimentares, int id, 
         int id_paciente, int id_nutricionista) :
    tipo_comida(tipo_comida), restricoes_alimentares(restricoes_alimentares), id(id),
    id_paciente(id_paciente), id_nutricionista(id_nutricionista) {}
};

#endif
