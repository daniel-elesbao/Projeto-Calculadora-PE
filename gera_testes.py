import random
import math

# Seed fixa para reprodutibilidade
random.seed(42)

def divisao_truncada(a, b):
    """
    Implementa divisão inteira truncada (comportamento do C).
    Python usa divisão floor (//), mas C trunca em direção a zero.
    """
    if b == 0:
        return "Erro"
    
    # Calcula o quociente e ajusta o sinal
    quociente = abs(a) // abs(b)
    
    # Se os sinais são diferentes, o resultado é negativo
    if (a < 0) != (b < 0):  # XOR: verdadeiro se sinais diferentes
        quociente = -quociente
    
    return quociente

def gerar_teste(nome_arquivo, operacao, qtd, min_dig, max_dig):
    print(f"Gerando {qtd} testes para {nome_arquivo}...")
    with open(nome_arquivo, 'w') as f, open(f"gabarito_{nome_arquivo}", 'w') as gab:
        for _ in range(qtd):
            a = random.randint(10**(min_dig-1), 10**max_dig)
            b = random.randint(10**(min_dig-1), 10**max_dig)
            
            # Ajuste para evitar divisão por zero em testes aleatórios
            if operacao in ['/', '%', 'M'] and b == 0:
                b = 1
            
            # Sinais negativos (apenas para operações que fazem sentido ou se o C suportar)
            if operacao not in ['M']: # MDC geralmente é com positivos
                if random.random() < 0.3: a *= -1
                if random.random() < 0.3: b *= -1
                if operacao in ['/', '%'] and b == 0: b = 1 # Garante não zero de novo
            
            f.write(f"{a} {operacao} {b}\n")
            
            res = 0
            try:
                if operacao == '+': res = a + b
                elif operacao == '-': res = a - b
                elif operacao == '*': res = a * b
                elif operacao == '/': res = divisao_truncada(a, b)
                elif operacao == '%': res = a % b
                elif operacao == 'M': res = math.gcd(a, b)
                gab.write(f"Resultado: {res}\n")
            except ZeroDivisionError:
                gab.write("Resultado: Erro\n")

# --- EXECUÇÃO ---

# 1. Soma e Subtração (Mantido 100)
gerar_teste("teste_soma.txt", "+", 100, 50, 100)
gerar_teste("teste_sub.txt", "-", 100, 50, 100)

# 2. Multiplicação (Aumentado para 100)
gerar_teste("teste_mult.txt", "*", 100, 30, 60)

# 3. Divisão (Novo: 100 testes)
gerar_teste("teste_div.txt", "/", 100, 30, 60)

# 4. MDC (Novo: 50 testes)
gerar_teste("teste_mdc.txt", "M", 50, 30, 60)

# 5. Fronteira (Aumentado para 50 misturando tipos)
print("Gerando 50 testes de fronteira...")
casos_base = [
    ("9"*30, "+", "1"), ("1"+"0"*30, "-", "1"), # Carry/Borrow
    ("0", "*", "12345"), ("12345", "*", "0"),   # Zeros mult
    ("500", "-", "500"), ("-50", "+", "50"),    # Zeros soma
    ("0", "/", "100"), ("100", "%", "1"),       # Divisões triviais
    ("17", "M", "13"), ("100", "M", "100")      # MDC primos/iguais
]

# CORREÇÃO CRÍTICA: Abrir ambos os arquivos juntos e processar linha por linha
with open("teste_fronteira.txt", 'w') as f, open("gabarito_teste_fronteira.txt", 'w') as gab:
    # Escreve casos base
    for a, op, b in casos_base:
        f.write(f"{a} {op} {b}\n")
        # Calcula gabarito (lógica simplificada para string->int)
        val_a, val_b = int(a), int(b)
        if op == '+': res = val_a + val_b
        elif op == '-': res = val_a - val_b
        elif op == '*': res = val_a * val_b
        elif op == '/': res = divisao_truncada(val_a, val_b)
        elif op == '%': res = val_a % val_b if val_b != 0 else "Erro"
        elif op == 'M': res = math.gcd(val_a, val_b)
        gab.write(f"Resultado: {res}\n")

    # Completa até 50 com casos aleatórios de fronteira
    ops = ["+", "-", "*", "/", "%", "M"]
    
    # SOLUÇÃO: Processar cada caso imediatamente (escreve teste E gabarito juntos)
    for _ in range(50 - len(casos_base)):
        op = random.choice(ops)
        a = random.randint(1, 10**40)
        
        # Gera operandos problemáticos baseados na operação
        if op == "*": 
            b = 0  # Multiplicação por zero
        elif op in ["/", "%"]: 
            b = 1  # Divisão por 1
        elif op == "-": 
            b = a  # Subtração resultando em zero
        elif op == "M":
            # CORREÇÃO: Gera o multiplicador e calcula b ANTES de qualquer escrita
            multiplicador = random.randint(1, 10)
            b = a * multiplicador  # Múltiplos
        else:
            b = 0  # Soma com zero
        
        # Escreve no arquivo de teste IMEDIATAMENTE
        f.write(f"{a} {op} {b}\n")
        
        # Calcula e escreve o gabarito IMEDIATAMENTE (com os mesmos a e b)
        if op == '+': 
            res = a + b
        elif op == '-': 
            res = a - b
        elif op == '*': 
            res = a * b
        elif op == '/': 
            res = divisao_truncada(a, b)
        elif op == '%': 
            res = a % b
        elif op == 'M': 
            res = math.gcd(a, b)
        
        gab.write(f"Resultado: {res}\n")

print("Todos os arquivos gerados com sucesso.")