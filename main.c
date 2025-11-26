#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "intgg.h"

/* Le uma string de tamanho arbitrario do arquivo ou teclado.
   Le caractere por caractere e usa realloc para aumentar o buffer. */
char* ler_input_dinamico(FILE *fluxo) {
    int capacidade = 64; /* Comeca pequeno */
    int tamanho = 0;
    char *buffer = (char*) malloc((size_t)capacidade * sizeof(char));
    int c;
    char *temp;

    if (!buffer) exit(1);

    /* 1. Pula espacos em branco (enter, espaco, tab) */
    while ((c = fgetc(fluxo)) != EOF && isspace(c));

    if (c == EOF) {
        free(buffer);
        return NULL; /* Caso EOF encerra */
    }

    /* 2. Le caracteres validos ate encontrar o proximo espaco ou fim de linha */
    do {
        /* Se o buffer encheu, dobra o tamanho */
        if (tamanho + 1 >= capacidade) {
            capacidade *= 2;
            temp = (char*) realloc(buffer, (size_t)capacidade * sizeof(char));
            if (!temp) {
                free(buffer);
                exit(1); /* Erro de alocacao */
            }
            buffer = temp;
        }
        buffer[tamanho++] = (char)c;
    } while ((c = fgetc(fluxo)) != EOF && !isspace(c));

    buffer[tamanho] = '\0'; /* Termina a string */
    return buffer;
}

/* Aloca espaco na memoria, converte string para IntGG e chama a operacao selecionada pelo usuario */
void processar_operacao(char op, char *str1, char *str2, FILE *saida) {
    /* Aloca IntGGs baseados no tamanho da string de entrada */
    IntGG *n1 = intgg_novo((int)strlen(str1));
    IntGG *n2 = intgg_novo((int)strlen(str2));
    IntGG *res = NULL;
    
    /* Converte strings para numeros */
    intgg_carregar_string(n1, str1);
    intgg_carregar_string(n2, str2);
    /* Seleciona a operacao */
    switch(op) {
        case '+': res = intgg_soma(n1, n2); break;
        case '-': res = intgg_subtracao(n1, n2); break;
        case '*': res = intgg_multiplicacao(n1, n2); break;
        case '/': res = intgg_divisao(n1, n2); break;
        case '%': res = intgg_modulo(n1, n2); break;
        case 'M': res = intgg_mdc(n1, n2); break;
        default: fprintf(stderr, "Operacao invalida\n");
    }

    /* Imprime e limpa (limpar -> requisito) */
    if (res) {
        fprintf(saida, "Resultado: ");
        intgg_imprimir(res, saida);
        fprintf(saida, "\n");
        intgg_liberar(res);
    }

    /* Limpa os operandos (requisito) */
    intgg_liberar(n1);
    intgg_liberar(n2);
}

/* Modo Interativo: Menu para usuario digitar no terminal */
void modo_interativo() {
    char op_char;
    char *op_str = NULL;
    char *s1 = NULL;
    char *s2 = NULL;
    
    printf("  _____      _      _         _____   _   _    _           _      _____      ____    _____       _     \n");
    printf(" / ____|    / \\    | |       / ____| | | | |  | |         / \\    |  _  \\    / __ \\  |  __ \\     / \\    \n");
    printf("| |        //_\\\\   | |      | |      | | | |  | |        //_\\\\   | | |  |  | |  | | | |__) |   //_\\\\   \n");
    printf("| |____   / ___ \\  | |____  | |____  | |_| |  | |____   / ___ \\  | |_|  |  | |__| | |  _  /   / ___ \\  \n");
    printf(" \\_____| /_/   \\_\\ |______|  \\_____|  \\___/   |______| /_/   \\_\\ |_____/    \\____/  |_| \\_\\  /_/   \\_\\ \n");
    printf("1. Soma (+)\n2. Subtracao (-)\n3. Multiplicacao (*)\n");
    printf("4. Divisao (/)\n5. Modulo (%%)\n6. MDC (M)\n");
    printf("S. Sair\n");

    while(1) {
        printf("\nOperacao: ");
        
        /* Le a operacao dinamicamente para evitar buffer overflow */
        op_str = ler_input_dinamico(stdin);
        if (!op_str) break;
        
        op_char = op_str[0];
        free(op_str); /* Char ja obtido. Libera a string (requisito) */

        if (op_char == 'S' || op_char == 's') break;

        printf("Operando 1: ");
        s1 = ler_input_dinamico(stdin);
        
        printf("Operando 2: ");
        s2 = ler_input_dinamico(stdin);

        if (s1 && s2) {
            processar_operacao(op_char, s1, s2, stdout);
        }

        /* Libera as strings de entrada apos o uso */
        if (s1) free(s1);
        if (s2) free(s2);
    }
}

/* Modo Arquivo: Le de um txt e grava em outro txt */
void modo_arquivo(char *caminho_entrada) {
    char nome_saida[256];
    FILE *fin;
    FILE *fout;
    char *s1 = NULL;
    char *s2 = NULL;
    char *op_str = NULL;

    fin = fopen(caminho_entrada, "r");
    if (!fin) {
        perror("Erro arquivo");
        return;
    }

    /* Gera nome do arquivo de saida (ex: entrada.txt -> saida_entrada.txt) */
    sprintf(nome_saida, "saida_%s", caminho_entrada);
    fout = fopen(nome_saida, "w");
    if (!fout) {
        fclose(fin);
        return;
    }
    
    /* Loop de leitura do arquivo */
    while (1) {
        s1 = ler_input_dinamico(fin);
        if (!s1) break; /* Fim do arquivo */

        op_str = ler_input_dinamico(fin);
        if (!op_str) { free(s1); break; }
        
        s2 = ler_input_dinamico(fin);
        if (!s2) { free(s1); free(op_str); break; }

        /* Processa a linha lida */
        processar_operacao(op_str[0], s1, s2, fout);

        free(s1);
        free(op_str);
        free(s2);
    }

    fclose(fin);
    fclose(fout);
    printf("Processamento concluido. Verifique '%s'.\n", nome_saida);
}

int main(int argc, char *argv[]) {
    /* Se passar argumento na linha de comando, usa modo arquivo */
    if (argc > 1) {
        modo_arquivo(argv[1]);
    } else {
        modo_interativo();
    }
    return 0;
}