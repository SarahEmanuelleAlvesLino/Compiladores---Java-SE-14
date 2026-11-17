#ifndef _OPERADOR_HPP_
#define _OPERADOR_HPP_
#include <string>
using namespace std;

class Operador {
public:
  virtual ~Operador() {}
  virtual string simbolo() const = 0;
};

#endif
