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
#include "ComandoIf.hpp"
#include "ComandoRetorno.hpp"
#include "ComandoLista.hpp"
#include "ComandoWhile.hpp"
#include "ExpressaoChamadaFuncao.hpp"
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

static ValorLiteral* busca_variavel(const string& nome, vector<map<string, ValorLiteral>>& scopes) {
  for (int i = scopes.size() - 1; i >= 0; --i) {
    if (scopes[i].count(nome) > 0) {
      return &scopes[i][nome];
    }
  }
  return NULL;
}

// Declarações antecipadas para permitir chamadas recursivas mútuas
static ValorLiteral avalia_expressao(Expressao* exp, vector<map<string, ValorLiteral>>& scopes, const map<string, Funcao*>& tabela_funcoes);
static void executa_comando(Comando* cmd, vector<map<string, ValorLiteral>>& scopes, Tipo* tipo_retorno_funcao, ValorLiteral& ultimo_valor_atribuido,const map<string, Funcao*>& tabela_funcoes);

static ValorLiteral avalia_expressao(Expressao* exp, vector<map<string, ValorLiteral>>& scopes, const map<string, Funcao*>& tabela_funcoes) {
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
    ValorLiteral inner = avalia_expressao(neg_exp->expressao, scopes, tabela_funcoes);
    if (neg_exp->op == "!") {
      if (inner.is_mock) {
        ValorLiteral mock;
        mock.tipo = new Tipo(Tipo::BOOL);
        mock.is_mock = true;
        return mock;
      }
      if (inner.tipo->valor != Tipo::BOOL) {
        cerr << "Erro semantico: Operador '!' aplicado a tipo nao-booleano" << endl;
        exit(1);
      }
      ValorLiteral res;
      res.tipo = new Tipo(Tipo::BOOL);
      res.valor_bool = !inner.valor_bool;
      return res;
    } else if (neg_exp->op == "-") {
      if (inner.is_mock) return inner;
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

  // Chamada de função: avalia argumentos no escopo atual, executa o corpo da função chamada
  if (ExpressaoChamadaFuncao* call_exp = dynamic_cast<ExpressaoChamadaFuncao*>(exp)) {
    if (call_exp->nome_funcao == NULL) {
      cerr << "Erro: Nome de funcao nulo em chamada" << endl;
      exit(1);
    }
    const string& nome_chamada = call_exp->nome_funcao->nome;
    map<string, Funcao*>::const_iterator it = tabela_funcoes.find(nome_chamada);

    // Função fantasma (não declarada ou assinatura incompatível): retorna mock neutro
    if (it == tabela_funcoes.end()) {
      ValorLiteral mock;
      mock.tipo = new Tipo(Tipo::INT);
      mock.is_mock = true;
      return mock;
    }
    Funcao* f_chamada = it->second;
    if (call_exp->parametros.size() != f_chamada->parametros.size()) {
      ValorLiteral mock;
      mock.tipo = new Tipo(Tipo::INT);
      mock.is_mock = true;
      return mock;
    }

    vector<ValorLiteral> args;
    for (size_t i = 0; i < call_exp->parametros.size(); ++i) {
      args.push_back(avalia_expressao(call_exp->parametros[i], scopes, tabela_funcoes));
    }

    vector<map<string, ValorLiteral>> callee_scopes;
    callee_scopes.push_back(map<string, ValorLiteral>());

    for (size_t i = 0; i < f_chamada->parametros.size(); ++i) {
      Variavel* pv = f_chamada->parametros[i];
      ValorLiteral val = args[i];
      if (pv->tipo->valor == Tipo::FLOAT && val.tipo->valor == Tipo::INT) {
        ValorLiteral prom;
        prom.tipo = new Tipo(Tipo::FLOAT);
        prom.valor_float = (float)val.valor_int;
        val = prom;
      }
      callee_scopes.back()[pv->nome->nome] = val;
    }

    ValorLiteral retorno;
    retorno.tipo = f_chamada->tipo_retorno;
    if (f_chamada->tipo_retorno->valor == Tipo::INT) retorno.valor_int = 0;
    else if (f_chamada->tipo_retorno->valor == Tipo::FLOAT) retorno.valor_float = 0.0f;
    else if (f_chamada->tipo_retorno->valor == Tipo::BOOL) retorno.valor_bool = false;

    for (size_t i = 0; i < f_chamada->comandos.size(); ++i) {
      executa_comando(f_chamada->comandos[i], callee_scopes, f_chamada->tipo_retorno, retorno, tabela_funcoes);
    }

    return retorno;
  }

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
    ValorLiteral esq = avalia_expressao(esq_node, scopes, tabela_funcoes);
    ValorLiteral dir = avalia_expressao(dir_node, scopes, tabela_funcoes);

    if (op == "&&" || op == "||") {
      if (esq.is_mock || dir.is_mock) {
        ValorLiteral mock; mock.tipo = new Tipo(Tipo::BOOL); mock.is_mock = true; return mock;
      }
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
      if (esq.is_mock || dir.is_mock) {
        ValorLiteral mock; mock.tipo = new Tipo(Tipo::BOOL); mock.is_mock = true; return mock;
      }
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
      if (esq.is_mock || dir.is_mock) {
        ValorLiteral mock; mock.tipo = new Tipo(Tipo::BOOL); mock.is_mock = true; return mock;
      }
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

    if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
      if (esq.is_mock || dir.is_mock) {
        ValorLiteral mock; mock.tipo = new Tipo(Tipo::INT); mock.is_mock = true; return mock;
      }
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

static void executa_comando(Comando* cmd, vector<map<string, ValorLiteral>>& scopes,Tipo* tipo_retorno_funcao, ValorLiteral& ultimo_valor_atribuido,const map<string, Funcao*>& tabela_funcoes) {
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
    if (v->tipo->valor == Tipo::INT) val.valor_int = 0;
    else if (v->tipo->valor == Tipo::FLOAT) val.valor_float = 0.0f;
    else if (v->tipo->valor == Tipo::BOOL) val.valor_bool = false;

    scopes.back()[v->nome->nome] = val;
    return;
  }

  if (ComandoAtribuicao* atr_cmd = dynamic_cast<ComandoAtribuicao*>(cmd)) {
    ValorLiteral val = avalia_expressao(atr_cmd->direita, scopes, tabela_funcoes);
    if (val.is_mock) return;
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
        var_val->valor_float = (float)val.valor_int;
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
    return;
  }

  if (ComandoRetorno* ret_cmd = dynamic_cast<ComandoRetorno*>(cmd)) {
    ValorLiteral val = avalia_expressao(ret_cmd->expressao, scopes, tabela_funcoes);

    if (val.is_mock) {
      // Mock de função fantasma: substitui pelo valor zero do tipo de retorno declarado
      val.is_mock = false;
      val.tipo = tipo_retorno_funcao;
      val.valor_int = 0; val.valor_float = 0.0f; val.valor_bool = false;
      ultimo_valor_atribuido = val;
      return;
    }

    if (tipo_retorno_funcao->valor == Tipo::INT) {
      if (val.tipo->valor != Tipo::INT) {
        cerr << "Erro semantico: Retorno de tipo incompativel" << endl;
        exit(1);
      }
    } else if (tipo_retorno_funcao->valor == Tipo::FLOAT) {
      if (val.tipo->valor == Tipo::INT) {
        ValorLiteral prom;
        prom.tipo = new Tipo(Tipo::FLOAT);
        prom.valor_float = (float)val.valor_int;
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
      executa_comando(lst_cmd->lista_comandos[i], scopes, tipo_retorno_funcao, ultimo_valor_atribuido, tabela_funcoes);
    }
    scopes.pop_back();
    return;
  }

  if (ComandoIf* ci = dynamic_cast<ComandoIf*>(cmd)) {
    ValorLiteral cond = avalia_expressao(ci->condicao, scopes, tabela_funcoes);
    if (cond.is_mock) {
      return;
    } else if (cond.valor_bool) {
      executa_comando(ci->bloco_true, scopes, tipo_retorno_funcao, ultimo_valor_atribuido, tabela_funcoes);
    } else if (ci->bloco_false != NULL) {
      executa_comando(ci->bloco_false, scopes, tipo_retorno_funcao, ultimo_valor_atribuido, tabela_funcoes);
    }
    return;
  }

  if (ComandoWhile* cw = dynamic_cast<ComandoWhile*>(cmd)) {
    (void)cw;
    return;
  }
}

ValorLiteral Analisador::calcula_retorno(Funcao *f, const vector<ValorLiteral> &params,const map<string, Funcao*>& tabela_funcoes) {
  if (f == NULL) {
    cerr << "Erro: Funcao nula" << endl;
    exit(1);
  }

  vector<map<string, ValorLiteral>> scopes;
  scopes.push_back(map<string, ValorLiteral>());

  if (params.size() != f->parametros.size()) {
    cerr << "Erro semantico: Numero incorreto de parametros passados" << endl;
    exit(1);
  }

  for (size_t i = 0; i < f->parametros.size(); ++i) {
    Variavel* param_var = f->parametros[i];
    ValorLiteral val = params[i];

    if (param_var->tipo->valor == Tipo::INT) {
      if (val.tipo->valor != Tipo::INT) {
        cerr << "Erro semantico: Tipo de parametro incorreto para '" << param_var->nome->nome << "'" << endl;
        exit(1);
      }
    } else if (param_var->tipo->valor == Tipo::FLOAT) {
      if (val.tipo->valor == Tipo::INT) {
        ValorLiteral prom;
        prom.tipo = new Tipo(Tipo::FLOAT);
        prom.valor_float = (float)val.valor_int;
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
  ultimo_valor_atribuido.tipo = f->tipo_retorno;
  if (f->tipo_retorno->valor == Tipo::INT) ultimo_valor_atribuido.valor_int = 0;
  else if (f->tipo_retorno->valor == Tipo::FLOAT) ultimo_valor_atribuido.valor_float = 0.0f;
  else if (f->tipo_retorno->valor == Tipo::BOOL) ultimo_valor_atribuido.valor_bool = false;

  for (size_t i = 0; i < f->comandos.size(); ++i) {
    executa_comando(f->comandos[i], scopes, f->tipo_retorno, ultimo_valor_atribuido, tabela_funcoes);
  }

  return ultimo_valor_atribuido;
}
