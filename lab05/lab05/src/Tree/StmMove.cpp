#include "StmMove.hpp"
#include "ExpTemp.hpp"
#include "ExpMem.hpp"
#include "../src-gram-java/FrameAcesso.hpp"
#include "../src-gram-java/FrameAcessoTemp.hpp"
#include "../src-gram-java/FrameAcessoNoFrame.hpp"
#include "../src-gram-java/ID.hpp"
#include <iostream>

using namespace std;

StmMove::StmMove() {
  destino = NULL;
  origem = NULL;
}

void StmMove::imprime() const {
  cout << "MOVE(";
  if (destino != NULL) destino->imprime(); else cout << "EXP_NAO_SUPORTADA";
  cout << ", ";
  if (origem != NULL) origem->imprime(); else cout << "EXP_NAO_SUPORTADA";
  cout << ")";
}

static Exp* resolve_destino(ID* nome, FrameFuncao* frame) {
  if (nome == NULL || frame == NULL) return NULL;

  for (size_t i = 0; i < frame->alocacoes.size(); ++i) {
    if (frame->alocacoes[i].first == nome->nome) {
      FrameAcesso* acesso = frame->alocacoes[i].second;
      if (FrameAcessoTemp* at = dynamic_cast<FrameAcessoTemp*>(acesso)) {
        return ExpTemp::gera_Exp_Temp(at);
      }
      if (FrameAcessoNoFrame* af = dynamic_cast<FrameAcessoNoFrame*>(acesso)) {
        return ExpMem::gera_Exp_Mem(af);
      }
    }
  }

  cerr << "StmMove::gera_Stm_Move: variavel '" << nome->nome
       << "' nao encontrada nas alocacoes do frame." << endl;
  return NULL;
}

StmMove* StmMove::gera_Stm_Move(ComandoAtribuicao* cmd, FrameFuncao* frame) {
  if (cmd == NULL) return NULL;

  StmMove* res = new StmMove();
  res->destino = resolve_destino(cmd->esquerda, frame);
  res->origem = Exp::gera_Exp_de_Expressao(cmd->direita, frame);
  return res;
}


StmMove* StmMove::gera_Stm_Move_Declaracao(ComandoDeclaracao* cmd, FrameFuncao* frame) {
  if (cmd == NULL || cmd->inicializacao == NULL || cmd->variavel == NULL) return NULL;

  StmMove* res = new StmMove();
  res->destino = resolve_destino(cmd->variavel->nome, frame);
  res->origem = Exp::gera_Exp_de_Expressao(cmd->inicializacao, frame);
  return res;
}
