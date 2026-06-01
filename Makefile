CXX     = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET  = banco

# Adicionados CartaoCredito.cpp e CompraCartao.cpp na lista de compilação
SRCS = main.cpp \
       Pessoa.cpp \
       Cliente.cpp \
       Gerente.cpp \
       Transacoes.cpp \
       Menu.cpp \
       CartaoCredito.cpp \
       CompraCartao.cpp

OBJS = $(SRCS:.cpp=.o)

# Regra padrão: compila tudo
all: $(TARGET)

# Linka os .o e gera o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Regras de compilação de cada .o
main.o: main.cpp Menu.h Transacoes.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Pessoa.o: Pessoa.cpp Pessoa.h
	$(CXX) $(CXXFLAGS) -c Pessoa.cpp

Cliente.o: Cliente.cpp Cliente.h Pessoa.h Transacoes.h CartaoCredito.h
	$(CXX) $(CXXFLAGS) -c Cliente.cpp

Gerente.o: Gerente.cpp Gerente.h Pessoa.h Cliente.h
	$(CXX) $(CXXFLAGS) -c Gerente.cpp

Transacoes.o: Transacoes.cpp Transacoes.h Cliente.h
	$(CXX) $(CXXFLAGS) -c Transacoes.cpp

Menu.o: Menu.cpp Menu.h Cliente.h Gerente.h Transacoes.h CartaoCredito.h
	$(CXX) $(CXXFLAGS) -c Menu.cpp

# Novas regras para as classes de Cartão de Crédito
CartaoCredito.o: CartaoCredito.cpp CartaoCredito.h Cliente.h Transacoes.h CompraCartao.h
	$(CXX) $(CXXFLAGS) -c CartaoCredito.cpp

CompraCartao.o: CompraCartao.cpp CompraCartao.h
	$(CXX) $(CXXFLAGS) -c CompraCartao.cpp

# Remove arquivos gerados
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean