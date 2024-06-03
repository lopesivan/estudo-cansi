#!/bin/bash

# Cria um pipe
pipe=$(mktemp -u)
mkfifo "$pipe"

# Função para executar o comando pwd e enviar o resultado para o pipe
function write_to_pipe {
    pwd > "$pipe"
}

# Função para ler o resultado do pipe e imprimir
function read_from_pipe {
    local result
    read -r result < "$pipe"
    echo "Current directory: $result"
}

# Executa as funções em paralelo
write_to_pipe &
read_from_pipe &

# Espera que ambos os processos terminem
wait

# Remove o pipe
rm "$pipe"

