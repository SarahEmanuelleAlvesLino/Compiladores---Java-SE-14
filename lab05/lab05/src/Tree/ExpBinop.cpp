#include "ExpBinop.hpp"
#include "Exp.hpp"
#include "OperadorSoma.hpp"
#include "OperadorSubtracao.hpp"
#include "OperadorMultiplicacao.hpp"
#include "OperadorDivisao.hpp"
#include "OperadorMod.hpp"
#include "OperadorMenor.hpp"
#include "OperadorMenorIgual.hpp"
#include "OperadorMaior.hpp"
#include "OperadorMaiorIgual.hpp"
#include "OperadorIgual.hpp"
#include "OperadorDiferente.hpp"
#include "OperadorAnd.hpp"
#include "OperadorOr.hpp"
#include "../src-gram-java/ExpressaoSoma.hpp"
#include "../src-gram-java/ExpressaoSubtracao.hpp"
#include "../src-gram-java/ExpressaoMultiplicacao.hpp"
#include "../src-gram-java/ExpressaoDivisao.hpp"
#include "../src-gram-java/ExpressaoMod.hpp"
#include "../src-gram-java/ExpressaoMenor.hpp"
#include "../src-gram-java/ExpressaoIgualdade.hpp"
#include <iostream>

using namespace std;

ExpBinop::ExpBinop() {
  op = NULL;
  esq = NULL;
  dir = NULL;
}

void ExpBinop::imprime() const {
  cout << "BINOP(" << (op != NULL ? op->simbolo() : "?") << ", ";
  if (esq != NULL) esq->imprime(); else cout << "EXP_NAO_SUPORTADA";
  cout << ", ";
  if (dir != NULL) dir->imprime(); else cout << "EXP_NAO_SUPORTADA";
  cout << ")";
}

ExpBinop* ExpBinop::gera_Exp_Binop(Expressao* e, FrameFuncao* frame) {
  ExpBinop* res = new ExpBinop();

  if (ExpressaoSoma* es = dynamic_cast<ExpressaoSoma*>(e)) {
    res->op = new OperadorSoma();
    res->esq = Exp::gera_Exp_de_Expressao(es->esquerda, frame);
    res->dir = Exp::gera_Exp_de_Expressao(es->direita, frame);
    return res;
  }

  if (ExpressaoSubtracao* es = dynamic_cast<ExpressaoSubtracao*>(e)) {
    res->op = new OperadorSubtracao();
    res->esq = Exp::gera_Exp_de_Expressao(es->esquerda, frame);
    res->dir = Exp::gera_Exp_de_Expressao(es->direita, frame);
    return res;
  }

  if (ExpressaoMultiplicacao* es = dynamic_cast<ExpressaoMultiplicacao*>(e)) {
    res->op = new OperadorMultiplicacao();
    res->esq = Exp::gera_Exp_de_Expressao(es->esquerda, frame);
    res->dir = Exp::gera_Exp_de_Expressao(es->direita, frame);
    return res;
  }

  if (ExpressaoDivisao* es = dynamic_cast<ExpressaoDivisao*>(e)) {
    res->op = new OperadorDivisao();
    res->esq = Exp::gera_Exp_de_Expressao(es->esquerda, frame);
    res->dir = Exp::gera_Exp_de_Expressao(es->direita, frame);
    return res;
  }

  if (ExpressaoMod* es = dynamic_cast<ExpressaoMod*>(e)) {
    res->op = new OperadorMod();
    res->esq = Exp::gera_Exp_de_Expressao(es->esquerda, frame);
    res->dir = Exp::gera_Exp_de_Expressao(es->direita, frame);
    return res;
  }

  if (ExpressaoMenor* em = dynamic_cast<ExpressaoMenor*>(e)) {
    if (em->op == "<") {
      res->op = new OperadorMenor();
    } else if (em->op == "<=") {
      res->op = new OperadorMenorIgual();
    } else if (em->op == ">") {
      res->op = new OperadorMaior();
    } else if (em->op == ">=") {
      res->op = new OperadorMaiorIgual();
    } else {
      cerr << "ExpBinop::gera_Exp_Binop: operador relacional desconhecido: " << em->op << endl;
      delete res;
      return NULL;
    }
    res->esq = Exp::gera_Exp_de_Expressao(em->esquerda, frame);
    res->dir = Exp::gera_Exp_de_Expressao(em->direita, frame);
    return res;
  }

  if (ExpressaoIgualdade* ei = dynamic_cast<ExpressaoIgualdade*>(e)) {
    if (ei->op == "==") {
      res->op = new OperadorIgual();
    } else if (ei->op == "!=") {
      res->op = new OperadorDiferente();
    } else if (ei->op == "&&") {
      res->op = new OperadorAnd();
    } else if (ei->op == "||") {
      res->op = new OperadorOr();
    } else {
      cerr << "ExpBinop::gera_Exp_Binop: operador logico/igualdade desconhecido: " << ei->op << endl;
      delete res;
      return NULL;
    }
    res->esq = Exp::gera_Exp_de_Expressao(ei->esquerda, frame);
    res->dir = Exp::gera_Exp_de_Expressao(ei->direita, frame);
    return res;
  }

  cerr << "ExpBinop::gera_Exp_Binop: tipo de Expressao nao e uma operacao binaria conhecida." << endl;
  delete res;
  return NULL;
}
