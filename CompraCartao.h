#ifndef COMPRACARTAO_H
#define COMPRACARTAO_H
#include <string>

using namespace std;

class CompraCartao {
private:
    string descricao;
    double valorTotal;
    int numeroParcelas;
    int parcelasPagas;
    double valorParcela;
    string dataCompra;

public:
    CompraCartao(string descricao, double valorTotal, int numeroParcelas, string dataCompra);
    ~CompraCartao();

    string getDescricao() const;
    double getValorTotal() const;
    int getNumeroParcelas() const;
    int getParcelasPagas() const;
    double getValorParcela() const;
    string getDataCompra() const;

    void setParcelasPagas(int qtd);
};

#endif