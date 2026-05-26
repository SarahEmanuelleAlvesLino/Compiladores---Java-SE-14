#include <algorithm>
#include<vector>
#include<string>
#include<iostream>
#include <fstream>   
#include<sstream>
#include<map>
#include<stack>
using namespace std;

#include "Gramatica.hpp"
#include "TabelaLR1.hpp"
#include "Arvore.hpp"
#include "Parser.hpp"
#include "Funcao.hpp"
#include "Analisador.hpp"


int main(int argc, char * argv[]) {
  if (argc != 3 && argc != 4 && argc != 1) {
    cerr << "Parametros nomes dos arquivos: 1) csv com gramatica, 2) csv com tabela LR1, opcional: 3) arquivo com parametros" << endl;
    return 1;
  }
  string nome_gramatica, nome_tab_lr1;
  if (argc == 1) {
    nome_gramatica = string("gramatica.conf");
    nome_tab_lr1 = string("tabela_lr1.conf");
  } else {
    nome_gramatica = string(argv[1]);
    nome_tab_lr1 = string(argv[2]);
  }

  ifstream arq_gramatica(nome_gramatica);
  ifstream arq_tabela_lr1(nome_tab_lr1);
  if (arq_tabela_lr1.fail() || arq_gramatica.fail()) {
    cerr << "Falha ao abrir arquivos: " << 
      ((arq_gramatica.fail()) ? nome_gramatica : "") << ", " << 
      ((arq_tabela_lr1.fail()) ? nome_tab_lr1 : "") << endl;
    return 1;    
  }
  Parser parser(arq_gramatica, arq_tabela_lr1);
  parser.tabela.debug();
  parser.gram.debug();

  
  Arvore_parse arv = parser.executa_parse(cin);
  cerr << "Parse executado" << endl;
  arv.debug();
  Funcao* func = Funcao::extrai_funcao(arv.raiz);
  if (func == NULL) {
    cerr << "Erro: Nao foi possivel extrair a funcao da arvore de parse" << endl;
    return 1;
  }
  func->debug();
  
  vector<ValorLiteral> parametros_passados;
  if (argc >= 4) {
    ifstream arq_params(argv[3]);
    if (arq_params.fail()) {
      cerr << "Falha ao abrir arquivo de parametros: " << argv[3] << endl;
      return 1;
    }
    string linha;
    size_t param_idx = 0;
    while (getline(arq_params, linha)) {
      if (!linha.empty() && linha.back() == '\r') {
        linha.pop_back();
      }
      if (linha.empty()) continue;
      if (param_idx >= func->parametros.size()) {
        cerr << "Erro semantico: Mais parametros fornecidos no arquivo do que o esperado pela funcao" << endl;
        return 1;
      }
      Variavel* param_var = func->parametros[param_idx];
      ValorLiteral val;
      val.tipo = param_var->tipo;
      if (param_var->tipo->valor == Tipo::INT) {
        val.valor_int = atoi(linha.c_str());
      } else if (param_var->tipo->valor == Tipo::FLOAT) {
        val.valor_float = atof(linha.c_str());
      } else if (param_var->tipo->valor == Tipo::BOOL) {
        val.valor_bool = (linha == "true" || linha == "1" || linha == "TRUE" || linha == "True");
      }
      parametros_passados.push_back(val);
      param_idx++;
    }
    if (param_idx < func->parametros.size()) {
      cerr << "Erro semantico: Menos parametros fornecidos no arquivo do que o esperado pela funcao" << endl;
      return 1;
    }
  } else {
    // Default values fallback
    for (size_t i = 0; i < func->parametros.size(); ++i) {
      Variavel* param_var = func->parametros[i];
      ValorLiteral val;
      val.tipo = param_var->tipo;
      if (param_var->tipo->valor == Tipo::INT) {
        val.valor_int = (i + 1) * 10;
      } else if (param_var->tipo->valor == Tipo::FLOAT) {
        val.valor_float = (i + 1) * 10.0f;
      } else if (param_var->tipo->valor == Tipo::BOOL) {
        val.valor_bool = true;
      }
      parametros_passados.push_back(val);
    }
  }

  Analisador ana;
  ValorLiteral ret = ana.calcula_retorno(func, parametros_passados);
  cout << "Retorno calculado:" << endl;
  cout << ret.como_string() << endl;
  return 0;
}
