#ifndef MENU_H
#define MENU_H
#include <iostream>
using namespace std;
void printMenu(){
    cout << "====== SISTEMA DE GERENCIAMENTO DE BANCO ======\n";
    cout << "1. Cadastrar cliente\n";
    cout << "2. Cadastrar gerente\n";
    cout << "3. Criar transação\n";
    cout << "4. Exibir extrato de um cliente\n";
    cout << "5. Associar gerente a cliente\n";
    cout << "6. Listar clientes\n";
    cout << "7. Listar gerentes\n";
    cout << "8. Salvar dados e sair\n";
    cout << "==================================================" << endl <<
            "Escolha uma opção: ";
}


#endif