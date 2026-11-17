#ifndef _OPERADOR_MENOR_IGUAL_HPP_
#define _OPERADOR_MENOR_IGUAL_HPP_
#include "Operador.hpp"

class OperadorMenorIgual : public Operador {
public:
  string simbolo() const { return "LE"; }
};

#endif
