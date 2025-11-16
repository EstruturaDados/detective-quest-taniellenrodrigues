#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 20

// ----------------------------------------------------------
// ESTRUTURA DAS SALAS (Árvore Binária da Mansão)
// ----------------------------------------------------------
typedef struct sala {
    char nome[50];
    char pista[100];   
    struct sala *esq;
    struct sala *dir;
} Sala;

// ----------------------------------------------------------
// ESTRUTURA DA ÁRVORE DE PISTAS (BST)
// ----------------------------------------------------------
typedef struct pistaNode {
    char pista[100];
    struct pistaNode *esq;
    struct pistaNode *dir;
} PistaNode;

// ----------------------------------------------------------
// ESTRUTURA DA TABELA HASH (pista → suspeito)
// ----------------------------------------------------------
typedef struct hashItem {
    char pista[100];
    char suspeito[50];
    struct hashItem *prox;  // para colisões (encadeamento)
} HashItem;

// vetor da tabela hash
HashItem *tabelaHash[TAM_HASH];

// ===================== FUNÇÕES AUXILIARES =====================

// Hash simples pela soma dos caracteres
int hash(const char *str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % TAM_HASH;
}

// ----------------------------------------------------------
// criarSala()
// Cria dinamicamente um cômodo com nome e pista
// ----------------------------------------------------------
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

// ----------------------------------------------------------
// inserirPista() – insere pista na BST (ordenada)
// ----------------------------------------------------------
PistaNode* inserirPista(PistaNode *raiz, const char *p) {
    if (raiz == NULL) {
        PistaNode *novo = malloc(sizeof(PistaNode));
        strcpy(novo->pista, p);
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }
    if (strcmp(p, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, p);
    else
        raiz->dir = inserirPista(raiz->dir, p);
    return raiz;
}

// ----------------------------------------------------------
// inserirNaHash() – guarda pista → suspeito
// ----------------------------------------------------------
void inserirNaHash(const char *pista, const char *suspeito) {
    int indice = hash(pista);

    HashItem *novo = malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

// ----------------------------------------------------------
// encontrarSuspeito() – devolve suspeito associado à pista
// ----------------------------------------------------------
char* encontrarSuspeito(const char *pista) {
    int indice = hash(pista);
    HashItem *aux = tabelaHash[indice];

    while (aux != NULL) {
        if (strcmp(aux->pista, pista) == 0)
            return aux->suspeito;
        aux = aux->prox;
    }
    return NULL;
}

// ----------------------------------------------------------
// exibirPistas() – imprime a BST em ordem (a–z)
// ----------------------------------------------------------
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf(" - %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

// ----------------------------------------------------------
// contarPistasDeSuspeito() – recursivo na BST
// conta quantas pistas apontam para um suspeito
// ----------------------------------------------------------
int contarPistasDeSuspeito(PistaNode *raiz, const char *suspeito) {
    if (raiz == NULL) return 0;

    int total = contarPistasDeSuspeito(raiz->esq, suspeito);

    char *sus = encontrarSuspeito(raiz->pista);
    if (sus != NULL && strcmp(sus, suspeito) == 0)
        total++;

    total += contarPistasDeSuspeito(raiz->dir, suspeito);
    return total;
}

// ----------------------------------------------------------
// explorarSalas() – navegação da mansão + coleta de pistas
// ----------------------------------------------------------
void explorarSalas(Sala *atual, PistaNode **pistas) {
    char op;

    while (1) {
        printf("\n===============================\n");
        printf("Voce esta na sala: %s\n", atual->nome);
        printf("===============================\n");

        // mostra e coleta pista automaticamente
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
        }

        printf("\nEscolha seu caminho:\n");
        printf(" (e) Ir para a esquerda\n");
        printf(" (d) Ir para a direita\n");
        printf(" (s) Sair da mansao\n");
        printf("Opcao: ");
        scanf(" %c", &op);

        if (op == 'e' && atual->esq != NULL)
            atual = atual->esq;
        else if (op == 'd' && atual->dir != NULL)
            atual = atual->dir;
        else if (op == 's') {
            printf("\nExploracao encerrada!\n");
            return;
        } else {
            printf("Opcao invalida!\n");
        }
    }
}

// ----------------------------------------------------------
// verificarSuspeitoFinal() – julgamento final
// ----------------------------------------------------------
void verificarSuspeitoFinal(PistaNode *pistas) {
    char nomeSuspeito[50];

    printf("\n===========================\n");
    printf("     JULGAMENTO FINAL\n");
    printf("===========================\n");
    printf("Digite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", nomeSuspeito);

    int qtd = contarPistasDeSuspeito(pistas, nomeSuspeito);

    printf("\nPistas que incriminam %s: %d\n", nomeSuspeito, qtd);

    if (qtd >= 2)
        printf("\nVEREDITO: A acusacao procede! O suspeito e o culpado!\n");
    else
        printf("\nVEREDITO: Acusacao insuficiente. O suspeito deve ser liberado.\n");
}

// ============================== MAIN ==============================

int main() {
    // inicializa tabela hash
    for (int i = 0; i < TAM_HASH; i++)
        tabelaHash[i] = NULL;

    // ------------ CRIACAO DA MANSÃO (fixa) ------------
    Sala *hall = criarSala("Hall de Entrada", "Pegada enfraquecida na poeira");
    Sala *cozinha = criarSala("Cozinha", "Faca fora do suporte");
    Sala *biblioteca = criarSala("Biblioteca", "Pagina arrancada de diario");
    Sala *jardim = criarSala("Jardim", "Pegada de bota suja");
    Sala *porão = criarSala("Porao", "Caixa aberta recentemente");
    Sala *escritorio = criarSala("Escritorio", "Envelope amassado no lixo");

    // ligações (árvore binária)
    hall->esq = cozinha;
    hall->dir = biblioteca;

    cozinha->esq = jardim;
    cozinha->dir = porão;

    biblioteca->esq = escritorio;
    biblioteca->dir = NULL;

    // ------------ tabela hash: pista → suspeito ------------
    inserirNaHash("Pegada enfraquecida na poeira", "Sr. Victor");
    inserirNaHash("Faca fora do suporte", "Chef Marcos");
    inserirNaHash("Pagina arrancada de diario", "Sra. Helena");
    inserirNaHash("Pegada de bota suja", "Chef Marcos");
    inserirNaHash("Caixa aberta recentemente", "Sr. Victor");
    inserirNaHash("Envelope amassado no lixo", "Sra. Helena");

    // ------------ exploração ------------
    PistaNode *pistas = NULL;

    printf("=====================================\n");
    printf("     DETECTIVE QUEST - NIVEL MESTRE\n");
    printf("=====================================\n");

    explorarSalas(hall, &pistas);

    // ------------ exibir pistas ------------
    printf("\nPistas coletadas:\n");
    exibirPistas(pistas);

    // ------------ julgamento ------------
    verificarSuspeitoFinal(pistas);

    return 0;
}
