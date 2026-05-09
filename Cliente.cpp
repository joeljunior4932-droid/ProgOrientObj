#include "Transacoes.h"
#include "Cliente.h"
#include <iostream>
void Cliente::exibirDados() const{
    Pessoa::exibirDados();
    cout << this->remuneracao;
    cout << this->tipoDeConta;
    cout << this->taxaDeRendimento;
    cout << this->saldo;
    for(int i =0; i<this->transacoes.size(); i++)
        this->transacoes[i]->exibirTransacoes();
}
Cliente::Cliente(string login, string senha, string nome, string trabalho, double remuneracao,
             string tipoDeConta, double taxaDeRendimento , double saldo): 
             Pessoa(login, senha, nome, trabalho), remuneracao(remuneracao), tipoDeConta(tipoDeConta), 
             taxaDeRendimento(taxaDeRendimento), saldo(saldo), transacoes(0){}
Cliente::~Cliente(){}

void Cliente::setTransacao(Transacoes* transacao){
    this->transacoes.push_back(transacao);
}
vector<Transacoes*> Cliente::getExtrato() const{
    return this->transacoes;
}
string Cliente::getTipoDeConta() const{
    return this->tipoDeConta;
}
void Cliente::setTipoDeConta(const string& tipo){
    this->tipoDeConta = tipo;
}
double Cliente::getSaldo() const{
    return this->saldo;
}
void Cliente::setSaldo(double saldo){
    this->saldo = saldo;
}
double Cliente::getRendimento() const{
    return this->taxaDeRendimento;
}
void Cliente::setRendimento(double rendimento){
    this->taxaDeRendimento = rendimento;
}
