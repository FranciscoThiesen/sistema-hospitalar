#ifndef PREESCRICAO_MEDICAMENTO_H
#define PREESCRICAO_MEDICAMENTO_H

struct preescricao_medicamento {
    int id;
    double dose;
    preescricao_medicamento() {}
    preescricao_medicamento(int id, int dose) : id(id), dose(dose) {}
};



#endif
