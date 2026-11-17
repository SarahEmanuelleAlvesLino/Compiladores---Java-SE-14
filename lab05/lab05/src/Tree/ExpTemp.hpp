#ifndef _EXP_TEMP_HPP_
#define _EXP_TEMP_HPP_
#include "Exp.hpp"
#include "../src-gram-java/Expressao.hpp"
#include "../src-gram-java/FrameAcessoTemp.hpp"

class ExpTemp : public Exp {
public:
  int id;
  ExpTemp();
  virtual void imprime() const;
  static ExpTemp* gera_Exp_Temp(FrameAcessoTemp* acesso);
};

#endif
