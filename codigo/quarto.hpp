#ifndef QUARTO_H
#define QUARTO_H
#include <unordered_set>
#include <cassert>
#include <vector>
#include <optional>

using namespace std;

struct quarto {
    int id;
    bool ocupado;
    optional<int> id_ocupacao;

    quarto() {
        ocupado = false;
    }
    
    quarto(int id) : id(id) {
        ocupado = false;
    }
};


#endif
