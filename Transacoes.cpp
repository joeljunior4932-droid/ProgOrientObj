#include "Transacoes.h"
#include <iostream>
#include "Cliente.h"


Transacoes::Transacoes(string tipo, string data, string horario, double valor): tipo(tipo), data(data), 
horario(horario), valor(valor), clientes(0){}
Transacoes::~Transacoes(){}
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
vector<Cliente*> Transacoes::getClientes() const{
    return this->clientes;
}
void Transacoes::setClientes(Cliente *cliente){
    this->clientes.push_back(cliente);
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
    for(int i =0; i<this->clientes.size(); i++)
        this->clientes[i]->exibirDados();

        
}
