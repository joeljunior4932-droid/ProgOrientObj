#include "Transacoes.h"
#include "Cliente.h"
#include <iostream>

void Cliente::exibirDados() const{
    Pessoa::exibirDados(); //exibimos o da pessoa primeiro p dps imprimir os especificos 
    cout << "remuneracao: " << this->remuneracao << endl;
    cout << "tipo de conta: " << this->tipoDeConta << endl;
    cout << "taxa de rendimento da conta: " << this->taxaDeRendimento << endl;
    cout << "saldo da conta: " << this->saldo << endl; 
    for(int i =0; i<(int)this->transacoes.size(); i++)
        this->transacoes[i]->exibirTransacoes();
}
Cliente::Cliente(string nome, string login, string senha, string trabalho, string dataNasc, double remuneracao,
             string tipoDeConta, double taxaDeRendimento , double saldo): 
             Pessoa( login, senha, nome, trabalho, dataNasc), remuneracao(remuneracao), tipoDeConta(tipoDeConta), 
             taxaDeRendimento(taxaDeRendimento), saldo(saldo), gerente(), transacoes(0), cartao(nullptr) {}

Cliente::~Cliente(){
    for (int i = 0; i < (int)this->transacoes.size(); i++) {
        delete this->transacoes[i];
        //cliente recebeu as transacoes e é resp por deletar qdo 'morrer'
        //em transacoes, fazemos o clear - so esquece o ponteiro, n o deleta, so aponta p ele nao criou ele 
    }
    if(this->cartao != nullptr){
        delete this->cartao;
    }
}

void Cliente::setTransacao(Transacoes* transacao){
    this->transacoes.push_back(transacao);
    //add uma transacao ao final do vetor de transacoes do cliente 
    //chamamos td vez q uma transacao eh criada cffc
}

vector<Transacoes*>& Cliente::getExtrato(){
    return this->transacoes;
}

string Cliente::getTipoDeConta() const{
    return this->tipoDeConta;
}

double Cliente::getRemuneracao() const{
    return this->remuneracao;
}

void Cliente::setTipoDeConta(const string& tipo){
    this->tipoDeConta = tipo;
}

double Cliente::getSaldo() const{
    return this->saldo;
}

string Cliente::getGerente() const{
    return this->gerente;
}

void Cliente::setGerente(const string &x){
    this->gerente = x;
}

void Cliente::setSaldo(double saldo){
    this->saldo = saldo;
}
bool Cliente::operator<(const Cliente& x) const{
    return this->nome < x.nome;
}

double Cliente::getRendimento() const{
    return this->taxaDeRendimento;
}

void Cliente::setRendimento(double rendimento){
    this->taxaDeRendimento = rendimento;
}

CartaoCredito* Cliente::getCartao() const {
    return this->cartao;
}

void Cliente::setCartao(CartaoCredito* c) {
    this->cartao = c;
}