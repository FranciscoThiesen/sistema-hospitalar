#ifndef CONSUMO_DIETA_H
#define CONSUMO_DIETA_H
#include <string>
#include <optional>

struct consumo_dieta {
    int id, id_dieta, id_turno;
    std::string forma_ingestao;
    bool efetivacao_consumo;
    std::optional<int> id_enfermeiro;

    consumo_dieta() {}
    consumo_dieta(int id, int id_dieta, int id_turno, std::string forma_ingestao) : id(id), id_dieta(id_dieta), id_turno(id_turno), forma_ingestao(forma_ingestao) {
        efetivacao_consumo = false;
    }

    void confirmacao_do_consumo()
    {
        efetivacao_consumo = true;
    }
};
#endif

