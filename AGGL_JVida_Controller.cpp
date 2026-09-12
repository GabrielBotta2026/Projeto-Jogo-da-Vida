//DOM-AGGL-Controller.cpp - ETAPA 5.
//11/11/2025 - Grupo: AGGL
//Ana Clara Boaventura Dattoli
//Gabriel Teixeira Botta
//Guilherme Almeida Coutinho
//Livia de Aguiar Almeida Figueiredo

#include "AGGL_JVida_Controller.h"
#include "AGGL_JVida_Model.h"
#include "AGGL_JVida_View.h"
#include <windows.h>

static char g_arquivoAtual[100];

static void atualizarMapaNaView(){
    //1. Obter listas do Model
    ListaCelula* vivas = Model_getListaVivas();
    ListaCelula* vizinhas = NULL;
    
    //Só gera a lista de vizinhas se o modo estiver ativo (otimização)
    if (View_getModoVizinhas()){
        vizinhas = Model_getListaVizinhasMortas();
    }

    //2. Passar listas para a View
    View_apresentarMapa(vivas, vizinhas);

    //3. Limpar a memória das listas
    Model_limparLista(vivas);
    if (vizinhas != NULL){
        Model_limparLista(vizinhas);
    }
}

//Funções auxiliares, estáticas para serem "privadas" a este arquivo
static void configurarSimulacao(){
    int dimensao = View_getDimensaoDoUsuario();
    Model_inicializar(dimensao);
    g_arquivoAtual[0] = '\0';
    View_exibirMensagemInicializacao();
    atualizarMapaNaView();
}

static void gerenciarCelulas(){
    int linha, coluna;
    
    while (1){ //Loop infinito, sai com break
        View_getCoordenadas(&linha, &coluna);

        if (linha == -1){
            break; //Usuário decidiu sair do modo de edição
        }

        if (linha < 0 || linha >= Model_getDimensao() || coluna < 0 || coluna >= Model_getDimensao()){
            View_exibirMensagemCoordenadaInvalida();
            continue; //Pede novas coordenadas
        }
        
        bool foiAdicionada = Model_inverterEstadoCelula(linha, coluna);
        if (foiAdicionada){
            View_exibirMensagemCelulaAdicionada(linha, coluna);
        } else{
            View_exibirMensagemCelulaRemovida(linha, coluna);
        }
        
        atualizarMapaNaView(); //Mostra o mapa atualizado
    }
}

static void iniciarSimulacao(){
    int numGeracoes = View_getNumeroGeracoes();
    int velocidade = View_getVelocidadeSimulacao();
    int delay_ms = 0;

    // Mapeia a velocidade para milissegundos
    if (velocidade == 1) delay_ms = 1000;
    else if (velocidade == 2) delay_ms = 500;
    else if (velocidade == 3) delay_ms = 200;
    //Se velocidade == 0, delay_ms continua 0 (modo passo-a-passo)

    for (int i = 0; i < numGeracoes; i++){
        View_limparTela();
        Model_avancarGeracao();
        atualizarMapaNaView();

        if (velocidade == 0){
            View_aguardarEnter();
        }else{
            Sleep(delay_ms);
        }
    }
    View_exibirMensagemSimulacaoConcluida(numGeracoes);
}

void Controller_executar(){
    configurarSimulacao();
    
    int escolha;
    do {
        escolha = View_getEscolhaMenu();
        switch (escolha) {
            case 1:
                gerenciarCelulas();
                break;
            case 2:
                View_alternarModoVizinhas();
                atualizarMapaNaView(); //Atualiza o mapa para mostrar a mudança
                break;
            case 3:
                Model_limparMapa();
                View_exibirMensagemMapaLimpo();
                atualizarMapaNaView(); //Atualiza o mapa para mostrar que foi limpo
                break;
            case 4:
                iniciarSimulacao();
                break;
            case 5: // Salvar
            	{
                // Se já temos um arquivo aberto, perguntamos se quer continuar nele
                if (g_arquivoAtual[0] != '\0') {
                    printf("Arquivo atual: %s\n", g_arquivoAtual);
                    printf("1. Adicionar a sequencia deste arquivo\n");
                    printf("2. Criar um NOVO arquivo (iniciar nova sequencia)\n");
                    int subEscolha;
                    scanf("%d", &subEscolha);
                    while(getchar()!='\n'); // Limpa buffer
                    
                    if (subEscolha == 2) {
                        g_arquivoAtual[0] = '\0'; // Reseta para pedir novo nome
                    }
                }

                if (g_arquivoAtual[0] == '\0') {
                    char nomeTemp[100];
                    View_obterNomeArquivo(nomeTemp, 100);
                    
                    FILE* fLimpa = fopen(nomeTemp, "wb");
                    if (fLimpa) fclose(fLimpa);

                    if (Model_salvarGeracao(nomeTemp)) {
                        strcpy(g_arquivoAtual, nomeTemp);
                        View_exibirMensagemSalvo();
                    } else {
                        View_exibirMensagemErroSalvar();
                    }
                } else {
                    // Adiciona na sequência do arquivo existente
                    if (Model_salvarGeracao(g_arquivoAtual)) {
                        View_exibirMensagemSalvo();
                    } else {
                        View_exibirMensagemErroSalvar();
                    }
                }
                break;
            }
            case 6:
            { 
                char nomeTemp[100];
                //Sempre pede um nome de arquivo para carregar
                View_obterNomeArquivo(nomeTemp, 100);
                
                //Reseta o cursor de leitura para o novo arquivo
                Model_resetarCursorLeitura(); 
                
                //Tenta carregar a primeira geração do arquivo
                int status = Model_carregarProximaGeracao(nomeTemp);
                
                if (status == 1){
                    strcpy(g_arquivoAtual, nomeTemp);
                    View_exibirMensagemCarregado();
                    atualizarMapaNaView(); 
                } else if (status == 0){
                    View_exibirMensagemFimArquivo();
                } else{ // -1 = Erro
                    View_exibirMensagemErroCarregar();
                }
                break;
            }
            case 0:
                View_exibirMensagemEncerramento();
                break;
            default:
                View_exibirMensagemOpcaoInvalida();
                break;
        }
    } while (escolha != 0);
}
