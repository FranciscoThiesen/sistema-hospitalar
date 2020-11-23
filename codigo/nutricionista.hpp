#ifndef NUTRICIONISTA_H
#define NUTRICIONISTA_H

#include "dieta.hpp"

struct nutricionista {
    int id;
    std::vector< dieta > dietas; 
    nutricionista() {}
    nutricionista(int id) : id(id) {}
};

#endif
