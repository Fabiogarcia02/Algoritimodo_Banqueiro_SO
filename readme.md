🏦 Algoritmo do Banqueiro em C (WSL / Ubuntu)

📌 Descrição

Este projeto implementa o Algoritmo do Banqueiro, utilizado em Sistemas Operacionais para evitar deadlocks durante a alocação de recursos.

O programa simula múltiplos clientes (threads) solicitando e liberando recursos de forma concorrente, garantindo que o sistema permaneça sempre em um estado seguro.

⚙️ Requisitos

Para executar o projeto no Windows, é necessário:

WSL (Windows Subsystem for Linux) instalado
Distribuição Linux (Ubuntu recomendado)
GCC (compilador C)
Biblioteca pthread (já incluída no build-essential)

🛠️ Instalação no WSL

No terminal do Ubuntu, execute:

sudo apt update
sudo apt install build-essential -y

▶️ Como executar o projeto
📍 1. Abrir o WSL

Abra o Ubuntu pelo Windows.

📍 2. Acessar a pasta do projeto

Se o projeto estiver no Windows, acesse via:

cd /mnt/c/Users/SEU_USUARIO/caminho/da/pasta

Diretamente pelo VS Code:

code .

⚠️ Necessário ter a extensão WSL instalada no VS Code

📍 3. Compilar o código
gcc banker.c -o banker -lpthread
📍 4. Executar o programa

O programa recebe 3 valores iniciais de recursos:

./banker <r1> <r2> <r3>

Exemplo:

./banker 10 5 7

💡 Observação

Não é necessário abrir o VS Code para executar o programa.
Tudo pode ser feito diretamente pelo terminal do Ubuntu (WSL).
