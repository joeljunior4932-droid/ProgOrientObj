#ifndef PESSOA_H
#define PESSOA_H
#include <string>
using namespace std;
class Pessoa{
    private:
        string login;
        string senha;
    
    public:
        string nome;
        string trabalho;
        string dataNasc;
        virtual void exibirDados() const;
        //construtor e destrutor
        Pessoa(string = "", string = "", string = "", string = "", string = "");
        
        virtual ~Pessoa();
        //destrutor
        //virtual sig que as classes filhas podem sobrescrever o metodo com sua propria versao

        //metodos 
        void setLogin(const string& login);
        string getLogin() const;
        void setSenha(const string& senha);
        string getSenha() const;
        //const sig que o metodo nao modifica o objeto

};
//definimos a estrutura da classe

#endif //PESSOA_H