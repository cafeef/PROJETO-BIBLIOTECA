typedef struct emprestimos {
    int codigo, codigo_livro, cod_leitor;
    char data_emp[20], data_devp[20];
    int status; //0 - ativo | 1 - finalizado
} Temprestimos;