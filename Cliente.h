#ifndef CLIENTE_H
#define CLIENTE_H
#include "Pessoa.h"
#include <vector>
#include <string>
class Transacoes; //avisa q existe pois ambos se referenciam
//temos q usar ponteiros p compilador aceitar
class Cliente: public Pessoa{
    //herda de pessoa os metodos publicos - nome, login etc
    private:
        double remuneracao;
        string tipoDeConta; //poupanca, corrente
        double taxaDeRendimento;
        double saldo;
        string gerente;
        vector<Transacoes*> transacoes;
        
        
    public:
        //override sobrescreve o metodo virtual de pessoa 
        void exibirDados() const override;
        Cliente(string = "", string ="", string ="", string ="", string = "", double = 0.0,
             string = "", double = 0.0, double =0.0);
        ~Cliente();

        void setTransacao(Transacoes* transacao);
        vector<Transacoes*> getExtrato() const;
        string getTipoDeConta() const;
        void setTipoDeConta(const string& tipo);
        double getRemuneracao() const;
        double getSaldo() const;
        string getGerente() const;
        void setGerente(const string &x);
        void setSaldo(double saldo);
        double getRendimento() const;
        void setRendimento(double rendimento);
        

};
#endif







