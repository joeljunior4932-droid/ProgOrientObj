#include "CompraCartao.h"

CompraCartao::CompraCartao(string descricao, double valorTotal, int numeroParcelas, string dataCompra) :
    descricao(descricao), valorTotal(valorTotal), numeroParcelas(numeroParcelas), 
    parcelasPagas(0), dataCompra(dataCompra) {
    
    if (numeroParcelas > 0) {
        this->valorParcela = valorTotal / numeroParcelas;
    } else {
        this->valorParcela = valorTotal;
    }
}

CompraCartao::~CompraCartao() {}

string CompraCartao::getDescricao() const { 
    return this->descricao; 
}

double CompraCartao::getValorTotal() const { 
    return this->valorTotal; 
}

int CompraCartao::getNumeroParcelas() const { 
    return this->numeroParcelas; 
}

int CompraCartao::getParcelasPagas() const { 
    return this->parcelasPagas; 
}

double CompraCartao::getValorParcela() const { 
    return this->valorParcela; 
}

string CompraCartao::getDataCompra() const { 
    return this->dataCompra; 
}

void CompraCartao::setParcelasPagas(int qtd) {
    this->parcelasPagas = qtd;
}