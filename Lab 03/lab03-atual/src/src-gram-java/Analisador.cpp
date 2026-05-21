#include "Analisador.hpp"
#include "ExpressaoValor.hpp"
#include "ExpressaoVariavel.hpp"
#include "ExpressaoNegacao.hpp"
#include "ExpressaoSoma.hpp"
#include "ExpressaoSubtracao.hpp"
#include "ExpressaoMultiplicacao.hpp"
#include "ExpressaoDivisao.hpp"
#include "ExpressaoMod.hpp"
#include "ExpressaoIgualdade.hpp"
#include "ExpressaoMenor.hpp"
#include "ComandoDeclaracao.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoRetorno.hpp"
#include "ComandoLista.hpp"
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

// Helper to look up a variable in the lexical scope stack
static ValorLiteral* busca_variavel(const string& nome, vector<map<string, ValorLiteral>>& scopes) {
  for (int i = scopes.size() - 1; i >= 0; --i) {
    if (scopes[i].count(nome) > 0) {
      return &scopes[i][nome];
    }
  }
  return NULL;
}

// Recursive expression evaluator
static ValorLiteral avalia_expressao(Expressao* exp, vector<map<string, ValorLiteral>>& scopes) {
  if (exp == NULL) {
    cerr << "Erro: Expressao nula" << endl;
    exit(1);
  }

  if (ExpressaoValor* v_exp = dynamic_cast<ExpressaoValor*>(exp)) {
    if (v_exp->valor == NULL) {
      cerr << "Erro: Valor literal nulo" << endl;
      exit(1);
    }
    return *(v_exp->valor);
  }

  if (ExpressaoVariavel* var_exp = dynamic_cast<ExpressaoVariavel*>(exp)) {
    if (var_exp->nome == NULL) {
      cerr << "Erro: Nome de variavel nulo" << endl;
      exit(1);
    }
    ValorLiteral* var_val = busca_variavel(var_exp->nome->nome, scopes);
    if (var_val == NULL) {
      cerr << "Erro semantico: Variavel '" << var_exp->nome->nome << "' nao declarada" << endl;
      exit(1);
    }
    return *var_val;
  }

  if (ExpressaoNegacao* neg_exp = dynamic_cast<ExpressaoNegacao*>(exp)) {
    ValorLiteral inner = avalia_expressao(neg_exp->expressao, scopes);
    if (neg_exp->op == "!") {
      if (inner.tipo->valor != Tipo::BOOL) {
        cerr << "Erro semantico: Operador '!' aplicado a tipo nao-booleano" << endl;
        exit(1);
      }
      ValorLiteral res;
      res.tipo = new Tipo(Tipo::BOOL);
      res.valor_bool = !inner.valor_bool;
      return res;
    } else if (neg_exp->op == "-") {
      if (inner.tipo->valor == Tipo::INT) {
        ValorLiteral res;
        res.tipo = new Tipo(Tipo::INT);
        res.valor_int = -inner.valor_int;
        return res;
      } else if (inner.tipo->valor == Tipo::FLOAT) {
        ValorLiteral res;
        res.tipo = new Tipo(Tipo::FLOAT);
        res.valor_float = -inner.valor_float;
        return res;
      } else {
        cerr << "Erro semantico: Operador '-' unario aplicado a tipo nao-numerico" << endl;
        exit(1);
      }
    } else if (neg_exp->op == "+") {
      if (inner.tipo->valor != Tipo::INT && inner.tipo->valor != Tipo::FLOAT) {
        cerr << "Erro semantico: Operador '+' unario aplicado a tipo nao-numerico" << endl;
        exit(1);
      }
      return inner;
    }
  }

  // All other binary expressions
  Expressao* esq_node = NULL;
  Expressao* dir_node = NULL;
  string op = "";

  if (ExpressaoSoma* soma_exp = dynamic_cast<ExpressaoSoma*>(exp)) {
    esq_node = soma_exp->esquerda;
    dir_node = soma_exp->direita;
    op = "+";
  } else if (ExpressaoSubtracao* sub_exp = dynamic_cast<ExpressaoSubtracao*>(exp)) {
    esq_node = sub_exp->esquerda;
    dir_node = sub_exp->direita;
    op = "-";
  } else if (ExpressaoMultiplicacao* mult_exp = dynamic_cast<ExpressaoMultiplicacao*>(exp)) {
    esq_node = mult_exp->esquerda;
    dir_node = mult_exp->direita;
    op = "*";
  } else if (ExpressaoDivisao* div_exp = dynamic_cast<ExpressaoDivisao*>(exp)) {
    esq_node = div_exp->esquerda;
    dir_node = div_exp->direita;
    op = "/";
  } else if (ExpressaoMod* mod_exp = dynamic_cast<ExpressaoMod*>(exp)) {
    esq_node = mod_exp->esquerda;
    dir_node = mod_exp->direita;
    op = "%";
  } else if (ExpressaoIgualdade* ig_exp = dynamic_cast<ExpressaoIgualdade*>(exp)) {
    esq_node = ig_exp->esquerda;
    dir_node = ig_exp->direita;
    op = ig_exp->op;
  } else if (ExpressaoMenor* men_exp = dynamic_cast<ExpressaoMenor*>(exp)) {
    esq_node = men_exp->esquerda;
    dir_node = men_exp->direita;
    op = men_exp->op;
  }

  if (esq_node != NULL && dir_node != NULL) {
    ValorLiteral esq = avalia_expressao(esq_node, scopes);
    ValorLiteral dir = avalia_expressao(dir_node, scopes);

    if (op == "&&" || op == "||") {
      if (esq.tipo->valor != Tipo::BOOL || dir.tipo->valor != Tipo::BOOL) {
        cerr << "Erro semantico: Operador logico '" << op << "' requer operandos booleanos" << endl;
        exit(1);
      }
      ValorLiteral res;
      res.tipo = new Tipo(Tipo::BOOL);
      if (op == "&&") res.valor_bool = esq.valor_bool && dir.valor_bool;
      else res.valor_bool = esq.valor_bool || dir.valor_bool;
      return res;
    }

    if (op == "==" || op == "!=") {
      if (esq.tipo->valor == Tipo::BOOL && dir.tipo->valor == Tipo::BOOL) {
        ValorLiteral res;
        res.tipo = new Tipo(Tipo::BOOL);
        if (op == "==") res.valor_bool = esq.valor_bool == dir.valor_bool;
        else res.valor_bool = esq.valor_bool != dir.valor_bool;
        return res;
      } else if ((esq.tipo->valor == Tipo::INT || esq.tipo->valor == Tipo::FLOAT) &&
                 (dir.tipo->valor == Tipo::INT || dir.tipo->valor == Tipo::FLOAT)) {
        float f_esq = (esq.tipo->valor == Tipo::INT) ? esq.valor_int : esq.valor_float;
        float f_dir = (dir.tipo->valor == Tipo::INT) ? dir.valor_int : dir.valor_float;
        ValorLiteral res;
        res.tipo = new Tipo(Tipo::BOOL);
        if (op == "==") res.valor_bool = f_esq == f_dir;
        else res.valor_bool = f_esq != f_dir;
        return res;
      } else {
        cerr << "Erro semantico: Tipos incompativeis para comparacao de igualdade '" << op << "'" << endl;
        exit(1);
      }
    }

    if (op == "<" || op == "<=" || op == ">" || op == ">=") {
      if ((esq.tipo->valor != Tipo::INT && esq.tipo->valor != Tipo::FLOAT) ||
          (dir.tipo->valor != Tipo::INT && dir.tipo->valor != Tipo::FLOAT)) {
        cerr << "Erro semantico: Operador relacional '" << op << "' requer operandos numericos" << endl;
        exit(1);
      }
      float f_esq = (esq.tipo->valor == Tipo::INT) ? esq.valor_int : esq.valor_float;
      float f_dir = (dir.tipo->valor == Tipo::INT) ? dir.valor_int : dir.valor_float;
      ValorLiteral res;
      res.tipo = new Tipo(Tipo::BOOL);
      if (op == "<") res.valor_bool = f_esq < f_dir;
      else if (op == "<=") res.valor_bool = f_esq <= f_dir;
      else if (op == ">") res.valor_bool = f_esq > f_dir;
      else if (op == ">=") res.valor_bool = f_esq >= f_dir;
      return res;
    }

    // Arithmetic operators
    if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
      if ((esq.tipo->valor != Tipo::INT && esq.tipo->valor != Tipo::FLOAT) ||
          (dir.tipo->valor != Tipo::INT && dir.tipo->valor != Tipo::FLOAT)) {
        cerr << "Erro semantico: Operador aritmetico '" << op << "' requer operandos numericos" << endl;
        exit(1);
      }
      if (esq.tipo->valor == Tipo::INT && dir.tipo->valor == Tipo::INT) {
        ValorLiteral res;
        res.tipo = new Tipo(Tipo::INT);
        if (op == "+") res.valor_int = esq.valor_int + dir.valor_int;
        else if (op == "-") res.valor_int = esq.valor_int - dir.valor_int;
        else if (op == "*") res.valor_int = esq.valor_int * dir.valor_int;
        else if (op == "/") {
          if (dir.valor_int == 0) {
            cerr << "Erro: Divisao por zero" << endl;
            exit(1);
          }
          res.valor_int = esq.valor_int / dir.valor_int;
        }
        else if (op == "%") {
          if (dir.valor_int == 0) {
            cerr << "Erro: Modulo por zero" << endl;
            exit(1);
          }
          res.valor_int = esq.valor_int % dir.valor_int;
        }
        return res;
      } else {
        float f_esq = (esq.tipo->valor == Tipo::INT) ? esq.valor_int : esq.valor_float;
        float f_dir = (dir.tipo->valor == Tipo::INT) ? dir.valor_int : dir.valor_float;
        ValorLiteral res;
        res.tipo = new Tipo(Tipo::FLOAT);
        if (op == "+") res.valor_float = f_esq + f_dir;
        else if (op == "-") res.valor_float = f_esq - f_dir;
        else if (op == "*") res.valor_float = f_esq * f_dir;
        else if (op == "/") {
          if (f_dir == 0.0f) {
            cerr << "Erro: Divisao por zero" << endl;
            exit(1);
          }
          res.valor_float = f_esq / f_dir;
        }
        else if (op == "%") {
          if (f_dir == 0.0f) {
            cerr << "Erro: Modulo por zero" << endl;
            exit(1);
          }
          res.valor_float = fmod(f_esq, f_dir);
        }
        return res;
      }
    }
  }

  cerr << "Erro: Tipo de expressao desconhecido" << endl;
  exit(1);
}

// Helper to execute a command
static void executa_comando(Comando* cmd, vector<map<string, ValorLiteral>>& scopes, Tipo* tipo_retorno_funcao, ValorLiteral& ultimo_valor_atribuido) {
  if (cmd == NULL) return;

  if (ComandoDeclaracao* dec_cmd = dynamic_cast<ComandoDeclaracao*>(cmd)) {
    Variavel* v = dec_cmd->variavel;
    if (v == NULL) return;
    if (scopes.back().count(v->nome->nome) > 0) {
      cerr << "Erro semantico: Variavel '" << v->nome->nome << "' ja declarada neste escopo" << endl;
      exit(1);
    }
    ValorLiteral val;
    val.tipo = v->tipo;
    // Initial default values
    if (v->tipo->valor == Tipo::INT) val.valor_int = 0;
    else if (v->tipo->valor == Tipo::FLOAT) val.valor_float = 0.0f;
    else if (v->tipo->valor == Tipo::BOOL) val.valor_bool = false;

    scopes.back()[v->nome->nome] = val;
    return;
  }

  if (ComandoAtribuicao* atr_cmd = dynamic_cast<ComandoAtribuicao*>(cmd)) {
    ValorLiteral val = avalia_expressao(atr_cmd->direita, scopes);
    ValorLiteral* var_val = busca_variavel(atr_cmd->esquerda->nome, scopes);
    if (var_val == NULL) {
      cerr << "Erro semantico: Atribuicao a variavel '" << atr_cmd->esquerda->nome << "' nao declarada" << endl;
      exit(1);
    }

    if (var_val->tipo->valor == Tipo::INT) {
      if (val.tipo->valor != Tipo::INT) {
        cerr << "Erro semantico: Atribuicao de tipo incompativel para variavel '" << atr_cmd->esquerda->nome << "'" << endl;
        exit(1);
      }
      var_val->valor_int = val.valor_int;
    } else if (var_val->tipo->valor == Tipo::FLOAT) {
      if (val.tipo->valor == Tipo::INT) {
        var_val->valor_float = val.valor_int; // implicit promotion
      } else if (val.tipo->valor == Tipo::FLOAT) {
        var_val->valor_float = val.valor_float;
      } else {
        cerr << "Erro semantico: Atribuicao de tipo incompativel para variavel '" << atr_cmd->esquerda->nome << "'" << endl;
        exit(1);
      }
    } else if (var_val->tipo->valor == Tipo::BOOL) {
      if (val.tipo->valor != Tipo::BOOL) {
        cerr << "Erro semantico: Atribuicao de tipo incompativel para variavel '" << atr_cmd->esquerda->nome << "'" << endl;
        exit(1);
      }
      var_val->valor_bool = val.valor_bool;
    }

    ultimo_valor_atribuido = *var_val;
    return;
  }

  if (ComandoRetorno* ret_cmd = dynamic_cast<ComandoRetorno*>(cmd)) {
    ValorLiteral val = avalia_expressao(ret_cmd->expressao, scopes);
    
    // Type check return value against function signature
    if (tipo_retorno_funcao->valor == Tipo::INT) {
      if (val.tipo->valor != Tipo::INT) {
        cerr << "Erro semantico: Retorno de tipo incompativel" << endl;
        exit(1);
      }
    } else if (tipo_retorno_funcao->valor == Tipo::FLOAT) {
      if (val.tipo->valor == Tipo::INT) {
        // promote to float
        ValorLiteral prom;
        prom.tipo = new Tipo(Tipo::FLOAT);
        prom.valor_float = val.valor_int;
        val = prom;
      } else if (val.tipo->valor != Tipo::FLOAT) {
        cerr << "Erro semantico: Retorno de tipo incompativel" << endl;
        exit(1);
      }
    } else if (tipo_retorno_funcao->valor == Tipo::BOOL) {
      if (val.tipo->valor != Tipo::BOOL) {
        cerr << "Erro semantico: Retorno de tipo incompativel" << endl;
        exit(1);
      }
    }

    ultimo_valor_atribuido = val;
    return;
  }

  if (ComandoLista* lst_cmd = dynamic_cast<ComandoLista*>(cmd)) {
    scopes.push_back(map<string, ValorLiteral>());
    for (size_t i = 0; i < lst_cmd->lista_comandos.size(); ++i) {
      executa_comando(lst_cmd->lista_comandos[i], scopes, tipo_retorno_funcao, ultimo_valor_atribuido);
    }
    scopes.pop_back();
    return;
  }
}

// Simulates a function execution and returns the last assigned value
ValorLiteral Analisador::calcula_retorno(Funcao *f, const vector<ValorLiteral> &params) {
  if (f == NULL) {
    cerr << "Erro: Funcao nula" << endl;
    exit(1);
  }

  vector<map<string, ValorLiteral>> scopes;
  scopes.push_back(map<string, ValorLiteral>()); // Outermost function parameter scope

  if (params.size() != f->parametros.size()) {
    cerr << "Erro semantico: Numero incorreto de parametros passados" << endl;
    exit(1);
  }

  for (size_t i = 0; i < f->parametros.size(); ++i) {
    Variavel* param_var = f->parametros[i];
    ValorLiteral val = params[i];

    // Validate type of passed parameter against declaration
    if (param_var->tipo->valor == Tipo::INT) {
      if (val.tipo->valor != Tipo::INT) {
        cerr << "Erro semantico: Tipo de parametro incorreto para '" << param_var->nome->nome << "'" << endl;
        exit(1);
      }
    } else if (param_var->tipo->valor == Tipo::FLOAT) {
      if (val.tipo->valor == Tipo::INT) {
        // promote
        ValorLiteral prom;
        prom.tipo = new Tipo(Tipo::FLOAT);
        prom.valor_float = val.valor_int;
        val = prom;
      } else if (val.tipo->valor != Tipo::FLOAT) {
        cerr << "Erro semantico: Tipo de parametro incorreto para '" << param_var->nome->nome << "'" << endl;
        exit(1);
      }
    } else if (param_var->tipo->valor == Tipo::BOOL) {
      if (val.tipo->valor != Tipo::BOOL) {
        cerr << "Erro semantico: Tipo de parametro incorreto para '" << param_var->nome->nome << "'" << endl;
        exit(1);
      }
    }

    scopes.back()[param_var->nome->nome] = val;
  }

  ValorLiteral ultimo_valor_atribuido;
  // Initialize with default of function type in case no assignments occur
  ultimo_valor_atribuido.tipo = f->tipo_retorno;
  if (f->tipo_retorno->valor == Tipo::INT) ultimo_valor_atribuido.valor_int = 0;
  else if (f->tipo_retorno->valor == Tipo::FLOAT) ultimo_valor_atribuido.valor_float = 0.0f;
  else if (f->tipo_retorno->valor == Tipo::BOOL) ultimo_valor_atribuido.valor_bool = false;

  for (size_t i = 0; i < f->comandos.size(); ++i) {
    executa_comando(f->comandos[i], scopes, f->tipo_retorno, ultimo_valor_atribuido);
  }

  return ultimo_valor_atribuido;
}
