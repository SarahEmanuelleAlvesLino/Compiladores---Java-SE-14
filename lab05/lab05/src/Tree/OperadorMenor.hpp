#ifndef _OPERADOR_MENOR_HPP_
#define _OPERADOR_MENOR_HPP_
#include "Operador.hpp"

class OperadorMenor : public Operador {
public:
  string simbolo() const { return "LT"; }
};

#endif
