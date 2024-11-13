#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

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
    char data_emp[20], data_devp[20];
    int status; //0 - ativo | 1 - finalizado
} Temprestimos;

typedef struct reserva {
    int codigo, codigo_livro, cod_leitor;
    char data_reserva[20];
} Treserva;
void limpar();
Tlivro *inicializa_livros (int *num_linhasLivro);
Tleitor *inicializa_leitor(int *num_linhasLeitor);
Tfuncionario *inicializa_funcionarios(int *num_linhasFuncionario);
Temprestimos *inicializa_emprestimos (int *num_linhasEmprestimos);
Treserva *inicializa_reserva (int *num_linhasReserva);
void adicionarUsuario(Tleitor **leitores, int *quantidade);
void novoacesso(Tleitor **leitores, int *quantidade, Temprestimos **emprestimos, int *quantidadeem, Tlivro **livros, int *quantidadeli);
int diferenca_tempo(const char *d1, const char *d2);
int diasNoMes(int mes, int ano);
void adicionarDias(char *data, int dias, char *novadata);
void relatorio(Tlivro **plivros, int *numlinhasLivro, Treserva **preservas, int *numlinhasReserva, Temprestimos **pemprestimo, int *numlinhasEmprestimo);
void cadastro_funcionario(Tfuncionario **funcionarios, int *quantidade);

int main() {
    limpar();
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    FILE *livros, *leitores, *funcionarios, *emprestimos, *reservas; //criação dos ponteiros que irão ler os arquivos
    int num_linhasLivro = 0, num_linhasLeitor = 0, num_linhasFuncionario = 0, num_linhasEmprestimo = 0, num_linhasReserva = 0; //contador i pro for e num_linhas pra quantidade de linhas de cada arquivo
    Tlivro *plivros = NULL;
    Tleitor *pleitor = NULL;
    Tfuncionario *pfuncionario = NULL;
    Temprestimos *pemprestimos = NULL;
    Treserva *preserva = NULL;
    plivros = inicializa_livros(&num_linhasLivro);
    pleitor = inicializa_leitor(&num_linhasLeitor);
    pfuncionario = inicializa_funcionarios(&num_linhasFuncionario);
    pemprestimos = inicializa_emprestimos(&num_linhasEmprestimo);
    preserva = inicializa_reserva(&num_linhasReserva);
    if(!livros || !leitores || !funcionarios || !emprestimos || !reservas) { //verificação se o arquivo foi aberto
        printf("Erro! Impossível de abrir um dos arquivos.\n");
        exit(1);
    }
    else{
         printf("Sucesso! Arquivos inicializados.\n"); 
    }

   int sl = 0;
   while(sl != 5) {
    limpar();
    printf("\n| BIBLIOTECA VIRTUAL\n\n1 | Novo acesso\n2 | Consulta de acervo\n3 | Relatórios\n4 | Administração \n5 | Sair");
    printf("\n\nDigite o código da ação: ");
    scanf("%d", &sl);
    switch (sl)
    {
    case 1:
        novoacesso(&pleitor, &num_linhasLeitor, &pemprestimos, &num_linhasEmprestimo, &plivros, &num_linhasLivro);
        break;
    case 3:
        relatorio(&plivros, &num_linhasLivro, &preserva, &num_linhasReserva, &pemprestimos, &num_linhasEmprestimo);
        break;
    case 4:
        cadastro_funcionario(&pfuncionario, &num_linhasFuncionario);
        break;
    case 5: 
        printf("\nAté a próxima!\n");
        break;
    default:
        printf("Opção inválida!\n");
        break;
    }
    
   }
}

void limpar() {
    #ifdef _WIN32
        system("cls");  // Limpa o terminal no Windows
    #else
        system("clear");  // Limpa o terminal no Linux/macOS
    #endif
}
/*
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
*/

Tlivro *inicializa_livros (int *num_linhas) {
    FILE *livros;
    Tlivro *plivros; //criação do ponteiro que vai mexer com a struct
    #ifdef _WIN32
    livros = fopen("C:.\\dados\\livros.txt", "r"); // Caminho para Windows
    #else
        livros = fopen("./dados/livros.txt", "r"); // Caminho para Linux/macOS
    #endif
    fscanf(livros, "%d", num_linhas);//recebe a quantidade de linhas
    plivros = (Tlivro *)malloc(sizeof(Tlivro) * *num_linhas);
    
    for (int i = 0; i < *num_linhas; i++) { //leitura dos dados e armazenamento nas structs
        fscanf(livros, "%d %s %s %s %d %d %d", &(plivros + i)->codigo, &(plivros + i)->titulo, &(plivros + i)->autor, &(plivros + i)->genero, &(plivros + i)->status, &(plivros + i)->num_reservas, 
        &(plivros + i)->qnt_total);
    }
    return plivros;
}

Tleitor *inicializa_leitor(int *num_linhasLeitor){
    FILE *leitores;
    Tleitor *pleitor; //criação do ponteiro que vai mexer com a struct
    //criação de um vetor de structs dinâmico
    #ifdef _WIN32// Caminho para Windows
    leitores = fopen("C:.\\dados\\leitores.txt", "r");
    #else // Caminho para Linux/macOS
        leitores = fopen("./dados/leitores.txt", "r");
    #endif
    fscanf(leitores, "%d", num_linhasLeitor);
    pleitor = (Tleitor *)malloc(sizeof(Tleitor) * *num_linhasLeitor);  // Aloca memória para os leitores
    // Lê os dados dos leitores (nome, e-mail, etc.)
    for (int i = 0; i < *num_linhasLeitor; i++) {
        fscanf(leitores, "%d %s %s %d %d", &(pleitor + i)->codigo, &(pleitor + i)->nome, &(pleitor + i)->email, &(pleitor + i)->qtd_emprestados, &(pleitor + i)->hist_multas);
    }
    return pleitor;
}

Tfuncionario *inicializa_funcionarios (int *num_linhasFuncionario) {
    FILE *funcionarios;
    Tfuncionario *pfuncionario; //criação do ponteiro que vai mexer com a struct
    #ifdef _WIN32
    funcionarios = fopen("C:.\\dados\\funcionarios.txt", "r"); // Caminho para Windows
    #else
        funcionarios = fopen("./dados/funcionarios.txt", "r"); // Caminho para Linux/macOS
    #endif
    fscanf(funcionarios, "%d", num_linhasFuncionario);//recebe a quantidade de linhas
    pfuncionario = (Tfuncionario *)malloc(sizeof(Tfuncionario) * *num_linhasFuncionario);
    
    for (int i = 0; i < *num_linhasFuncionario; i++) { //leitura dos dados e armazenamento nas structs
        fscanf(funcionarios, "%d %s %d %d %d", &(pfuncionario + i)->codigo, &(pfuncionario + i)->nome, &(pfuncionario + i)->cargo, &(pfuncionario + i)->total_emprestimos, &(pfuncionario + i)->total_devolucoes);
    }
    return pfuncionario;
}

Temprestimos *inicializa_emprestimos (int *num_linhasEmprestimos) {
    FILE *emprestimos;
    Temprestimos *pemprestimo; //criação do ponteiro que vai mexer com a struct
    #ifdef _WIN32
    emprestimos = fopen("C:.\\dados\\emprestimos.txt", "r"); // Caminho para Windows
    #else
        emprestimos = fopen("./dados/emprestimos.txt", "r"); // Caminho para Linux/macOS
    #endif
    fscanf(emprestimos, "%d", num_linhasEmprestimos);//recebe a quantidade de linhas
    pemprestimo = (Temprestimos *)malloc(sizeof(Temprestimos) * *num_linhasEmprestimos);
    
    for (int i = 0; i < *num_linhasEmprestimos; i++) { //leitura dos dados e armazenamento nas structs
        fscanf(emprestimos, "%d %d %d %s %s %d", &(pemprestimo + i)->codigo, &(pemprestimo + i)->codigo_livro, &(pemprestimo + i)->cod_leitor, &(pemprestimo + i)->data_emp, &(pemprestimo + i)->data_devp, &(pemprestimo + i)->status);
    }
    return pemprestimo;
}

Treserva *inicializa_reserva (int *num_linhasReserva) {
    FILE *reservas;
    Treserva *preserva; //criação do ponteiro que vai mexer com a struct
    #ifdef _WIN32
    reservas = fopen("C:.\\dados\\reservas.txt", "r"); // Caminho para Windows
    #else
        reservas = fopen("./dados/reservas.txt", "r"); // Caminho para Linux/macOS
    #endif
    fscanf(reservas, "%d", num_linhasReserva);//recebe a quantidade de linhas
    preserva = (Treserva *)malloc(sizeof(Treserva) * *num_linhasReserva);
    
    for (int i = 0; i < *num_linhasReserva; i++) { //leitura dos dados e armazenamento nas structs
        fscanf(reservas, "%d %d %d %s", &(preserva + i)->codigo, &(preserva + i)->codigo_livro, &(preserva + i)->cod_leitor, &(preserva + i)->data_reserva);
    }
    return preserva;
}

void adicionarUsuario(Tleitor **leitores, int *quantidade) {

    // Realoca memória -  adiciona um item a Tleitor
    Tleitor *temp = realloc(*leitores, (*quantidade + 1) * sizeof(Tleitor));
    if (temp == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    // Atualiza o ponteiro de leitores
    *leitores = temp;

    // Adição de 1 Leitor a quantidade
   
    printf("Digite o nome do usuário: ");
    scanf("%s", &(*leitores)[*quantidade].nome);
    getchar(); // Limpa o buffer

    printf("Digite o e-mail do usuário: ");
    scanf("%s", &(*leitores)[*quantidade].email);
    getchar(); // Limpa o buffer


    // Gerar código para o usuário
   
    (*leitores)[*quantidade].codigo = *quantidade+1;  // Código sequencial

    // Inicializa os outros campos como 0
    (*leitores)[*quantidade].qtd_emprestados = 0;
    (*leitores)[*quantidade].hist_multas = 0;

     (*quantidade)++;
}

void reescreverLeitor(Tleitor **leitores, int *quantidade){

   FILE *reescrita;
    int i;

    #ifdef _WIN32
        reescrita = fopen("C:.\\dados\\leitores.txt", "w");
    #else
        reescrita = fopen("./dados/leitores.txt", "w");
    #endif

    if (reescrita == NULL) {
        printf("Erro ao abrir o arquivo em modo de edição!\n");
        return;  // Retorna sem fazer nada se o arquivo não for aberto
    }

    // Escreve a quantidade de leitores no arquivo
    fprintf(reescrita, "%d\n", *quantidade);  // Escreve o número de leitores no começo do arquivo

    for (i = 0; i < *quantidade; i++) {
        // Escreve os dados de cada leitor no arquivo
        fprintf(reescrita, "%d %s %s %d %d\n",
                (*leitores)[i].codigo, (*leitores)[i].nome, 
                (*leitores)[i].email, (*leitores)[i].qtd_emprestados, 
                (*leitores)[i].hist_multas);
    }

    fclose(reescrita);  // Fecha o arquivo

    printf("Arquivo reescrito com sucesso!\n");
}

void novoacesso(Tleitor **leitores, int *quantidade, Temprestimos **emprestimos, int *quantidadeem, Tlivro **livros, int *quantidadeli) {
    limpar();
    getchar(); // Limpa o buffer
    int op;
    printf("| NOVO ACESSO\n\n1 | Usuário existente\n2 | Adicionar usuário\n\nDigite a operação: ");
    scanf("%d", &op);
    getchar(); // Limpa o buffer
    if (op == 1) {
        limpar();
        printf("| NOVO ACESSO\n\nDigite o código do usuário:");
        scanf("%d", &op);
        getchar(); // Limpa o buffer

        printf("| LEITOR\nCódigo:%d\nNome: %s\nE-mail: %s\n",   
        (*leitores)[op-1].codigo,  
        (*leitores)[op-1].nome,
        (*leitores)[op-1].email);
        int cod = op-1;

        printf("\n\n1 | Empréstimo\n2 | Renovar/Devolver livro");
        scanf("%d", &op);
        getchar(); // Limpa o buffer
        if(op==2){
            int i;
            limpar();
            printf("Livros locados - %s",  (*leitores)[cod].nome);
            printf("\n");
            for(i=0;i<*quantidadeem; i++){
                printf("%d", (*emprestimos)[i].cod_leitor);
               if(((*emprestimos)[i].cod_leitor == (*leitores)[cod].codigo) && ((*emprestimos)[i].status == 0)){
                int j;
                char nlivro[20];
                 printf("FER ABRIU!!!!");
                for(j = 0; j < *quantidadeli; j++) {
                    if ((*livros)[j].codigo == (*emprestimos)[i].codigo_livro) {
                        // Aqui você pode realizar o que deseja quando o código do livro corresponde
                    strcpy(nlivro, (*livros)[j].titulo);
                    }
            }

                printf("\n%d | Livro: %s | Dat. empr: %s - Dat. dev: %s",(*emprestimos)[i].codigo, nlivro, (*emprestimos)[i].data_emp,(*emprestimos)[i].data_devp);
            }
            int lop;
            printf("\nDigite o código do livro para operação: ");
            scanf("%d", &lop);
            getchar(); // Limpa o buffer

            printf("\n\n1 | Renovar empréstimo\n2 | Devolver livro ");
            scanf("%d", &op);
            getchar(); // Limpa o buffer
            if(op == 1){
                char* novadata[20]; 
                int dif = diferenca_tempo((*emprestimos)[lop].data_emp,(*emprestimos)[lop].data_devp);
               
                if(dif == 0){
                  
                    adicionarDias((*emprestimos)[lop].data_devp, 7, *novadata);
                    //NAO ESTA FUNCIONADO NOVADATA
                    printf("Livro renovado! A nova data de devolução é: %s",novadata);
                   
                }
            }

        }

        printf("\nPressione qualquer tecla para voltar ao menu...\n");
        getchar();  // Aguarda o usuário pressionar uma tecla antes de continuar


    } else if (op == 2) {
        limpar();
        printf("| NOVO ACESSO\n\n");
        // Chamando a função para adicionar o usuário
        adicionarUsuario(&*leitores, &*quantidade);
        
        // Exibindo informações após a adição do usuário
        limpar();
        printf("| NOVO ACESSO\n\n");
        printf("| NOVO USUÁRIO CADASTRADO\nCódigo:%d\nNome: %s\nE-mail: %s\n",   
        (*leitores)[*quantidade - 1].codigo,  
        (*leitores)[*quantidade - 1].nome,
        (*leitores)[*quantidade - 1].email);

        reescreverLeitor(&*leitores, &*quantidade);

        // Você pode adicionar aqui um "pause" se necessário
        printf("\nPressione qualquer tecla para voltar ao menu...\n");
        getchar();  // Aguarda o usuário pressionar uma tecla antes de continuar
    }
}
}

//Função de separação de datas


int diferenca_tempo(const char *d1, const char *d2) {
    struct tm t1 = {0}, t2 = {0};
    time_t time1, time2;
    
    sscanf(d1, "%d-%d-%d", &t1.tm_mday, &t1.tm_mon, &t1.tm_year);
    sscanf(d2, "%d-%d-%d", &t2.tm_mday, &t2.tm_mon, &t2.tm_year);
    
    t1.tm_mon -= 1; t1.tm_year -= 1900;
    t2.tm_mon -= 1; t2.tm_year -= 1900;

    time1 = mktime(&t1);
    time2 = mktime(&t2);

    return (difftime(time2, time1) / (60 * 60 * 24) > 7) ? 1 : 0;
}

int diasNoMes(int mes, int ano) {
    // Número de dias de cada mês
    if (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12)
        return 31;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11)
        return 30;
    // Fevereiro, verificando ano bissexto
    return (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0)) ? 29 : 28;
}

void adicionarDias(char *data, int dias, char *novadata) {
 

    int dia, mes, ano;
    sscanf(data, "%d-%d-%d", &dia, &mes, &ano);

    // Adicionando os dias à data
    dia += dias;

    // Verificando o dia, mês e ano após a adição


    // Ajustando a data se o dia ultrapassar o número de dias do mês
    while (dia > diasNoMes(mes, ano)) {
        dia -= diasNoMes(mes, ano);
        mes++;
        if (mes > 12) {
            mes = 1;
            ano++;
        }
    }

    printf("Devolver em: %02d-%02d-%d\n", dia, mes, ano);

    // Formatando a nova data
    sprintf(novadata, "%02d-%02d-%d", dia, mes, ano);
    return;
}

void relatorio(Tlivro **plivros, int *numlinhasLivro, Treserva **preservas, int *numlinhasReserva, Temprestimos **pemprestimo, int *numlinhasEmprestimo) {
    int op;
    int *vcontador = (int *)malloc(sizeof(int) * *numlinhasLivro);
    for (int i = 0; i < *numlinhasLivro; i++)
        *(vcontador + i) = 0;
    printf("\n| RELATÓRIOS\n1 - LIVROS\n");
    scanf("%d", &op);
    switch (op) {
    case 1:
        
        /* todos os livros disponíveis e emprestados */
        printf("\nLivros disponíveis: \n");
        for (int i = 0; i < *numlinhasLivro; i++) {
            if ((*plivros)[i].status)
                printf("%s\n", (*plivros)[i].titulo);
        }
        printf("\nLivros emprestados: \n");
        for (int i = 0; i < *numlinhasLivro; i++) {
            if ((*plivros)[i].status == 3)
                printf("%s\n", (*plivros)[i].titulo);
        }
        printf("\n");
        //livros mais emprestados e reservas em aberto
        int livro;
        int j;
        for (int i = 0; i < *numlinhasEmprestimo; i++) {
            printf("%d ", (*pemprestimo)[i].codigo_livro);
            
        break;
    default:
        break;
    }

}
}

void cadastro_funcionario(Tfuncionario **funcionarios, int *quantidade) {
    Tfuncionario *temp = realloc(*funcionarios, (*quantidade + 1) * sizeof(Tfuncionario));
    if (temp == NULL) {
        printf("Erro ao alocar memória.\n");
        getchar();
        exit(1);
    
    }
    // Atualiza o ponteiro de funcionarios
    *funcionarios = temp;

    // Adição de 1 funcionário a quantidade
   
    printf("Digite o nome do funcionário: ");
    scanf("%s", &(*funcionarios)[*quantidade].nome);
    getchar(); // Limpa o buffer

    printf("| 1 - operador |\n| 2 - auxiliar |\n| 3 - administrador |\nDigite o cargo do funcionário: ");
    scanf("%d", &(*funcionarios)[*quantidade].cargo);
    getchar(); // Limpa o buffer

    // Gerar código para o funcionário
   
    (*funcionarios)[*quantidade].codigo = *quantidade+1;  // Código sequencial

    (*funcionarios)[*quantidade].total_emprestimos = 0;
    (*funcionarios)[*quantidade].total_devolucoes = 0;

     (*quantidade)++;
     printf("\nFuncionário cadastrado\n");
     getchar();
}