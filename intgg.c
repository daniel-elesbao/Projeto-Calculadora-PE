#include "intgg.h"

/* =========================================================================
 * FUNCOES AUXILIARES (INTERNAS)
 * Estas funcoes ajudam na logica mas nao sao visiveis fora deste arquivo.
 * ========================================================================= */

/* Remove zeros a esquerda (ex: 00123 -> 123) e ajusta tamanho */
static void remover_zeros(IntGG *n) {
    while (n->tamanho > 1 && n->digitos[n->tamanho - 1] == 0) {
        n->tamanho--;
    }
    /* Se sobrou apenas zero, o sinal deve ser positivo por convencao */
    if (n->tamanho == 1 && n->digitos[0] == 0) {
        n->sinal = 1;
    }
}

/* Compara o valor absoluto de dois numeros (ignora sinal)
 * Retorna: 1 se |a| > |b|, -1 se |a| < |b|, 0 se iguais */
static int comparar_absoluto(IntGG *a, IntGG *b) {
    int i;
    if (a->tamanho > b->tamanho) return 1;
    if (a->tamanho < b->tamanho) return -1;
    
    for (i = a->tamanho - 1; i >= 0; i--) {
        if (a->digitos[i] > b->digitos[i]) return 1;
        if (a->digitos[i] < b->digitos[i]) return -1;
    }
    return 0;
}

/* Cria uma copia de um numero */
static IntGG* intgg_copiar(IntGG *origem) {
    IntGG *copia = intgg_novo(origem->capacidade);
    copia->tamanho = origem->tamanho;
    copia->sinal = origem->sinal;
    memcpy(copia->digitos, origem->digitos, (size_t)origem->capacidade * sizeof(int));
    return copia;
}

/* Multiplica um numero por 10 (shift left) e soma um digito */
static void intgg_shift_add(IntGG *n, int val) {
    int i;
    if (n->tamanho == 1 && n->digitos[0] == 0) {
        n->digitos[0] = val; /* Se for 0, apenas substitui */
        return;
    }
    /* Desloca tudo para cima */
    if (n->tamanho + 1 >= n->capacidade) {
        n->capacidade *= 2;
        n->digitos = (int*) realloc(n->digitos, (size_t)n->capacidade * sizeof(int));
    }
    for (i = n->tamanho; i > 0; i--) {
        n->digitos[i] = n->digitos[i-1];
    }
    n->digitos[0] = val;
    n->tamanho++;
    remover_zeros(n);
}

/* =========================================================================
 * IMPLEMENTACAO DAS FUNCOES PUBLICAS
 * ========================================================================= */

static IntGG* abs_soma(IntGG *a, IntGG *b);
static IntGG* abs_subtracao(IntGG *a, IntGG *b); /* Assume |a| >= |b| */

IntGG* intgg_novo(int capacidade_inicial) {
    IntGG *n;
    if (capacidade_inicial < 1) capacidade_inicial = 1;
    n = (IntGG*) malloc(sizeof(IntGG));
    if (!n) exit(1);

    n->digitos = (int*) calloc((size_t)capacidade_inicial, sizeof(int));
    if (!n->digitos) { free(n); exit(1); }

    n->capacidade = capacidade_inicial;
    n->tamanho = 1; /* Comeca representando o numero 0 */
    n->sinal = 1; 
    return n;
}

void intgg_liberar(IntGG *n) { /* Libera memoria alocada (requisito) */
    if (n) {
        if (n->digitos) free(n->digitos);
        free(n);
    }
}

void intgg_carregar_string(IntGG *n, const char *str) {
    int len = (int)strlen(str);
    int inicio = 0;
    int idx = 0;
    int i;

    memset(n->digitos, 0, (size_t)n->capacidade * sizeof(int)); /* Zera digitos */
    n->tamanho = 0;
    n->sinal = 1;
    if (str[0] == '-') {
        n->sinal = -1;
        inicio = 1;
    } else if (str[0] == '+') {
        inicio = 1;
    }

    if (len > n->capacidade) {
        n->capacidade = len + 10;
        n->digitos = (int*) realloc(n->digitos, (size_t)n->capacidade * sizeof(int)); /* Ajusta capacidade */
    }

    /* Converte char para int e armazena invertido (Little Endian) */
    for (i = len - 1; i >= inicio; i--) {
        if (str[i] >= '0' && str[i] <= '9') {
            n->digitos[idx++] = str[i] - '0';
        }
    }
    n->tamanho = (idx == 0) ? 1 : idx;
    remover_zeros(n);
}

void intgg_imprimir(IntGG *n, FILE *saida) {
    int i;
    if (n->tamanho == 0) {
        fprintf(saida, "0\n");
        return;
    }
    
    /* Se for zero, nao imprime sinal */
    if (n->tamanho == 1 && n->digitos[0] == 0) n->sinal = 1;

    if (n->sinal == -1) fprintf(saida, "-");
    
    /* Imprime do final para o comeco (Little Endian) */
    for (i = n->tamanho - 1; i >= 0; i--) {
        fprintf(saida, "%d", n->digitos[i]);
    }
}

/* =========================================================================
 * OPERACOES ARITMETICAS
 * ========================================================================= */

/* Soma (|A| + |B|) */
static IntGG* abs_soma(IntGG *a, IntGG *b) {
    int max_len = (a->tamanho > b->tamanho ? a->tamanho : b->tamanho) + 1;
    IntGG *res = intgg_novo(max_len);
    int carry = 0;
    int i;

    res->tamanho = max_len;

    for (i = 0; i < max_len; i++) {
        int valA = (i < a->tamanho) ? a->digitos[i] : 0;
        int valB = (i < b->tamanho) ? b->digitos[i] : 0;
        int soma = valA + valB + carry;
        res->digitos[i] = soma % 10;
        carry = soma / 10;
    }
    remover_zeros(res);
    return res;
}

/* Subtrai (|A| - |B|). Assume |A| >= |B| */
static IntGG* abs_subtracao(IntGG *a, IntGG *b) {
    IntGG *res = intgg_novo(a->tamanho);
    int empresta = 0;
    int i;

    res->tamanho = a->tamanho;
    for (i = 0; i < a->tamanho; i++) {
        int valA = a->digitos[i];
        int valB = (i < b->tamanho) ? b->digitos[i] : 0;
        int sub = valA - valB - empresta;

        if (sub < 0) {
            sub += 10;
            empresta = 1;
        } else {
            empresta = 0;
        }
        res->digitos[i] = sub;
    }
    remover_zeros(res);
    return res;
}

IntGG* intgg_soma(IntGG *a, IntGG *b) {
    IntGG *res;
    /* Se sinais iguais: soma e mantem sinal */
    if (a->sinal == b->sinal) {
        res = abs_soma(a, b);
        res->sinal = a->sinal;
    } else {
        /* Sinais diferentes: subtrai menor do maior */
        if (comparar_absoluto(a, b) >= 0) { /* |A| >= |B| */
            res = abs_subtracao(a, b);
            res->sinal = a->sinal;
        } else { /* |B| > |A| */
            res = abs_subtracao(b, a);
            res->sinal = b->sinal;
        }
    }
    return res;
}

IntGG* intgg_subtracao(IntGG *a, IntGG *b) {
    /* A - B eh o mesmo que A + (-B) */
    IntGG *b_neg = intgg_copiar(b);
    IntGG *res;
    b_neg->sinal *= -1; /* Inverte sinal de B */
    res = intgg_soma(a, b_neg);
    
    intgg_liberar(b_neg);
    return res;
}

IntGG* intgg_multiplicacao(IntGG *a, IntGG *b) {
    int len_res = a->tamanho + b->tamanho;
    IntGG *res = intgg_novo(len_res);
    int i, j;
    
    /* Inicializa com zeros */
    res->tamanho = len_res;
    
    /* Algoritmo de Multiplicacao Longa */
    for (i = 0; i < a->tamanho; i++) {
        int carry = 0;
        for (j = 0; j < b->tamanho || carry; j++) {
            int valB = (j < b->tamanho) ? b->digitos[j] : 0;
            /* long eh suficiente aqui pois Base 10 nao estoura 32-bit int com facilidade */
            long atual = res->digitos[i + j] + (long)a->digitos[i] * valB + carry;
            
            res->digitos[i + j] = (int)(atual % 10);
            carry = (int)(atual / 10);
        }
    }
    res->sinal = a->sinal * b->sinal;
    remover_zeros(res);
    return res;
}

/* Funcao que realiza Divisao Euclidiana
 * Retorna Quociente (modo == 1) ou Resto (modo == 2) */
static IntGG* div_mod_core(IntGG *dividendo, IntGG *divisor, int modo) {
    IntGG *quociente;
    IntGG *resto;
    IntGG *abs_divisor;
    int i;

    /* Verifica divisao por zero */
    if (divisor->tamanho == 1 && divisor->digitos[0] == 0) {
        fprintf(stderr, "Erro: Divisao por zero.\n");
        return intgg_novo(1);
    }
    quociente = intgg_novo(dividendo->tamanho);
    resto = intgg_novo(10); /* inicializa com tudo zerado */
    
    /* Configura quociente */
    quociente->tamanho = dividendo->tamanho;

    abs_divisor = intgg_copiar(divisor);
    abs_divisor->sinal = 1;

    /* Algoritmo de "Long Division" (Divisao na chave)
       Percorre o dividendo do digito mais significativo para o menos */
    for (i = dividendo->tamanho - 1; i >= 0; i--) {
        int count = 0;
        /* Baixa o proximo digito: Resto = Resto * 10 + digito[i] */
        intgg_shift_add(resto, dividendo->digitos[i]);

        /* Verifica quantas vezes o divisor cabe no resto atual */
        while (comparar_absoluto(resto, abs_divisor) >= 0) {
            IntGG *temp = abs_subtracao(resto, abs_divisor);
            intgg_liberar(resto);
            resto = temp;
            count++;
        }
        quociente->digitos[i] = count;
    }

    remover_zeros(quociente);
    remover_zeros(resto);
    
    /* Define sinais
       Divisao: Sinais iguais (+), diferentes (-) */
    quociente->sinal = dividendo->sinal * divisor->sinal;
    
    /* Modulo: Sinal segue o dividendo (no C padrao) */
    resto->sinal = dividendo->sinal;

    intgg_liberar(abs_divisor);

    if (modo == 1) { /* Retornar Quociente */
        intgg_liberar(resto);
        return quociente;
    } else { /* Retornar Resto */
        intgg_liberar(quociente);
        return resto;
    }
}

IntGG* intgg_divisao(IntGG *a, IntGG *b) {
    return div_mod_core(a, b, 1);
}

IntGG* intgg_modulo(IntGG *a, IntGG *b) {
   /* Obtém o resto padrão da divisão*/
    IntGG *resto = div_mod_core(a, b, 2);
    /* CORREÇÃO:
     * Se o resto não for zero e tiver sinal diferente do divisor,
     * somamos o divisor ao resto.
     * Ex: -101 % 10 
     * Resto C = -1 (sinal negativo, divisor positivo)
     * Ajuste = -1 + 10 = 9 
     */
    if (resto->sinal != b->sinal) {
        /* Verifica se o resto é zero */
        if (!(resto->tamanho == 1 && resto->digitos[0] == 0)) {
            IntGG *temp = intgg_soma(resto, b);
            intgg_liberar(resto);
            return temp;
        }
    }
    return resto;
}

/* MDC usando Algoritmo de Euclides Iterativo 
   MDC(a, b) = MDC(b, a % b) */
IntGG* intgg_mdc(IntGG *a, IntGG *b) {
    IntGG *x = intgg_copiar(a);
    IntGG *y = intgg_copiar(b);
    IntGG *resto;
    
    /* MDC trabalha com positivos */
    x->sinal = 1;
    y->sinal = 1;

    while (!(y->tamanho == 1 && y->digitos[0] == 0)) { /* Enquanto y != 0 */
        resto = intgg_modulo(x, y);
        intgg_liberar(x); /* Libera o antigo X */
        x = y;            /* X vira Y */
        y = resto;        /* Y vira Resto */
    }
    intgg_liberar(y); /* Y eh zero aqui */
    return x; /* O resultado esta em X */
}