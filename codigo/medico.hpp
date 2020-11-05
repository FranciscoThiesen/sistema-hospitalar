#ifndef MEDICO_H
#define MEDICO_H

#include <string>
#include <vector>

struct medico {
    int CRM;
    std::vector< std::string > especialidades;

    medico() {}
    medico(int CRM, std::vector< std::string > especialidades) : CRM(CRM), especialidades(especialidades) {}
};

#endif
