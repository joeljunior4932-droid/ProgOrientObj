#include "CartaoCredito.h"
#include "Cliente.h"
#include "Transacoes.h"
#include <iostream>


//construtor e destrutor
CartaoCredito::CartaoCredito(string numero, double limite, Cliente* cliente) :
    numero(numero), limite(limite), limiteUtilizado(0.0), bloqueado(false), cliente(cliente), compras(0) {}

CartaoCredito::~CartaoCredito() {
    for (int i = 0; i < (int)this->compras.size(); i++) {
        delete this->compras[i];
    }
    this->compras.clear();
}


//getters e setters
string CartaoCredito::getNumero() const { 
    return this->numero; 
}

double CartaoCredito::getLimite() const { 
    return this->limite; 
}

void CartaoCredito::setLimite(double novoLimite) { 
    this->limite = novoLimite; 
}

double CartaoCredito::getLimiteUtilizado() const { 
    return this->limiteUtilizado; 
}

void CartaoCredito::setLimiteUtilizado(double val) { 
    this->limiteUtilizado = val; 
}

bool CartaoCredito::getBloqueado() const { 
    return this->bloqueado; 
}

void CartaoCredito::setBloqueado(bool status) { 
    this->bloqueado = status; 
}

Cliente* CartaoCredito::getCliente() const { 
    return this->cliente; 
}

vector<CompraCartao*>& CartaoCredito::getCompras() { 
    return this->compras; 
}

bool CartaoCredito::realizarCompra(string descricao, double valor, int parcelas, string data) {
    if (this->bloqueado) {
        cout << "Erro: Cartão está bloqueado!" << endl; //caso exista um bloqueio no cartão, não é possível realizar compras
        return false;
    }
    if (parcelas < 1 || parcelas > 12) {
        cout << "Erro: Parcelamento inválido. Máximo de 12x." << endl; //validação no número de parcelas nas compras do cartão
        return false;
    }
    if (this->limiteUtilizado + valor > this->limite) {
        cout << "Erro: Limite insuficiente! Limite disponível: R$ " << (this->limite - this->limiteUtilizado) << endl;//validação do valor da compra
        return false;
    }

    CompraCartao* novaCompra = new CompraCartao(descricao, valor, parcelas, data);
    this->compras.push_back(novaCompra);
    this->limiteUtilizado += valor;
    
    cout << "Compra de R$ " << valor << " realizada com sucesso em " << parcelas << "x." << endl; //se chegou neste print, a compra funcionou
    return true;
}

void CartaoCredito::exibirFatura() const {
    //função com objetivo de mostrar a fatura do cartão, com os itens que foram comprados, e se o cartão está bloqueado ou não
    cout << "\n===== FATURA DO CARTÃO " << this->numero << " =====" << endl;
    cout << "Limite Total: R$ " << this->limite << endl;
    cout << "Limite Utilizado (Fatura atual): R$ " << this->limiteUtilizado << endl;
    cout << "Limite Disponível: R$ " << (this->limite - this->limiteUtilizado) << endl;
    cout << "Status: " << (this->bloqueado ? "BLOQUEADO" : "ATIVO") << endl;
    cout << "--- Compras Registradas ---" << endl;
    
    if (this->compras.empty()) {
        cout << "Nenhuma compra registrada." << endl;
    } else {
        for (int i = 0; i < (int)this->compras.size(); i++) {
            cout << "Data: " << this->compras[i]->getDataCompra() 
                 << " | Desc: " << this->compras[i]->getDescricao() 
                 << " | Valor: R$ " << this->compras[i]->getValorTotal() 
                 << " (" << this->compras[i]->getNumeroParcelas() << "x de R$ " 
                 << this->compras[i]->getValorParcela() << ")" << endl;
        }
    }
    cout << "====================================\n" << endl;
}

bool CartaoCredito::pagarFatura(double valorPago, string data, string horario) {
    if (valorPago <= 0) {
        cout << "Valor inválido." << endl; //caso o cliente tente pagar a fatura com valores negativos
        return false;
    }
    if (valorPago > this->limiteUtilizado) {
        //caso o cliente tente pagar a fatura com um valor maior que o real
        cout << "Aviso: Valor maior que a fatura atual. Ajustando para o valor da fatura (R$ " << this->limiteUtilizado << ")." << endl;
        valorPago = this->limiteUtilizado;
    }
    if (this->cliente->getSaldo() < valorPago) {
        //caso o cliente não tenha dinheiro para pagar a fatura
        cout << "Erro: Saldo insuficiente na conta bancária para pagar a fatura! Saldo: R$ " << this->cliente->getSaldo() << endl;
        return false;
    }

    // Debita do saldo da conta
    this->cliente->setSaldo(this->cliente->getSaldo() - valorPago);
    
    // Libera o limite do cartão
    this->limiteUtilizado -= valorPago;

    // Registra a transação de pagamento
    Transacoes* t = new Transacoes("pagamento_fatura", data, horario, valorPago);
    t->setClientes(this->cliente);
    this->cliente->setTransacao(t);

    cout << "Pagamento de R$ " << valorPago << " realizado com sucesso!" << endl;
    return true;
}