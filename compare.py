import os

def ler_resultados(caminho_arquivo):
    """
    Lê um arquivo de resultados e retorna uma lista de strings com os valores.
    Espera-se que cada linha de resultado comece com 'Resultado: '.
    """
    resultados = []
    if not os.path.exists(caminho_arquivo):
        print(f"⚠️  Arquivo não encontrado: {caminho_arquivo}")
        return None

    with open(caminho_arquivo, 'r') as f:
        for linha in f:
            linha = linha.strip()
            # Filtra apenas as linhas que contêm a resposta final
            if linha.startswith("Resultado: "):
                # Extrai o valor após 'Resultado: '
                valor = linha.split("Resultado: ")[1].strip()
                resultados.append(valor)
    return resultados

def comparar_arquivos(teste_nome):
    """
    Compara o arquivo de saída do C com o gabarito do Python para um dado teste.
    """
    arquivo_saida_c = f"saida_{teste_nome}.txt"
    arquivo_gabarito_py = f"gabarito_{teste_nome}.txt"

    print(f"🔍 Analisando: {teste_nome}...")

    res_c = ler_resultados(arquivo_saida_c)
    res_py = ler_resultados(arquivo_gabarito_py)

    if res_c is None or res_py is None:
        print("   ❌ Falha: Um dos arquivos de entrada não existe.")
        return False

    # Verifica se a quantidade de resultados é a mesma
    if len(res_c) != len(res_py):
        print(f"   ⚠️  Aviso: Quantidade de linhas difere! C: {len(res_c)}, Gabarito: {len(res_py)}")
    
    erros = 0
    total = min(len(res_c), len(res_py))
    
    for i in range(total):
        if res_c[i] != res_py[i]:
            erros += 1
            if erros <= 5: # Mostra apenas os primeiros 5 erros
                print(f"   ❌ Erro na linha {i+1}:")
                print(f"      Esperado: {res_py[i]}")
                print(f"      Obtido:   {res_c[i]}")
    
    if erros == 0:
        print(f"   ✅ SUCESSO: Todos os {total} resultados conferem!")
        print("-" * 40)
        return True
    else:
        print(f"   ❌ FALHA: Encontrados {erros} erros em {total} testes.")
        print("-" * 40)
        return False

def main():
    print("=== RELATÓRIO DE COMPARAÇÃO AUTOMÁTICA ===\n")
    
    # Lista completa de todos os testes
    testes = [
        "teste_soma",
        "teste_sub",
        "teste_mult",
        "teste_div",      # ← ADICIONADO
        "teste_mdc",      # ← ADICIONADO
        "teste_fronteira"
    ]

    sucessos = 0
    falhas = 0
    
    for t in testes:
        resultado = comparar_arquivos(t)
        if resultado:
            sucessos += 1
        else:
            falhas += 1
    
    # Resumo final
    print("\n" + "=" * 40)
    print("📊 RESUMO FINAL")
    print("=" * 40)
    print(f"✅ Testes bem-sucedidos: {sucessos}")
    print(f"❌ Testes com falhas: {falhas}")
    print(f"📈 Taxa de sucesso: {sucessos}/{sucessos + falhas} ({100 * sucessos // (sucessos + falhas) if (sucessos + falhas) > 0 else 0}%)")
    print("=" * 40)
    
    if falhas == 0:
        print("\n🎉 PARABÉNS! Todos os testes passaram! 🎉\n")
    else:
        print(f"\n⚠️  {falhas} teste(s) falharam. Revise os erros acima.\n")

if __name__ == "__main__":
    main()