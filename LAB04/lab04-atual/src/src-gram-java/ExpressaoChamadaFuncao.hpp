#ifndef _EXPRESSAO_CHAMADA_FUNCAO_HPP_
#define _EXPRESSAO_CHAMADA_FUNCAO_HPP_
#include "Expressao.hpp"
#include "ID.hpp"
#include <vector>

using namespace std;

class ExpressaoChamadaFuncao : public Expressao
{
public:
    ID *nome_funcao;
    vector<Expressao *> parametros;

    ExpressaoChamadaFuncao();
    void debug_com_tab(int tab);
};

#endif