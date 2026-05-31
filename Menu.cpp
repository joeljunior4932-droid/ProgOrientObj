#include <iostream>
#include <vector>
#include "Cliente.h"
#include <algorithm>
#include "Gerente.h"
#include <string>
#include "Transacoes.h"
#include <fstream>
#include <sstream>
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
            string tipo, data, horario, valorStr, nomeRecebedor;
            getline(ss, tipo,          ',');
            getline(ss, data,          ',');
            getline(ss, horario,       ',');
            getline(ss, valorStr,      ',');
            getline(ss, nomeRecebedor, ',');

            double valor = stod(valorStr);
            Transacoes* t = new Transacoes(tipo, data, horario, valor);
            t->setClientes(c);
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

        Gerente* g = new Gerente(login, senha, nome, dataNasc, trabalho);

        // Os clientes vinculados sao guardados apenas por referencia no CSV;
        // a vinculacao completa em memoria depende de carregarClientes ter rodado antes.
        // Por ora apenas avanca os campos para nao corromper a leitura.
        for (int i = 0; i < qtd; i++) {
            string nomeCli, loginCli, tipoConta;
            getline(ss, nomeCli,  ',');
            getline(ss, loginCli, ',');
            getline(ss, tipoConta,',');
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
            // Nome do recebedor: primeiro cliente diferente do proprio ou o proprio se so houver um
            string nomeRecebedor = "";
            if (!t->getClientes().empty())
                nomeRecebedor = t->getClientes()[0]->nome;

            arq << t->getTipo() << ',' << t->getData()    << ',' << t->getHorario() << ','
            << t->getValor()   << ',' << nomeRecebedor   << ',';
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

    c->setGerente(nomGer); //cli guarda o nome de ger
    g->setClientes(c); //ger gaurda o ponteiro p cli
    cout << "Associação realizada: " << nomCli << "->" << nomGer << endl;

}

Transacoes* criarTransacao(Cliente* client1, Cliente* client2 = nullptr){
    //nullptr é so usado em transferencias, é um parametro opcional
    cout << "Inserção de dados da transações: "<< endl;
    cout << "Tipo: ";
    string tipo, data, horario;
    cin >> tipo;
    transform(tipo.begin(), tipo.end(), tipo.begin(), ::tolower); //garante q maiusc e minusc sejam tradados igualmente 
    cout << "Data(DD/MM/ANO): ";
    cin >> data;
    cout << "Horário(HH:MM:SS): ";
    cin >> horario;
    cout << "Valor da transação: ";
    double valor;
    cin >> valor;
    
    Transacoes *t = new Transacoes(tipo, data, horario, valor);
    //add os clientes envolvidos na transacao 
    t->setClientes(client1);
    if (client2 != nullptr) {
        t->setClientes(client2);
    }
    return t; 
}

Cliente* cadastrarCliente(vector<Cliente*>& clientes){
    cout << "--------Inserção de dados do Cliente---------" << endl;
    cout << "Digite o nome do cliente: "; 
    string nome;
    cin.ignore();
    getline(cin, nome);
    if (buscarClientePorNome(clientes, nome)) {
        cout << "Cliente" << nome << "já está cadastrado!" << endl;
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
        cout << "Você quer fazer uma transação para o banco: (sim/nao)";
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
    Cliente *x = new Cliente(nome, login, senha, trabalho, data, remuneracao, tipoConta.data(), taxaRendimento, saldo);
    //cria o obj e add a transacao inicial se tiver
    if(temTransacoes)
        x->setTransacao(criarTransacao(x));

    return x;  
}

