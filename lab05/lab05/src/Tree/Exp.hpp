#ifndef _EXP_HPP_
#define _EXP_HPP_
#include "../src-gram-java/Expressao.hpp"
#include "../src-gram-java/FrameFuncao.hpp"
using namespace std;

class Exp {
public:
  Exp();
  virtual ~Exp() {}
  virtual void imprime() const = 0;
  static Exp* gera_Exp_de_Expressao(Expressao* e, FrameFuncao* frame);
};

#endif
