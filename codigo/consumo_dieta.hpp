#ifndef CONSUMO_DIETA_H
#define CONSUMO_DIETA_H
#include <string>

struct consumo_dieta {
    std::string forma_de_ingestao;
    time_t horario_consumo;
    bool efetivacao_consumo;

    consumo_dieta() {}
    consumo_dieta(std::string forma_de_ingestao, time_t horario_consumo) : forma_de_ingestao(forma_de_ingestao), horario_consumo(horario_consumo) {
        efetivacao_consumo = false;
    }

    void confirmacao_do_consumo()
    {
        efetivacao_consumo = true;
    }
};
#endif

