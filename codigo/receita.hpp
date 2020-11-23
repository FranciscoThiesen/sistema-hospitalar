#ifndef RECEITA_H
#define RECEITA_H

#include "prescricao_medicamento.hpp"
#include <vector>

struct receita {
    int id, id_medico_responsavel, id_paciente;
    std::vector< prescricao_medicamento > prescricoes;
    receita() {} 
    receita(int id, int id_medico_responsavel, int id_paciente) : 
        id(id), id_medico_responsavel(id_medico_responsavel), id_paciente(id_paciente) {}
};

#endif
