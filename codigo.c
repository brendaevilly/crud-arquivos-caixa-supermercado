#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

    typedef struct{
        char nome[100], categoria[100];
        int quantidade;
        float preco;
    }Produto;//Struct Produto.

    typedef struct{
        int dia,mes,ano;
    }DATA;//Struct data

    typedef struct{
        char nome[100],rg[12], cpf[15];
        DATA dia_do_nascimento;
    }CLIENTE;//Struct cliente

    typedef struct{
        char cpfcliente[15];
        char nproduto[100];
        int quanti;
        float valorvenda;
    }Vendas;//Struct vendas.

    void menu_inicial();

    void menu_produto();
    void cadastrar_produto(Produto *produto, int *contador_de_produtos);
    void consultar_produtos(int *contador_de_produtos);
    void editar_produto(int *contador_de_produtos);
    void excluir_produto(Produto *produto);
    void listar_todos_os_produtos();

    void menu_cliente();
    void cadastrar_clientes(CLIENTE *clientes);
    void listar_todos_clientes(CLIENTE *cliente);
    void consultar_cliente(CLIENTE *cliente);
    void excluir_cliente(CLIENTE *cliente);
    void editar_cliente();
    void editar_dados_dos_clientes(CLIENTE *cliente, int opcao,int *cliente_editado);
    int validar_cpf(const char *cpf);
    int verificar_dados_iguais(FILE *arquivo, const char *dado, int tipo);
    int validar_data(int dia, int mes, int ano);
    int validar_rg(const char *rg);

    void menu_vendas();
    void sanitizar_string(char *dest, const char *src);
    void realizar_vendas(Produto *produto, CLIENTE *cliente, Vendas *vendas);
    void valortotalcaixa(Vendas *venda);
    void historicovendas(Vendas *venda);
    void quantidadevendas(Vendas *venda);

    int main(){
        int opcao_inicial = -1, voltar_ou_continuar = -1;
        int contador_de_produtos; //Variavel passada por parâmetro para ponteiros em funções, auxiliando a manipulação do arquivo produtos.
        Produto produto;
        CLIENTE cliente;
        Vendas venda;

        do{
            if(opcao_inicial == -1){
                menu_inicial();
                scanf("%d", &opcao_inicial);
            }
            switch(opcao_inicial){
                case 1:
                    voltar_ou_continuar = -1;
                    menu_produto();
                    scanf("%d", &voltar_ou_continuar);
                    switch(voltar_ou_continuar){
                        case 1:
                            cadastrar_produto(&produto, &contador_de_produtos);
                        break;
                        case 2:
                            consultar_produtos(&contador_de_produtos);
                        break;
                        case 3:
                            editar_produto(&contador_de_produtos);
                        break;
                        case 4:
                            excluir_produto(&produto);
                        break;
                        case 5:
                            listar_todos_os_produtos();
                        break;
                        case 6:
                            menu_inicial();
                            scanf("%d", &opcao_inicial);
                        break;
                        default:
                            printf("Opcao invalida! Tente novamente.\n");
                            menu_produto();
                            scanf("%d", &voltar_ou_continuar);
                        break;
                    }
                break;
                case 2:
                    voltar_ou_continuar = -1;
                    menu_cliente();
                    scanf("%d", &voltar_ou_continuar);
                    switch(voltar_ou_continuar){
                        case 1:
                            cadastrar_clientes(&cliente);
                        break;
                        case 2: 
                            consultar_cliente(&cliente);
                        break; 
                        case 3:
                            editar_cliente();
                        break;
                        case 4:
                            excluir_cliente(&cliente);
                        break;
                        case 5:
                            listar_todos_clientes(&cliente);
                        break;
                        case 6:
                            menu_inicial();
                            scanf("%d", &opcao_inicial);
                        break;
                        default:
                            printf("Opcao invalida! Tente novamente.\n");
                            menu_cliente();
                            scanf("%d", &voltar_ou_continuar);
                        break;
                    }
                break;
                case 3:
                    voltar_ou_continuar = -1;
                    menu_vendas();
                    scanf("%d", &voltar_ou_continuar);
                    switch(voltar_ou_continuar){
                    case 1:
                    realizar_vendas(&produto, &cliente, &venda);
                    break;
                    case 2: 
                    valortotalcaixa(&venda);
                    break;
                    case 3:
                    historicovendas(&venda);
                    break;
                    case 4:
                    quantidadevendas(&venda);
                    break;
                    case 6:
                        menu_inicial();
                        scanf("%d", &opcao_inicial);
                    break;
                    default: 
                        printf("Opcao invalida! Tente novamente.\n");
                        menu_vendas();
                        scanf("%d", &voltar_ou_continuar);
                    break;
                    }
                break;    
                default:
                    printf("Voce deseja sair do programa?\n");
                    printf("Digite:\n-1 - Voltar ao menu inical.\n0 - Finalizar programa.\n");
                    scanf("%d", &opcao_inicial);
                break;
            }
        }while(opcao_inicial != 0);

        system("PAUSE");
        return 0;
    }

    void menu_inicial(){
        printf("--- MENU INICIAL ---\n");
        printf("1 - PRODUTO\n2 - CLIENTE\n3 - VENDA\n0 - FINALIZAR PROGRAMA\n");
        printf("Escolha uma opcao: ");
        return;
    }

    //FUNÇÕES DE PRODUTO.

    void menu_produto(){
        printf("--- MENU PRODUTO ---\n");
        printf("1 - CADASTRAR PRODUTO\n2 - CONSULTAR PRODUTO\n3 - EDITAR PRODUTO\n4 - EXCLUIR PRODUTO\n5 - LISTAR TODOS OS PRODUTOS\n6 - RETORNAR AO MENU INICIAL\n");
        printf("Escolha uma opcao: ");
        return;
    }

    void cadastrar_produto(Produto *produto, int *contador_de_produtos){
        FILE *arquivo;
        *contador_de_produtos = 0;
        int verificar_se_existe = 0;
        arquivo = fopen("produtos_em_estoque.txt", "r");//Abre o arquivo no modo read.
        if (arquivo == NULL){//Verifica se o arquivo foi aberto.
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        while(fgetc(arquivo) != EOF){//Conta as linhas do arquivo até chegar no fim.
            if (feof(arquivo)){
                break; 
            }
            (*contador_de_produtos)++;
        }
        rewind(arquivo);//Volta ao início do arquivo.
        Produto produtos_em_estoque[*contador_de_produtos];
        for(int i=1; i<=*contador_de_produtos; i++){//Passa os produtos do arquivo para o vetor.
            fscanf(arquivo, "%s %s %d %f\n", produtos_em_estoque[i].nome, produtos_em_estoque[i].categoria, &produtos_em_estoque[i].quantidade, &produtos_em_estoque[i].preco);
        }
        fclose(arquivo);//Fecha o arquivo.
        arquivo = fopen("produtos_em_estoque.txt", "a+");//Abre o arquivo no modo append/update.
        if (arquivo == NULL){//Verifica se o arquivo foi aberto.
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        printf("--- CADASTRO DE PRODUTO ---\n");
        printf("\nNome do produto: ");
        setbuf(stdin, NULL);
        scanf("%[^\n]", produto->nome);
        getchar();
        for(int i=1; i<=*contador_de_produtos; i++){//Passa os produtos do arquivo para o vetor.
            if(strcmp(produto->nome, produtos_em_estoque[i].nome) == 0){
              verificar_se_existe = 1;  
              break;
            } 
        }
        if(verificar_se_existe == 1){
            printf("\n[ERROR] Esse produto ja esta em estoque!\n");
            return;
        }    
        printf("\nCategoria do produto: ");
        setbuf(stdin, NULL);
        scanf("%[^\n]", produto->categoria);
        getchar();
        printf("\nQuantidade em estoque: ");
        scanf("%d", &produto->quantidade);
        printf("\nPreco unitario: ");
        scanf("%f", &produto->preco);
        fprintf(arquivo, "%s %s %d %.2f\n", produto->nome, produto->categoria, produto->quantidade, produto->preco);//Escreve as informações dos campos da variavel produto do tipo Produto no fim do arquivo.
        printf("\n--- CADASTRO REALIZADO COM SUCESSO ---\n");
        fclose(arquivo);//Fecha arquivo.
        return;
    }

    void consultar_produtos(int *contador_de_produtos){
        FILE *arquivo;
        *contador_de_produtos = 0;
        char produto_procurado[100];
        printf("\nNome do produto buscado: ");//Coleta do nome do produto para a busca no arquivo.
        setbuf(stdin, NULL);
        scanf("%[^\n]", produto_procurado);
        getchar();
        arquivo = fopen("produtos_em_estoque.txt", "r");//Abre o arquivo no modo read.
        if (arquivo == NULL){//Verifica se o arquivo foi aberto.
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        while(fgetc(arquivo) != EOF){//Conta as linhas do arquivo até chegar no fim.
            if (feof(arquivo)){
                break; 
            }
            (*contador_de_produtos)++;
        }
        rewind(arquivo);//Retorna ao início do arquivo.
        Produto produtos_em_estoque[*contador_de_produtos];//Vetor do tipo Produto para ler os produtos do arquivo.
        for(int i=1; i<=*contador_de_produtos; i++){//Passagem dos produtos do arquivo para o vetor.
            fscanf(arquivo, "%s %s %d %f\n", produtos_em_estoque[i].nome, produtos_em_estoque[i].categoria, &produtos_em_estoque[i].quantidade, &produtos_em_estoque[i].preco);
        }
        fclose(arquivo);//Fecha arquivo.
        for(int i=1; i<=*contador_de_produtos; i++){//Busca pelo nome usando a função strcmp da biblioteca string.h.
            if(strcmp(produto_procurado, produtos_em_estoque[i].nome) == 0){
                printf("\n--- PRODUTO ENCONTRADO ---\n");
                printf("--- INFORMACOES DO PRODUTO ---\n");
                printf("%s\n%s\n%d\n%.2f\n", produtos_em_estoque[i].nome, produtos_em_estoque[i].categoria, produtos_em_estoque[i].quantidade, produtos_em_estoque[i].preco);//Mostra o produto no terminal.
                return;
            }
        }
        printf("\n--- O PRODUTO NAO FOI ENCONTRADO NO ESTOQUE ---\n");
        return;
    }

    void editar_produto(int *contador_de_produtos){
        FILE *arquivo;
        *contador_de_produtos = 0; 
        int encontrado_ou_nao = -1;
        char produto_procurado[100];
        int opcoes_de_edicao = -1;
        char buffer[256];
        printf("\nNome do produto buscado: ");//Coleta do nome do produto para a busca no arquivo.
        setbuf(stdin, NULL);
        scanf("%[^\n]", produto_procurado);
        getchar();
        printf("\nVoce quer editar todas as informacaoes do produto? (1 - SIM | 0 - NAO): ");//Opcoes de edicao do produto.
        scanf("%d", &opcoes_de_edicao);
        arquivo = fopen("produtos_em_estoque.txt", "r");//Abre o arquivo no modo read.
        if (arquivo == NULL){//Verifica se o arquivo foi aberto.
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        while(fgets(buffer, sizeof(buffer), arquivo) != NULL){//Conta as linhas do arquivo.
            (*contador_de_produtos)++;
        }
        rewind(arquivo);//Retorna ao início do arquivo.
        Produto produtos_em_estoque[*contador_de_produtos];//Vetor do tipo Produto para ler os produtos do arquivo.
        for(int i=1; i<=*contador_de_produtos; i++){//Passagem dos produtos do arquivo para o vetor.
            fscanf(arquivo, "%s %s %d %f\n", produtos_em_estoque[i].nome, produtos_em_estoque[i].categoria, &produtos_em_estoque[i].quantidade, &produtos_em_estoque[i].preco);
        }
        fclose(arquivo);//Fecha arquivo.
        for(int i=1; i<=*contador_de_produtos; i++){//Encontra o produto se ele existir no arquivo.
            if (strcmp(produto_procurado, produtos_em_estoque[i].nome) == 0){
                encontrado_ou_nao = i;
                printf("\n--- PRODUTO ENCONTRADO ---\n");
                break;
            }
        }
        if (encontrado_ou_nao == -1){//Verifica se foi encontrado.
            printf("\n--- Produto nao encontrado ---\n");
            return;
        }
        char nome_editado[100];
        int verificar_se_existe = 0;
        if (opcoes_de_edicao == 1){//Editar o produto conforme a preferência do usuário.
            printf("--- EDITAR PRODUTO ---\n");
            printf("\nNome (Atual: %s): ", produtos_em_estoque[encontrado_ou_nao].nome);
            setbuf(stdin, NULL);
            scanf("%[^\n]", nome_editado);
            getchar();
            for(int i=1; i<=*contador_de_produtos; i++){
                if(i != encontrado_ou_nao && strcmp(nome_editado, produtos_em_estoque[i].nome) == 0){
                    verificar_se_existe = 1;
                    break;
                }
            }
            if(verificar_se_existe == 1){
            printf("\n[ERROR] Esse produto ja esta em estoque! O nome nao pode ser alterado!\n");
            return; 
            }else{
                strcpy(produtos_em_estoque[encontrado_ou_nao].nome, nome_editado);
            }
            printf("\nCategoria (Atual: %s): ", produtos_em_estoque[encontrado_ou_nao].categoria);
            setbuf(stdin, NULL);
            scanf("%[^\n]", produtos_em_estoque[encontrado_ou_nao].categoria);
            getchar();
            printf("\nQuantidade (Atual: %d): ", produtos_em_estoque[encontrado_ou_nao].quantidade);
            scanf("%d", &produtos_em_estoque[encontrado_ou_nao].quantidade);
            printf("\nPreco (Atual: %.2f): ", produtos_em_estoque[encontrado_ou_nao].preco);
            scanf("%f", &produtos_em_estoque[encontrado_ou_nao].preco);
        }else if(opcoes_de_edicao == 0){
            int tipos_de_edicao;
            printf("\n1 - EDITAR NOME E CATEGORIA\n2 - EDITAR PRECO\n3 - EDITAR QUANTIDADE\nEscolha uma opcao: ");
            scanf("%d", &tipos_de_edicao);
            if(tipos_de_edicao == 1){
                printf("--- EDITAR PRODUTO ---\n");
                printf("\nNome (Atual: %s): ", produtos_em_estoque[encontrado_ou_nao].nome);
                setbuf(stdin, NULL);
                scanf("%[^\n]", nome_editado);
                getchar();
                for(int i=1; i<=*contador_de_produtos; i++){
                    if(i != encontrado_ou_nao && strcmp(nome_editado, produtos_em_estoque[i].nome) == 0){
                        verificar_se_existe = 1;
                        break;
                    }
                }
                if(verificar_se_existe == 1){
                    printf("\n[ERROR] Esse produto ja esta em estoque! O nome nao pode ser alterado!\n");
                    return; 
                }else{
                    strcpy(produtos_em_estoque[encontrado_ou_nao].nome, nome_editado);
                }
                printf("\nCategoria (Atual: %s): ", produtos_em_estoque[encontrado_ou_nao].categoria);
                setbuf(stdin, NULL);
                scanf("%[^\n]", produtos_em_estoque[encontrado_ou_nao].categoria);
                getchar();
            }else if(tipos_de_edicao == 2){
                printf("\nPreco (Atual: %.2f): ", produtos_em_estoque[encontrado_ou_nao].preco);
                scanf("%f", &produtos_em_estoque[encontrado_ou_nao].preco);
            }else if(tipos_de_edicao == 3){
                printf("\nQuantidade (Atual: %d): ", produtos_em_estoque[encontrado_ou_nao].quantidade);
                scanf("%d", &produtos_em_estoque[encontrado_ou_nao].quantidade);
            }else{
                printf("\n[ERROR] Escolha uma opcao valida!\n1 - EDITAR NOME E CATEGORIA\n2 - EDITAR PRECO\n3 - EDITAR QUANTIDADE\nEscolha uma opcao: ");
                scanf("%d", &tipos_de_edicao);
            }
        }else{
            printf("\n[ERROR] Escolha uma opcao valida!\nVoce quer editar todas as informacaoes do produto? (1 - SIM | 0 - NAO)");
            scanf("%d", &opcoes_de_edicao);
        }
        arquivo = fopen("produtos_em_estoque.txt", "w");//Abre arquivo no modo write.
        if (arquivo == NULL){//Verifica se o arquivo foi aberto.
            printf("[ERROR] Nao foi possivel abrir o arquivo para escrita.\n");
            return;
        }
        for(int i=1; i<=*contador_de_produtos; i++){//Escreve o produto editado no arquivo;
            fprintf(arquivo, "%s %s %d %.2f\n", produtos_em_estoque[i].nome, produtos_em_estoque[i].categoria, produtos_em_estoque[i].quantidade, produtos_em_estoque[i].preco);
        }
        fclose(arquivo);//Fecha arquivo.
        printf ("--- PRODUTO ATUALIZADO ---\n");
    }

    void excluir_produto(Produto *produto){
        FILE *arquivo, *auxiliar;
        char produto_para_excluir[100];
        int auxi = 0;
        arquivo = fopen("produtos_em_estoque.txt", "r");//Abre o arquivo no modo read.
        if (arquivo == NULL){//Verifica se o arquivo foi aberto.
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        auxiliar = fopen("auxiliar.txt", "w");//Abre arquivo auxiliar no modo write.
        if (auxiliar == NULL){//Verifica se o arquivo foi aberto.
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        printf("Digite o nome do produto para excluir: \n");//Encontrar produto pelo nome.
        setbuf(stdin, NULL);
        scanf("%[^\n]", produto_para_excluir);
        while(fscanf(arquivo, "%s %s %d %f", produto->nome, produto->categoria, &produto->quantidade, &produto->preco) == 4){//Escreve no arquivo auxiliar apenas os produtos com nome diferente do inserido pelo usuário.
            if(strcmp(produto->nome, produto_para_excluir) == 0){
                auxi = 1;
            }else{
                fprintf(auxiliar, "%s %s %d %.2f\n", produto->nome, produto->categoria, produto->quantidade, produto->preco);
            }
        }
        fclose(arquivo);//Fecha arquivo.
        fclose(auxiliar);//Fecha arquivo auxiliar.
        if(auxi == 1){//Se o produto for encontrado.
            remove("produtos_em_estoque.txt");//Remove o arquivo atual.
            rename("auxiliar.txt", "produtos_em_estoque.txt");//Renomeia o arquivo auxiliar com o nome do "atual".
            printf("--- PRODUTO EXCLUIDO COM SUCESSO ---\n");
        }else{//Se o produto arquivo não for encontrado.
            remove("auxiliar.txt");//Remove o arquivo auxiliar.
            printf("--- O PRODUTO NAO FOI ENCONTRADO NO ESTOQUE ---\n");
        }
    }

    void listar_todos_os_produtos(){
        FILE *arquivo;
        char linha[256];
        arquivo = fopen("produtos_em_estoque.txt", "r");//Abre o arquivo no modo read.
        if (arquivo == NULL){//Verifica se o arquivo foi aberto.
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        fseek(arquivo, 0, SEEK_END);//Vai para o fim do arquivo.
        long tamanho = ftell(arquivo);//Retorna a posição do cursor de leitura dentro do arquivo, se for 0, ele está vazio.
        if(tamanho == 0){
            printf("\n--- O ESTOQUE ESTA VAZIO ---\n");
            fclose(arquivo);
            return;
        }
        rewind(arquivo);//Retorna ao início do arquivo.
        printf ("\n--- PRODUTOS EM ESTOQUE ---\n");
        while(fgets(linha, sizeof(linha), arquivo) != NULL){//fgets para ler o conteúdo de cada linha do arquivo, atribui a variável linha e mostra no terminal.
            printf("%s\n", linha);
        }
        fclose(arquivo);
    }

    //FUNÇÕES DE CLIENTE.

    void menu_cliente(){
        printf ("--- MENU CLIENTE---\n");
        printf ("1 - CADASTRAR CLIENTE\n2 - CONSULTAR CLIENTE\n3 - EDITAR CLIENTE\n4 - EXCLUIR CLIENTE\n5 - LISTAR TODOS OS CLIENTES\n6 - RETORNAR AO MENU INICIAL\n");
        printf ("Escolha uma opcao: ");
        return;
    }

    int validar_cpf(const char *cpf){
        if(strlen(cpf) != 14){
            return 0;//CPF inválido se não tem 14 caracteres contando com . e -
        }
        return 1;//CPF válido.
    }

    int validar_rg(const char *rg){
        if(strlen(rg) != 11){
            return 0;//CPF inválido se não tem 14 caracteres contando com -
        }
        return 1;//CPF válido.
    }

    int verificar_dados_iguais(FILE *arquivo, const char *dado, int tipo){ //função que verifica os dados repetidos, recebe o arquivo, o dado(cpf,rg) e se o tipo, se é para verificar rg ou cpf.
        CLIENTE cliente_verificar;//Variável do tipo cliente que será usado para verificar os registros do arquivo.
        rewind(arquivo);
        while (fscanf(arquivo, "%99[^,], %14[^,], %11[^,], %d, %d, %d\n",//Lê os dados do arquivo.
            cliente_verificar.nome, cliente_verificar.cpf, cliente_verificar.rg,
            &cliente_verificar.dia_do_nascimento.dia, &cliente_verificar.dia_do_nascimento.mes,
            &cliente_verificar.dia_do_nascimento.ano) == 6){
                if ((tipo == 1 && strcmp(cliente_verificar.cpf, dado) == 0) ||//Tipo 1 verifica se o cpf.
                    (tipo == 2 && strcmp(cliente_verificar.rg, dado) == 0)){//Tipo 2 verifica se o rg.
                    return 1;//Dado duplicado.
                }
        }
        return 0;//Dado não encontrado.
    }

    int validar_data(int dia, int mes, int ano){//Função para validar datas.
        if(mes < 1 || mes > 12 || ano > 2025){//Mes tem que ser dentre 1 e 12.
            return 0;
        }
        if(mes==2){
            return (dia >= 1 && dia <= 28);//Mes de fevereiro só retorna de um a 28 dias.
        }else if(mes == 4 || mes == 6 || mes == 9 || mes == 11){//Esses meses retornam de 1 a 30 dias.
            return (dia >= 1 && dia <= 30);
        }else{
            return (dia >= 1 && dia <= 31);//Esses retornam de 1 a 31 dias.
        }
    }

    void cadastrar_clientes(CLIENTE *clientes){//Ponteiro de struct para facilitar o acesso aos objetos.
        FILE *arquivo = fopen("cliente.txt", "a+");//Abre o aquivo para inserir.
        FILE *arquivo_verificar=fopen("cliente.txt","r");//Abre o arquivo para ler e verificar.
        char cpf_cliente[15];
        char rg_cliente[11];
        int duplicado;
        int dia, mes,ano;
        if(arquivo == NULL|| arquivo_verificar==NULL) {//Condição para se caso o arquivo não conseguir abrir.
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        printf("\nNome do cliente: ");//Entrada de dados do cliente.
        setbuf(stdin, NULL);
        scanf(" %[^\n]", clientes->nome);//Devido ao pontteiro do tipo struct, podemos acessar a struct por essa seta.
        getchar();
        printf("\nCPF do cliente: (formato: 000.000.000-00) ");
        setbuf(stdin, NULL);
        scanf(" %[^\n]", cpf_cliente);//Variável char de cpf lida.
        getchar();
        while(validar_cpf(cpf_cliente) == 0) {//Função valida se não há erro na quantidade de caracteres dentro da variavel.                         
            printf("[ERROR] CPF invalido. Digite novamente: ");
            scanf("%[^\n]", cpf_cliente);
            getchar();
        }
        duplicado = verificar_dados_iguais(arquivo, cpf_cliente, 1);//Função que verifica se há outro cpf igual no arquivo.
        if(duplicado){
            printf("[ERROR] O CPF informado ja está cadastrado.\n");
            fclose(arquivo);
            return;
        }
        strcpy(clientes->cpf, cpf_cliente);
        arquivo_verificar=fopen("cliente.txt","r");//Arquivo aberto novamente para leitura.
        printf("\nRG do cliente: formato(000000000-0) ");
        setbuf(stdin, NULL);
        scanf(" %[^\n]", rg_cliente);
        getchar();
        while(validar_rg(rg_cliente) == 0){//Função valida se não há erro na quantidade de caracteres dentro da variavel.
            printf("[ERROR] CPF invalido. Digite novamente: ");
            scanf("%[^\n]", rg_cliente);
            getchar();
        }
        duplicado = verificar_dados_iguais(arquivo, rg_cliente, 2);//Verifica se há outro rg igual.
        if(duplicado){
            printf("[ERROR] O RG informado ja esta cadastrado.\n");
            fclose(arquivo);
            return;
        }
        strcpy(clientes->rg, rg_cliente);
        printf("\nData de nascimento: "); 
        scanf("%d %d %d", &dia,&mes,&ano);
        while(!validar_data(dia, mes, ano)){//Verifica se a data é valida.
            printf("[ERROR] Data invalida. Digite novamente (dia mes ano): ");
            scanf("%d %d %d", &dia, &mes, &ano);
        }
        clientes->dia_do_nascimento.dia = dia;
        clientes->dia_do_nascimento.mes = mes;
        clientes->dia_do_nascimento.ano = ano;
        fprintf(arquivo, "%s, %s, %s, %d, %d, %d\n", clientes->nome, clientes->cpf, clientes->rg, clientes->dia_do_nascimento.dia, clientes->dia_do_nascimento.mes, clientes->dia_do_nascimento.ano);
        printf("\n--- CADASTRO REALIZADO COM SUCESSO ---\n");//escreve as informações no arquivo.
        fclose(arquivo);//Fecha o arquivo de registro.
        return;
    }

    void listar_todos_clientes(CLIENTE *cliente) {
        FILE *arquivo ;
        arquivo= fopen("cliente.txt", "r");//Abre o arquivo em modo leitura.
        char linha[256];
        int arquivo_vazio = 1;
        if (arquivo == NULL) {
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        printf("\n--- LISTA DE CLIENTES ---\n");
        while(fgets(linha, sizeof(linha), arquivo) != NULL){//Lê cada linha do arquivo para e imprime.
            if(sscanf(linha, "%99[^,], %14[^,], %11[^,], %d, %d, %d",cliente->nome, cliente->cpf, cliente->rg,&cliente->dia_do_nascimento.dia, &cliente->dia_do_nascimento.mes, &cliente->dia_do_nascimento.ano) == 6){
                printf("Nome: %s\nCPF: %s\nRG: %s\nData de Nascimento: %02d/%02d/%04d\n",cliente->nome, cliente->cpf, cliente->rg, cliente->dia_do_nascimento.dia, cliente->dia_do_nascimento.mes, cliente->dia_do_nascimento.ano);
                printf("-------------------------\n");
                arquivo_vazio = 0;
            }
        }
        if(arquivo_vazio == 1){
            printf("\n--- LISTA VAZIA ---\n");
        }
        fclose(arquivo);
        return;
    }

    void consultar_cliente(CLIENTE *cliente) {//Função que procura um cliente.
        FILE *arquivo;
        arquivo = fopen("cliente.txt", "r");//Abre o arquivo em modo leitura.
        char linha[256];
        char cpf_procurado[15];
        int encontrado = 0;
        if (arquivo == NULL) {
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        printf("CPF do cliente procurado: ");//Captura o cpf do cliente procurado.
        scanf(" %[^\n]", cpf_procurado);
        while(fgets(linha, sizeof(linha), arquivo) != NULL) {//Lê cada linha do arquivo, se cada linha obtiver 6 dados e se o cpf digitado for o mesmo da linha do arquivo, então o cliente foi encontrado.
            if(sscanf(linha, "%99[^,], %14[^,], %11[^,], %d, %d, %d",cliente->nome, cliente->cpf, cliente->rg, &cliente->dia_do_nascimento.dia, &cliente->dia_do_nascimento.mes,&cliente->dia_do_nascimento.ano) == 6){
                if(strcmp(cliente->cpf, cpf_procurado) == 0){
                    printf("--- CLIENTE ENCONTRADO ---\n");
                    printf("Nome: %s\nCPF: %s\nRG: %s\nData de nascimento: %02d/%02d/%04d\n",cliente->nome, cliente->cpf, cliente->rg, cliente->dia_do_nascimento.dia, cliente->dia_do_nascimento.mes,cliente->dia_do_nascimento.ano);
                    encontrado = 1;
                    break;
                }
            }
        }
        if(!encontrado){
            printf("--- CLIENTE NÃO ENCONTRADO ---\n");
        }
        fclose(arquivo);
    }

    void editar_dados_dos_clientes(CLIENTE *cliente, int opcao,int *cliente_editado){// Função auxiliar para editar dados dos clientes. Recebe como parametro um ponteiro do tipo struct, um inteiro que le as opções do switch case e outro ponteiro que fiscaliza a edição dos clientes
        FILE *arquivo = fopen("cliente.txt", "r"); 
        if(arquivo==NULL){
            printf("[ERROR] Nao foi possivel abrir o arquivo.\n");
            return;
        }
        char dado_temporario[100];//Variável que será usada para para ler e checar os dados(cpf, rg).
        int duplicado = 0;//Variável auxiliar para verificar se há ou não dados duplicados.
        int dia, mes, ano;//Variáveis que irão ser temporarias para verificação das datas.
        switch(opcao){
            case 1://Editar todos os campos.
            printf("Digite o novo nome (atual: %s): ", cliente->nome);//Edita o nome.
            setbuf(stdin, NULL);
            scanf("%[^\n]", cliente->nome);
            getchar();
            printf("Digite o novo CPF (atual: %s): ", cliente->cpf);//Edita cpf.
            setbuf(stdin, NULL);
            scanf(" %[^\n]", dado_temporario);//A variável dado_temporário guarda o cpf que será analisado.
            getchar();

            while(validar_cpf(dado_temporario) == 0){                               
                printf("[ERROR] CPF invalido. Digite novamente: ");
                scanf("%[^\n]", dado_temporario);
                getchar();
            }
            duplicado = verificar_dados_iguais(arquivo, dado_temporario, 1);//A função verifica se o dado temporário já existe no arquivo.
            if(duplicado){//Se duplicado=0, então é imitida uma mensagem e o arquivo é fechado, retornando para o menu de clientes.
                printf("[ERROR] O CPF informado ja esta cadastrado.\n");
                fclose(arquivo);
                return;
            }
            strcpy(cliente->cpf, dado_temporario);//Se não há registros de cpf igual, o dado temporario passa a valer cliente.cpf.
            printf("Digite o novo RG (atual: %s): ", cliente->rg);//Cadastro de no rg.
            setbuf(stdin, NULL);
            scanf("%[^\n]", dado_temporario);
            while(validar_rg(dado_temporario) == 0){//Função valida se não há erro na quantidade de caracteres dentro da variavel.                         
                printf("[ERROR] CPF invalido. Digite novamente: ");
                scanf("%[^\n]", dado_temporario);
                getchar();
            }
            duplicado = verificar_dados_iguais(arquivo, dado_temporario, 2);//Função verifica se existe um rg com o mesmo número no arquivo.
            if(duplicado){//Se duplicado=0, então é imitida uma mensagem e o arquivo é fechado, retornando para o menu de clientes.
                printf("[ERROR] O RG informado ja esta cadastrado.\n");
                fclose(arquivo);
                return;
            }
            strcpy(cliente->rg, dado_temporario);//Se não há registros de rg iguais, o dado temporarrio passa a valer cliente.cpf.
            printf("Digite a nova data de nascimento (dia mes ano): ");
            scanf("%d %d %d", &dia, &mes, &ano);
            while(!validar_data(dia, mes, ano)){//A função verifica se dia, mes e ano são váilidas. Se qualuqer uma não for, é exibida uma mensagem de erro e tente novamente.
                printf("[ERROR] Data invalida. Digite novamente (dia mes ano): ");
                scanf("%d %d %d", &dia, &mes, &ano);
            }
            cliente->dia_do_nascimento.dia = dia;//As variáveis passam a valer a indicação da struct.
            cliente->dia_do_nascimento.mes = mes;
            cliente->dia_do_nascimento.ano = ano;
            *cliente_editado=1;// cada vez que o cliente é editado, o ponteiro recebe um
            break; 
            case 2://Edita apenas o nome.
            printf("Digite o novo nome (atual: %s): ", cliente->nome);
            setbuf(stdin, NULL);
            scanf("%[^\n]", cliente->nome);
              *cliente_editado=1;
            break;
            case 3://Edita apenas o CPF.
            printf("Digite o novo CPF (atual: %s): ", cliente->cpf);
            setbuf(stdin, NULL);
            scanf("%[^\n]", dado_temporario);
            while(validar_cpf(dado_temporario) == 0){//Função valida se não há erro na quantidade de caracteres dentro da variavel.
                printf("[ERROR] CPF invalido. Digite novamente: ");
                scanf("%[^\n]", dado_temporario);
                getchar();
            }
            duplicado = verificar_dados_iguais(arquivo, dado_temporario, 1);//Verificação de dados iguais para cpf.
            if(duplicado){
                printf("[ERROR] O CPF informado ja esta cadastrado.\n");
                fclose(arquivo);
                return;
            }
            strcpy(cliente->cpf, dado_temporario);
              *cliente_editado=1;
            break; 
            case 4://Edita apenas o rg.
            printf("Digite o novo RG (atual: %s): ", cliente->rg);
            setbuf(stdin, NULL);
            scanf("%[^\n]", dado_temporario);
            while(validar_rg(dado_temporario) == 0){//Função valida se não há erro na quantidade de caracteres dentro da variavel.
                printf("[ERROR] RG invalido. Digite novamente: ");
                scanf("%[^\n]", dado_temporario);
                getchar();
            }
            duplicado = verificar_dados_iguais(arquivo, dado_temporario, 2);
            if(duplicado){
                printf("[ERROR] O RG informado ja esta cadastrado.\n");
                fclose(arquivo);
                return;
            }
            strcpy(cliente->rg, dado_temporario);
              *cliente_editado=1;
            break; 
            case 5://Edita apenas a data de nascimento.
            printf("Digite a nova data de nascimento (dia mes ano): ");
            scanf("%d %d %d", &dia, &mes, &ano);
            while (!validar_data(dia, mes, ano)) {
                printf("[ERROR] Data invalida. Digite novamente (dia mes ano): ");
                scanf("%d %d %d", &dia, &mes, &ano);
            }
            cliente->dia_do_nascimento.dia = dia;
            cliente->dia_do_nascimento.mes = mes;
            cliente->dia_do_nascimento.ano = ano;
              *cliente_editado=1;
            break; 
            default:
            printf("[ERROR] Opçao invalida.\n");
            break; 
        }
        fclose(arquivo);
    }

    void editar_cliente() {//Função que edita informações do cliente.
        FILE *arquivo = fopen("cliente.txt", "r");//Abre o arquivo principal para leitura.
        FILE *arquivo_auxiliar = fopen("cliente_auxiliar.txt", "w");//Abre um arquivo auxiliar para escrever.
        if (arquivo == NULL || arquivo_auxiliar == NULL) {//Verifica a entrada.
            printf("[ERROR] Nao foi possivel abrir os arquivos.\n");
            return;
        }
        char cpf_procurado[15];//Variável que captura o cpf do cliente.
        char linha[256];//Representa cada linha do arquivo.
        CLIENTE cliente;//Variável cliente da struct.
        int encontrado = 0;//Variável auxiliar.
        int cliente_editado=0; 
        printf("Digite o CPF do cliente que deseja editar: ");
        setbuf(stdin, NULL);
        scanf("%[^\n]", cpf_procurado);
        while(fgets(linha, sizeof(linha), arquivo) != NULL){//Lê cada linha do arquivo.
            if(sscanf(linha, "%99[^,], %14[^,], %11[^,], %d, %d, %d",//Se cada linha lida obtiver 6 registros de informações.
                    cliente.nome, cliente.cpf, cliente.rg,
                    &cliente.dia_do_nascimento.dia,
                    &cliente.dia_do_nascimento.mes,
                    &cliente.dia_do_nascimento.ano) == 6){
                if(strcmp(cliente.cpf, cpf_procurado) == 0){//Se o cpf capturado for o mesmo que o cpf no arquivo, o cliente está encontrado.
                    encontrado = 1;
                    printf("--- CLIENTE ENCONTRADO ---\n");
                    int opcao_menu_editar;
                    printf("1- EDITAR TUDO\n");
                    printf("2- EDITAR NOME\n");
                    printf("3- EDITAR CPF\n");
                    printf("4- EDITAR RG\n");
                    printf("5- EDITAR DATA DE NASCIMENTO\n");
                    printf("Escolha uma opçao: ");
                    scanf("%d", &opcao_menu_editar);
                    //Chama a função para editar os dados do cliente.
                    editar_dados_dos_clientes(&cliente, opcao_menu_editar,&cliente_editado);//Função que permite editar informações, recebe como parametro a variável cliente e a opção digitada no menu.
                    if (cliente_editado) { // Se a variavel de fiscalização se manter 0, o arquivo auxiliar printa no arquivo a linha alterada
                    fprintf(arquivo_auxiliar, "%s, %s, %s, %d, %d, %d\n",
                            cliente.nome, cliente.cpf, cliente.rg,
                            cliente.dia_do_nascimento.dia,
                            cliente.dia_do_nascimento.mes,
                            cliente.dia_do_nascimento.ano);
                }else{ // Caso não tenha havido alteração, apenas copia a linha original
                    fprintf(arquivo_auxiliar, "%s, %s, %s, %d, %d, %d\n",
                    cliente.nome, cliente.cpf, cliente.rg,
                    cliente.dia_do_nascimento.dia,
                    cliente.dia_do_nascimento.mes,
                    cliente.dia_do_nascimento.ano);

                }
                //Atualiza o arquivo auxiliar com o cliente editado ou a linha original.
            }else{
                fprintf(arquivo_auxiliar, "%s", linha); //Copia linhas inválidas sem alteração.
            }
        }else{
            fprintf(arquivo_auxiliar, "%s", linha);
        }
    }
        fclose(arquivo);
        fclose(arquivo_auxiliar);
        if (encontrado && cliente_editado) {
        remove("cliente.txt");
        rename("cliente_auxiliar.txt", "cliente.txt");
        printf("--- CLIENTE EDITADO COM SUCESSO ---\n");
    } else if (encontrado && !cliente_editado) {
        // Caso o cliente tenha sido encontrado, mas não houve edição
        printf("--- NENHUMA ALTERACAO REALIZADA ---\n");
        remove("cliente_auxiliar.txt");
    } else {
        remove("cliente_auxiliar.txt");
        printf("--- CLIENTE NAO ENCONTRADO ---\n");
    }
}

    void excluir_cliente(CLIENTE *cliente){
        FILE *arquivo = fopen("cliente.txt", "r");//Abre arquivo para leitura.
        FILE *arquivo_auxiliar = fopen("cliente_auxiliar.txt", "w");//Cria um arquivo auxiliar.
        char cpf_procurado[15];
        int encontrado = 0;
        if (arquivo == NULL ) {//Verifica se é ou não nulo.
            printf("[ERROR] Nao foi possivel abrir o arquivo.\n");
            return;
        }
        if (arquivo_auxiliar == NULL ) {//Verifica se é ou não nulo.
            printf("[ERROR] Nao foi possivel abrir o arquivo.\n");
            return;
        }
        printf("CPF do cliente que deseja excluir: ");
        setbuf(stdin, NULL);
        scanf(" %[^\n]", cpf_procurado);
        getchar();
        while(fscanf(arquivo, "%99[^,], %14[^,], %11[^,], %d, %d, %d\n",cliente->nome, cliente->cpf, cliente->rg,&cliente->dia_do_nascimento.dia,&cliente->dia_do_nascimento.mes,&cliente->dia_do_nascimento.ano) == 6){
            if(strcmp(cliente->cpf, cpf_procurado) == 0){
                encontrado = 1;//Cliente encontrado, será excluído.
            }else{
                //Escreve no arquivo auxiliar os clientes não excluídos.
                fprintf(arquivo_auxiliar, "%s, %s, %s, %d, %d, %d\n",
                    cliente->nome, cliente->cpf, cliente->rg,
                    cliente->dia_do_nascimento.dia, 
                    cliente->dia_do_nascimento.mes, 
                    cliente->dia_do_nascimento.ano);
            }
        }
        fclose(arquivo);
        fclose(arquivo_auxiliar);
        if(encontrado){
            remove("cliente.txt");//Remove o arquivo original.
            rename("cliente_auxiliar.txt", "cliente.txt");//Renomeia o auxiliar.
            printf("--- CLIENTE EXCLUIDO COM SUCESSO ---\n");
        }else{
            remove("cliente_auxiliar.txt");//Remove o auxiliar se nada foi excluído.
            printf("--- CLIENTE NAO ENCONTRADO ---\n");
        }
    }

    //FUNÇÕES DE VENDAS.

    void menu_vendas(){
        printf("--- MENU VENDAS ---\n");
        printf("1 - REALIZAR VENDAS\n2 - VALOR TOTAL DO CAIXA\n3 - HISTORICO DE VENDAS\n4 - QUANTIDADE DE VENDAS\n6 - RETORNAR AO MENU INICIAL\n");
        printf("Escolha uma opcao: ");
        return;   
    }

    void sanitizar_string(char *dest, const char *src){
        int j = 0;
        for (int i = 0; src[i] != '\0'; i++) {
            if (isdigit(src[i])) {//Verifica se o caractere é numérico.
                dest[j++] = src[i];
            }
        }
        dest[j] = '\0';//Adiciona o terminador de string.
    }

    void realizar_vendas(Produto *produto, CLIENTE *cliente, Vendas *venda){
        FILE *arquivo_vendas, *arquivo_produtos, *arquivo_cliente, *produto_temp;
        int quantidadeprodutos = 0, clientecadastrados = 0, produtonoarquivo = 0;
        char verificarproduto[100], verificarcpf[15];
        char cpf_sanitizado[15];
        char cpf_arquivo_sanitizado[15];
        //Abrir arquivos.
        arquivo_cliente = fopen("cliente.txt", "r");
        if (!arquivo_cliente) {
            perror("Erro ao abrir o arquivo de clientes");
            return;
        }
        arquivo_produtos = fopen("produtos_em_estoque.txt", "r");
        if (!arquivo_produtos) {
            perror("Erro ao abrir o arquivo de produtos");
            fclose(arquivo_cliente);
            return;
        }
        arquivo_vendas = fopen("vendas_realizadas.txt", "a+");
        if (!arquivo_vendas) {
            perror("Erro ao abrir o arquivo de vendas");
            fclose(arquivo_cliente);
            fclose(arquivo_produtos);
            return;
        }
        produto_temp = fopen("produtos_temp.txt", "w");
        if (!produto_temp) {
            perror("Erro ao abrir o arquivo temporario");
            fclose(arquivo_cliente);
            fclose(arquivo_produtos);
            fclose(arquivo_vendas);
            return;
        }
        //Verificar CPF do cliente.
        printf("Digite o CPF do cliente: ");
        scanf("%s", verificarcpf);
        sanitizar_string(cpf_sanitizado, verificarcpf);
        while (fscanf(arquivo_cliente, "%[^,], %[^,], %[^,], %d, %d, %d\n",
                  cliente->nome, cliente->cpf, cliente->rg,
                  &cliente->dia_do_nascimento.dia,
                  &cliente->dia_do_nascimento.mes,
                  &cliente->dia_do_nascimento.ano) == 6) {
        //Sanitizar o CPF do arquivo.
            sanitizar_string(cpf_arquivo_sanitizado, cliente->cpf);
            //Comparar CPFs sanitizados.
            if (strcmp(cpf_sanitizado, cpf_arquivo_sanitizado) == 0){
                clientecadastrados = 1;
                break;
            }
        }
        fclose(arquivo_cliente);
        if (!clientecadastrados) {
            printf("[ERRO] Cliente com CPF '%s' nao encontrado.\n", verificarcpf);
            return;
        }
        printf("[SUCESSO] Cliente '%s' encontrado.\n", cliente->nome);
        //Verificar nome do produto.
        printf("Digite o nome do produto: ");
        scanf("%s", verificarproduto);
        while (fscanf(arquivo_produtos, "%s %s %d %f",
                    produto->nome, produto->categoria,
                    &produto->quantidade, &produto->preco) == 4) {
            if (strcmp(verificarproduto, produto->nome) == 0) {
                produtonoarquivo = 1;
                printf("Digite a quantidade desejada: ");
                scanf("%d", &quantidadeprodutos);
                if (quantidadeprodutos <= 0 || quantidadeprodutos > produto->quantidade) {
                    printf("[ERRO] Quantidade invalida ou estoque insuficiente.\n");
                    fclose(arquivo_produtos);
                    fclose(arquivo_vendas);
                    fclose(produto_temp);
                    remove("produtos_temp.txt");
                    return;
                }
                //Atualizar estoque e preparar venda.
                produto->quantidade -= quantidadeprodutos;
                venda->quanti = quantidadeprodutos;
                venda->valorvenda = quantidadeprodutos * produto->preco;
                strcpy(venda->nproduto, produto->nome);
                strcpy(venda->cpfcliente, verificarcpf);
            }
            fprintf(produto_temp, "%s %s %d %.2f\n",
                    produto->nome, produto->categoria,
                    produto->quantidade, produto->preco);
        }
        fclose(arquivo_produtos);
        fclose(produto_temp);
        if (!produtonoarquivo) {
            printf("[ERRO] Produto nao encontrado.\n");
            remove("produtos_temp.txt");
            fclose(arquivo_vendas);
            return;
        }
        //Atualizar arquivos.
        if (remove("produtos_em_estoque.txt") != 0 || rename("produtos_temp.txt", "produtos_em_estoque.txt") != 0) {
            perror("[ERRO] Nao foi possivel atualizar o estoque");
            fclose(arquivo_vendas);
            return;
        }
        fprintf(arquivo_vendas, "%s %s %d %.2f\n",
                venda->cpfcliente, venda->nproduto,
                venda->quanti, venda->valorvenda);
        fclose(arquivo_vendas);
        printf("Venda realizada com sucesso! Valor total: %.2f\n", venda->valorvenda);
    }
     
    void valortotalcaixa(Vendas *venda){
        FILE*arquivo_venda;
        float totalcaixa = 0;
        arquivo_venda = fopen("vendas_realizadas.txt", "r");
        if(arquivo_venda == NULL){
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        while (fscanf(arquivo_venda, "%s %s %d %f", venda->cpfcliente, venda->nproduto, &venda->quanti, &venda->valorvenda)!= EOF){
            totalcaixa += venda->valorvenda;
        }
        fclose(arquivo_venda);
        printf("valor total do caixa: %.2f\n", totalcaixa);
    }

    void historicovendas(Vendas *venda){
        FILE*arquivo_venda;
        arquivo_venda = fopen("vendas_realizadas.txt", "r");
        if(arquivo_venda == NULL){
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        while(fscanf(arquivo_venda, "%s %s %d %f", venda->cpfcliente, venda->nproduto, &venda->quanti, &venda->valorvenda)!=EOF){
            printf("Historico de vendas: Cliente: %s, Produto: %s, Quantidade: %d, Valor: %.2f\n", venda->cpfcliente, venda->nproduto, venda->quanti, venda->valorvenda);
        }
        fclose(arquivo_venda);
    }

    void quantidadevendas(Vendas *venda){
        FILE *arquivo_vendas;
        int totalvenda= 0;
        arquivo_vendas = fopen("vendas_realizadas.txt", "r");
        if(arquivo_vendas == NULL){
            printf("[ERROR] Nao foi possivel abrir o arquivo para leitura.\n");
            return;
        }
        while (fscanf(arquivo_vendas, "%s %s %d %f", venda->cpfcliente, venda->nproduto, &venda->quanti, &venda->valorvenda)!=EOF){
            totalvenda += venda->quanti;
        }
        fclose(arquivo_vendas);
        printf("Quantidade de produtos vendidos: %d\n", totalvenda);
    }

