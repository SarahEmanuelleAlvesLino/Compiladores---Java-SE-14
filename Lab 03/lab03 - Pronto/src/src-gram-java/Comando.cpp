#include "Comando.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoDeclaracao.hpp"
#include "ComandoLista.hpp"
#include "ComandoRetorno.hpp"
#include "ID.hpp"
#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

// Função auxiliar para extrair o ID a partir de uma expressão Posfixa (Posfixa -> Primeiro -> ID)
ID* extrai_id_de_posfixa(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->simb == "Posfixa") {
    return extrai_id_de_posfixa(no->filhos[0]);
  }
  if (no->simb == "Primeiro" && no->regra == 100) { // Primeiro -> ID (0-based: 100)
    return ID::extrai_ID(no->filhos[0]);
  }
  return NULL;
}

vector<Comando*> Comando::extrai_lista_comandos(No_arv_parse* no) {
  // ListaCmd. 31) ListaCmd -> ListaCmd Comando, 32) ListaCmd -> '' (0-based: 30 e 31)
  if (no->regra == 31) return vector<Comando*>(); // 0-based: 31
  
  // Tem que ser regra 30 (0-based: 30)
  vector<Comando*> res = extrai_lista_comandos(no->filhos[0]);
  res.push_back(extrai_comando(no->filhos[1]));
  return res;
}

Comando* Comando::extrai_comando(No_arv_parse* no) {
  if (no == NULL) return NULL;

  if (no->simb == "Comando") {
    // 33) Comando -> CmdFechado, 34) Comando -> CmdAberto (0-based: 32 e 33)
    return extrai_comando(no->filhos[0]);
  }
  
  if (no->simb == "CmdFechado") {
    // 38) CmdFechado -> OutroCmd (0-based: 37)
    if (no->regra == 37) {
      return extrai_comando(no->filhos[0]);
    }
    // IF/WHILE/FOR não são suportados nas classes de AST atuais, retornamos NULL
    return NULL;
  }

  if (no->simb == "CmdAberto") {
    // IF/WHILE/FOR não são suportados nas classes de AST atuais, retornamos NULL
    return NULL;
  }

  if (no->simb == "OutroCmd") {
    if (no->regra == 42) { // OutroCmd -> TipoVar ListaId SEMICOLON (0-based: 42)
      ComandoDeclaracao* res42 = new ComandoDeclaracao();
      res42->variavel = new Variavel();
      res42->variavel->tipo = Tipo::extrai_Tipo(no->filhos[0]);

      
      No_arv_parse* lista_id = no->filhos[1];
      while (lista_id->regra == 13) { // ListaId -> ListaId COMMA ID (0-based: 13)
        lista_id = lista_id->filhos[2];
      }
      // Agora lista_id é ListaId -> ID (0-based: 14)
      res42->variavel->nome = ID::extrai_ID(lista_id->filhos[0]);
      return res42;

      /*
      No_arv_parse *no_lista = no->filhos[1];
      while (no_lista->regra == 13){ // Regra 13: ListaId -> ListaId COMMA ID
        no_lista = no_lista->filhos[0];
      }

      // Regra 14: ListaId -> ID
      res42->variavel->nome = ID::extrai_ID(no_lista->filhos[0]);
      return res42;
      */
    }
    else if (no->regra == 43) { // OutroCmd -> TipoVar ID ASSIGN Expressao SEMICOLON (0-based: 43)
      ComandoDeclaracao* res43 = new ComandoDeclaracao();
      res43->variavel = new Variavel();
      res43->variavel->tipo = Tipo::extrai_Tipo(no->filhos[0]);
      res43->variavel->nome = ID::extrai_ID(no->filhos[1]);
      return res43;
    }
    else if (no->regra == 44) { // OutroCmd -> Posfixa ASSIGN Expressao SEMICOLON (0-based: 44)
      ComandoAtribuicao* res44 = new ComandoAtribuicao();
      res44->esquerda = extrai_id_de_posfixa(no->filhos[0]);
      res44->direita = Expressao::extrai_expressao(no->filhos[2]);
      return res44;
    }
    else if (no->regra == 51) { // OutroCmd -> RETURN Expressao SEMICOLON (0-based: 51)
      ComandoRetorno* res51 = new ComandoRetorno();
      res51->expressao = Expressao::extrai_expressao(no->filhos[1]);
      return res51;
    }
    else if (no->regra == 52) { // OutroCmd -> RETURN SEMICOLON (0-based: 52)
      ComandoRetorno* res52 = new ComandoRetorno();
      res52->expressao = NULL;
      return res52;
    }
    else if (no->regra == 54) { // OutroCmd -> Bloco (0-based: 54)
      ComandoLista* res54 = new ComandoLista();
      // Bloco -> LBRACE ListaCmd RBRACE (0-based: 29)
      res54->lista_comandos = extrai_lista_comandos(no->filhos[0]->filhos[1]);
      return res54;
    }
  }

  return NULL;
}

void Comando::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "Comando generico"<< endl;
}
