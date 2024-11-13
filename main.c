#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
int comparar(const char *d1);
int diasNoMes(int mes, int ano);
void adicionarDias(char *data, int dias, char *novadata);
void escrever_relatorio(Tlivro **plivros, int *numlinhasLivro, Treserva **preservas, int *numlinhasReserva, Temprestimos **pemprestimo, int *numlinhasEmprestimo, Tleitor **pleitor, int *numlinhasLeitor, Tfuncionario **pfuncionario, int *numlinhasFuncionario);
void relatorio();
void cadastro_funcionario(Tfuncionario **funcionarios, int *quantidade);
void reescreverFuncionario(Tfuncionario **funcionarios, int *quantidadefun);
void cadastrar_emprestimo(Temprestimos **emprestimos, int *quantidade, int codl, int codleitor);
int diferenca(const char *data);

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
    case 2:
        consulta_acervo(&plivros, &num_linhasLivro);
        Sleep(5000);
        break;
    case 3:
        relatorio();
        Sleep(10000);
        break;
    case 4:
        cadastro_funcionario(&pfuncionario, &num_linhasFuncionario);
        break;
    case 5: 
        escrever_relatorio(&plivros, &num_linhasLivro, &preserva, &num_linhasReserva, &pemprestimos, &num_linhasEmprestimo, &pleitor, &num_linhasLeitor, &pfuncionario, &num_linhasFuncionario);
        printf("\nAté a próxima!\n");
        exit(1);
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
        op = 0;
        scanf("%d", &op);
        getchar(); // Limpa o buffer

        printf("| LEITOR\nCódigo:%d\nNome: %s\nE-mail: %s\n",   
        (*leitores)[op-1].codigo,  
        (*leitores)[op-1].nome,
        (*leitores)[op-1].email);
        int cod = op-1;

        printf("\n\n1 | Empréstimo\n2 | Renovar/Devolver livro");
        op = 0;
        scanf("%d", &op);
        getchar(); // Limpa o buffer
         printf("%d", op);
        if(op==1){
            int i, total;
            total =0;
            limpar();
             
            for(i=0;i<*quantidadeem; i++){
               if(((*emprestimos)[i].cod_leitor == (*leitores)[cod].codigo) && ((*emprestimos)[i].status == 0)){
                total = total+1;
                int j;
               }
            }
             
            if(total == 4){
                printf("Cota máxima de empréstimos realizada.\nVocê possuí 4 livros locados;\n\nDevolva algum livro antes de locar outro.");
            }
            else{
                int cod2;
                
                 printf("%s, você possui %d/4 livro(s) locados.",  (*leitores)[cod].nome, total);
                 printf("\nDigite o código do livro que deseja locar: ");
                
                scanf("%d", &cod2);
                getchar(); // Limpa o buffer
                
                

                int disp = (*livros)[cod2-1].status;
                if(disp == 1){
                    printf("Confirmar locação de '%s', para %s?", (*livros)[cod2-1].titulo, (*leitores)[cod].nome);
                     getchar();
                     cadastrar_emprestimo(&*emprestimos, &*quantidadeem, cod2-1, (*leitores)[cod].codigo);
                     (*livros)[cod2-1].codigo = 3;
                     
                }
                if(disp ==3){
                    printf("O Livro está emprestado e já possui uma reserva pendente.");
                     getchar();
                }
                if(disp==2){
                    printf("O Livro está indisponível no momento, porém pode ser reservado.");
                     getchar();;
                }
                
                
            }
           

        }
        
        if(op==2){
            int i;
            limpar();
            printf("Livros locados - %s",  (*leitores)[cod].nome);
            printf("\n");
            int total = 0;
            for(i=0;i<*quantidadeem; i++){
               if(((*emprestimos)[i].cod_leitor == (*leitores)[cod].codigo) && ((*emprestimos)[i].status == 0)){
                total++;
                int j;
                char nlivro[20];
                for(j = 0; j < *quantidadeli; j++) {
                    if ((*livros)[j].codigo == (*emprestimos)[i].codigo_livro) {
                    strcpy(nlivro, (*livros)[j].titulo);
                    }
            }

                printf("\n%d | Livro: %s | Dat. empr: %s - Dat. dev: %s",(*emprestimos)[i].codigo, nlivro, (*emprestimos)[i].data_emp,(*emprestimos)[i].data_devp);
            }
            if(total!=0){
                int lop;
            printf("\nDigite o código do livro para operação: ");
             op = 0;
            scanf("%d", &lop);
            getchar(); // Limpa o buffer

            printf("\n\n1 | Renovar empréstimo\n2 | Devolver livro ");
             op = 0;
            scanf("%d", &op);
            getchar(); // Limpa o buffer
            if(op == 1){
                char* novadata[20]; 
                int dif = comparar((*emprestimos)[lop-1].data_devp);
               
                if(dif == 0){
                
                    adicionarDias((*emprestimos)[lop-1].data_devp, 7, &novadata);
                    strcpy((*emprestimos)[lop-1].data_devp, novadata);
                    printf("%s",(*emprestimos)[lop-1].data_devp);
                    reescreverEmprestimo(&*emprestimos, &*quantidadeem);
                    printf("Livro renovado! A nova data de devolução é: %s",novadata);
                   
                }
                else{
                    limpar();
                    int dias = diferenca((*emprestimos)[lop-1].data_devp);
                    int multa = dias*2;
                    printf("Não é possível renovar seu empréstimo.\nMulta aplicável (%d dias em atraso)\nMulta em aberto de R$%.2f.\n\n",dias, (float)multa);
                    printf("\nPressione qualquer tecla para pagar multa e devolver");
                    getchar();  // Aguarda o usuário pressionar uma tecla antes de continuar
                    (*emprestimos)[lop-1].status = 1;
                    reescreverEmprestimo(&*emprestimos, &*quantidadeem);
                    printf("Livro devolvido! Obrigado.");
                     op = 0;
                }
            }
            else{

                char* novadata[20]; 
                int dif = comparar((*emprestimos)[lop-1].data_devp);
               
                if(dif == 0){
                
                    (*emprestimos)[lop-1].status = 1;
                    reescreverEmprestimo(&*emprestimos, &*quantidadeem);
                    printf("Livro devolvido! Obrigado.");
                   
                }
                else{
                    limpar();
                    int dias = diferenca((*emprestimos)[lop-1].data_devp);
                    int multa = dias*2;
                    printf("Não é possível renovar seu empréstimo.\nMulta aplicável (%d dias em atraso)\nMulta em aberto de R$%.2f.\n\n",dias, (float)multa);
                    printf("\nPressione qualquer tecla para pagar multa e devolver");
                    getchar();  // Aguarda o usuário pressionar uma tecla antes de continuar
                    (*emprestimos)[lop-1].status = 1;
                    reescreverEmprestimo(&*emprestimos, &*quantidadeem);
                    printf("Livro devolvido! Obrigado.");
                     op = 0;
                    

                }

            }

        }
        else{
            printf("Usuário sem livros locados no momento.");
            printf("\nPressione qualquer tecla para voltar ao menu.");
            op = 0;
            getchar();  // Aguarda o usuário pressionar uma tecla antes de continuar
            limpar();
            return;
        }}
            

   


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
        printf("\nPressione qualquer tecla para voltar ao menu...");
        getchar();  // Aguarda o usuário pressionar uma tecla antes de continuar
    }
}
}



//Função de separação de datas


int comparar(const char *d1) {
    struct tm t1 = {0}, t2 = {0};
    time_t time1, time2;

    // Converte a string de data no formato DD-MM-YYYY para a estrutura tm
    sscanf(d1, "%d-%d-%d", &t1.tm_mday, &t1.tm_mon, &t1.tm_year);
    t1.tm_mon -= 1;    // Ajusta o mês (tm_mon vai de 0 a 11)
    t1.tm_year -= 1900; // Ajusta o ano (tm_year conta a partir de 1900)

    // Obtém a data atual
    time(&time2);
    t2 = *localtime(&time2);

    // Converte ambas as estruturas tm para time_t
    time1 = mktime(&t1);
    time2 = mktime(&t2);
    
    // Compara as datas
    if (difftime(time1, time2) < 0) {
        return 1;  // d1 é antes da data atual
    } else {
        return 0;  // d1 não é antes da data atual
    }
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

    

    // Formatando a nova data
    sprintf(novadata, "%02d-%02d-%d", dia, mes, ano);
 
    return;
}

void escrever_relatorio(Tlivro **plivros, int *numlinhasLivro, Treserva **preservas, int *numlinhasReserva, Temprestimos **pemprestimo, int *numlinhasEmprestimo, Tleitor **pleitor, int *numlinhasLeitor, Tfuncionario **pfuncionario, int *numlinhasFuncionario) {
    FILE *relatorio;
    int op, i = 0, contMa = 0, posiMa = 0, contaMaF = 0, posiMaF = 0;
    int *vcont = (int *)malloc(sizeof(int) * *numlinhasLivro);
    int *movimentacoes = (int *)malloc(sizeof(int) * *numlinhasFuncionario);
    for (i = 0; i < *numlinhasLivro; i++) {
        *(vcont + i) = 0;
    }
    for (int i = 0; i < *numlinhasFuncionario; i++)
        *(movimentacoes + i) = 0;
    #ifdef _WIN32
        relatorio = fopen("C:.\\dados\\relatorio.txt", "w");
    #else
        relatorio = fopen("./dados/relatorio.txt", "w");
    #endif
        /* todos os livros disponíveis e emprestados */
        fprintf(relatorio, "Livros disponíveis: \n");
        for (int i = 0; i < *numlinhasLivro; i++) {
            if ((*plivros)[i].status == 1)
                fprintf(relatorio, "%s - %s\n", (*plivros)[i].titulo, (*plivros)[i].autor);
        }
        fprintf(relatorio, "\nLivros emprestados: \n");
        for (int i = 0; i < *numlinhasEmprestimo; i++) {
            if ((*pemprestimo)[i].status == 0)
                fprintf(relatorio, "%s\n", (*plivros)[((*pemprestimo)[i].codigo_livro - 1)].titulo);
        }
        int livro = 0;
        //livros mais emprestados e reservas em aberto
        fprintf(relatorio, "\nLivro mais emprestado: \n");
        for (int i = 0; i < *numlinhasEmprestimo; i++) {
            livro = (*pemprestimo)[i].codigo_livro;
            *(vcont + (livro - 1)) += 1;
        }
        for (int i = 0; i < *numlinhasLivro; i++) { 
            if (*(vcont + i) > contMa) {
                contMa = *(vcont + i);
                posiMa = i;
            }
        }
        fprintf(relatorio, "%s com %d empréstimos\n", (*plivros)[posiMa].titulo, contMa);
        fprintf(relatorio, "\nReservas em aberto: \n");
        for (int i = 0; i < *numlinhasReserva; i++) {
            fprintf(relatorio, "Livro: %s | Data reserva: %s | Leitor: %s \n", (*plivros)[((*preservas)[i].codigo_livro) - 1].titulo, (*preservas)[i].data_reserva, (*pleitor)[((*preservas)[i].cod_leitor) - 1].nome);
        }
        
        fprintf(relatorio, "\nFuncionário com maior número de movimentações:\n");//ver condição para dois com o mesmo número de movimentações
        for (int i = 0; i < *numlinhasFuncionario; i++)
            *(movimentacoes + i) = (*pfuncionario)[i].total_devolucoes + (*pfuncionario)[i].total_emprestimos;
        for (int i = 0; i < *numlinhasFuncionario; i++) {
            if (*(movimentacoes + i) > contaMaF) {
                contaMaF = *(movimentacoes + i);
                posiMaF = i;
            }
            
        }
        
        fprintf(relatorio, "Funcionário: %s | Total de movimentações: %d\nTotal empréstimos: %d | Total devoluções: %d\n", (*pfuncionario)[posiMaF].nome, contaMaF, (*pfuncionario)[posiMaF].total_emprestimos, (*pfuncionario)[posiMaF].total_devolucoes);
        //leitores com emprestimos ativos e histórico de multas
        
        fprintf(relatorio, "Leitores com empréstimos ativos: \n");
        for (int i = 0; i < *numlinhasEmprestimo; i++) {
            if (!((*pemprestimo)[i].status))
                fprintf("Leitor: %s\n", (*pleitor)[((*pemprestimo)[i].cod_leitor - 1)].nome);
        }
        fprintf(relatorio, "Leitores com empréstimos ativos: \n");
        for (int i = 0; i < *numlinhasLeitor; i++) {
            if ((*pleitor)[i].hist_multas > 0)
                fprintf("Leitor: %s\nQuantidade multas: %d\n", (*pleitor)[i].nome, (*pleitor)[i].hist_multas);
        }
       
        free(vcont);
        free(movimentacoes);
        fclose(relatorio);
        printf("Relatório escrito com sucesso!\n");
}


void relatorio() {
    FILE *relatorio;
    #ifdef _WIN32
        relatorio = fopen("C:.\\dados\\relatorio.txt", "r");
    #else
        relatorio = fopen("./dados/relatorio.txt", "r");
    #endif
    if (relatorio == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), relatorio) != NULL) {
        printf("%s", buffer);
    }
    printf("\n");
    fclose(relatorio);
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
    scanf("%s", (*funcionarios)[*quantidade].nome);
    getchar(); // Limpa o buffer

    printf(" 1 | operador \n 2 | auxiliar \n 3 | administrador \n\nDigite o cargo do funcionário: ");
    scanf("%d", &(*funcionarios)[*quantidade].cargo);
    getchar(); // Limpa o buffer

    // Gerar código para o funcionário
   
    (*funcionarios)[*quantidade].codigo = *quantidade + 1;  // Código sequencial

    (*funcionarios)[*quantidade].total_emprestimos = 0;
    (*funcionarios)[*quantidade].total_devolucoes = 0;

     (*quantidade)++;
    reescreverFuncionario(&*funcionarios, &*quantidade);
    printf("\nFuncionário cadastrado\n");
    getchar();
}

void cadastrar_emprestimo(Temprestimos **emprestimos, int *quantidade, int codl, int codleitor) {
    Temprestimos *temp = realloc(*emprestimos, (*quantidade + 1) * sizeof(Temprestimos));
    if (temp == NULL) {
        printf("Erro ao alocar memória.\n");
        getchar();
        exit(1);
    
    }
    // Atualiza o ponteiro de funcionarios
    *emprestimos = temp;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char data[20];
    // Formatar a data como DD-MM-YYYY
    sprintf(data, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);


    (*emprestimos)[*quantidade].codigo = *quantidade + 1;  // Código sequencial
    (*emprestimos)[*quantidade].codigo_livro = codl;
    (*emprestimos)[*quantidade].cod_leitor = codleitor;
    strcpy((*emprestimos)[*quantidade].data_emp, data);
    char novadata[20];
    adicionarDias((*emprestimos)[*quantidade].data_emp, 7, &novadata);
    strcpy((*emprestimos)[*quantidade].data_devp, novadata);
    (*emprestimos)[*quantidade].status = 0;
     (*quantidade)++;
   
    reescreverEmprestimo(&*emprestimos, &*quantidade);
    getchar();
}

void reescreverFuncionario(Tfuncionario **funcionarios, int *quantidadefun) {
   FILE *reescrita;
    int i;

    #ifdef _WIN32
        reescrita = fopen("C:.\\dados\\funcionarios.txt", "w");
    #else
        reescrita = fopen("./dados/funcionarios.txt", "w");
    #endif

    if (reescrita == NULL) {
        printf("Erro ao abrir o arquivo em modo de edição!\n");
        return;  // Retorna sem fazer nada se o arquivo não for aberto
    }

    // Escreve a quantidade de leitores no arquivo
    fprintf(reescrita, "%d\n", *quantidadefun);  // Escreve o número de leitores no começo do arquivo

    for (i = 0; i < *quantidadefun; i++) {
        // Escreve os dados de cada leitor no arquivo
        fprintf(reescrita, "%d %s %d %d %d\n",
                (*funcionarios)[i].codigo, 
                (*funcionarios)[i].nome, 
                (*funcionarios)[i].cargo, 
                (*funcionarios)[i].total_emprestimos, 
                (*funcionarios)[i].total_devolucoes);
    }
    fclose(reescrita);  // Fecha o arquivo
    printf("Arquivo reescrito com sucesso!\n");
}


void reescreverEmprestimo(Temprestimos **emprestimos, int *quantidadeem){

   FILE *reescrita;
    int i;

    #ifdef _WIN32
        reescrita = fopen("C:.\\dados\\emprestimos.txt", "w");
    #else
        reescrita = fopen("./dados/emprestimos.txt", "w");
    #endif

    if (reescrita == NULL) {
        printf("Erro ao abrir o arquivo em modo de edição!\n");
        return;  // Retorna sem fazer nada se o arquivo não for aberto
    }

    // Escreve a quantidade de leitores no arquivo
    fprintf(reescrita, "%d\n", *quantidadeem);  // Escreve o número de leitores no começo do arquivo

    for (i = 0; i < *quantidadeem; i++) {
        // Escreve os dados de cada leitor no arquivo
        fprintf(reescrita, "%d %d %d %s %s %d\n",
                (*emprestimos)[i].codigo, 
                (*emprestimos)[i].codigo_livro, 
                (*emprestimos)[i].cod_leitor, 
                (*emprestimos)[i].data_emp, 
                (*emprestimos)[i].data_devp,
                (*emprestimos)[i].status);
    }

    fclose(reescrita);  // Fecha o arquivo
    printf("Arquivo reescrito com sucesso!\n");
}

int diferenca(const char *data) {
    struct tm dataFornecida = {0};
    sscanf(data, "%d-%d-%d", &dataFornecida.tm_mday, &dataFornecida.tm_mon, &dataFornecida.tm_year);
    dataFornecida.tm_year -= 1900; // Ajustar ano
    dataFornecida.tm_mon -= 1;     // Ajustar mês

    time_t agora = time(NULL);
    time_t timeFornecida = mktime(&dataFornecida);

    return (difftime(agora, timeFornecida) / (60 * 60 * 24));
}


void consulta_acervo(Tlivro **plivros, int *num_linhasLivro){
    limpar();
    getchar(); // Limpa o buffer
    int op, tem_algo = 0;
    printf("| CONSULTAR ACERVO\n\n1 | Todos os livros\n2 | Livros disponíveis\n3 | Livros emprestados\n4 | Livros reservados\n\nDigite a operação: ");
    scanf("%d", &op);
    getchar(); // Limpa o buffer
    switch(op){
    case 1:
        printf("\nTodos os Livros:\n");
        for (int i = 0; i < *num_linhasLivro; i++)
            printf("%s - %s\n", (*plivros)[i].titulo, (*plivros)[i].autor);
        break;
    case 2:
        printf("\nLivros disponíveis:\n");
        for (int i = 0; i < *num_linhasLivro; i++) {
            if ((*plivros)[i].status == 1){
                tem_algo = 1;
                printf("%s - %s\n", (*plivros)[i].titulo, (*plivros)[i].autor);
            }
        }
        break;
    case 3:
        printf("\nLivros emprestados:\n");
        for (int i = 0; i < *num_linhasLivro; i++) {
            if ((*plivros)[i].status == 3){
                tem_algo = 1;
                printf("%s - %s\n", (*plivros)[i].titulo, (*plivros)[i].autor);
            }
        }
        break;
    case 4:
        printf("\nLivros reservados:\n");
        for (int i = 0; i < *num_linhasLivro; i++) {
            if ((*plivros)[i].status == 2){
                tem_algo = 1;
                printf("%s - %s\n", (*plivros)[i].titulo, (*plivros)[i].autor);
            }
        }
        break;
    case 5: 
        printf("\nConsulta cancelada!\n");
        break;
    default:
        printf("Opção inválida!\n");
        break;
    }
    return 0;
}