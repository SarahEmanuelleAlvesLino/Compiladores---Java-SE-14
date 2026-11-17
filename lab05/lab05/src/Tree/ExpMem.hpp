#ifndef _EXP_MEM_HPP_
#define _EXP_MEM_HPP_
#include "Exp.hpp"
#include "../src-gram-java/Expressao.hpp"
#include "../src-gram-java/FrameAcessoNoFrame.hpp"

class ExpMem : public Exp {
public:
  Exp* endereco;
  ExpMem();
  void imprime() const;
  static ExpMem* gera_Exp_Mem(FrameAcessoNoFrame* acesso);
};

#endif
