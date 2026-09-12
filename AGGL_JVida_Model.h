//DOM-AGGL-Model.h - ETAPA 5.
//11/11/2025 - Grupo: AGGL
//Ana Clara Boaventura Dattoli
//Gabriel Teixeira Botta
//Guilherme Almeida Coutinho
//Livia de Aguiar Almeida Figueiredo

#ifndef MODEL_H
#define MODEL_H

//Constantes
const int MAX_DIM = 60;
const char VIVA = 'O';
const char MORTA = '.';
const char VIZINHA_MORTA = '+';

struct ListaCelula{
    int linha;
    int coluna;
    struct ListaCelula* proximo;
};

//Protótipos das funções do Model
void Model_inicializar(int dim);
char Model_getEstadoCelula(int linha, int coluna);
int Model_getDimensao();
bool Model_inverterEstadoCelula(int linha, int coluna);
int Model_contarVizinhosVivos(int linha, int coluna);
void Model_limparMapa();
int Model_getGeracaoAtual();
void Model_avancarGeracao();
ListaCelula* Model_getListaVivas();
ListaCelula* Model_getListaVizinhasMortas();
void Model_limparLista(ListaCelula* lista);
bool Model_salvarGeracao(const char* nomeArquivo);
int Model_carregarProximaGeracao(const char* nomeArquivo);
void Model_resetarCursorLeitura();

#endif //MODEL_H
