#ifndef GALESHAPLEY_H
#define GALESHAPLEY_H
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <list>
#include <utility>
#include "Cidade.h"

using namespace std;

class GaleShapley
{
    public:
        int tamanhoConjunto;
        vector<pair<int,int>> paresFinais;
        /** Default constructor */
        GaleShapley(vector<Cidade> cidades);
        /** Default destructor */
        virtual ~GaleShapley();
    protected:
    private:
        vector<Cidade> cidades;
        int *propostas, *proximaProposta, *recusas, *casamentos;
};

#endif // GALESHAPLEY_H
