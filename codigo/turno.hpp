#ifndef TURNO_H
#define TURNO_H
#include <string>

struct turno {
    int hora_inicio, hora_fim;
    std::string dia_da_semana;
     
    turno() {}
    turno(int hora_inicio, int hora_fim, std::string dia_da_semana) : hora_inicio(hora_inicio), hora_fim(hora_fim), dia_da_semana(dia_da_semana) {}
};

// Persistência json
// Uma class controladora para cada um dos casos de uso
	// bloated controlers
#endif
