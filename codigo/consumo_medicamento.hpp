#ifndef CONSUMO_MEDICAMENTO_H
#define CONSUMO_MEDICAMENTO_H
#include <string>
#include <ctime>
#include <optional>

struct consumo_medicamento 
{
    int id, id_prescricao;
    std::string forma_ingestao;
    bool efetivacao_consumo;
    int id_turno;
    std::optional<int> id_enfermeiro;

    consumo_medicamento() {}
    consumo_medicamento(int id, int id_prescricao, std::string forma_ingestao, int id_turno) : id(id), id_prescricao(id_prescricao), forma_ingestao(forma_ingestao), id_turno(id_turno) 
    {
        efetivacao_consumo = false; 
    }
    

    void confirmacao_do_consumo() 
    {
        efetivacao_consumo = true;
    }
};

#endif
