#ifndef CLIENTE_H
#define CLIENTE_H
#include "Pessoa.h"
#include <vector>
class Transacoes;
class Cliente: public Pessoa{

    private:
        double remuneracao;
        string tipoDeConta;
        double taxaDeRendimento;
        double saldo;
        vector<Transacoes*> transacoes;
        

    public:
        void exibirDados() const override;
        Cliente(string = "", string ="", string ="", string ="", double = 0.0,
             string = "", double = 0.0, double =0.0);
        ~Cliente(){}

        void setTransacao(Transacoes* transacao);
        vector<Transacoes*> getExtrato() const;
        string getTipoDeConta() const;
        void setTipoDeConta(const string& tipo);
        double getSaldo() const;
        void setSaldo(double saldo);
        double getRendimento() const;
        void setRendimento(double rendimento);
        

};
#endif







