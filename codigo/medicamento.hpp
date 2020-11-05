#ifndef MEDICAMENTO_H
#define MEDICAMENTO_H
#include <string>

struct medicamento {
    int id;
    std::string nome, bula;
    
    medicamento() {}
    medicamento(int id, std::string nome, std::string bula) : id(id), nome(nome), bula(bula) {}
};
#endif
