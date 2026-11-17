#ifndef _OPERADOR_IGUAL_HPP_
#define _OPERADOR_IGUAL_HPP_
#include "Operador.hpp"

class OperadorIgual : public Operador {
public:
  string simbolo() const { return "EQ"; }
};

#endif
