#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void limpar();

typedef struct {
    char titulo[100];
    char autor[100];
    int ano;
} Livro;

int main() {
   limpar();
   SetConsoleOutputCP(CP_UTF8);

   printf("| BIBLIOTECA VIRTUAL\n\n1 | Novo acesso\n2 | Consulta de acervo\n3 | Relatórios\n4 | Administração");

   int sl;
   while(sl>4 || sl<1) {
    printf("\n\nDigite o código da ação: ");
    scanf("%d", &sl);
   }
}

void limpar() {
    #ifdef _WIN32
        system("cls");  // Limpa o terminal no Windows
    #else
        system("clear");  // Limpa o terminal no Linux/macOS
    #endif
}

void adicionarLivroDinamicamente(Livro **livros, int *quantidade) {
    // Realoca memória para armazenar mais um livro
    *livros = realloc(*livros, (*quantidade + 1) * sizeof(Livro));
    if (*livros == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    // Solicita informações do livro
    printf("Digite o título do livro: ");
    fgets((*livros)[*quantidade].titulo, 100, stdin);
    (*livros)[*quantidade].titulo[strcspn((*livros)[*quantidade].titulo, "\n")] = 0;

    printf("Digite o autor do livro: ");
    fgets((*livros)[*quantidade].autor, 100, stdin);
    (*livros)[*quantidade].autor[strcspn((*livros)[*quantidade].autor, "\n")] = 0;

    printf("Digite o ano de publicação: ");
    scanf("%d", &(*livros)[*quantidade].ano);
    getchar(); // Limpa o buffer

    // Incrementa a quantidade de livros
    (*quantidade)++;
}




