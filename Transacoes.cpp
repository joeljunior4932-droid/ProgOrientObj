#include "Transacoes.h"
#include <iostream>
#include "Cliente.h"

Transacoes::Transacoes(string tipo, string data, string horario, double valor): 
            tipo(tipo), data(data), horario(horario), valor(valor), clientes(0){}
Transacoes::~Transacoes(){
    //apenas limpamos o vetor sem deletar os objetos Cliente
    this->clientes.clear();
}
string Transacoes::getTipo() const{
    return this->tipo;
}
string Transacoes::getData() const{
    return this->data;
}
string Transacoes::getHorario() const{
    return this->horario;
}
double Transacoes::getValor() const{
    return this->valor;
}
vector<Cliente*>& Transacoes::getClientes(){
    return this->clientes;
}
void Transacoes::setClientes(Cliente *cliente){
    this->clientes.push_back(cliente);
    //add um cliente ao vetor 
    //push adiciona um ponteiro ao final do vetor p add cliente 
}
void Transacoes::setData(const string& data){
    this->data = data;
}
void Transacoes::setHorario(const string& horario){
    this->horario = horario;
}
void Transacoes::setValor(double valor){
    this->valor = valor;
}

void Transacoes::exibirTransacoes() const{
    cout << "Tipo: " << this->tipo << endl;
    cout << "Data: " << this->data << endl;
    cout << "Horário: " << this->horario << endl;
    cout << "Valor : " << this->valor << endl;
    cout << "Clientes:\n";
    //percorrendo o vetor de clientes para imprimir todos 
    //depos/saque imprime 1 nome
    //trans imprime 2 nomes 
    for(int i =0; i<(int)this->clientes.size(); i++)
        cout << " " << this->clientes[i]->nome << endl; 
    //colocamos int pois size eh unsigned, sem sinal, so aceita positivos 
        
}
