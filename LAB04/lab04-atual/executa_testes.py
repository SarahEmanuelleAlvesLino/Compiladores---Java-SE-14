import os
import subprocess
import glob

# Project root directory
raiz_projeto = os.path.dirname(os.path.abspath(__file__))
pasta_ins = os.path.join(raiz_projeto, 'ins')

# Find all *.tokens files in ins/
tokens_files = glob.glob(os.path.join(pasta_ins, '*.tokens'))
tokens_files.sort()

if not tokens_files:
    print("Nenhum arquivo .tokens encontrado na pasta ins/")
    exit(1)

print(f"Encontrados {len(tokens_files)} casos de teste. Iniciando execucao...\n")

compiler_path = os.path.join(raiz_projeto, 'compilador')
gramatica_path = os.path.join(raiz_projeto, 'gramatica-9', 'gramatica.conf')
tabela_path = os.path.join(raiz_projeto, 'gramatica-9', 'tabela_lr1.conf')

valid_passed = 0
invalid_passed = 0
total_valid = 0
total_invalid = 0

for tokens_file in tokens_files:
    nome_base = os.path.splitext(os.path.basename(tokens_file))[0]
    is_error_case = nome_base.startswith('e')
    
    params_file = os.path.join(pasta_ins, f"{nome_base}.params")
    saida_file = os.path.join(pasta_ins, f"{nome_base}.saida")
    
    # Construct running arguments
    cmd = [compiler_path, gramatica_path, tabela_path]
    if os.path.exists(params_file):
        cmd.append(params_file)
        
    # Execute the compiler
    with open(tokens_file, 'r') as infile:
        res = subprocess.run(
            cmd,
            stdin=infile,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            cwd=raiz_projeto
        )
        
    if is_error_case:
        total_invalid += 1
        # Invalid test case should fail (non-zero return code) or print semantic error in stderr/stdout
        if res.returncode != 0:
            print(f"❌ [Erro Esperado OK] {nome_base}: Rejeitado corretamente.")
            # Write both stderr and stdout to the .saida file for errors as well, so the user can inspect them
            with open(saida_file, 'w') as outfile:
                outfile.write(res.stderr)
                outfile.write(res.stdout)
            invalid_passed += 1
        else:
            print(f"🔴 [FALHA] {nome_base}: Codigo invalido foi aceito sem erro!")
            print("Stdout:", res.stdout)
            print("Stderr:", res.stderr)
    else:
        total_valid += 1
        if res.returncode == 0:
            # Write BOTH stderr (terminal trace, parsing steps, AST, etc.) and stdout (interpreted return value)
            with open(saida_file, 'w') as outfile:
                outfile.write(res.stderr)
                outfile.write(res.stdout)
            
            # Extract calculated return value from the last lines of stdout
            stdout_lines = [l for l in res.stdout.splitlines() if l.strip()]
            ret_val = stdout_lines[-1] if stdout_lines else "N/A"
            print(f"✅ [OK] {nome_base}: Sucesso. Retorno: {ret_val}")
            valid_passed += 1
        else:
            print(f"🔴 [FALHA] {nome_base}: Falhou ao compilar/executar!")
            print("Stderr:", res.stderr)

print("\n--- Resumo dos Testes ---")
print(f"Casos validos: {valid_passed}/{total_valid} passados")
print(f"Casos com erro: {invalid_passed}/{total_invalid} passados")
if valid_passed == total_valid and invalid_passed == total_invalid:
    print("🎉 Todos os testes passaram perfeitamente e as saidas foram geradas com todo o conteudo do terminal!")
else:
    print("⚠️ Alguns testes falharam.")
