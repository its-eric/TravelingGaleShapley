#include "GaleShapley.h"
#include "Cidade.h"

using namespace std;

int procuraChaveVetorPares(int chave, vector<pair<int,int>> v)
{
    for(unsigned int i = 0; i < v.size(); i++)
    {
        if(v.at(i).first == chave) return i;
    }
    return -1;
}

GaleShapley::GaleShapley(vector<Cidade> cidadesRecebidas):tamanhoConjunto(cidadesRecebidas.size()), cidades(cidadesRecebidas)
{

    int propostasRodada, pretendida, pretendente, cidadesSolteiras = tamanhoConjunto;

    casamentos = (int*) malloc(sizeof(int) * tamanhoConjunto);
    propostas = (int*) malloc(sizeof(int) * tamanhoConjunto);
    proximaProposta = (int*) malloc(sizeof(int) * tamanhoConjunto);
    recusas = (int*) malloc(sizeof(int) * tamanhoConjunto);

    for(int i = 0; i < tamanhoConjunto; i++)
    {
        casamentos[i] = -1;
        if(cidades.at(i).conj == 'A')
        {
            recusas[i] = 1;
            propostas[i] = cidades.at(i).preferencias.at(0).first;
            proximaProposta[i] = 1;
        }
        else
        {
            recusas[i] = -1;
            proximaProposta[i] = -1;
            propostas[i] = -1;
        }
    }

    do
    {
        propostasRodada = 0;
        for(int i = 0; i < tamanhoConjunto; i++)
        {
            if(recusas[i] > 0) //Exclui as mulheres
            {
                propostasRodada++;
                pretendente = i + 1;
                pretendida = propostas[i];
                //cout << "Cidade " << pretendente << " propoe casamento a cidade " << pretendida << endl;
                if(casamentos[pretendida - 1] == -1){
                    casamentos[pretendida - 1] = pretendente;
                    casamentos[pretendente - 1] = pretendida;
                    recusas[i] = 0;
                    //cout << "Cidade " << pretendida << " casou com cidade " << pretendente << endl;
                    cidadesSolteiras = cidadesSolteiras - 2;
                }
                else if(procuraChaveVetorPares(pretendente, cidades.at(pretendida - 1).preferencias) <
                         procuraChaveVetorPares(casamentos[pretendida - 1], cidades.at(pretendida - 1).preferencias))
                {
                    recusas[casamentos[pretendida - 1]] = 1;
                    //cout << "Cidade " << casamentos[pretendida - 1] << " esta solteira..." << endl;
                    cidadesSolteiras = cidadesSolteiras + 2;
                    casamentos[pretendida - 1] = pretendente;
                    casamentos[pretendente - 1] = pretendida;
                    recusas[i] = 0;
                    //cout << "Cidade " << pretendida << " casou com cidade " << pretendente  << endl;
                    cidadesSolteiras = cidadesSolteiras - 2;
                }
                else
                {
                    //cout << "Cidade " << pretendida << " recusou." << endl;
                }

                // Se ainda há proposta possivel será feita na proxima vez
                if(cidadesSolteiras > 0){
                    propostas[i] = cidades.at(i).preferencias.at(proximaProposta[i]).first;
                    proximaProposta[i]++;
                }
            }

        }
    }
    while(propostasRodada != 0);

    //cout << endl << "Sugerida a rota : ";
    for(int i = 0; i < tamanhoConjunto; i++){
        if(cidades.at(i).conj == 'A'){
            //cout << i+1 << " -> " << casamentos[i] << " -> ";
            paresFinais.push_back(make_pair(i+1,casamentos[i]));
        }
    }

}

GaleShapley::~GaleShapley()
{
    free(casamentos);
    free(propostas);
    free(proximaProposta);
    free(recusas);
}
