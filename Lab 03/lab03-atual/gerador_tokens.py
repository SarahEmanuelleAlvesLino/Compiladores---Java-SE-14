import os
import subprocess
import glob

# Descobre automaticamente a pasta raiz onde este script está (Lab03-atual)
raiz_projeto = os.path.dirname(os.path.abspath(__file__))
pasta_ins = os.path.join(raiz_projeto, 'ins')

# Busca os arquivos .java na pasta ins/
arquivos_java = glob.glob(os.path.join(pasta_ins, '*.java'))

if not arquivos_java:
    print("Nenhum arquivo .java encontrado na pasta ins/")
else:
    print(f"Encontrados {len(arquivos_java)} arquivos. Iniciando...\n")

    for arquivo_java in arquivos_java:
        nome_base = os.path.splitext(os.path.basename(arquivo_java))[0]
        arquivo_tokens = os.path.join(pasta_ins, f"{nome_base}.tokens")
        
        print(f"Gerando tokens para: {nome_base}.java ...")
        
        with open(arquivo_java, 'r') as infile, open(arquivo_tokens, 'w') as outfile:
            # cwd=raiz_projeto força o terminal a rodar o Java na pasta raiz!
            subprocess.run(['java', 'JavaSE14'], stdin=infile, stdout=outfile, cwd=raiz_projeto)
        
       
            
    print("\n✅ SUCESSO! Tokens gerados perfeitamente!")