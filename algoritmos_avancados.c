#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala da mansão
typedef struct sala {
    char nome[50];
    struct sala *esquerda;
    struct sala *direita;
} Sala;

// Função que cria uma sala dinamicamente
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memoria para a sala.\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função que permite o jogador explorar a mansão
void explorarSalas(Sala *atual) {
    char escolha;

    // Enquanto existir uma sala para explorar
    while (atual != NULL) {
        printf("\n=====================================\n");
        printf("Voce esta na: %s\n", atual->nome);
        printf("=====================================\n");

        // Caso a sala não tenha caminhos, o jogo termina
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Voce chegou ao fim da exploracao.\n");
            break;
        }

        // Mostra opções ao jogador
        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Ir para a esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Ir para a direita  -> %s\n", atual->direita->nome);
        printf("  (s) Sair da exploracao\n");
        printf("Opcao: ");
        scanf(" %c", &escolha);

        // Limpa o buffer
        while (getchar() != '\n');

        // Executa a escolha
        if (escolha == 'e' && atual->esquerda != NULL)
            atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita != NULL)
            atual = atual->direita;
        else if (escolha == 's') {
            printf("\nVoce decidiu encerrar a exploracao.\n");
            break;
        } else {
            printf("\nCaminho invalido! Tente novamente.\n");
        }
    }
}

// Função principal que monta o mapa da mansão e inicia o jogo
int main() {
    // Criação manual da estrutura da mansão (árvore binária)
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *porao = criarSala("Porao Misterioso");
    Sala *sotao = criarSala("Sotao Assombrado");

    // Conexões (montagem da árvore)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;

    cozinha->esquerda = porao;
    cozinha->direita = sotao;

    // Introdução
    printf("=====================================\n");
    printf("  DETECTIVE QUEST - DESAFIO NOVATO\n");
    printf("=====================================\n");
    printf("Bem-vindo(a) à mansão! Sua missão é explorar os cômodos.\n");
    printf("Escolha entre os caminhos à esquerda (e) ou direita (d).\n");
    printf("Boa sorte, detetive!\n");

    // Inicia a exploração
    explorarSalas(hall);

    // Liberação de memória (boa prática)
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(porao);
    free(sotao);

    printf("\nJogo encerrado. Ate a proxima investigacao!\n");
    return 0;
}
