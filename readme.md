🏦 Algoritmo do Banqueiro em C (WSL / Ubuntu)

📌 Descrição

Este projeto implementa o Algoritmo do Banqueiro, utilizado em Sistemas Operacionais para evitar deadlocks durante a alocação de recursos.

O programa simula múltiplos clientes (threads) solicitando e liberando recursos de forma concorrente, verificando sempre se o sistema permanece em estado seguro.

⚙️ Requisitos

Para executar o projeto no WSL (Ubuntu), é necessário ter:

WSL instalado no Windows
Distribuição Linux (Ubuntu recomendada)
GCC (compilador C)
Biblioteca pthread (já incluída no build-essential)
 Instalação no WSL (se ainda não tiver ferramentas)

Execute no terminal do Ubuntu:

sudo apt update
sudo apt install build-essential -y
🚀 Como executar o projeto (WSL / Ubuntu)
📍 1. Abrir o WSL e entrar na pasta do projeto
cd /mnt/c/Users/Fábio/Desktop/trabalhoSO/Algoritimodo_Banqueiro_SO
📍 2. Compilar o código
gcc banker.c -o banker -pthread
📍 3. Executar o programa

O programa precisa de 3 valores iniciais de recursos:

./banker <r1> <r2> <r3>
 Exemplo:
./banker 10 5 7
