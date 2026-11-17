#ifndef _EXP_CALL_HPP_
#define _EXP_CALL_HPP_
#include "Exp.hpp"
#include "../src-gram-java/Expressao.hpp"
#include "../src-gram-java/ExpressaoChamadaFuncao.hpp"
#include<vector>
using namespace std;

class ExpCall : public Exp {
public:
  Exp* nome_funcao;
  vector<Exp*> parametros;
  ExpCall();
  void imprime() const;
  static ExpCall* gera_Exp_Call(ExpressaoChamadaFuncao* e, FrameFuncao* frame);
};

#endif
