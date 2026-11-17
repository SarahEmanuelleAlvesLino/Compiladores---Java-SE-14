#include "FrameFuncao.hpp"
#include "FrameAcessoNoFrame.hpp"
#include "FrameAcessoTemp.hpp"
#include "FrameAcesso.hpp"
#include "Variavel.hpp"
#include "ID.hpp"
#include "Comando.hpp"
#include "ComandoDeclaracao.hpp"
#include "ComandoAtribuicao.hpp"
#include "ComandoLista.hpp"
#include "ComandoIf.hpp"
#include "ComandoRetorno.hpp"
#include "ComandoWhile.hpp"
#include "Expressao.hpp"
#include "ExpressaoChamadaFuncao.hpp"
#include "ExpressaoVariavel.hpp"
#include "ExpressaoSoma.hpp"
#include "ExpressaoSubtracao.hpp"
#include "ExpressaoMultiplicacao.hpp"
#include "ExpressaoDivisao.hpp"
#include "ExpressaoMod.hpp"
#include "ExpressaoIgualdade.hpp"
#include "ExpressaoMenor.hpp"
#include "ExpressaoNegacao.hpp"
#include <map>
#include <string>
#include <iostream>

using namespace std;

FrameFuncao::FrameFuncao()
{
  tamanho_frame = 40; 
  n_param_entrada = 0;
  n_maximo_param_saida = 0;
  n_pseudo_registradores = 0;
  n_variaveis_no_frame = 0;
}


void rastreia_expressao(Expressao *expr, bool eh_parametro, map<string, bool> &var_vai_pro_frame, int &max_args)
{
  if (expr == NULL)
    return;

  if (ExpressaoVariavel *ev = dynamic_cast<ExpressaoVariavel *>(expr))
  {
    string nome = ev->nome->nome;
    if (eh_parametro)
    {
      var_vai_pro_frame[nome] = true;
    }
    else
    {
      if (var_vai_pro_frame.find(nome) == var_vai_pro_frame.end())
      {
        var_vai_pro_frame[nome] = false;
      }
    }
  }
  else if (ExpressaoChamadaFuncao *ec = dynamic_cast<ExpressaoChamadaFuncao *>(expr))
  {
    if ((int)ec->parametros.size() > max_args)
    {
      max_args = ec->parametros.size();
    }
    for (size_t i = 0; i < ec->parametros.size(); ++i)
    {
      rastreia_expressao(ec->parametros[i], true, var_vai_pro_frame, max_args);
    }
  }
  else if (ExpressaoSoma *op = dynamic_cast<ExpressaoSoma *>(expr))
  {
    rastreia_expressao(op->esquerda, eh_parametro, var_vai_pro_frame, max_args);
    rastreia_expressao(op->direita, eh_parametro, var_vai_pro_frame, max_args);
  }
  else if (ExpressaoSubtracao *op = dynamic_cast<ExpressaoSubtracao *>(expr))
  {
    rastreia_expressao(op->esquerda, eh_parametro, var_vai_pro_frame, max_args);
    rastreia_expressao(op->direita, eh_parametro, var_vai_pro_frame, max_args);
  }
  else if (ExpressaoMultiplicacao *op = dynamic_cast<ExpressaoMultiplicacao *>(expr))
  {
    rastreia_expressao(op->esquerda, eh_parametro, var_vai_pro_frame, max_args);
    rastreia_expressao(op->direita, eh_parametro, var_vai_pro_frame, max_args);
  }
  else if (ExpressaoDivisao *op = dynamic_cast<ExpressaoDivisao *>(expr))
  {
    rastreia_expressao(op->esquerda, eh_parametro, var_vai_pro_frame, max_args);
    rastreia_expressao(op->direita, eh_parametro, var_vai_pro_frame, max_args);
  }
  else if (ExpressaoMod *op = dynamic_cast<ExpressaoMod *>(expr))
  {
    rastreia_expressao(op->esquerda, eh_parametro, var_vai_pro_frame, max_args);
    rastreia_expressao(op->direita, eh_parametro, var_vai_pro_frame, max_args);
  }
  else if (ExpressaoIgualdade *op = dynamic_cast<ExpressaoIgualdade *>(expr))
  {
    rastreia_expressao(op->esquerda, eh_parametro, var_vai_pro_frame, max_args);
    rastreia_expressao(op->direita, eh_parametro, var_vai_pro_frame, max_args);
  }
  else if (ExpressaoMenor *op = dynamic_cast<ExpressaoMenor *>(expr))
  {
    rastreia_expressao(op->esquerda, eh_parametro, var_vai_pro_frame, max_args);
    rastreia_expressao(op->direita, eh_parametro, var_vai_pro_frame, max_args);
  }
  else if (ExpressaoNegacao *op = dynamic_cast<ExpressaoNegacao *>(expr))
  {
    rastreia_expressao(op->expressao, eh_parametro, var_vai_pro_frame, max_args);
  }
}

// Função que caminha nos comandos
void rastreia_comandos(Comando *cmd, map<string, bool> &var_vai_pro_frame, int &max_args)
{
  if (cmd == NULL)
    return;

  if (ComandoLista *cl = dynamic_cast<ComandoLista *>(cmd))
  {
    for (size_t i = 0; i < cl->lista_comandos.size(); ++i)
    {
      rastreia_comandos(cl->lista_comandos[i], var_vai_pro_frame, max_args);
    }
  }
  else if (ComandoDeclaracao *cd = dynamic_cast<ComandoDeclaracao *>(cmd))
  {
    if (cd->variavel != NULL && cd->variavel->nome != NULL)
    {
      string nome = cd->variavel->nome->nome;
      if (var_vai_pro_frame.find(nome) == var_vai_pro_frame.end())
      {
        var_vai_pro_frame[nome] = false;
      }
    }

  }
  else if (ComandoAtribuicao *ca = dynamic_cast<ComandoAtribuicao *>(cmd))
  {
    if (ca->esquerda != NULL)
    {
      string nome = ca->esquerda->nome; // Pega a string de dentro do ID
      if (var_vai_pro_frame.find(nome) == var_vai_pro_frame.end())
      {
        var_vai_pro_frame[nome] = false;
      }
    }
    rastreia_expressao(ca->direita, false, var_vai_pro_frame, max_args);
  }
  else if (ComandoRetorno *cr = dynamic_cast<ComandoRetorno *>(cmd))
  {
    rastreia_expressao(cr->expressao, false, var_vai_pro_frame, max_args);
  }
  else if (ComandoIf *ci = dynamic_cast<ComandoIf *>(cmd))
  {
    rastreia_expressao(ci->condicao, false, var_vai_pro_frame, max_args);
    rastreia_comandos(ci->bloco_true, var_vai_pro_frame, max_args);
    if (ci->bloco_false != NULL)
      rastreia_comandos(ci->bloco_false, var_vai_pro_frame, max_args);
  }
  else if (ComandoWhile *cw = dynamic_cast<ComandoWhile *>(cmd))
  {
    rastreia_expressao(cw->condicao, false, var_vai_pro_frame, max_args);
    rastreia_comandos(cw->bloco, var_vai_pro_frame, max_args);
  }
}

void atualiza_acesso_expressao(Expressao *expr, map<string, FrameAcesso *> &mapa_memoria)
{
  if (expr == NULL)
    return;

  if (ExpressaoVariavel *ev = dynamic_cast<ExpressaoVariavel *>(expr))
  {
    if (mapa_memoria.find(ev->nome->nome) != mapa_memoria.end())
    {
      ev->acesso = mapa_memoria[ev->nome->nome];
    }
  }
  else if (ExpressaoChamadaFuncao *ec = dynamic_cast<ExpressaoChamadaFuncao *>(expr))
  {
    for (size_t i = 0; i < ec->parametros.size(); ++i)
      atualiza_acesso_expressao(ec->parametros[i], mapa_memoria);
  }
  else if (ExpressaoSoma *op = dynamic_cast<ExpressaoSoma *>(expr))
  {
    atualiza_acesso_expressao(op->esquerda, mapa_memoria);
    atualiza_acesso_expressao(op->direita, mapa_memoria);
  }
  else if (ExpressaoSubtracao *op = dynamic_cast<ExpressaoSubtracao *>(expr))
  {
    atualiza_acesso_expressao(op->esquerda, mapa_memoria);
    atualiza_acesso_expressao(op->direita, mapa_memoria);
  }
  else if (ExpressaoMultiplicacao *op = dynamic_cast<ExpressaoMultiplicacao *>(expr))
  {
    atualiza_acesso_expressao(op->esquerda, mapa_memoria);
    atualiza_acesso_expressao(op->direita, mapa_memoria);
  }
  else if (ExpressaoDivisao *op = dynamic_cast<ExpressaoDivisao *>(expr))
  {
    atualiza_acesso_expressao(op->esquerda, mapa_memoria);
    atualiza_acesso_expressao(op->direita, mapa_memoria);
  }
  else if (ExpressaoMod *op = dynamic_cast<ExpressaoMod *>(expr))
  {
    atualiza_acesso_expressao(op->esquerda, mapa_memoria);
    atualiza_acesso_expressao(op->direita, mapa_memoria);
  }
  else if (ExpressaoIgualdade *op = dynamic_cast<ExpressaoIgualdade *>(expr))
  {
    atualiza_acesso_expressao(op->esquerda, mapa_memoria);
    atualiza_acesso_expressao(op->direita, mapa_memoria);
  }
  else if (ExpressaoMenor *op = dynamic_cast<ExpressaoMenor *>(expr))
  {
    atualiza_acesso_expressao(op->esquerda, mapa_memoria);
    atualiza_acesso_expressao(op->direita, mapa_memoria);
  }
  else if (ExpressaoNegacao *op = dynamic_cast<ExpressaoNegacao *>(expr))
  {
    atualiza_acesso_expressao(op->expressao, mapa_memoria);
  }
}

void atualiza_acesso_comandos(Comando *cmd, map<string, FrameAcesso *> &mapa_memoria)
{
  if (cmd == NULL)
    return;

  if (ComandoLista *cl = dynamic_cast<ComandoLista *>(cmd))
  {
    for (size_t i = 0; i < cl->lista_comandos.size(); ++i)
      atualiza_acesso_comandos(cl->lista_comandos[i], mapa_memoria);
  }
  else if (ComandoAtribuicao *ca = dynamic_cast<ComandoAtribuicao *>(cmd))
  {
    atualiza_acesso_expressao(ca->direita, mapa_memoria);
  }
  else if (ComandoRetorno *cr = dynamic_cast<ComandoRetorno *>(cmd))
  {
    atualiza_acesso_expressao(cr->expressao, mapa_memoria);
  }
  else if (ComandoIf *ci = dynamic_cast<ComandoIf *>(cmd))
  {
    atualiza_acesso_expressao(ci->condicao, mapa_memoria);
    atualiza_acesso_comandos(ci->bloco_true, mapa_memoria);
    if (ci->bloco_false != NULL)
      atualiza_acesso_comandos(ci->bloco_false, mapa_memoria);
  }
  else if (ComandoWhile *cw = dynamic_cast<ComandoWhile *>(cmd))
  {
    atualiza_acesso_expressao(cw->condicao, mapa_memoria);
    atualiza_acesso_comandos(cw->bloco, mapa_memoria);
  }
}

FrameFuncao *FrameFuncao::gera_frame_de_funcao(Funcao *fun)
{
  FrameFuncao *frame = new FrameFuncao();

  map<string, FrameAcesso *> mapa_memoria;

  frame->n_param_entrada = fun->parametros.size();
  int offset_parametro = 8; // O primeiro parâmetro começa em FP+8

  for (size_t i = 0; i < fun->parametros.size(); ++i)
  {
    Variavel *param = fun->parametros[i];

    // Cria o acesso na memória (Frame) com a posição correta
    FrameAcessoNoFrame *acesso_param = new FrameAcessoNoFrame();
    acesso_param->posicao_no_frame = offset_parametro;

    // Salva no nosso dicionário e na lista ordenada de alocações
    mapa_memoria[param->nome->nome] = acesso_param;
    frame->alocacoes.push_back(make_pair(param->nome->nome, (FrameAcesso*)acesso_param));

    // Próximo parâmetro ocupará a próxima "gaveta" de 8 bytes
    offset_parametro += 8;
  }

  map<string, bool> var_vai_pro_frame;
  int max_args_saida = 0;

  for (size_t i = 0; i < fun->comandos.size(); ++i)
  {
    rastreia_comandos(fun->comandos[i], var_vai_pro_frame, max_args_saida);
  }

  frame->n_maximo_param_saida = max_args_saida;

  int offset_frame_local = -40; // Variáveis da RAM começam em FP-40
  int id_temp = 1;              // Temporários da CPU começam no ID 1

  for (map<string, bool>::iterator it = var_vai_pro_frame.begin(); it != var_vai_pro_frame.end(); ++it)
  {
    string nome_var = it->first;
    bool vai_pro_frame = it->second;

    if (mapa_memoria.find(nome_var) != mapa_memoria.end())
    {
      continue;
    }

    if (vai_pro_frame)
    {
      // Vai para a memória (RAM)
      FrameAcessoNoFrame *acesso_local = new FrameAcessoNoFrame();
      acesso_local->posicao_no_frame = offset_frame_local;
      mapa_memoria[nome_var] = acesso_local;
      frame->alocacoes.push_back(make_pair(nome_var, (FrameAcesso*)acesso_local));

      offset_frame_local -= 8;
      frame->n_variaveis_no_frame++;
      frame->tamanho_frame += 8; // Custa 8 bytes no tamanho final da função
    }
    else
    {
      // Vai para o processador (Pseudo-Registrador)
      FrameAcessoTemp *acesso_temp = new FrameAcessoTemp();
      acesso_temp->id = id_temp;
      mapa_memoria[nome_var] = acesso_temp;
      frame->alocacoes.push_back(make_pair(nome_var, (FrameAcesso*)acesso_temp));

      id_temp++;
      frame->n_pseudo_registradores++; // Aumenta a contagem de temporários
    }
  }
  for (size_t i = 0; i < fun->comandos.size(); ++i)
  {
    atualiza_acesso_comandos(fun->comandos[i], mapa_memoria);
  }

  frame->tamanho_frame += 8 * frame->n_maximo_param_saida;

  return frame;

}

void FrameFuncao::imprime()
{
  cout << "tamanho_frame: " << tamanho_frame << endl;
  cout << "numero_parametros_entrada: " << n_param_entrada << endl;
  cout << "numero_maximo_param_saida: " << n_maximo_param_saida << endl;
  cout << "numero_pseudo_registradores: " << n_pseudo_registradores << endl;
  cout << "numero_variaveis_no_frame: " << n_variaveis_no_frame << endl;

  cout << endl <<" - Alocacao de variaveis: " << endl;
  for (size_t i = 0; i < alocacoes.size(); ++i) {
    const string& nome = alocacoes[i].first;
    FrameAcesso* acesso = alocacoes[i].second;
    cout << "     " << nome << " -> ";
    if (FrameAcessoNoFrame* af = dynamic_cast<FrameAcessoNoFrame*>(acesso)) {
      int pos = af->posicao_no_frame;
      cout << "Frame (FP";
      if (pos >= 0) cout << "+";
      cout << pos << ")" << endl;
    } else if (FrameAcessoTemp* at = dynamic_cast<FrameAcessoTemp*>(acesso)) {
      cout << "Reg (t" << at->id << ")" << endl;
    }
  }
}
