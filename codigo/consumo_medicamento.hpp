#ifndef CONSUMO_MEDICAMENTO_H
#define CONSUMO_MEDICAMENTO_H
#include <string>
#include <ctime>

struct consumo_medicamento 
{
    std::string forma_ingestao;
    time_t hora_do_consumo;
    bool efetivacao_consumo;

    consumo_medicamento() {}
    consumo_medicamento(std::string forma_ingestao, time_t hora_do_consumo) : forma_ingestao(forma_ingestao), hora_do_consumo(hora_do_consumo) 
    {
        efetivacao_consumo = false; 
    }
    
    void confirmacao_do_consumo() 
    {
        efetivacao_consumo = true;
    }
};

#endif
