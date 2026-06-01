#include <iostream>
#include <vector>
#include "Cliente.h"
#include <algorithm>
#include "Gerente.h"
#include <string>
#include "Transacoes.h"
#include <fstream>
#include <sstream>
#include "CartaoCredito.h"
using namespace std;

void printMenu(){
    cout << "====== SISTEMA DE GERENCIAMENTO DE BANCO ======\n";
    cout << "1. Cadastrar cliente\n";
    cout << "2. Cadastrar gerente\n";
    cout << "3. Criar transação\n";
    cout << "4. Exibir extrato de um cliente\n";
    cout << "5. Associar gerente a cliente\n";
    cout << "6. Listar clientes\n";
    cout << "7. Listar gerentes\n";
    cout << "8. Salvar dados e sair\n";
    cout << "9. Gerenciar Cartões de Crédito\n";
    cout << "==================================================" << endl <<
            "Escolha uma opção: ";
}

Cliente* buscarClientePorNome(vector<Cliente*>& clientes, const string& nome) {
    //percorre o vetor e retorna o primeiro cliente com aquele nome
    //auto é como se fosse um for q percorre todas as posicoes do vetor 
    for (auto c : clientes)
        if (c->nome == nome)
            return c;
    return nullptr;
}

Gerente* buscarGerentePorNome(vector<Gerente*>& gerentes, const string& nome) {
    for (auto g : gerentes)
        if (g->nome == nome)
            return g;
    return nullptr;
}

//carregar os arquivos de cliente e gerente e reconstroi os objetos na memoria
//nome,login,senha,trabalho,dataNasc,remuneracao,tipoConta,rendimento,
//saldo,gerente,qtdTransacoes,[tipo,data,horario,valor,nomeRecebedor,]

void carregarClientes(vector<Cliente*>& clientes, const string& arquivo) {
   //le o arquivo inteiro e reconstroi os objetos na memoria  
    ifstream arq(arquivo); //abre o arquivo para leitura 
    if (!arq) return; // arquivo ainda nao existe, ignora

    string linha;
    while (getline(arq, linha)) {
        if (linha.empty()) continue; //pula a linha se ela for vazia, caso haja uma linha em branco no final do arq 
        stringstream ss(linha); //transforma uma string em um stream para usarmos getLine nela 

        string nome, login, senha, trabalho, dataNasc;
        string remuneracaoStr, tipoConta, rendimentoStr, saldoStr, gerente, qtdStr;

        getline(ss, nome,          ',');
        getline(ss, login,         ',');
        getline(ss, senha,         ',');
        getline(ss, trabalho,      ',');
        getline(ss, dataNasc,      ',');
        getline(ss, remuneracaoStr,',');
        getline(ss, tipoConta,     ',');
        getline(ss, rendimentoStr, ',');
        getline(ss, saldoStr,      ',');
        getline(ss, gerente,       ',');
        getline(ss, qtdStr,        ',');

        double remuneracao = stod(remuneracaoStr);
        double rendimento  = stod(rendimentoStr);
        double saldo       = stod(saldoStr);
        int    qtd         = stoi(qtdStr);

        Cliente* c = new Cliente(nome,login, senha, trabalho, dataNasc,
        remuneracao, tipoConta, rendimento, saldo);
        c->setGerente(gerente);

        // Le as transacoes registradas para este cliente
        for (int i = 0; i < qtd; i++) {
            string tipo, data, horario, valorStr, nomeRecebedor, nomeTransferidor;
            getline(ss, tipo,          ',');
            getline(ss, data,          ',');
            getline(ss, horario,       ',');
            getline(ss, valorStr,      ',');
            getline(ss, nomeRecebedor, ',');
            getline(ss, nomeTransferidor, ',');

            double valor = stod(valorStr);
            Transacoes* t = new Transacoes(tipo, data, horario, valor);
            Cliente *receb = new Cliente(nomeRecebedor);
            Cliente *transf = new Cliente(nomeTransferidor);
            t->setClientes(receb);
            t->setClientes(transf);
            c->setTransacao(t);
        }
        clientes.push_back(c); //add o cliente ao final do vetor 
    }
}

void carregarGerentes(vector<Gerente*>& gerentes, const string& arquivo) {
    ifstream arq(arquivo);
    if (!arq) return;

    string linha;
    while (getline(arq, linha)) {
        if (linha.empty()) continue;
        stringstream ss(linha);

        string nome, login, senha, trabalho, dataNasc, qtdStr;
        getline(ss, nome,    ',');
        getline(ss, login,   ',');
        getline(ss, senha,   ',');
        getline(ss, trabalho,',');
        getline(ss, dataNasc,',');
        getline(ss, qtdStr,  ',');

        int qtd = stoi(qtdStr);

        Gerente* g = new Gerente(login, senha, nome,trabalho, dataNasc);

        // Os clientes vinculados sao guardados apenas por referencia no CSV;
        // a vinculacao completa em memoria depende de carregarClientes ter rodado antes.
        // Por ora apenas avanca os campos para nao corromper a leitura.
        for (int i = 0; i < qtd; i++) {
            string nomeCli, loginCli, tipoConta;
            getline(ss, nomeCli,  ',');
            getline(ss, loginCli, ',');
            getline(ss, tipoConta,',');
            Cliente *p = new Cliente(nomeCli, loginCli, "", "", "", 0,
             tipoConta);
            g->setClientes(p);
        }

        gerentes.push_back(g);
    }
}

void salvarClientes(vector<Cliente*>& clientes, const string& arquivo) {
    ofstream arq(arquivo); // abre o arquivo para escrita, apagando td q tinha antes 
    //pois reescrevemos c o estado atual da memoria 
    if (!arq) {
        cout << "Erro ao abrir " << arquivo << " para escrita." << endl;
        return;
    }

    for (auto c : clientes) {
        //percorre os clientes e escreve os dados no csv 
        vector<Transacoes*> extrato = c->getExtrato();
        // Campos basicos do cliente
        arq << c->nome << ',' << c->getLogin() << ',' << c->getSenha() << ',' << c->trabalho << ','
            << c->dataNasc  << ',' << c->getRemuneracao()<< ',' << c->getTipoDeConta() << ','
            << c->getRendimento() << ',' << c->getSaldo() << ',' << c->getGerente() << ','
            << extrato.size() << ',';

        // Uma transacao por vez; cada uma ocupa 5 campos
        for (int i = 0; i < (int)extrato.size(); i++) {
            Transacoes* t = extrato[i];
            
            string nomeRecebedor;
            string nomeTransferidor = "";
            if (!t->getClientes().empty()){
                nomeRecebedor = t->getClientes()[0]->nome;
                if(t->getTipo() == "transferencia")
                    nomeTransferidor = t->getClientes()[1]->nome;
            }

            arq << t->getTipo() << ',' << t->getData()    << ',' << t->getHorario() << ','
            << t->getValor()   << ',' << nomeRecebedor   << ',' << nomeTransferidor << ',';
        }
        arq << '\n';
    }
}

void salvarGerentes(vector<Gerente*>& gerentes, const string& arquivo) {
    ofstream arq(arquivo);
    if (!arq) {
        cout << "Erro ao abrir " << arquivo << " para escrita." << endl;
        return;
    }

    for (auto g : gerentes) {
        vector<Cliente*> clientes = g->getClientes();

        arq << g->nome << ',' << g->getLogin() << ',' << g->getSenha() << ','
            << g->trabalho << ',' << g->dataNasc << ',' << clientes.size() << ',';

        for (int i = 0; i < (int)clientes.size(); i++) {
            arq << clientes[i]->nome << ',' << clientes[i]->getLogin() << ',' << clientes[i]->getTipoDeConta()<< ',';
        }
        arq << '\n';
    }
}

Gerente* cadastrarGerente(vector<Gerente*>& gerente){ 
    cout << "Digite o nome do gerente que deseja cadastrar: ";
    string nome;
    cin.ignore(); //ignora o \n que ficou no buffer para o getline n ler errado
    getline(cin, nome); //le o nome completo com espacos 
    if (buscarGerentePorNome(gerente, nome)) {
        cout << "Gerente " << nome << " já está cadastrado!" << endl;
        return nullptr;
    }
    cout<< "Digite o login: ";
    string login, senha;

    cin >> login;
    cout << "Digite a senha: ";
    cin >> senha;
    cout << "Digite a data de nascimento(DD:MM:ANO): "; 
    string data;
    cin >> data;

    Gerente* x = new Gerente(login, senha, nome, "Gerente", data);
    return x;
}

void associaGC(vector<Cliente*>& clientes, vector<Gerente*>& gerentes){
    string nomCli, nomGer;
    cout << "Digite o nome do cliente e do gerente: ";
    cin >> nomCli >> nomGer;
    Cliente* c = buscarClientePorNome(clientes, nomCli);
    Gerente* g = buscarGerentePorNome(gerentes, nomGer);
    //busca nos vetores e sai se nao existir, se existir chamamos os setters
    if (!c) {
        cout << "Cliente não encontrado" << endl;
        return;
    }
    if (!g) {
        cout << "Gerente não encontrado" << endl;
        return;
    }
    if(c->getGerente() == ""){
        c->setGerente(nomGer); //cli guarda o nome de ger
        g->setClientes(c); //ger gaurda o ponteiro p cli
        cout << "Associação realizada: " << nomCli << "->" << nomGer << endl;
    }
    else{
        if(c->getGerente() == g->nome){
            cout << "esse cliente ja esta associado a esse gerente!!" << endl;
            return;
        }
        else{
            Gerente* antigo = buscarGerentePorNome(gerentes, c->getGerente());
            for (auto it = antigo->getClientes().begin(); it != antigo->getClientes().end(); ++it) {
                if ((*it)->nome == c->nome) {
                    antigo->getClientes().erase(it);        // remove o ponteiro do vetor
                    break;                     // sai do loop
                }
            }
            c->setGerente(nomGer); //cli guarda o nome de ger
            g->setClientes(c); //ger gaurda o ponteiro p cli
            cout << "Associação realizada: " << nomCli << "->" << nomGer << endl;
        }    
    }
}

Transacoes* transacaoBanco(Cliente* client1, double saldo){
    //nullptr é so usado em transferencias, é um parametro opcional
    cout << "Inserção de dados do deposito: "<< endl;
    
    string data, horario;
    cout << "Data(DD/MM/ANO): ";
    cin >> data;
    cout << "Horário(HH:MM:SS): ";
    cin >> horario;
    
    Transacoes *t = new Transacoes("deposito", data, horario, saldo);
    //add os clientes envolvidos na transacao 
    t->setClientes(client1);
    return t; 
}

Cliente* cadastrarCliente(vector<Cliente*>& clientes){
    cout << "--------Inserção de dados do Cliente---------" << endl;
    cout << "Digite o nome do cliente: "; 
    string nome;
    cin.ignore();
    getline(cin, nome);
    if (buscarClientePorNome(clientes, nome)) {
        cout << "Cliente " << nome << " já está cadastrado!" << endl;
        return nullptr;
    }
    cout << "Digite a data de nascimento(DD:MM:ANO): "; 
    string data;
    cin >> data;
    cout << "Digite o trabalho: "; 
    string trabalho;
    cin >> trabalho;
    cout << "Digite a remuneração: "; 
    double remuneracao;
    cin>> remuneracao;
    cout << "Digite o tipo de conta: "; 
    string tipoConta;
    cin >> tipoConta;
    double saldo;
    bool temTransacoes;
    while(1){
        cout << "Você quer fazer uma transação para o banco: (sim/nao): ";
        string option;
        cin>>option;

        transform(option.begin(), option.end(), option.begin(), ::tolower);
        if(option == "sim"){
            temTransacoes = true;
            cout << "Qual valor de saldo voce quer ter: ";
            cin >> saldo; 
            break;
        }
        else if(option == "nao"){
            temTransacoes = false;
            saldo = 0;
            break;
        }
        cout << "Opção invalida" << endl;

    }

    transform(tipoConta.begin(), tipoConta.end(), tipoConta.begin(), ::tolower);
    double taxaRendimento = 0;
    if(tipoConta == "poupança"){
        if(remuneracao < 5000)
            taxaRendimento = 0.05;
        else if(remuneracao >= 5000){
            taxaRendimento = 0.10;

        }
    }

    cout << "Digite o login: ";
    string login, senha;
    cin >> login;
    cout << "Digite a senha: ";
    cin >> senha;
    Cliente *x = new Cliente(nome, login, senha, trabalho, data, remuneracao, tipoConta, taxaRendimento, saldo);
    //cria o obj e add a transacao inicial se tiver
    if(temTransacoes)
        x->setTransacao(transacaoBanco(x, saldo));

    return x;  
}

void carregarCartoes(vector<Cliente*>& clientes, const string& arquivo) {
    ifstream arq(arquivo);
    if (!arq) return;

    string linha;
    while (getline(arq, linha)) {
        if (linha.empty()) continue;
        stringstream ss(linha);

        string nomeCli, numero, limiteStr, utilStr, bloqStr, qtdStr;
        getline(ss, nomeCli, ',');
        getline(ss, numero, ',');
        getline(ss, limiteStr, ',');
        getline(ss, utilStr, ',');
        getline(ss, bloqStr, ',');
        getline(ss, qtdStr, ',');

        Cliente* c = buscarClientePorNome(clientes, nomeCli);
        if (!c) continue; // Cliente não existe mais

        double limite = stod(limiteStr);
        double util = stod(utilStr);
        bool bloqueado = (bloqStr == "1");
        int qtd = stoi(qtdStr);

        CartaoCredito* cartao = new CartaoCredito(numero, limite, c);
        cartao->setLimiteUtilizado(util);
        cartao->setBloqueado(bloqueado);

        for (int i = 0; i < qtd; i++) {
            string desc, vTotalStr, nParcStr, pPagasStr, dataC;
            getline(ss, desc, ',');
            getline(ss, vTotalStr, ',');
            getline(ss, nParcStr, ',');
            getline(ss, pPagasStr, ',');
            getline(ss, dataC, ',');

            CompraCartao* compra = new CompraCartao(desc, stod(vTotalStr), stoi(nParcStr), dataC);
            compra->setParcelasPagas(stoi(pPagasStr));
            cartao->getCompras().push_back(compra);
        }
        c->setCartao(cartao);
    }
}

void salvarCartoes(vector<Cliente*>& clientes, const string& arquivo) {
    ofstream arq(arquivo);
    if (!arq) return;

    for (auto c : clientes) {
        CartaoCredito* cartao = c->getCartao();
        if (cartao) {
            vector<CompraCartao*>& compras = cartao->getCompras();
            arq << c->nome << ',' << cartao->getNumero() << ',' << cartao->getLimite() << ','
                << cartao->getLimiteUtilizado() << ',' << (cartao->getBloqueado() ? "1" : "0") << ','
                << compras.size() << ',';

            for (auto cmp : compras) {
                arq << cmp->getDescricao() << ',' << cmp->getValorTotal() << ','
                    << cmp->getNumeroParcelas() << ',' << cmp->getParcelasPagas() << ','
                    << cmp->getDataCompra() << ',';
            }
            arq << '\n';
        }
    }
}

void menuCartao(vector<Cliente*>& clientes, vector<Gerente*>& gerentes) {
    //função para o submenu do cartão de crédito
    while (true) {
        cout << "\n--- SUB-MENU: CARTÕES DE CRÉDITO ---" << endl;
        cout << "1. Criar cartão para um cliente" << endl;
        cout << "2. Alterar limite do cartão" << endl;
        cout << "3. Realizar compra parcelada" << endl;
        cout << "4. Bloquear/Desbloquear cartão" << endl;
        cout << "5. Visualizar fatura" << endl;
        cout << "6. Pagar fatura" << endl;
        cout << "7. Voltar" << endl;
        cout << "Escolha: ";
        
        int opc;
        if (!(cin >> opc)) { cin.clear(); cin.ignore(1000, '\n'); continue; }

        if (opc == 7) break;

        if (opc == 1) { // Criar o cartão
            string nGer, nCli;
            cout << "Nome do Gerente: "; cin >> nGer;
            cout << "Nome do Cliente: "; cin >> nCli;
            
            Gerente* g = buscarGerentePorNome(gerentes, nGer);
            Cliente* c = buscarClientePorNome(clientes, nCli);
            
            if (!g || !c) { 
                cout << "Gerente ou Cliente não encontrado." << endl; 
                continue; 
            }

            if (c->getGerente() != g->nome) { 
                cout << "Este cliente não pertence a este gerente." << endl; 
                continue; 
            }

            if (c->getCartao() != nullptr) { 
                cout << "Cliente já possui um cartão." << endl;
                continue; 
            }

            /*
            criação do limite inicial do cartão cmo base na remuneração do cliente, sendo que se a remuneração for menor que 2000,00, o limite será de 500,00
            se a remunareção for maior ou igual a 2000 e menor ou igual a 5000, o limite será de 2000,00, e se a remuneração for maior do que 5000, o limite
            será de 5000,00.
            */
            double rem = c->getRemuneracao();
            double limite = 0;
            if (rem < 2000) limite = 500;
            else if (rem >= 2000 && rem <= 5000) limite = 2000;
            else limite = 5000;

            string numGerado = "CC-" + to_string(rand() % 10000 + 1000);
            CartaoCredito* novoCartao = new CartaoCredito(numGerado, limite, c);
            c->setCartao(novoCartao);
            
            cout << "Cartão criado! Número: " << numGerado << " | Limite: R$ " << limite << endl;
            
        } else if (opc == 2) { // Alterar Limite
            string nGer, nCli;
            cout << "Nome do Gerente: "; cin >> nGer;
            cout << "Nome do Cliente: "; cin >> nCli;
            Gerente* g = buscarGerentePorNome(gerentes, nGer);
            Cliente* c = buscarClientePorNome(clientes, nCli);
            
            if (!g || !c || c->getGerente() != g->nome || !c->getCartao()) {
                cout << "Dados inválidos ou cliente sem cartão/associação." << endl; continue;
            }
            
            cout << "Novo limite (Mínimo R$ 100): ";
            double novoL; cin >> novoL;
            if (novoL < 100) { cout << "O limite não pode ser menor que 100." << endl; continue; }
            
            c->getCartao()->setLimite(novoL);
            cout << "Limite alterado com sucesso." << endl;

        } else if (opc == 3) { // Compra
            string nCli;
            cout << "Nome do Cliente: "; cin >> nCli;
            Cliente* c = buscarClientePorNome(clientes, nCli);
            if (!c || !c->getCartao()) { cout << "Cliente não encontrado ou não possui cartão." << endl; continue; }

            string desc, data;
            double valor; int parc;
            cout << "Descrição da compra: "; cin >> desc;
            cout << "Valor total: "; cin >> valor;
            cout << "Número de parcelas (1 a 12): "; cin >> parc;
            cout << "Data (DD/MM/ANO): "; cin >> data;

            c->getCartao()->realizarCompra(desc, valor, parc, data);

        } else if (opc == 4) { // Bloqueio
            string nCli, perfil;
            cout << "Nome do Cliente: "; cin >> nCli;
            cout << "Quem está solicitando (gerente/cliente)? "; cin >> perfil;
            Cliente* c = buscarClientePorNome(clientes, nCli);
            if (!c || !c->getCartao()) { cout << "Cliente inválido." << endl; continue; }

            if (perfil == "cliente") {
                c->getCartao()->setBloqueado(true);
                cout << "Cartão bloqueado (Desbloqueio apenas com gerente)." << endl;
            } else if (perfil == "gerente") {
                int acao;
                cout << "1 - Bloquear | 2 - Desbloquear: "; cin >> acao;
                c->getCartao()->setBloqueado(acao == 1);
                cout << "Ação de bloqueio/desbloqueio concluída." << endl;
            } else {
                cout << "Perfil inválido." << endl;
            }

        } else if (opc == 5) { // Visualizar Fatura
            string nCli;
            cout << "Nome do Cliente: "; cin >> nCli;
            Cliente* c = buscarClientePorNome(clientes, nCli);
            if (!c || !c->getCartao()) { cout << "Inválido." << endl; continue; }
            
            c->getCartao()->exibirFatura();

        } else if (opc == 6) { // Pagar Fatura
            string nCli, data, horario;
            double valor;
            cout << "Nome do Cliente: "; cin >> nCli;
            Cliente* c = buscarClientePorNome(clientes, nCli);
            if (!c || !c->getCartao()) { cout << "Inválido." << endl; continue; }

            if (c->getCartao()->getLimiteUtilizado() <= 0) {
                cout << "Não há valores a serem pagos na fatura no momento." << endl;
                continue; // Retorna ao sub-menu sem pedir os dados de pagamento
            }

            cout << "Sua fatura atual é R$ " << c->getCartao()->getLimiteUtilizado() << endl;
            cout << "Valor a pagar: "; cin >> valor;
            cout << "Data: "; cin >> data;
            cout << "Horário: "; cin >> horario;

            c->getCartao()->pagarFatura(valor, data, horario);
        }
    }
}

