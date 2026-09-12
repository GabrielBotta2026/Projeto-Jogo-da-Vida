//DOM-AGGL-View.h - ETAPA 5.
//11/11/2025 - Grupo: AGGL
//Ana Clara Boaventura Dattoli
//Gabriel Teixeira Botta
//Guilherme Almeida Coutinho
//Livia de Aguiar Almeida Figueiredo

#ifndef VIEW_H
#define VIEW_H
#include "AGGL_JVida_Model.h"

//Protótipos das funções da Visão
void View_apresentarMapa(ListaCelula* listaVivas, ListaCelula* listaVizinhas);
int View_getDimensaoDoUsuario();
int View_getEscolhaMenu();
void View_getCoordenadas(int* linha, int* coluna);

//Funções específicas para cada mensagem
void View_exibirMensagemInicializacao();
void View_exibirMensagemCelulaAdicionada(int linha, int coluna);
void View_exibirMensagemCelulaRemovida(int linha, int coluna);
void View_exibirMensagemCoordenadaInvalida();
void View_exibirMensagemOpcaoInvalida();
void View_exibirMensagemEncerramento();
void View_alternarModoVizinhas();
bool View_getModoVizinhas();
void View_exibirMensagemMapaLimpo();
int View_getNumeroGeracoes();
int View_getVelocidadeSimulacao();
void View_limparTela();
void View_aguardarEnter();
void View_exibirMensagemSimulacaoConcluida(int numGeracoes);
void View_exibirMensagemSalvo();
void View_exibirMensagemErroSalvar();
void View_exibirMensagemCarregado();
void View_exibirMensagemErroCarregar();
void View_exibirMensagemFimArquivo();
void View_obterNomeArquivo(char* buffer, int tamanho);

#endif //VIEW_H
