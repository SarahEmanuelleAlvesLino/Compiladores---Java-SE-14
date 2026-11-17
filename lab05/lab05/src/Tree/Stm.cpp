#include "Stm.hpp"
#include "StmMove.hpp"
#include "StmSeq.hpp"
#include "StmLabel.hpp"
#include "StmJump.hpp"
#include "StmCJump.hh"
#include "Exp.hpp"
#include "ExpTemp.hpp"
#include "../src-gram-java/ComandoAtribuicao.hpp"
#include "../src-gram-java/ComandoLista.hpp"
#include "../src-gram-java/ComandoIf.hpp"
#include "../src-gram-java/ComandoWhile.hpp"
#include "../src-gram-java/ComandoFor.hpp"
#include "../src-gram-java/ComandoDoWhile.hpp"
#include "../src-gram-java/ComandoDeclaracao.hpp"
#include "../src-gram-java/ComandoRetorno.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static string novo_label(int &contador_labels)
{
  return "L" + to_string(contador_labels++);
}

static Stm *encadeia_seq(vector<Stm *> &stms)
{
  Stm *res = NULL;
  for (int i = (int)stms.size() - 1; i >= 0; --i)
  {
    if (stms[i] == NULL)
      continue;
    res = (res == NULL) ? stms[i] : StmSeq::gera_Stm_Seq(stms[i], res);
  }
  return res;
}

Stm *Stm::gera_Stm_de_Comando(Comando *cmd, FrameFuncao *frame, int &contador_labels, const string &nome_funcao)
{
  if (cmd == NULL)
    return NULL;

  if (ComandoAtribuicao *ca = dynamic_cast<ComandoAtribuicao *>(cmd))
  {
    return StmMove::gera_Stm_Move(ca, frame);
  }

  if (ComandoLista *cl = dynamic_cast<ComandoLista *>(cmd))
  {
    vector<Stm *> stms;
    for (size_t i = 0; i < cl->lista_comandos.size(); ++i)
    {
      stms.push_back(Stm::gera_Stm_de_Comando(cl->lista_comandos[i], frame, contador_labels, nome_funcao));
    }
    return encadeia_seq(stms);
  }

  if (ComandoDeclaracao *cd = dynamic_cast<ComandoDeclaracao *>(cmd))
  {

    if (cd->inicializacao == NULL)
    {
      return NULL;
    }
    return StmMove::gera_Stm_Move_Declaracao(cd, frame);
  }

  if (ComandoIf *ci = dynamic_cast<ComandoIf *>(cmd))
  {
    Exp *cond = Exp::gera_Exp_de_Expressao(ci->condicao, frame);
    string l_true = novo_label(contador_labels);

    vector<Stm *> partes;
    if (ci->bloco_false != NULL)
    {
      string l_false = novo_label(contador_labels);
      string l_fim = novo_label(contador_labels);
      partes.push_back(StmCJump::gera_Stm_CJump(cond, l_true, l_false));
      partes.push_back(StmLabel::gera_Stm_Label(l_true));
      partes.push_back(Stm::gera_Stm_de_Comando(ci->bloco_true, frame, contador_labels, nome_funcao));
      partes.push_back(StmJump::gera_Stm_Jump(l_fim));
      partes.push_back(StmLabel::gera_Stm_Label(l_false));
      partes.push_back(Stm::gera_Stm_de_Comando(ci->bloco_false, frame, contador_labels, nome_funcao));
      partes.push_back(StmLabel::gera_Stm_Label(l_fim));
    }
    else
    {
      string l_fim = novo_label(contador_labels);
      partes.push_back(StmCJump::gera_Stm_CJump(cond, l_true, l_fim));
      partes.push_back(StmLabel::gera_Stm_Label(l_true));
      partes.push_back(Stm::gera_Stm_de_Comando(ci->bloco_true, frame, contador_labels, nome_funcao));
      partes.push_back(StmLabel::gera_Stm_Label(l_fim));
    }
    return encadeia_seq(partes);
  }

  if (ComandoWhile *cw = dynamic_cast<ComandoWhile *>(cmd))
  {
    string l_inicio = novo_label(contador_labels);
    string l_corpo = novo_label(contador_labels);
    string l_fim = novo_label(contador_labels);

    vector<Stm *> partes;
    partes.push_back(StmLabel::gera_Stm_Label(l_inicio));
    partes.push_back(StmCJump::gera_Stm_CJump(Exp::gera_Exp_de_Expressao(cw->condicao, frame), l_corpo, l_fim));
    partes.push_back(StmLabel::gera_Stm_Label(l_corpo));
    partes.push_back(Stm::gera_Stm_de_Comando(cw->bloco, frame, contador_labels, nome_funcao));
    partes.push_back(StmJump::gera_Stm_Jump(l_inicio));
    partes.push_back(StmLabel::gera_Stm_Label(l_fim));
    return encadeia_seq(partes);
  }

  if (ComandoFor *cf = dynamic_cast<ComandoFor *>(cmd))
  {
    string l_inicio = novo_label(contador_labels);
    string l_corpo = novo_label(contador_labels);
    string l_fim = novo_label(contador_labels);

    vector<Stm *> partes;
    if (cf->inicializacao != NULL)
    {
      partes.push_back(Stm::gera_Stm_de_Comando(cf->inicializacao, frame, contador_labels, nome_funcao));
    }
    partes.push_back(StmLabel::gera_Stm_Label(l_inicio));
    if (cf->condicao != NULL)
    {
      partes.push_back(StmCJump::gera_Stm_CJump(Exp::gera_Exp_de_Expressao(cf->condicao, frame), l_corpo, l_fim));
    }
    else
    {
      partes.push_back(StmJump::gera_Stm_Jump(l_corpo));
    }
    partes.push_back(StmLabel::gera_Stm_Label(l_corpo));
    partes.push_back(Stm::gera_Stm_de_Comando(cf->corpo, frame, contador_labels, nome_funcao));
    if (cf->passo != NULL)
    {
      partes.push_back(Stm::gera_Stm_de_Comando(cf->passo, frame, contador_labels, nome_funcao));
    }
    partes.push_back(StmJump::gera_Stm_Jump(l_inicio));
    partes.push_back(StmLabel::gera_Stm_Label(l_fim));
    return encadeia_seq(partes);
  }

  if (ComandoDoWhile *dw = dynamic_cast<ComandoDoWhile *>(cmd))
  {
    string l_corpo = novo_label(contador_labels);
    string l_fim = novo_label(contador_labels);

    vector<Stm *> partes;
    partes.push_back(StmLabel::gera_Stm_Label(l_corpo));
    partes.push_back(Stm::gera_Stm_de_Comando(dw->corpo, frame, contador_labels, nome_funcao));
    partes.push_back(StmCJump::gera_Stm_CJump(Exp::gera_Exp_de_Expressao(dw->condicao, frame), l_corpo, l_fim));
    partes.push_back(StmLabel::gera_Stm_Label(l_fim));
    return encadeia_seq(partes);
  }

  if (ComandoRetorno *ret = dynamic_cast<ComandoRetorno *>(cmd))
  {

    Exp *valor_retorno = Exp::gera_Exp_de_Expressao(ret->expressao, frame);

    ExpTemp *rv = new ExpTemp();
    rv->id = 100;
    StmMove *move_rv = new StmMove();
    move_rv->destino = rv;
    move_rv->origem = valor_retorno;

    StmJump *jump_epilogo = StmJump::gera_Stm_Jump(nome_funcao + "_return");

    // 4) Encadeia MOVE e JUMP.
    return StmSeq::gera_Stm_Seq(move_rv, jump_epilogo);
  }

  return NULL;
}