#ifndef TRANSACOES_H
#define TRANSACOES_H
#include <string>
#include <vector>
using namespace std;
class Cliente;
class Transacoes{
    private:
        string tipo, data, horario;
        double valor;
        vector<Cliente*> clientes;
        //ponteiro p os clientes envolvidos na transacao 
        
    public:
        Transacoes(string ="", string ="", string = "", double = 0.0);
        virtual ~Transacoes();
        string getTipo() const;
        string getData() const;
        string getHorario() const;
        double getValor() const;
        vector<Cliente*>& getClientes();
        void setClientes(Cliente *cliente);
        void setData(const string& data);
        void setHorario(const string& horario);
        void setValor(double valor);
        void exibirTransacoes()const;
       
    
};

#endif