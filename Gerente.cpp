#include "Gerente.h"
#include "Cliente.h"
#include <iostream> 

Gerente::Gerente(string login, string senha, string nome, string trabalho, string dataNasc):
    Pessoa(login, senha, nome, trabalho, dataNasc), clientes(0){}
Gerente::~Gerente(){
    // O vetor clientes guarda ponteiros emprestados (owned pelos vetores do main),
    // portanto apenas limpamos o vetor sem deletar os objetos Cliente
    this->clientes.clear();
}
bool Gerente::operator<(const Gerente& x) const{
    return this->nome < x.nome;
}
vector<Cliente*>& Gerente::getClientes(){
    return this->clientes;
}
void Gerente::setClientes(Cliente* x){
    this->clientes.push_back(x);
}
void Gerente::exibirDados() const {
    Pessoa::exibirDados();
    //imprime os dados do gerente e de cada cliente
    for(int i =0; i<(int)this->clientes.size(); i++)
        this->clientes[i]->exibirDados();
}