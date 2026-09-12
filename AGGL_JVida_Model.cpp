//DOM-AGGL-Model.cpp - ETAPA 5.
//11/11/2025 - Grupo: AGGL
//Ana Clara Boaventura Dattoli
//Gabriel Teixeira Botta
//Guilherme Almeida Coutinho
//Livia de Aguiar Almeida Figueiredo

#include "AGGL_JVida_Model.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>

//Variáveis estáticas para encapsular os dados do modelo neste arquivo.
static char matriz[MAX_DIM][MAX_DIM];
static int dimensao;
static int geracao;
static long g_posicaoLeitura = 0;

static ListaCelula* addCelula(ListaCelula* cabeca, int l, int c){
    ListaCelula* nova = (ListaCelula*) malloc(sizeof(ListaCelula));
    nova->linha = l;
    nova->coluna = c;
    nova->proximo = cabeca;
    return nova; // Retorna a nova cabeça da lista
}

void Model_inicializar(int dim){
    if (dim >= 10 && dim <= MAX_DIM){
        dimensao = dim;
        geracao = 0;
        g_posicaoLeitura = 0;
        for (int i = 0; i < dimensao; ++i){
            for (int j = 0; j < dimensao; ++j){
                matriz[i][j] = MORTA;
            }
        }
    }
}

char Model_getEstadoCelula(int linha, int coluna){
    if (linha >= 0 && linha < dimensao && coluna >= 0 && coluna < dimensao){
        return matriz[linha][coluna];
    }
    return ' '; //Retorno para coordenada inválida
}

int Model_getDimensao(){
    return dimensao;
}

bool Model_inverterEstadoCelula(int linha, int coluna){
    if (Model_getEstadoCelula(linha, coluna) == MORTA){
        matriz[linha][coluna] = VIVA;
        return true; //Célula foi adicionada
    } else{
        matriz[linha][coluna] = MORTA;
        return false; //Célula foi removida
    }
}

int Model_contarVizinhosVivos(int linha, int coluna){
    int cont = 0;
    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++){
            if (i == 0 && j == 0) continue; //Pula a própria célula

            if (Model_getEstadoCelula(linha + i, coluna + j) == VIVA){
                cont++;
            }
        }
    }
    return cont;
}

void Model_limparMapa(){
    for (int i = 0; i < dimensao; ++i){
        for (int j = 0; j < dimensao; ++j){
            matriz[i][j] = MORTA;
        }
    }
}

int Model_getGeracaoAtual(){
    return geracao;
}

void Model_avancarGeracao(){
    static int contagemVizinhosMortos[MAX_DIM][MAX_DIM];
    memset(contagemVizinhosMortos, 0, sizeof(contagemVizinhosMortos));

    ListaCelula* listaVivasProxima = NULL;
    ListaCelula* listaVivasAtual = Model_getListaVivas();
    ListaCelula* pViva = listaVivasAtual;

    while (pViva != NULL){
        int vizinhosVivos = 0;

        for (int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++){
                if (i == 0 && j == 0) continue;
                
                int nl = pViva->linha + i;
                int nc = pViva->coluna + j;
                char estadoVizinho = Model_getEstadoCelula(nl, nc);

                if (estadoVizinho == VIVA){
                    vizinhosVivos++;
                } else if (estadoVizinho == MORTA){
                    contagemVizinhosMortos[nl][nc]++;
                }
            }
        }

        if (vizinhosVivos == 2 || vizinhosVivos == 3){
            listaVivasProxima = addCelula(listaVivasProxima, pViva->linha, pViva->coluna);
        }
        
        pViva = pViva->proximo;
    }

    for (int i = 0; i < dimensao; i++){
        for (int j = 0; j < dimensao; j++){
            if (contagemVizinhosMortos[i][j] == 3){
                listaVivasProxima = addCelula(listaVivasProxima, i, j);
            }
        }
    }

    for (int i = 0; i < dimensao; ++i){
        for (int j = 0; j < dimensao; ++j){
            matriz[i][j] = MORTA;
        }
    }
    
    ListaCelula* pProx = listaVivasProxima;
    while (pProx != NULL){
        matriz[pProx->linha][pProx->coluna] = VIVA;
        pProx = pProx->proximo;
    }

    Model_limparLista(listaVivasAtual);
    Model_limparLista(listaVivasProxima);
    
    geracao++;
}

ListaCelula* Model_getListaVivas(){
    ListaCelula* cabeca = NULL;
    for (int i = 0; i < dimensao; i++){
        for (int j = 0; j < dimensao; j++){
            if (matriz[i][j] == VIVA){
                cabeca = addCelula(cabeca, i, j);
            }
        }
    }
    return cabeca;
}

ListaCelula* Model_getListaVizinhasMortas(){
    ListaCelula* cabeca = NULL;
    for (int i = 0; i < dimensao; i++){
        for (int j = 0; j < dimensao; j++){
            // É uma vizinha-morta se está MORTA e tem > 0 vizinhos vivos
            if (matriz[i][j] == MORTA && Model_contarVizinhosVivos(i, j) > 0){
                cabeca = addCelula(cabeca, i, j);
            }
        }
    }
    return cabeca;
}

void Model_limparLista(ListaCelula* lista){
    ListaCelula* atual = lista;
    while (atual != NULL){
        ListaCelula* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}

void Model_resetarCursorLeitura(){
    g_posicaoLeitura = 0;
}

bool Model_salvarGeracao(const char* nomeArquivo){
    FILE* f = fopen(nomeArquivo, "ab"); //"ab" = Append Binary (Adicionar Binário)
    if (f == NULL) return false;

    //Contar células vivas
    ListaCelula* vivas = Model_getListaVivas();
    int cont = 0;
    ListaCelula* p = vivas;
    while (p != NULL) { cont++; p = p->proximo; }

    //Escreve os "cabeçalhos"
    fwrite(&dimensao, sizeof(int), 1, f);
    fwrite(&geracao, sizeof(int), 1, f);
    fwrite(&cont, sizeof(int), 1, f);

    //Escreve as coordenadas das células vivas
    p = vivas;
    while (p != NULL) {
        fwrite(&(p->linha), sizeof(int), 1, f);
        fwrite(&(p->coluna), sizeof(int), 1, f);
        p = p->proximo;
    }

    fclose(f);
    Model_limparLista(vivas);
    return true;
}

int Model_carregarProximaGeracao(const char* nomeArquivo){
    FILE* f = fopen(nomeArquivo, "rb"); //"rb" = Read Binary (Ler Binário)
    if (f == NULL) return -1; //Arquivo não existe

    //Move o ponteiro do arquivo para a última posição lida
    fseek(f, g_posicaoLeitura, SEEK_SET);

    //Verifica se chegou ao fim do arquivo
    if (feof(f) || fgetc(f) == EOF) {
        fclose(f);
        g_posicaoLeitura = 0; //Reinicia o cursor
        return 0; //0 = Fim do arquivo
    }
    fseek(f, -1, SEEK_CUR);

    int dimLida, geracaoLida, contLido;

    fread(&dimLida, sizeof(int), 1, f);
    fread(&geracaoLida, sizeof(int), 1, f);
    fread(&contLido, sizeof(int), 1, f);

    //Verifica se a dimensão é compatível com o mapa atual
    if (dimLida != dimensao) {
        fclose(f);
        return -1; //-1 = Erro (dimensão incompatível)
    }

    //Carrega a geração
    Model_limparMapa(); //Limpa o tabuleiro atual
    geracao = geracaoLida; //Define o número da geração lida

    for (int i = 0; i < contLido; i++) {
        int l, c;
        fread(&l, sizeof(int), 1, f);
        fread(&c, sizeof(int), 1, f);
        if (Model_getEstadoCelula(l, c) == MORTA) { //Proteção contra coordenadas inválidas
            matriz[l][c] = VIVA;
        }
    }

    //Atualiza o cursor para a próxima leitura
    g_posicaoLeitura = ftell(f);
    fclose(f);
    return 1; //1 = Sucesso
}
