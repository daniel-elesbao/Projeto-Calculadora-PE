#!/bin/bash

# Compila o projeto antes de rodar (importante!)
make

echo "--- Rodando Testes de Soma ---"
./calc teste_soma.txt

echo "--- Rodando Testes de Subtracao ---"
./calc teste_sub.txt

echo "--- Rodando Testes de Multiplicacao ---"
./calc teste_mult.txt

echo "--- Rodando Testes de Divisao ---"
./calc teste_div.txt

echo "--- Rodando Testes de MDC ---"
./calc teste_mdc.txt

echo "--- Rodando Testes de Fronteira ---"
./calc teste_fronteira.txt

echo "--- Testes Concluidos ---"