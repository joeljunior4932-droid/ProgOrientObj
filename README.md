# ProgOrientObj

Este é um projeto prático desenvolvido como parte dos meus estudos na graduação em Ciência da Computação na UFOP. O objetivo principal foi aplicar conceitos de Orientação a Objetos e aprofundar o entendimento sobre o gerenciamento manual de memória e alocação dinâmica em C++, simulando as operações internas de uma agência bancária via terminal.

## Decisões Técnicas e Arquitetura

O foco do desenvolvimento não foi apenas criar um menu funcional, mas estruturar como as entidades se relacionam na memória e como garantir a persistência segura do estado da aplicação. 

Abaixo estão as principais decisões de projeto:

* **Modelagem e Polimorfismo:** Criei a classe base abstrata `Pessoa` para encapsular os atributos e métodos comuns a qualquer usuário do sistema (como login, senha e exibição básica). As classes `Cliente` e `Gerente` herdam de Pessoa, estendendo suas particularidades e sobrescrevendo métodos virtuais de acordo com suas regras de negócio.
* **Gerenciamento do Ciclo de Vida (Raw Pointers):** Todo o sistema utiliza alocação dinâmica gerida através de vetores de ponteiros (`std::vector<T*>`). Isso me obrigou a pensar cuidadosamente no *ownership* (posse) dos objetos para evitar vazamentos de memória (*memory leaks*) ou erros de *double free*:
  * A classe `Cliente` tem a posse exclusiva de suas transações. Sendo assim, o destrutor de `Cliente` é o encarregado de iterar pelo extrato e executar o `delete` em cada objeto da classe `Transacoes` associado a ele.
  * Por outro lado, a classe `Gerente` apenas guarda referências dos clientes sob sua responsabilidade. Seu destrutor aplica apenas um `.clear()` no vetor, liberando a lista sem tentar deletar os clientes originais que já são gerenciados pelo fluxo principal.
* **Persistência de Dados em CSV:** Implementei a leitura e escrita utilizando `std::ifstream`, `std::ofstream` e `std::stringstream`. O desafio aqui foi criar a lógica de *parsing* que converte as strings do arquivo texto e reconstrói toda a teia de ponteiros e associações em tempo de execução sempre que o programa é iniciado.

## Como compilar e executar

O projeto utiliza apenas a biblioteca padrão do C++ e não possui dependências externas. Foi desenvolvido e testado nativamente em ambiente Linux (Ubuntu).

Para compilar via terminal usando o GCC:

```bash
g++ main.cpp Pessoa.cpp Cliente.cpp Gerente.cpp Transacoes.cpp Menu.cpp -o banco -Wall
