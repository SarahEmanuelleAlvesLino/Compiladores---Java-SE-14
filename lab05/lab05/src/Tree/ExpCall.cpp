#include "ExpCall.hpp"
#include "ExpName.hpp"
#include "../src-gram-java/ID.hpp"
#include <iostream>

using namespace std;

ExpCall::ExpCall() {
  nome_funcao = NULL;
}

void ExpCall::imprime() const {
  cout << "CALL(";
  if (nome_funcao != NULL) nome_funcao->imprime(); else cout << "NAME(?)";
  for (size_t i = 0; i < parametros.size(); ++i) {
    cout << ", ";
    if (parametros[i] != NULL) parametros[i]->imprime(); else cout << "EXP_NAO_SUPORTADA";
  }
  cout << ")";
}

ExpCall* ExpCall::gera_Exp_Call(ExpressaoChamadaFuncao* e, FrameFuncao* frame) {
  if (e == NULL) return NULL;

  ExpCall* res = new ExpCall();

  string nome = (e->nome_funcao != NULL) ? e->nome_funcao->nome : "?";
  res->nome_funcao = ExpName::gera_Exp_Name(nome);

  for (size_t i = 0; i < e->parametros.size(); ++i) {
    res->parametros.push_back(Exp::gera_Exp_de_Expressao(e->parametros[i], frame));
  }

  return res;
}
