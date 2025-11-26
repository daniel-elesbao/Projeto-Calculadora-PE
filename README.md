# Projeto-Calculadora-PE
# Calc IntGG - Calculadora de Inteiros Gigantes

**Disciplina:** MCTA028-15 - PROGRAMAÇÃO ESTRUTURADA - Roberto Sadao - 2025.3  
**Universidade:** Universidade Federal do ABC 
**Linguagem:** C (ANSI)  
**Data:** Novembro/2025

## 1. Descrição do Projeto

A **Calculadora IntGG** é uma calculadora de linha de comando capaz de realizar operações aritméticas com números inteiros de tamanho arbitrário . Diferente dos tipos primitivos da linguagem C (como `unsigned long long`), a estrutura IntGG suporta números limitados apenas pela memória disponível no computador, permitindo cálculos com centenas ou milhares de dígitos.

O projeto foi implementado sem o uso de bibliotecas externas de BigInt, utilizando alocação dinâmica de memória e algoritmos clássicos de aritmética.

Jupyter Notebook hospedado no Google Colab demonstrando as funcionalidades e explicando decisões de projeto:
https://colab.research.google.com/drive/1jxnFTkx7UAjVUEFpc_MifSWjYek3Lo3x?usp=sharing

## 2. Funcionalidades

O programa suporta as seguintes operações através de um menu interativo ou leitura de arquivos:

* **Soma (+)**
* **Subtração (-)**
* **Multiplicação (*)**
* **Divisão Inteira (/)**
* **Módulo / Resto (%)**
* **Máximo Divisor Comum (M)** - *Operação Personalizada*

## 3. Estrutura de Arquivos

* `main.c`: Gerencia a interface com o usuário (CLI), leitura de arquivos e orquestração das operações. Possui leitura dinâmica de buffer para suportar entradas infinitas.
* `intgg.h`: Arquivo de cabeçalho que define a estrutura opaca `IntGG` e os protótipos das funções públicas.
* `intgg.c`: Implementação da lógica matemática.
* `Makefile`: Script de automação para compilação do projeto.

## 4. Como Compilar

Para compilar, abra o terminal na pasta do projeto e execute:

```bash
make
```

## **5\. Metodologia de Testes e Validação**

Para garantir a correção aritmética e a robustez do gerenciamento de memória, o projeto adotou uma estratégia de validação baseada em **Testes Automatizados Comparativos** (também conhecida como *Differential Testing*).

A validação consiste em confrontar os resultados gerados pela nossa implementação em C (IntGG) contra um "Oráculo" confiável — neste caso, a implementação nativa de inteiros de precisão arbitrária da linguagem Python.

### **5.1 Categorias de Teste**

Os casos de teste foram divididos em quatro categorias críticas para cobrir diferentes vetores de falha:

1. **Testes de Fronteira (Edge Cases):**  
   * Focam em situações que costumam quebrar algoritmos de aritmética de precisão arbitrária (*Long Arithmetic*).  
   * *Exemplos:* Propagação de "vai-um" em cascata (99...99 \+ 1), propagação de empréstimo (100...00 \- 1), operações resultantes em zero, e identidade (soma com 0, multiplicação por 1).  
2. **Testes de Sinais e Aritmética de Inteiros:**  
   * Validam a lógica de decisão de sinais.  
   * *Exemplos:* Somar dois números negativos deve resultar em uma soma de magnitudes com sinal negativo; subtrair um negativo é equivalente a somar um positivo (A \- (-B) \= A \+ B).  
3. **Testes de Estresse (Massive Numbers):**  
   * Envolvem operandos com centenas de dígitos (ex: 50 a 200 dígitos).  
   * O objetivo é forçar múltiplas realocações de memória (realloc) no vetor dinâmico e validar a estabilidade e performance dos algoritmos $O(N^2)$ (como a multiplicação).  
4. **Operações Complexas (Divisão e MDC):**  
   * Verificam a integridade do algoritmo de Divisão Euclidiana ("Long Division") e a recursividade (ou iteração) do Algoritmo de Euclides para o Máximo Divisor Comum.

### **5.2 Ferramenta de Automação (Python Script)**

Foi desenvolvido um script auxiliar em Python para gerar massas de teste automaticamente. O fluxo de validação funciona da seguinte maneira:

1. **Geração:** O script gera pares de números aleatórios gigantes.  
2. **Cálculo do Gabarito:** O Python calcula o resultado esperado nativamente.  
3. **Execução:** O programa C é invocado para processar os mesmos números.
4. **Verificação:** Compara-se a saída gerada pelo programa C (saida\_\*.txt) com o gabarito.

### **5.3 Instruções de Execução dos Testes**

Para reproduzir a bateria de testes, siga os passos abaixo no terminal:

Passo 1: Compilar o projeto  

Passo 2: Gerar os arquivos de teste

Utilize o script Python fornecido para criar os arquivos .txt com centenas de operações aleatórias.
```bash
python3 gera_testes.py
```  

Passo 3: Executar a calculadora contra os arquivos

Script Shell para rodar tudo

```bash
sudo chmod +x testes.sh
./testes.sh
```

Passo 4: Verificar os resultados

Vamos comparar a saída do programa C com o gabarito gerado pelo Python.

```bash
python3 compare.py
```