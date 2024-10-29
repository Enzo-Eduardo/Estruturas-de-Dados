#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct tarefa {
    int id;
    char titulo[100];
    int prioridade;
    char data_criacao[11]; // Formato: DD/MM/YYYY
    struct tarefa* prox;
    struct tarefa* ant;
} Tarefa;

Tarefa* criar_tarefa(int id, const char* titulo, int prioridade, const char* data_criacao) {
    Tarefa* nova = (Tarefa*)malloc(sizeof(Tarefa));
    nova->id = id;
    strcpy(nova->titulo, titulo);
    nova->prioridade = prioridade;
    strcpy(nova->data_criacao, data_criacao);
    return nova;
}

void inserir_ordenado(Tarefa** cabeca, Tarefa* nova) {
    if (!*cabeca) {
        *cabeca = nova;
        nova->prox = nova->ant = nova;
    } else {
        Tarefa* atual = *cabeca;
        while (1) {
            if (nova->prioridade < atual->prioridade) {
                nova->prox = atual;
                nova->ant = atual->ant;
                atual->ant->prox = nova;
                atual->ant = nova;
                if (atual == *cabeca) *cabeca = nova;
                break;
            }
            atual = atual->prox;
            if (atual == *cabeca) {
                nova->prox = *cabeca;
                nova->ant = (*cabeca)->ant;
                (*cabeca)->ant->prox = nova;
                (*cabeca)->ant = nova;
                break;
            }
        }
    }
}

void remover(Tarefa** cabeca, int id) {
    if (!*cabeca) return;
    Tarefa* atual = *cabeca;
    do {
        if (atual->id == id) {
            if (atual->ant) atual->ant->prox = atual->prox;
            if (atual->prox) atual->prox->ant = atual->ant;
            if (atual == *cabeca) *cabeca = atual->prox;
            free(atual);
            return;
        }
        atual = atual->prox;
    } while (atual != *cabeca);
}

void buscar(Tarefa* cabeca, int id) {
    Tarefa* atual = cabeca;
    do {
        if (atual->id == id) {
            printf("ID: %d | Título: %s | Prioridade: %d | Data: %s\n", atual->id, atual->titulo, atual->prioridade, atual->data_criacao);
            return;
        }
        atual = atual->prox;
    } while (atual != cabeca);
    printf("Tarefa não encontrada.\n");
}

void listar(Tarefa* cabeca) {
    Tarefa* atual = cabeca;
    do {
        printf("ID: %d | Título: %s | Prioridade: %d | Data: %s\n", atual->id, atual->titulo, atual->prioridade, atual->data_criacao);
        atual = atual->prox;
    } while (atual != cabeca);
}

void libera_lista(Tarefa** cabeca) {
    if (!*cabeca) return;
    Tarefa* atual = *cabeca;
    Tarefa* prox;
    do {
        prox = atual->prox;
        free(atual);
        atual = prox;
    } while (atual != *cabeca);
    *cabeca = NULL;
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    Tarefa* cabeca = NULL;
    int opcao, id, prioridade;
    char titulo[100], data_criacao[11];

    while (1) {
        printf("\n1. Adicionar Tarefa\n2. Remover Tarefa\n3. Buscar Tarefa\n4. Listar Tarefas\n0. Sair\nEscolha: ");
        scanf("%d", &opcao);
        if (opcao == 1) {
            printf("ID: "); scanf("%d", &id);
            getchar(); printf("Título: "); fgets(titulo, sizeof(titulo), stdin); titulo[strcspn(titulo, "\n")] = 0;
            printf("Prioridade: "); scanf("%d", &prioridade);
            printf("Data (DD/MM/YYYY): "); scanf("%s", data_criacao);
            inserir_ordenado(&cabeca, criar_tarefa(id, titulo, prioridade, data_criacao));
        } else if (opcao == 2) {
            printf("ID: "); scanf("%d", &id); remover(&cabeca, id);
        } else if (opcao == 3) {
            printf("ID: "); scanf("%d", &id); buscar(cabeca, id);
        } else if (opcao == 4) {
            listar(cabeca);
        } else if (opcao == 0) {
            libera_lista(&cabeca); break;
        }
    }
    return 0;
}

