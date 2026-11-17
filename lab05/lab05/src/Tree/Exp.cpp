#include "Exp.hpp"
#include "ExpConst.hpp"
#include "ExpBinop.hpp"
#include "ExpTemp.hpp"
#include "ExpMem.hpp"
#include "ExpCall.hpp"
#include "../src-gram-java/ExpressaoValor.hpp"
#include "../src-gram-java/ExpressaoVariavel.hpp"
#include "../src-gram-java/ExpressaoChamadaFuncao.hpp"
#include "../src-gram-java/ExpressaoSoma.hpp"
#include "../src-gram-java/ExpressaoSubtracao.hpp"
#include "../src-gram-java/ExpressaoMultiplicacao.hpp"
#include "../src-gram-java/ExpressaoDivisao.hpp"
#include "../src-gram-java/ExpressaoMod.hpp"
#include "../src-gram-java/ExpressaoMenor.hpp"
#include "../src-gram-java/ExpressaoIgualdade.hpp"
#include "../src-gram-java/FrameFuncao.hpp"
#include "../src-gram-java/FrameAcesso.hpp"
#include "../src-gram-java/FrameAcessoTemp.hpp"
#include "../src-gram-java/FrameAcessoNoFrame.hpp"
#include <iostream>

using namespace std;

Exp::Exp() { }

Exp* Exp::gera_Exp_de_Expressao(Expressao* e, FrameFuncao* frame) {
  if (e == NULL) return NULL;

  if (ExpressaoValor* ev = dynamic_cast<ExpressaoValor*>(e)) {
    return ExpConst::gera_Exp_Const(ev);
  }

  if (dynamic_cast<ExpressaoSoma*>(e)          ||
      dynamic_cast<ExpressaoSubtracao*>(e)     ||
      dynamic_cast<ExpressaoMultiplicacao*>(e) ||
      dynamic_cast<ExpressaoDivisao*>(e)       ||
      dynamic_cast<ExpressaoMod*>(e)           ||
      dynamic_cast<ExpressaoMenor*>(e)         ||
      dynamic_cast<ExpressaoIgualdade*>(e)) {
    return ExpBinop::gera_Exp_Binop(e, frame);
  }

  if (ExpressaoVariavel* ev = dynamic_cast<ExpressaoVariavel*>(e)) {
    if (ev->acesso == NULL) {
      cerr << "Exp::gera_Exp_de_Expressao: variavel '"
           << (ev->nome != NULL ? ev->nome->nome : "?")
           << "' sem FrameAcesso associado." << endl;
      return NULL;
    }

    if (FrameAcessoTemp* at = dynamic_cast<FrameAcessoTemp*>(ev->acesso)) {
      return ExpTemp::gera_Exp_Temp(at);
    }
    if (FrameAcessoNoFrame* af = dynamic_cast<FrameAcessoNoFrame*>(ev->acesso)) {
      return ExpMem::gera_Exp_Mem(af);
    }

    cerr << "Exp::gera_Exp_de_Expressao: FrameAcesso de tipo desconhecido para variavel '"
         << ev->nome->nome << "'." << endl;
    return NULL;
  }

  // Chamada de funcao -> ExpCall(NAME(nome), arg1, arg2, ...)
  if (ExpressaoChamadaFuncao* ec = dynamic_cast<ExpressaoChamadaFuncao*>(e)) {
    return ExpCall::gera_Exp_Call(ec, frame);
  }

  cerr << "Exp::gera_Exp_de_Expressao: tipo de Expressao nao suportado ainda." << endl;
  return NULL;
}
