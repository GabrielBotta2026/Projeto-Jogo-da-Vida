//DOM-AGGL-View.cpp - ETAPA 5.
//11/11/2025 - Grupo: AGGL
//Ana Clara Boaventura Dattoli
//Gabriel Teixeira Botta
//Guilherme Almeida Coutinho
//Livia de Aguiar Almeida Figueiredo

#include "AGGL_JVida_View.h"
#include "AGGL_JVida_Model.h" //A View precisa do Model para saber o que desenhar
#include <cstdio>
#include <stdlib.h>

static bool exibirVizinhasMortas = false;

static int contarLista(ListaCelula* lista){
    int cont = 0;
    ListaCelula* p = lista;
    while (p != NULL){
        cont++;
        p = p->proximo;
    }
    return cont;
}

static void imprimirLista(ListaCelula* lista){
    ListaCelula* p = lista;
    int cont = 0;
    while (p != NULL){
        printf("(%d,%d) ", p->linha, p->coluna);
        p = p->proximo;
        cont++;
        if (cont % 10 == 0){ //Quebra de linha a cada 10 coordenadas
            printf("\n");
        }
    }
    if (cont == 0){
        printf("(nenhuma)");
    }
    printf("\n\n");
}

void View_apresentarMapa(ListaCelula* listaVivas, ListaCelula* listaVizinhas){
    printf("Geracao: %d\n\n", Model_getGeracaoAtual());

    int dim = Model_getDimensao();
    bool modoVizinhas = View_getModoVizinhas();
    
    printf("\n   ");
    for (int j = 0; j < dim; ++j){
        printf("%02d ", j);
    }
    printf("\n");

    for (int i = 0; i < dim; ++i){
        printf("%02d ", i);
        for (int j = 0; j < dim; ++j) {
            char estado = Model_getEstadoCelula(i, j);
            
            if (estado == VIVA){
                printf(" %c ", VIVA);
            } else if (estado == MORTA){
                if (modoVizinhas && Model_contarVizinhosVivos(i, j) > 0){
                    printf(" %c ", VIZINHA_MORTA);
                } else{
                    printf(" %c ", MORTA);
                }
            }else{
                printf(" %c ", estado);
            }
        }
        printf("\n");
    }
    printf("\n");

    printf("--- Listas de Coordenadas ---\n");
    printf("Celulas Vivas (%d):\n", contarLista(listaVivas));
    imprimirLista(listaVivas);

    //Só exibe a lista de vizinhas-mortas se o modo estiver ativo
    if (modoVizinhas) {
        printf("Vizinhas Mortas (%d):\n", contarLista(listaVizinhas));
        imprimirLista(listaVizinhas);
    }
}

int View_getDimensaoDoUsuario() {
    int dim;
    do {
        printf("Selecione a dimensao do abrigo (de 10 a 60): ");
        scanf("%d", &dim);
        while (getchar() != '\n'); //Limpa buffer
        if (dim < 10 || dim > 60) {
            printf("Dimensao invalida, tente novamente.\n");
        }
    } while (dim < 10 || dim > 60);
    return dim;
}

int View_getEscolhaMenu() {
    int escolha = -1;
    printf("--- Jogo da Vida ---\n");
    printf("1. Adicionar/Remover celula viva\n");
    printf("2. Alternar exibicao de vizinhas-mortas (+)\n");
    printf("3. Limpar o mapa (matar todas as celulas)\n");
    printf("4. Iniciar Simulacao (Avancar geracoes)\n");
    printf("5. Salvar geracao atual no arquivo\n");
    printf("6. Carregar geracao salva do arquivo\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &escolha);
    while (getchar() != '\n'); //Limpa buffer
    return escolha;
}

void View_getCoordenadas(int* linha, int* coluna) {
    printf("Digite a linha e a coluna entre espaco (ou -1 para voltar): ");
    scanf("%d", linha);
    if (*linha != -1) {
        scanf("%d", coluna);
    } else {
        *coluna = -1;
    }
    while (getchar() != '\n'); //Limpa buffer
}

// Implementação das mensagens específicas
void View_exibirMensagemInicializacao() {
    printf("Tabuleiro inicializado. Todas as celulas estao mortas.\n");
}

void View_exibirMensagemCelulaAdicionada(int linha, int coluna) {
    printf("Celula adicionada na posicao (%d, %d).\n", linha, coluna);
}

void View_exibirMensagemCelulaRemovida(int linha, int coluna) {
    printf("Celula removida da posicao (%d, %d).\n", linha, coluna);
}

void View_exibirMensagemCoordenadaInvalida() {
    printf("Coordenada invalida. Tente novamente.\n");
}

void View_exibirMensagemOpcaoInvalida() {
    printf("Opcao invalida. Tente novamente.\n");
}

void View_exibirMensagemEncerramento() {
    printf("Encerrando o programa...\n");
}

void View_alternarModoVizinhas(){
    exibirVizinhasMortas = !exibirVizinhasMortas; //Inverte o valor (true vira false, false vira true)
    if (exibirVizinhasMortas) {
        printf("Exibicao de vizinhas-mortas ATIVADA.\n");
    } else {
        printf("Exibicao de vizinhas-mortas DESATIVADA.\n");
    }
}

bool View_getModoVizinhas(){
    return exibirVizinhasMortas;
}

void View_exibirMensagemMapaLimpo(){
    printf("Mapa limpo. Todas as celulas foram removidas.\n");
}

int View_getNumeroGeracoes(){
    int geracoes = 0;
    do{
        printf("Digite o numero de geracoes a simular (>= 1): ");
        scanf("%d", &geracoes);
        while (getchar() != '\n'); //Limpa buffer
        if (geracoes < 1){
            View_exibirMensagemOpcaoInvalida();
        }
    } while (geracoes < 1);
    return geracoes;
}

int View_getVelocidadeSimulacao(){
    int velocidade = -1;
    do{
        printf("Defina a velocidade (0 a 3):\n");
        printf("  0 - Passo-a-passo (pressione ENTER)\n");
        printf("  1 - Lento (1.0 seg)\n");
        printf("  2 - Medio (0.5 seg)\n");
        printf("  3 - Rapido (0.2 seg)\n");
        printf("Opcao: ");
        scanf("%d", &velocidade);
        while (getchar() != '\n'); //Limpa buffer
        if (velocidade < 0 || velocidade > 3){
            View_exibirMensagemOpcaoInvalida();
        }
    }while (velocidade < 0 || velocidade > 3);
    return velocidade;
}

void View_limparTela(){
    system("cls");
}

void View_aguardarEnter(){
    printf("\nPressione ENTER para avancar para a proxima geracao...");
    getchar(); //Aguarda o usuário pressionar Enter
}

void View_exibirMensagemSimulacaoConcluida(int numGeracoes){
    printf("Simulacao de %d geracoes concluida.\n\n", numGeracoes);
}

void View_exibirMensagemSalvo(){
    printf("Configuracao adicionada ao arquivo com sucesso.\n");
}

void View_exibirMensagemErroSalvar(){
    printf("Erro: Nao foi possivel gravar o arquivo.\n");
}

void View_exibirMensagemCarregado(){
    printf("Proxima configuracao da sequencia carregada.\n");
}

void View_exibirMensagemErroCarregar(){
    printf("Erro: Nao foi possivel carregar o arquivo (pode estar corrompido ou ter dimensao incompativel).\n");
}

void View_exibirMensagemFimArquivo(){
    printf("Fim da sequencia de configuracoes. O leitor retornara ao inicio na proxima leitura.\n");
}

void View_obterNomeArquivo(char* buffer, int tamanho){
    printf("Digite o nome do arquivo (ex: jogo1.dat): ");
    
    //Lê o nome do arquivo de forma segura
    scanf("%99s", buffer); //Lê até 99 caracteres para evitar overflow
    
    //Limpa o buffer de entrada (para consumir o ENTER)
    while (getchar() != '\n');
}
