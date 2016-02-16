#ifndef CIDADE_H
#define CIDADE_H
#include <iostream>
#include <vector>

using namespace std;

class Cidade
{
    public:
        int num, X, Y;
        char conj;
        vector<std::pair<int,int>> preferencias;
        /** Default constructor */
        Cidade(int numero, int x, int y);
        /** Default destructor */
        virtual ~Cidade();
    protected:
    private:
};

#endif // CIDADE_H
