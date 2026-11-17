#ifndef _OPERADOR_SUBTRACAO_HPP_
#define _OPERADOR_SUBTRACAO_HPP_
#include "Operador.hpp"

class OperadorSubtracao : public Operador {
public:
  string simbolo() const { return "MINUS"; }
};

#endif
