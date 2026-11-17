#include "Expressao.hpp"
#include "ExpressaoDivisao.hpp"
#include "ExpressaoIgualdade.hpp"
#include "ExpressaoMenor.hpp"
#include "ExpressaoMod.hpp"
#include "ExpressaoMultiplicacao.hpp"
#include "ExpressaoNegacao.hpp"
#include "ExpressaoSoma.hpp"
#include "ExpressaoSubtracao.hpp"
#include "ExpressaoValor.hpp"
#include "ExpressaoVariavel.hpp"
#include "ExpressaoChamadaFuncao.hpp"
#include <iostream>
#include <stack>
#include "../debug-util.hpp"

using namespace std;

Expressao* Expressao::extrai_expressao(No_arv_parse* no) {
  if (no == NULL) return NULL;

  switch(no->regra) {
  case 65: { // Expressao -> LogicaOu (0-based: 65)
    return extrai_expressao(no->filhos[0]);
  }
  case 66: { // LogicaOu -> LogicaOu LOGIC_OR LogicaE (0-based: 66)
    ExpressaoIgualdade* res = new ExpressaoIgualdade();
    res->op = "||";
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 67: { // LogicaOu -> LogicaE (0-based: 67)
    return extrai_expressao(no->filhos[0]);
  }
  case 68: { // LogicaE -> LogicaE LOGIC_AND Igualdade (0-based: 68)
    ExpressaoIgualdade* res = new ExpressaoIgualdade();
    res->op = "&&";
    res->esquerda = extrai_expressao(no->filhos[0]);
    res->direita = extrai_expressao(no->filhos[2]);
    return res;
  }
  case 69: { // LogicaE -> Igualdade (0-based: 69)
    return extrai_expressao(no->filhos[0]);
  }
  case 70: { // Igualdade -> Igualdade EQ Relacional (0-based: 70)
    ExpressaoIgualdade* res70 = new ExpressaoIgualdade();
    res70->op = "==";
    res70->esquerda = extrai_expressao(no->filhos[0]);
    res70->direita = extrai_expressao(no->filhos[2]);
    return res70;
  }
  case 71: { // Igualdade -> Igualdade NEQ Relacional (0-based: 71)
    ExpressaoIgualdade* res71 = new ExpressaoIgualdade();
    res71->op = "!=";
    res71->esquerda = extrai_expressao(no->filhos[0]);
    res71->direita = extrai_expressao(no->filhos[2]);
    return res71;
  }
  case 72: { // Igualdade -> Relacional (0-based: 72)
    return extrai_expressao(no->filhos[0]);
  }
  case 73: { // Relacional -> Relacional LT Aditiva (0-based: 73)
    ExpressaoMenor* res73 = new ExpressaoMenor();
    res73->op = "<";
    res73->esquerda = extrai_expressao(no->filhos[0]);
    res73->direita = extrai_expressao(no->filhos[2]);
    return res73;
  }
  case 74: { // Relacional -> Relacional LEQ Aditiva (0-based: 74)
    ExpressaoMenor* res74 = new ExpressaoMenor();
    res74->op = "<=";
    res74->esquerda = extrai_expressao(no->filhos[0]);
    res74->direita = extrai_expressao(no->filhos[2]);
    return res74;
  }
  case 75: { // Relacional -> Relacional GT Aditiva (0-based: 75)
    ExpressaoMenor* res75 = new ExpressaoMenor();
    res75->op = ">";
    res75->esquerda = extrai_expressao(no->filhos[0]);
    res75->direita = extrai_expressao(no->filhos[2]);
    return res75;
  }
  case 76: { // Relacional -> Relacional GEQ Aditiva (0-based: 76)
    ExpressaoMenor* res76 = new ExpressaoMenor();
    res76->op = ">=";
    res76->esquerda = extrai_expressao(no->filhos[0]);
    res76->direita = extrai_expressao(no->filhos[2]);
    return res76;
  }
  case 77: { // Relacional -> Aditiva (0-based: 77)
    return extrai_expressao(no->filhos[0]);
  }
  case 78: { // Aditiva -> Aditiva PLUS MultiDivRes (0-based: 78)
    ExpressaoSoma* res78 = new ExpressaoSoma();
    res78->esquerda = extrai_expressao(no->filhos[0]);
    res78->direita = extrai_expressao(no->filhos[2]);
    return res78;
  }
  case 79: { // Aditiva -> Aditiva MINUS MultiDivRes (0-based: 79)
    ExpressaoSubtracao* res79 = new ExpressaoSubtracao();
    res79->esquerda = extrai_expressao(no->filhos[0]);
    res79->direita = extrai_expressao(no->filhos[2]);
    return res79;
  }
  case 80: { // Aditiva -> MultiDivRes (0-based: 80)
    return extrai_expressao(no->filhos[0]);
  }
  case 81: { // MultiDivRes -> MultiDivRes STAR Unaria (0-based: 81)
    ExpressaoMultiplicacao* res81 = new ExpressaoMultiplicacao();
    res81->esquerda = extrai_expressao(no->filhos[0]);
    res81->direita = extrai_expressao(no->filhos[2]);
    return res81;
  }
  case 82: { // MultiDivRes -> MultiDivRes SLASH Unaria (0-based: 82)
    ExpressaoDivisao* res82 = new ExpressaoDivisao();
    res82->esquerda = extrai_expressao(no->filhos[0]);
    res82->direita = extrai_expressao(no->filhos[2]);
    return res82;
  }
  case 83: { // MultiDivRes -> MultiDivRes MOD Unaria (0-based: 83)
    ExpressaoMod* res83 = new ExpressaoMod();
    res83->esquerda = extrai_expressao(no->filhos[0]);
    res83->direita = extrai_expressao(no->filhos[2]);
    return res83;
  }
  case 84: { // MultiDivRes -> Unaria (0-based: 84)
    return extrai_expressao(no->filhos[0]);
  }
  case 85: { // Unaria -> EXCLAMATION Unaria (0-based: 85)
    ExpressaoNegacao* res85 = new ExpressaoNegacao();
    res85->op = "!";
    res85->expressao = extrai_expressao(no->filhos[1]);
    return res85;
  }
  case 86: { // Unaria -> MINUS Unaria (0-based: 86)
    ExpressaoNegacao* res86 = new ExpressaoNegacao();
    res86->op = "-";
    res86->expressao = extrai_expressao(no->filhos[1]);
    return res86;
  }
  case 87: { // Unaria -> PLUS Unaria (0-based: 87)
    ExpressaoNegacao* res87 = new ExpressaoNegacao();
    res87->op = "+";
    res87->expressao = extrai_expressao(no->filhos[1]);
    return res87;
  }
  case 91: { // Unaria -> Posfixa (0-based: 91)
    return extrai_expressao(no->filhos[0]);
  }
  case 96: { // Posfixa -> Primeiro (0-based: 96)
    return extrai_expressao(no->filhos[0]);
  }
  case 99:
  { // Posfixa -> Posfixa LPAR OpcionalListaArg RPAR (0-based: 99)
    ExpressaoChamadaFuncao *exp_call = new ExpressaoChamadaFuncao();

    // O nome da funcao vem do primeiro filho (Posfixa)
    ExpressaoVariavel *exp_var = dynamic_cast<ExpressaoVariavel *>(Expressao::extrai_expressao(no->filhos[0]));
    if (exp_var)
    {
      exp_call->nome_funcao = exp_var->nome;
      delete exp_var; 
    }

    
    No_arv_parse *lista_args = no->filhos[2];
    if (lista_args->regra == 111)
    {
      No_arv_parse *args_node = lista_args->filhos[0];
     
      stack<No_arv_parse *> args_stack;
      args_stack.push(args_node);
      while (!args_stack.empty())
      {
        No_arv_parse *curr = args_stack.top();
        args_stack.pop();
        if (curr->regra == 113)
        {                                  
          args_stack.push(curr->filhos[0]); 
          exp_call->parametros.insert(exp_call->parametros.begin(), Expressao::extrai_expressao(curr->filhos[2]));
        }
        else if (curr->regra == 114)
        { // ListaArg -> Expressao
          exp_call->parametros.insert(exp_call->parametros.begin(), Expressao::extrai_expressao(curr->filhos[0]));
        }
      }
    }
    return exp_call;
  }
  case 100: { // Primeiro -> ID (0-based: 100)
    ExpressaoVariavel* res100 = new ExpressaoVariavel();
    res100->nome = ID::extrai_ID(no->filhos[0]);
    return res100;
  }
  case 101: // Primeiro -> DECIMAL (0-based: 101)
  case 102: // Primeiro -> FLOAT_LITERAL (0-based: 102)
  case 103: // Primeiro -> TRUE (0-based: 103)
  case 104: { // Primeiro -> FALSE (0-based: 104)
    ExpressaoValor* res = new ExpressaoValor();
    res->valor = ValorLiteral::extrai_valor_literal(no->filhos[0]);
    return res;
  }
  case 108: { // Primeiro -> LPAR Expressao RPAR (0-based: 108)
    return extrai_expressao(no->filhos[1]);
  }
  default: {
    return NULL;
  }
  }
}

void Expressao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Expressao generica" << endl;
}
