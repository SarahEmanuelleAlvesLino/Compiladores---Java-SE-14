#ifndef _OPERADOR_DIFERENTE_HPP_
#define _OPERADOR_DIFERENTE_HPP_
#include "Operador.hpp"

class OperadorDiferente : public Operador {
public:
  string simbolo() const { return "NE"; }
};

#endif
