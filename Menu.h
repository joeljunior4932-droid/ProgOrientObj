#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <vector>
#include "Cliente.h"
#include <algorithm>
#include "Gerente.h"
#include <string>
#include "Transacoes.h"
#include <fstream>
#include <sstream>

void printMenu();
Cliente* buscarClientePorNome(vector<Cliente*>& clientes, const string& nome);
Gerente* buscarGerentePorNome(vector<Gerente*>& gerentes, const string& nome);
//carregamos os arquivos de cliente e gerente e reconstruimos os objetos na memoria
void carregarClientes(vector<Cliente*>& clientes, const string& arquivo);
void carregarGerentes(vector<Gerente*>& gerentes, const string& arquivo);
void salvarClientes(vector<Cliente*>& clientes, const string& arquivo);
void salvarGerentes(vector<Gerente*>& gerentes, const string& arquivo);
Gerente* cadastrarGerente(vector <Gerente*>& gerente);
void associaGC(vector<Cliente*>& clientes, vector<Gerente*>& gerentes);
Transacoes* transacaoBanco(Cliente* client1, double saldo);
Cliente* cadastrarCliente(vector<Cliente*>& clientes);

#endif