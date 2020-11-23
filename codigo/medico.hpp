#ifndef MEDICO_H
#define MEDICO_H

#include <cassert>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include "receita.hpp"
#include "paciente.hpp"

struct medico {

    int CRM;
    std::vector< std::string > especialidades;
    std::vector< receita > receitas_medicas;
    std::vector< paciente > lista_pacientes; 
    medico() {}
    medico(int CRM, std::vector< std::string > especialidades) : CRM(CRM), especialidades(especialidades) {}
};


#endif
