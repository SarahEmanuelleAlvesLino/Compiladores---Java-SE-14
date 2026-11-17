#ifndef _STM_SEQ_HPP_
#define _STM_SEQ_HPP_
#include "Stm.hpp"

class StmSeq : public Stm {
public:
  Stm* esq;
  Stm* dir;
  StmSeq();
  void imprime() const;
  static StmSeq* gera_Stm_Seq(Stm* esq, Stm* dir);
};

#endif
