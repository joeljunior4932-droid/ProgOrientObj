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
        virtual void exibirDados() const;
        //construtor e destrutor
        Pessoa(string login = "", string senha = "", string nome = "", string trabalho = "");
        
        virtual ~Pessoa();

        //metodos 
        
        
        void setLogin(const string& login);
        string getLogin() const;
        void setSenha(const string& senha);
        string getSenha() const;
        
        

};

#endif //PESSOA_H