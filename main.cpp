#include "Menu.h"
#include "Transacoes.h"
#include <iostream>
#include <algorithm>
using namespace std; 

int main(){
    vector<Cliente *> cliente;
    vector<Gerente *> gerente;
    //cria os vetores vazios

    carregarClientes(cliente, "clientes.csv");
    carregarGerentes(gerente, "gerentes.csv");
    //carrega os dados salvos nos arquivos - se n existir, comeca vazio mesmo
    sort(cliente.begin(), cliente.end());
    sort(gerente.begin(), gerente.end());
    while (1) {
        printMenu();
        int opcao;
        if (!(cin >> opcao)) {
            cout << "Voce nao digitou um numero!" << endl;

            cin.clear();              // limpa o estado de erro
            cin.ignore(1000, '\n');   // descarta a entrada errada

            continue;                 // volta para o início do while
        }
        if (opcao < 1 || opcao > 8) {
            cout << "Opção invalida! Deve ser de 1 a 8. " << endl;
            continue;
        }
        switch (opcao)
        {
            case 1:{
                Cliente* c = cadastrarCliente(cliente);
                if (c){
                    cliente.push_back(c); //add no vetor de clientes c as informacoes 
                    cout << "Cliente cadastrado com sucesso!" << endl;
                } //se n da erro de cliente ja cadastrado
                break;
            }
            case 2: {
                Gerente* g = cadastrarGerente(gerente);
                if (g){
                    gerente.push_back(g);
                    cout << "Gerente cadastrado com sucesso!" << endl;
                }
                break;
            }
            case 3:{
                string resp, nome, data, horario;
                cout << "Qual tipo de transação você deseja fazer(deposito, saque, transferencia)? ";
                cin >> resp;
                double valor;
                if (resp == "deposito") {
                
                    cout << "Nome do cliente que está realizando o deposito: ";
                    
                    cin >> nome; 
                
                    Cliente* c = buscarClientePorNome(cliente, nome);
              
                    if (!c) {
                        cout << "Cliente nao encontrado" << endl;
                        break;
                    }
                    cout << "Valor do depósito: ";
                    cin >> valor;

                    cout << "Data do depósito: ";
                    cin >> data;
                    cout << "Horario do depósito: ";
                    cin >> horario;

                    c->setSaldo(c->getSaldo() + valor);
                    Transacoes* t = new Transacoes(resp, data, horario, valor);
                    t->setClientes(c);
                    c->setTransacao(t);
                    cout << "Depósito realizado. Saldo de " << c->nome << " atualizado para: " << c->getSaldo() << endl;
                }
                else if (resp == "saque") {
         
                    cout << "Nome do cliente: ";
                    cin >> nome; 
                    Cliente* c = buscarClientePorNome(cliente, nome);
                    if (!c) {
                        cout << "Cliente não encontrado" << endl;
                        break;
                    }
                    cout << "Valor do saque: ";
                    cin >> valor;
                    if (valor > c->getSaldo()) {
                        cout << "Saldo insuficiente. O cliente tem : R$ "<< c->getSaldo() << endl;
                        break;
                    }
                    cout << "Data do saque: ";
                    cin >> data;
                    cout << "Horario do saque: ";
                    cin >> horario;

                    c->setSaldo(c->getSaldo() - valor);
                    Transacoes* t = new Transacoes(resp, data, horario, valor);
                    t->setClientes(c);
                    c->setTransacao(t);
                    cout << "Saque realizado. Saldo atualizado para: " << c->getSaldo() << endl;
                
                } else if (resp == "transferencia"){
                    cout << "Nome do cliente que vai transferir: ";
                    cin >> nome; 
                    Cliente* c = buscarClientePorNome(cliente, nome);
                    if (!c) {
                        cout << "Cliente de origem não encontrado" << endl;
                        break;
                    }

                    cout << "Nome do cliente que vai receber: ";
                    string loginRecebedor;
                    cin >> loginRecebedor; 
                    Cliente* recebedor = buscarClientePorNome(cliente, loginRecebedor);
                    if (!recebedor) {
                        cout << "Cliente de destino não encontrado" << endl;
                        break;
                    }

                    cout << "Valor da transferencia: ";
                    cin >> valor;
                    if (valor > c->getSaldo()) {
                        cout << "saldo insuficiente. O cliente tem: R$" << c->getSaldo() << endl;
                        break;
                    }
                    cout << "Data da trasnferencia:";
                    cin >> data;
                    cout << "Horario da trasnferencia: ";
                    cin >> horario;

                    c->setSaldo(c->getSaldo() - valor);
                    recebedor->setSaldo(recebedor->getSaldo() + valor);

                    //registrar transacao em ambos 
                    Transacoes* t1 = new Transacoes(resp, data, horario, valor);
                    t1->setClientes(recebedor);
                    t1->setClientes(c);
                    
                    c->setTransacao(t1);

                    Transacoes* t2 = new Transacoes(resp, data, horario, valor);
                    t2->setClientes(recebedor);
                    t2->setClientes(c);
                    
                    recebedor->setTransacao(t2);
                    //cria 2 transacoes, 1 pra cada cliente 

                    cout << "Transferencia realizada." << "Saldo de " << c->nome << " é : " << c->getSaldo() << endl;
                    cout << "Saldo de: " << recebedor->nome << " é: " << recebedor->getSaldo() << endl;
                } else {
                    cout << "Tipo de transação inválido!" << endl;
                }
                break;
            }
                
            case 4:{
                string nome;
                cout << "Digite o nome do cliente: ";
                cin >> nome;
                //primeiro buscamos na memoria caso os dados forem cadastrados na sessao atual
                Cliente* c= buscarClientePorNome(cliente, nome);
                if (c){
                    vector<Transacoes*> extrato = c->getExtrato();
                    cout << "\n===== EXTRATO DE " << c->nome << " =====" << endl;
                    if (extrato.empty()) { //verifica se o vetor esta vazio 
                        cout << "Nenhuma transação registrada." << endl;
                    } else {
                        for (int i = 0; i < (int)extrato.size(); i++) {
                            cout << "---- Transação " << i+1 << " ----" << endl;
                            cout << "Tipo:    " << extrato[i]->getTipo()    << endl;
                            cout << "Data:    " << extrato[i]->getData()    << endl;
                            cout << "Horario: " << extrato[i]->getHorario() << endl;
                            cout << "Valor:   " << extrato[i]->getValor()   << endl;
                            cout << "Recebedor: " << extrato[i]->getClientes()[0]->nome << endl;
                            if(extrato[i]->getTipo() == "transferencia")
                                cout << "Transferidor: " << extrato[i]->getClientes()[1]->nome << endl;
                            cout << " ---- ---- ---- ---- " << endl;

                        }
                    }
                    break;
                }else {
                    cout << "Cliente não encontrado." << endl;
                }
                break;
            }   
            case 5:{
                associaGC(cliente, gerente);
                break;
            }
            case 6: {
                string nome;
                cout << "Digite o nome do cliente que deseja listar: ";
                cin >> nome;

                Cliente* c = buscarClientePorNome(cliente, nome);
                if (c) {
                    cout << "\n===== INFORMACOES DE " << c->nome << " =====" << endl;
                    cout << "Login:            " << c->getLogin()      << endl;
                    cout << "Trabalho:         " << c->trabalho        << endl;
                    cout << "Data nasc.:       " << c->dataNasc        << endl;
                    cout << "Remuneracao:      " << c->getRemuneracao()<< endl;
                    cout << "Tipo de conta:    " << c->getTipoDeConta()<< endl;
                    cout << "Taxa rendimento:  " << c->getRendimento() << endl;
                    cout << "Saldo:            " << c->getSaldo()      << endl;
                    cout << "Gerente:          " << c->getGerente()    << endl;

                    vector<Transacoes*> extrato = c->getExtrato();
                    cout << "\n===== TRANSAÇÕES =====" << endl;
                    if (extrato.empty()) {
                        cout << "Nenhuma transação." << endl;
                    } else {
                        for (int i = 0; i < (int)extrato.size(); i++) {
                            cout << "Tipo:  " << extrato[i]->getTipo()    << endl;
                            cout << "Data:  " << extrato[i]->getData()    << endl;
                            cout << "Valor: " << extrato[i]->getValor()   << endl;
                            cout << "Horario: " << extrato[i]->getHorario()   << endl;
                            vector<Cliente*> envolvidos = extrato[i]->getClientes();
                            if(extrato[i]->getTipo()== "transferencia"){
                                
                                //vetor de qm envia e quem recebe]
                                //verificamos qual deles é o cliente ATUAL para mostrar o outro 
                                
                                cout << "Recebedor: " << envolvidos[0]->nome << endl;
                                cout << "Transferidor: " << envolvidos[1]->nome << endl;
                                cout << '\n';
                            }else{
                                cout << "Recebedor: " << envolvidos[0]->nome << endl;
                            }
                            cout << "--------------------" << endl;
                            
                        }
                    }
                    break;
                } else {
                    cout << "Cliente nao encontrado no banco de dados"<< endl;
                }
                break;
            }
            case 7:{
                string nome;
                cout << "Digite o nome do gerente que deseja listar: ";
                cin >> nome;
                Gerente* g = buscarGerentePorNome(gerente, nome);
                if (g) {
                    cout << "\n===== INFORMAÇÕES DO GERENTE " << nome << " =====" << endl;
                    cout << "Login:    " << g->getLogin() << endl;
                    cout << "Senha:    " << g->getSenha() << endl;
                    cout << "Trabalho: " << g->trabalho  << endl;
                    cout << "Data de nascimento:   " << g->dataNasc << endl;

                    vector<Cliente*> lista = g->getClientes();
                    for (int i = 0; i < (int)lista.size(); i++){
                        cout << "Cliente " << i+1 << ":    " << lista[i]->nome << endl;
                        cout << "Tipo de Conta:    " << lista[i]->getTipoDeConta()<< endl;
                        cout << "Login Cliente:    " << lista[i]->getLogin() << endl;
                    }
                    cout << "=====  ===== ===== ===== ===== =====" << endl;
                    break;
                } else {
                    cout << "Gerente não encontrado no banco de dados " << endl;
                }
                break;
                }
            case 8: //salvar dados e sair 
                while(1){
                    string opt;
                    cout << "voce quer salvar todos os dados em clientes.csv e em gerente.csv (apenas sim ou nao): ";
                    cin >> opt;
                    if(opt != "sim" && opt != "nao")
                        cout << "Opção inválida!!" << endl;
                    else if(opt == "sim"){
                        salvarClientes(cliente, "clientes.csv");
                        salvarGerentes(gerente, "gerentes.csv");
                        cout << "Dados salvos com sucesso" << endl;
                        break;
                    }
                    else {
                        cout << "Saindo sem salvar" << endl;
                        break;
                    }
                }
                //td q criamos com new deletamos 
                //os q guardam as ref - nao criaram nada -  so guarda a referencia, damos um clear 
                for (auto c : cliente) //p cada cliente no vetor, libera a memoria daquele especifico
                    delete c; //chama o destrutor do objeto, deleta as transacoes e dps libera o cliente
                for (auto g : gerente)
                    delete g; //limp o vetor, nao deleta 
                return 0;
        } //fecha switch
    } //fecha while
    return 0;
}