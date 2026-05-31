#include "Pessoa.h"
#include <iostream>

Pessoa::Pessoa(string login, string senha, string nome, string trabalho, string dataNasc): 
login(login), senha(senha), nome(nome), trabalho(trabalho), dataNasc(dataNasc){}

Pessoa::~Pessoa(){}  

//metodos
void Pessoa::exibirDados() const{
    cout << "Login: " << this->login << endl;
    cout << "Nome: " << this->nome << endl;
    cout << "Trabalho: " << this->trabalho << endl;
}

//geterres e seterres essenciais
void Pessoa::setLogin(const string& login){
    this->login = login;
}
string Pessoa::getLogin() const{
    return this->login;
}
//& referencia a string, com const nao altera a original
void Pessoa::setSenha(const string& senha){
    this->senha = senha;
}
string Pessoa::getSenha() const{
    return this->senha;
}
        