#ifndef CARTAOCREDITO_H
#define CARTAOCREDITO_H
#include <string>
#include <vector>
#include "CompraCartao.h"

class Cliente; // Declaração antecipada (Forward declaration) para evitar dependência circular

using namespace std;

class CartaoCredito {
private:
    string numero;
    double limite;
    double limiteUtilizado;
    bool bloqueado;
    Cliente* cliente;
    vector<CompraCartao*> compras;

public:
    CartaoCredito(string numero, double limite, Cliente* cliente);
    ~CartaoCredito();

    string getNumero() const;
    double getLimite() const;
    void setLimite(double novoLimite);
    double getLimiteUtilizado() const;
    void setLimiteUtilizado(double val);
    bool getBloqueado() const;
    void setBloqueado(bool status);
    Cliente* getCliente() const;
    vector<CompraCartao*>& getCompras();

    bool realizarCompra(string descricao, double valor, int parcelas, string data);
    void exibirFatura() const;
    bool pagarFatura(double valorPago, string data, string horario);
};

#endif