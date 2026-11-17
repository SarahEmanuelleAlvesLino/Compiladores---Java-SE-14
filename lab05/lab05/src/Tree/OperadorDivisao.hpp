#ifndef _OPERADOR_DIVISAO_HPP_
#define _OPERADOR_DIVISAO_HPP_
#include "Operador.hpp"

class OperadorDivisao : public Operador {
public:
  string simbolo() const { return "DIV"; }
};

#endif
