#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----------------------------
// ESTRUTURA DA MANSÃO (ÁRVORE BINÁRIA)
// -----------------------------
typedef struct sala {
    char nome[50];
    char pista[100];    // pista opcional associada ao cômodo
    struct sala *esquerda;
    struct sala *direita;
} Sala;

// -----------------------------
// ESTRUTURA DA ÁRVORE DE PISTAS (BST)
// -----------------------------
typedef struct pistaNode {
    char pista[100];
    struct pistaNode *esq;
    struct pistaNode *dir;
} PistaNode;

// -----------------------------------------------------------
// Função: criarSala()
// Cria dinamicamente um cômodo com seu nome e pista opcional
// -----------------------------------------------------------
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memoria para sala!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// -----------------------------------------------------------
// Função: inserirPista()
// Insere uma pista na BST mantendo a ordem alfabética
// -----------------------------------------------------------
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}

// -----------------------------------------------------------
// Função: exibirPistas()
// Mostra todas as pistas em ordem alfabética (in-order)
// -----------------------------------------------------------
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf(" - %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

// -----------------------------------------------------------
// Função: explorarSalasComPistas()
// Permite navegar pela mansão e coleta pistas automaticamente
// -----------------------------------------------------------
void explorarSalasComPistas(Sala *atual, PistaNode **arvorePistas) {
    char opcao;

    while (1) {
        printf("\n====================================\n");
        printf("Voce esta na: %s\n", atual->nome);
        printf("====================================\n");

        // Se o cômodo tiver pista, coletar automaticamente
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        // Exibir caminhos
        printf("\nEscolha seu caminho:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Direita  -> %s\n", atual->direita->nome);

        printf("  (s) Sair da exploracao\n");
        printf("Opcao: ");
        scanf(" %c", &opcao);

        // Limpar buffer
        while (getchar() != '\n');

        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        }
        else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        }
        else if (opcao == 's') {
            printf("\nExploracao encerrada.\n");
            return;
        }
        else {
            printf("Opcao invalida! Tente novamente.\n");
        }
    }
}

// -----------------------------------------------------------
// Função principal
// -----------------------------------------------------------
int main() {
    // Construção da mansão (manual e fixa)
    Sala *hall       = criarSala("Hall de Entrada", "Pegada suspeita na poeira");
    Sala *estar      = criarSala("Sala de Estar", "Objeto caido atras do sofa");
    Sala *cozinha    = criarSala("Cozinha", "Faca fora do lugar");
    Sala *jardim     = criarSala("Jardim", "");
    Sala *biblioteca = criarSala("Biblioteca", "Pagina arrancada de um livro");
    Sala *porao      = criarSala("Porao", "Ruido estranho vindo das caixas");
    Sala *sotao      = criarSala("Sotao", "");

    // Ligações da árvore da mansão
    hall->esquerda = estar;
    hall->direita  = cozinha;

    estar->esquerda = biblioteca;
    estar->direita  = jardim;

    cozinha->esquerda = porao;
    cozinha->direita  = sotao;

    // Árvore de pistas coletadas começa vazia
    PistaNode *pistas = NULL;

    // Apresentação
    printf("=====================================\n");
    printf("   DETECTIVE QUEST - NIVEL INTERMEDIARIO\n");
    printf("=====================================\n");
    printf("Explore a mansao e colete pistas!\n");

    // Exploração interativa
    explorarSalasComPistas(hall, &pistas);

    // Exibição final das pistas coletadas
    printf("\n\n=====================================\n");
    printf("Pistas coletadas durante a exploracao:\n");
    printf("=====================================\n");

    if (pistas == NULL) {
        printf("Nenhuma pista encontrada.\n");
    } else {
        exibirPistas(pistas);
    }

    printf("\nObrigado por jogar Detective Quest!\n");

    return 0;
}
