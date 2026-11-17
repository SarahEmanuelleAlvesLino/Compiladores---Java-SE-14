#ifndef _EXP_TEMPFRAMEPOINTER_HPP_
#define _EXP_TEMPFRAMEPOINTER_HPP_
#include "Exp.hpp"
#include "ExpTemp.hpp"
#include "../src-gram-java/Expressao.hpp"

class ExpTempFramePointer : public ExpTemp {
public:
  void imprime() const;
  static ExpTempFramePointer* gera_Exp_TempFramePointer();
};

#endif
