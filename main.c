#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void limpar();

typedef struct livros {
    int codigo;
    char titulo[20], autor[20], genero[15];
    int status; //1 - disponivel | 2 - reservado | 3 - emprestado
    int num_reservas, qnt_total;
} Tlivro;

typedef struct leitor {
    int codigo;
    char nome[20], email[15];
    int qtd_emprestados, hist_multas;
} Tleitor;

typedef struct funcionario {
    int codigo;
    char nome[20];
    int cargo; //1 - operador | 2 - auxiliar | 3 - administrador
    int total_emprestimos, total_devolucoes;
} Tfuncionario;

typedef struct emprestimos {
    int codigo, codigo_livro, cod_leitor;
    char data_emp[20], data_devp[15];
    int status; //0 - ativo | 1 - finalizado
} Temprestimos;

typedef struct reserva {
    int codigo, codigo_livro, cod_leitor;
    char data_reserva[20];
} Treserva;

int main() {
    limpar();
    SetConsoleOutputCP(CP_UTF8);
    FILE *livros, *leitores, *funcionarios, *emprestimos, *reservas; //criação dos ponteiros que irão ler os arquivos
    Tlivro *plivros; //criação do ponteiro que vai mexer com a struct
    int i, num_linhas; //contador i pro for e num_linhas pra quantidade de linhas de cada arquivo
    livros = fopen("C:.\\dados\\livros.txt", "r"); //abrindo um arquivo pelo windows
    if(!livros) { //verificação se o arquivo foi aberto
        printf("Arquivo não aberto\n");
        exit(1);
    }
    printf("Sucesso na abertura do arquivo\n"); 
    fscanf(livros, "%d", &num_linhas); //recebimento da quantidade de linhas do arquivo livros
    if (num_linhas == 5) //verificação
        printf("Sucesso na leitura de tamanho\n");
    plivros = (Tlivro *)malloc(sizeof(Tlivro) * num_linhas); //criação de um vetor de structs dinâmico
    for (i = 0; i < num_linhas; i++) { //leitura dos dados e armazenamento nas structs
        fscanf(livros, "%d %s %s %s %d %d %d", &(plivros + i)->codigo, &(plivros + i)->titulo, &(plivros + i)->autor, &(plivros + i)->genero, &(plivros + i)->status, &(plivros + i)->num_reservas, &(plivros + i)->qnt_total);
    }
    for (i = 0; i < num_linhas; i++) //impressão de cada struct
        printf("%d %s %s %s %d %d %d\n", ((plivros + i)->codigo), ((plivros + i)->titulo), ((plivros + i)->autor), ((plivros + i)->genero), ((plivros + i)->status), ((plivros + i)->num_reservas), ((plivros + i)->qnt_total));

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

void adicionarLivroDinamicamente(Tlivro **livros, int *quantidade) {
    // Realoca memória para armazenar mais um livro
    *livros = realloc(*livros, (*quantidade + 1) * sizeof(Tlivro));
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

    printf("Digite o gênero: ");
    scanf("%d", &(*livros)[*quantidade].genero);
    getchar(); // Limpa o buffer

    // Incrementa a quantidade de livros
    (*quantidade)++;
}




