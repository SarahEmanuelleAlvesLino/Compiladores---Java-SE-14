#ifndef _OPERADOR_MAIOR_IGUAL_HPP_
#define _OPERADOR_MAIOR_IGUAL_HPP_
#include "Operador.hpp"

class OperadorMaiorIgual : public Operador {
public:
  string simbolo() const { return "GE"; }
};

#endif
