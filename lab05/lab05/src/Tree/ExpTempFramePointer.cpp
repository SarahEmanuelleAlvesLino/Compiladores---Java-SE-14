#include "ExpTempFramePointer.hpp"
#include <iostream>

using namespace std;

void ExpTempFramePointer::imprime() const {
  cout << "TEMP(FP)";
}

static const int ID_FRAME_POINTER = 0;

ExpTempFramePointer* ExpTempFramePointer::gera_Exp_TempFramePointer() {
  ExpTempFramePointer* res = new ExpTempFramePointer();
  res->id = ID_FRAME_POINTER;
  return res;
}
