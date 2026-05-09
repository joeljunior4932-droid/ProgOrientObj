#ifndef GERENTE_H
#define GERENTE_H
#include "Pessoa.h"
#include <vector>
class Cliente;
class Gerente: public Pessoa{
    vector<Cliente *> clientes;

    public:
        Gerente(string = "", string ="", string ="", string ="");
        ~Gerente();
        vector<Cliente*> getClientes() const;
        void setClientes(Cliente* x);
        void exibirDados() const override;
        

};

#endif