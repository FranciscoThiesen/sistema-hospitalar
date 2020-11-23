#ifndef OCUPACAO_QUARTO_H
#define OCUPACAO_QUARTO_H
#include <ctime>
#include <cassert>
#include <iostream>
#include "quarto.hpp"

using namespace std;

struct ocupacao_quarto {
    int id, id_paciente;
    int data_inicio, data_fim;
    quarto quarto_ocupado;

    ocupacao_quarto() {} 
    // Em tempo de criação, temos que saber o quarto + id_paciente + id_ocupação
    ocupacao_quarto(int id, int id_paciente, quarto Q) : id(id), id_paciente(id_paciente) {
        assert(Q.ocupado == false);
        Q.ocupado = true;
        data_fim = -1;
        data_inicio = (int) time(NULL);
        quarto_ocupado = Q;
    }
};

#endif
