#ifndef _EXP_BINOP_HPP_
#define _EXP_BINOP_HPP_
#include "Exp.hpp"
#include "Operador.hpp"
#include "../src-gram-java/Expressao.hpp"

class ExpBinop : public Exp {
public:
  Operador* op;
  Exp* esq;
  Exp* dir;  
  ExpBinop();
  void imprime() const;
  static ExpBinop* gera_Exp_Binop(Expressao* e, FrameFuncao* frame);
};

#endif
