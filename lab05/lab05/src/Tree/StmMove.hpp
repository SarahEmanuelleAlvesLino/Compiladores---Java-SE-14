#ifndef _STM_MOVE_HPP_
#define _STM_MOVE_HPP_
#include "Stm.hpp"
#include "Exp.hpp"
#include "../src-gram-java/ComandoAtribuicao.hpp"
#include "../src-gram-java/ComandoDeclaracao.hpp"

class StmMove : public Stm {
public:
  Exp* destino;
  Exp* origem;
  StmMove();
  void imprime() const;
  static StmMove* gera_Stm_Move(ComandoAtribuicao* cmd, FrameFuncao* frame);
  static StmMove* gera_Stm_Move_Declaracao(ComandoDeclaracao* cmd, FrameFuncao* frame);
};

#endif
