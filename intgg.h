/* Biblioteca com a definicao do tipo intgg para inteiros grandes e */
/* as funcoes com as operacoes associadas a esse tipo. */
#ifndef INTGG_H
#define INTGG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int *digitos;       /* Array dinamico que armazena os numeros */
    int tamanho;        /* Tamanho do array */
    int capacidade;     /* Tamanho total de memoria alocada (para evitar realloc constante) */
    int sinal;          /* 1 para positivo, -1 para negativo */
} IntGG;

/* Cria um novo IntGG zerado com capacidade inicial especificada */
IntGG* intgg_novo(int capacidade_inicial);

/* Libera toda a memoria usada pelo numero */
void intgg_liberar(IntGG *n);

/* Imprime o numero na tela ou no arquivo */
void intgg_imprimir(IntGG *n, FILE *saida);

/* Converte string -> IntGG */
void intgg_carregar_string(IntGG *n, const char *str);

IntGG* intgg_soma(IntGG *a, IntGG *b);
IntGG* intgg_subtracao(IntGG *a, IntGG *b);
IntGG* intgg_multiplicacao(IntGG *a, IntGG *b);
IntGG* intgg_divisao(IntGG *a, IntGG *b);
IntGG* intgg_modulo(IntGG *a, IntGG *b);
IntGG* intgg_mdc(IntGG *a, IntGG *b); /* funcao extra */

#endif