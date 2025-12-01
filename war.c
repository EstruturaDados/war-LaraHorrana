#include <stdio.h>
#include <stdlib.h> // Para malloc, calloc, free e rand
#include <string.h> // Para strings
#include <time.h>   // Para srand

// Define o tamanho máximo da string de missão
#define TAMANHO_MAX_MISSAO 100 

// Variável Global para o tamanho do mapa (Prática comum de iniciante)
int G_NUM_TERRITORIOS = 0;

// Requisito: Definição da Struct Territorio
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// Vetor global de missões (Requisito: Vetor de Strings)
char* MISSOES[] = {
    "Missao 0: Vencer com no minimo 10 tropas em um territorio.",
    "Missao 1: Conquistar no minimo 2 territorios com a cor Azul.",
    "Missao 2: Eliminar todas as tropas da cor Vermelha.",
    "Missao 3: Conquistar o territorio chamado 'Alaska'.",
    "Missao 4: Ter no minimo 5 territorios sob seu controle."
};
// O numero total de missões
const int TOTAL_MISSOES = 5;

// ================================================================================================
// FUNÇÕES DE UTILIDADE E GESTÃO DE MEMÓRIA
// ================================================================================================

// Requisito: Exibir os dados dos territórios.
void exibirMapa(struct Territorio* mapa) {
    if (mapa == NULL || G_NUM_TERRITORIOS == 0) {
        printf("\n[ERRO] O mapa está vazio.\n");
        return;
    }
    
    printf("\n=== ESTADO ATUAL DO MAPA ===\n");
    for (int i = 0; i < G_NUM_TERRITORIOS; i++) {
        // Acesso via ponteiro e notação de array
        printf("ID: %d | Nome: %s | Cor: %s | Tropas: %d\n", 
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("============================\n");
}

// Requisito: Liberar toda a memória alocada dinamicamente.
void liberarMemoria(struct Territorio* mapa, char* missao) {
    if (mapa != NULL) {
        free(mapa);
    }
    if (missao != NULL) {
        free(missao);
    }
    printf("\nMemória liberada. Programa encerrado.\n");
}

// ================================================================================================
// FUNÇÕES DE MISSÃO E VITÓRIA
// ================================================================================================

// Requisito: Sorteia e atribui a missão usando passagem por referência.
// O 'destino' é o ponteiro para onde a missão será copiada.
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    // Sorteia um índice aleatório
    int indiceSorteado = rand() % totalMissoes;
    
    // Copia a missão sorteada para a variável alocada dinamicamente do jogador
    strcpy(destino, missoes[indiceSorteado]);
}

// Requisito: Verifica se o jogador cumpriu a missão (Lógica Simples).
// Recebe a missão e o mapa para checar as condições.
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho, char* corJogador) {
    
    // Lógica simples de verificação (apenas 3 das 5 missões implementadas)
    
    // Missão 0: "Vencer com no minimo 10 tropas em um territorio."
    if (strstr(missao, "10 tropas") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            // Checa a cor E as tropas
            if (mapa[i].tropas >= 10 && strcmp(mapa[i].cor, corJogador) == 0) {
                return 1; // Vencedor!
            }
        }
    }
    
    // Missão 4: "Ter no minimo 5 territorios sob seu controle."
    else if (strstr(missao, "5 territorios") != NULL) {
        int contagem = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                contagem++;
            }
        }
        if (contagem >= 5) {
            return 1; // Vencedor!
        }
    }

    // Missão 3: "Conquistar o territorio chamado 'Alaska'."
    else if (strstr(missao, "Alaska") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].nome, "Alaska") == 0 && strcmp(mapa[i].cor, corJogador) == 0) {
                return 1; // Vencedor!
            }
        }
    }
    
    return 0; // Não cumpriu a missão
}

// ================================================================================================
// FUNÇÃO DE ATAQUE
// ================================================================================================

// Requisito: Função de ataque que usa ponteiros para modificar as structs.
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    
    // Validações básicas (requisito)
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[ALERTA] Não pode atacar o próprio território!\n");
        return;
    }
    if (atacante->tropas <= 1) {
        printf("\n[ALERTA] Precisa de no mínimo 2 tropas para atacar.\n");
        return;
    }

    printf("\n--- SIMULAÇÃO DE ATAQUE ---\n");
    printf("Atacante (%s) vs Defensor (%s)\n", atacante->nome, defensor->nome);

    // Rolagem de Dados (1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("  Dado Atacante: %d | Dado Defensor: %d\n", dadoAtacante, dadoDefensor);

    // Lógica da Batalha (Simples)
    if (dadoAtacante > dadoDefensor) {
        printf("\n[RESULTADO] Atacante VENCEU!\n");
        
        // Atacante Conquista o Território
        strcpy(defensor->cor, atacante->cor);
        
        // Transfere metade das tropas (garantindo que 1 fique no atacante)
        int tropasTransferidas = (atacante->tropas - 1) / 2; 
        
        atacante->tropas -= tropasTransferidas;
        defensor->tropas = tropasTransferidas; // Defensor assume as tropas transferidas
        
        printf("Território %s foi conquistado! Tropas transferidas: %d\n", defensor->nome, tropasTransferidas);
        
    } else {
        // Defensor Vence (ou Empate)
        printf("\n[RESULTADO] Defensor VENCEU (ou empate).\n");
        
        // Atacante perde 1 tropa (mínimo de 1 tropa no território)
        atacante->tropas--;
        printf("Atacante perdeu 1 tropa. Tropas restantes: %d\n", atacante->tropas);
    }
    printf("--- FIM DO ATAQUE ---\n");
}

// ================================================================================================
// FUNÇÃO PRINCIPAL
// ================================================================================================

int main() {
    // Requisito: Inicia o gerador de números aleatórios.
    srand(time(NULL));

    struct Territorio* mapa = NULL; 
    char corJogadorPrincipal[10] = "Azul"; // Cor do jogador que testaremos a missão
    
    // Alocação dinâmica para a missão (Requisito: Uso de malloc)
    char* missaoJogador = (char*)malloc(TAMANHO_MAX_MISSAO * sizeof(char)); 
    if (missaoJogador == NULL) {
        printf("[ERRO FATAL] Falha ao alocar memória para a missão.\n");
        return 1;
    }
    
    printf("===========================================\n");
    printf("      PROJETO WAR ESTRUTURADO (NÍVEL FINAL)\n");
    printf("===========================================\n");
    
    // 1. Configuração Inicial e Alocação de Memória
    printf("Digite o número TOTAL de territórios (min 5): ");
    if (scanf("%d", &G_NUM_TERRITORIOS) != 1 || G_NUM_TERRITORIOS < 5) {
        printf("[ALERTA] Usando 5 territórios padrão.\n");
        G_NUM_TERRITORIOS = 5;
    }
    
    // Requisito: Alocação Dinâmica (Uso de calloc para inicializar com zero)
    mapa = (struct Territorio*)calloc(G_NUM_TERRITORIOS, sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("[ERRO FATAL] Falha ao alocar memória para o mapa.\n");
        free(missaoJogador);
        return 1;
    }
    
    // 2. Cadastro dos Dados Iniciais (Simplificado para teste)
    // O estudante inicializa na mão para garantir o teste da missão (ex: Alaska)
    printf("\n--- CADASTRO INICIAL (Para Teste) ---\n");
    strcpy(mapa[0].nome, "Alaska"); strcpy(mapa[0].cor, "Azul"); mapa[0].tropas = 3;
    strcpy(mapa[1].nome, "Brasil"); strcpy(mapa[1].cor, "Vermelho"); mapa[1].tropas = 5;
    strcpy(mapa[2].nome, "China"); strcpy(mapa[2].cor, "Azul"); mapa[2].tropas = 7;
    strcpy(mapa[3].nome, "Africa"); strcpy(mapa[3].cor, "Vermelho"); mapa[3].tropas = 4;
    strcpy(mapa[4].nome, "Europa"); strcpy(mapa[4].cor, "Azul"); mapa[4].tropas = 2;
    // O resto fica vazio ou pede input
    for (int i = 5; i < G_NUM_TERRITORIOS; i++) {
        printf("\nTerritório #%d (ID %d):\n", i + 1, i);
        printf("  Nome: "); scanf("%29s", mapa[i].nome); 
        printf("  Cor: "); scanf("%9s", mapa[i].cor);
        printf("  Tropas: "); scanf("%d", &mapa[i].tropas);
    }
    
    // 3. Atribuição de Missão
    atribuirMissao(missaoJogador, MISSOES, TOTAL_MISSOES);
    
    printf("\n--- STATUS DO JOGADOR %s ---\n", corJogadorPrincipal);
    printf("MISSÃO SECRETA: %s\n", missaoJogador); // Requisito: Exibir a missão no início.
    printf("------------------------------\n");
    
    exibirMapa(mapa);
    
    int idAtacante, idDefensor;
    char continuar = 's';
    int missaoCumprida = 0;

    // 4. Loop Principal do Jogo
    while (continuar == 's' || continuar == 'S') {
        
        printf("\n\n=== TURNO DE ATAQUE ===\n");
        printf("Digite o ID (0 a %d) do seu ATACANTE (Cor %s): ", G_NUM_TERRITORIOS - 1, corJogadorPrincipal);
        scanf("%d", &idAtacante);
        
        printf("Digite o ID (0 a %d) do DEFENSOR inimigo: ", G_NUM_TERRITORIOS - 1);
        scanf("%d", &idDefensor);

        // Chamada da função de ataque
        atacar(&mapa[idAtacante], &mapa[idDefensor]);
        
        exibirMapa(mapa);
        
        // Requisito: Verificar silenciosamente se a missão foi cumprida
        missaoCumprida = verificarMissao(missaoJogador, mapa, G_NUM_TERRITORIOS, corJogadorPrincipal);
        
        if (missaoCumprida) {
            printf("\n*\n");
            printf("            PARABÉNS! MISSÃO CUMPRIDA!\n");
            printf("  O jogador da cor %s VENCEU! Missão: %s\n", corJogadorPrincipal, missaoJogador);
            printf("*\n");
            break; // Sai do jogo
        } else {
            printf("\n[STATUS] Missão ainda não concluída.\n");
        }

        printf("\nDeseja realizar mais um ataque? (s/n): ");
        scanf(" %c", &continuar); // Uso do espaço antes do %c é um "truque" de iniciante para limpar o buffer
    }
    
    // 5. Liberação de Memória
    // Requisito: free() no final
    liberarMemoria(mapa, missaoJogador);
    
    return 0;
}