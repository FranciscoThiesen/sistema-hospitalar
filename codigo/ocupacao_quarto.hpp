#ifndef OCUPACAO_QUARTO_H
#define OCUPACAO_QUARTO_H
#include <ctime>
#include <cassert>
#include "quarto.hpp"

struct ocupacao_quarto {
    time_t data_inicio, data_fim;
    int id_quarto;
    int id_paciente;

    ocupacao_quarto(quarto& Q) {
        assert(Q.status_de_ocupacao == false);
        assert(quarto::quartos_livres.find(Q.id) != quarto::quartos_livres.end());

        Q.ocupa_quarto();
        id_quarto = Q.id;
    }
    
    void inicia_ocupacao() { data_inicio = time(0); }
    void finaliza_ocupacao() { data_fim = time(0); }
};

#endif
