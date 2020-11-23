#include <bits/stdc++.h>
#include "persistencia.hpp"

using namespace std;

int main()
{
    cout << "Bem vindo ao sistema de gerência hospitalar." << endl;
    cout << "Para inserir um novo médico, digite m e aperte enter" << endl;
    cout << "Para listar os médicos disponiveis, aperte l e aperte enter" << endl;
    cout << "Para sair do sistema, aperte x seguido de enter" << endl;
    
    auto P = persistencia();

    while(1) {
        string v;
        cin >> v;
        if(v == "m") 
        {
            cout << "Insira o CRM desse novo médico" << endl;
            int CRM; cin >> CRM;
            cout << "Insira a principal especialidade deste médico" << endl;
            string especialidade;
            cin >> especialidade;
            P.cadastra_medico(CRM, vector<string>(1, especialidade)); 
        }
        else if(v == "l") 
        {
            P.lista_medicos();
        }
        else if(v == "x") {
            break;
        }
        else {
            cout << "comando invalido" << endl;
        }

    }
    return 0;
}
