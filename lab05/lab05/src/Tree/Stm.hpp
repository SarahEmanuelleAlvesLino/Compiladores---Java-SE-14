#ifndef _STM_HPP_
#define _STM_HPP_
#include "../src-gram-java/Comando.hpp"
#include "../src-gram-java/FrameFuncao.hpp"
#include <string>

class Stm{
public:
  virtual ~Stm() {}
  virtual void imprime() const = 0;
  static Stm *gera_Stm_de_Comando(Comando *cmd, FrameFuncao *frame, int &contador_labels, const std::string &nome_funcao);
};

#endif