#include "Cidade.h"

using namespace std;

Cidade::Cidade(int numero, int x, int y): num(numero), X(x), Y(y)
{
    cout << "Criada a cidade " << num << " com posicao (" << X << "," << Y << ")" << endl;
}

Cidade::~Cidade()
{
    //dtor
}

