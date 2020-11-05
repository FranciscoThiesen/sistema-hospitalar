#ifndef QUARTO_H
#define QUARTO_H
#include <unordered_set>
#include <cassert>
#include <vector>

struct quarto {
    static int total_quartos; // guarda o total de quartos já criados!
    int id;
    bool status_de_ocupacao;
    
    static std::unordered_set<int> quartos_livres;
    static std::unordered_set<int> quartos_ocupados;

    quarto() {
        id = total_quartos++;
        status_de_ocupacao = false;
        quartos_livres.insert(id);
    }
    
    void ocupa_quarto() { 
        assert(status_de_ocupacao == false);
        quartos_livres.erase(id);
        quartos_ocupados.insert(id);
        status_de_ocupacao = true; 
    }
    void libera_quarto() { 
        assert(status_de_ocupacao == true);
        quartos_ocupados.erase(id);
        quartos_livres.insert(id);
        status_de_ocupacao = false; 
    }
};

int quarto::total_quartos = 0;

#endif
