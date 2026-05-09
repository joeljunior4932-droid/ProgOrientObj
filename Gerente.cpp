#include "Gerente.h"
#include "Cliente.h"

Gerente::Gerente(string login, string senha, string nome, string trabalho):
    Pessoa(login, senha, nome, trabalho), clientes(0){}
Gerente::~Gerente(){}
vector<Cliente*> Gerente::getClientes() const{
    return this->clientes;
}
void Gerente::setClientes(Cliente* x){
    this->clientes.push_back(x);
}
void Gerente::exibirDados() const {
    Pessoa::exibirDados();
    for(int i =0; i<this->clientes.size(); i++)
        this->clientes[i]->exibirDados();
}