#ifndef PRESCRICAO_MEDICAMENTO_H
#define PRESCRICAO_MEDICAMENTO_H

#include "medicamento.hpp"
#include "consumo_medicamento.hpp"
#include <string>
#include <vector>

struct prescricao_medicamento
{
  int id, id_receita;
  double dose;
  medicamento med;
  std::string forma_ingestao;
  std::vector<consumo_medicamento> consumos;

  prescricao_medicamento() {}
  prescricao_medicamento(int id, int id_receita, double dose,
                          medicamento med, std::string forma_ingestao)
      : id(id), id_receita(id_receita), dose(dose),
        med(med), forma_ingestao(forma_ingestao) {}
};

#endif
