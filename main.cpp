#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <array>
#include <list>
#include <cstdlib>
#include <cmath>
#include <random>
#include <functional>
#include <omp.h>
#include "Cidade.h"
#include "GaleShapley.h"

using namespace std;

int procuraChaveVetorParesMain(int chave, vector<pair<int,int>> v)
{
    for(unsigned int i = 0; i < v.size(); i++)
    {
        if(v.at(i).first == chave) return i;
    }
    return -1;
}

int calcularDistancia(Cidade cidadeA, Cidade cidadeB)
{
    int diferencaX = cidadeB.X - cidadeA.X;
    int diferencaY = cidadeB.Y - cidadeA.Y;

    return (int) sqrt(pow(abs(diferencaX),2) + pow(abs(diferencaY),2));
}

// Código adaptado de shell sort.
void ordenarListaPares(vector<pair<int,int>> *v)
{
    int n = v->size();
    if(n>0)
    {
        int i, j, aux, dist=1;
        do
        {
            dist = 3*dist +1;
        }
        while(dist<n);
        do
        {
            dist/= 3;
            for(i=dist; i<n; i++)
            {
                aux= v->at(i).second;
                j= i-dist;
                while(j>=0 && aux < v->at(j).second)
                {
                    v->at(j + dist).second =v->at(j).second;
                    j-=dist;
                }
                v->at(j + dist).second = aux;
            }
        }
        while(dist>1);
    }
}


void separarConjuntos(vector<Cidade> *lista)
{
    vector<int> controlePosicoes;
    vector<Cidade>::iterator it;
    bool proConjuntoA = true;
    srand((omp_get_thread_num() + 1 )*time(NULL));

    for(it = lista->begin(); it != lista->end(); ++it)
    {
        controlePosicoes.push_back(0);
    }

    int i, aux, numCidades = controlePosicoes.size();

    //cout << endl << "Tentando selecionar o conjunto A..." << endl;
    // Populando os conjuntos com valores aleatorios nao usados
    for(i = 0; i < numCidades; i++)
    {
        do
        {
            aux = rand() % numCidades;
        }
        while(controlePosicoes[aux] == -1);
        if(proConjuntoA){
            controlePosicoes[aux] = -1;
            lista->at(aux).conj = 'A';
            proConjuntoA = false;
        }
        else{
            controlePosicoes[aux] = -1;
            lista->at(aux).conj = 'B';
            proConjuntoA = true;
        }
        //cout << "Selecionei a cidade " << lista->at(aux).num << endl;
    }

}

void construirPreferencias(vector<Cidade> *lista)
{
    vector<Cidade>::iterator itCidade, itCidadeOposta;
    vector<pair<int,int>>::iterator it;

    for(itCidade = lista->begin(); itCidade != lista->end(); ++itCidade)
    {
        if(itCidade->conj == 'A')
        {
            for(itCidadeOposta = lista->begin(); itCidadeOposta != lista->end(); ++itCidadeOposta)
            {
                if(itCidadeOposta->conj == 'B')
                {
                    itCidade->preferencias.push_back(make_pair(itCidadeOposta->num,calcularDistancia(*itCidade, *itCidadeOposta)));
                }
            }

            ordenarListaPares(&itCidade->preferencias);

            /*cout << endl << "A lista ordenada da cidade " << i+1 << " eh:" << endl;
            // Mostrando a lista ordenada construída para a cidade i do conjunto A
            for (it lista->at(i).preferencias.begin(); it != lista->at(i).preferencias.end(); ++it)
            {
                //cout << "Cidade " << it->first << " com distancia " << it->second << endl;
            }*/
        }


        else if(itCidade->conj == 'B')
        {
            for(itCidadeOposta = lista->begin(); itCidadeOposta != lista->end(); ++itCidadeOposta)
            {
                if(itCidadeOposta->conj == 'A')
                {
                    itCidade->preferencias.push_back(make_pair(itCidadeOposta->num,calcularDistancia(*itCidade, *itCidadeOposta)));
                }
            }

            ordenarListaPares(&itCidade->preferencias);


            /*cout << endl << "A lista ordenada da cidade " << i+1 << " eh:" << endl;
            // Mostrando a lista ordenada construída para a cidade i do conjunto A
            for (it= lista->at(i).preferencias.begin(); it != lista->at(i).preferencias.end(); ++it)
            {
                //cout << "Cidade " << it->first << " com distancia " << it->second << endl;
            }*/
        }
    }
}

int encontrarRota(vector<Cidade> listaCidades)
{
    //cout << "Thread " << omp_get_thread_num() << " vai procurar uma rota..." << endl;

    int custoTotal = 0;
    vector<pair<int,int>>::iterator it;

    separarConjuntos(&listaCidades);
    construirPreferencias(&listaCidades);

    GaleShapley solucao(listaCidades);
    for(it = solucao.paresFinais.begin(); it != solucao.paresFinais.end(); it++)
    {
        int chave = procuraChaveVetorParesMain(it->second, listaCidades.at(it->first - 1).preferencias);
        int custoAresta = listaCidades.at(it->first - 1).preferencias.at(chave).second;
        //cout << "Entre " << it->first << " e " << it->second << " o custo eh " << custoAresta << endl;
        custoTotal += custoAresta;
    }
    //cout << " de custo total " << custoTotal;
    return custoTotal;
}

int main()
{
    string linha, nomeArquivo;
    vector<Cidade> listaCidades;
    vector<string> aux;

    cout << "Digite o nome do arquivo que deseja abrir: " << endl;
    cin >> nomeArquivo;
    ifstream arquivo(nomeArquivo, ios::in);

    int menorCusto;

    if (arquivo.is_open())
    {
        while (getline(arquivo, linha))
        {
            stringstream ss(linha);
            while (ss >> linha)
            {
                aux.push_back(linha);
            }
            listaCidades.emplace_back(Cidade(stoi(aux.at(0)), stoi(aux.at(1)), stoi(aux.at(2))));
            aux.clear();
        }
        arquivo.close();
    }
    else{
        cout << "Este arquivo nao existe." << endl;
        return 1;
    }

    menorCusto = encontrarRota(listaCidades);

    //cout << "A thread main achou " << menorCusto << "... A seguir as threads tentarao!" << endl;


    # pragma omp parallel
    {
        int custoEncontrado = encontrarRota(listaCidades);
        //cout << "A thread " << omp_get_thread_num() << " achou " << custoEncontrado << endl;
        # pragma omp critical
        if(custoEncontrado < menorCusto){
            menorCusto = custoEncontrado;
        }
    }

    cout << "O menor custo de todas foi " << menorCusto << endl;


    return 0;
}
