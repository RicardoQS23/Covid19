#include "covid19.h"
/********************************************************************* L E I T U R A  D E  I N P U T  E  E S C R I T A  D E  O U T P U T *********************************************************************/
/**
 * @brief determina o tipo de ficheiro de entrada/saída 
 * 
 * @param ficheiro_io char *: nome do ficheiro de entrada/saída
 * @return tipo_ficheiro : BINARIO caso os últimos três caracteres do nome do ficheiro sejam "dat", caso contrario retorna CSV
 */
tipo_ficheiro file_type(char *ficheiro_io)
{
    int length;
    length = strlen(ficheiro_io);
    if (ficheiro_io[length - 3] == 'd' && ficheiro_io[length - 2] == 'a' && ficheiro_io[length - 1] == 't')
        return BINARIO;
    return CSV;
}

/**
 * @brief determina o tipo de leitura do ficheiro de entrada consoante o tipo de ficheiro
 * 
 * @param input_file char *:nome do ficheiro de entrada
 * @param tipo_leitura leitura: tipo de leitura consoante o input do utilizador na linha de comando
 * @param cont_input char *: nome do continente caso o tipo de leitura seja CONTINENTE
 * @return Lista_Princ* : retorna a cabeça de lista da lista principal
 */
Lista_Princ *read_data(char *input_file, leitura tipo_leitura, char *cont_input)
{
    FILE *fp = NULL;
    Lista_Princ *head = NULL;
    tipo_ficheiro f_type = file_type(input_file);

    if (f_type == BINARIO) //Modo de leitura para ficheiro binario
    {
        if ((fp = fopen(input_file, "rb")) == NULL) //inicio da leitura do ficheiro
        {
            printf("Erro na leitura do ficheiro binário.\n");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        else
        {
            head = read_binary_file(fp);
            fclose(fp);
        }
    }
    else //Modo de leitura para ficheiro csv
    {
        if ((fp = fopen(input_file, "r")) == NULL)
        {
            printf("Erro na leitura do ficheiro csv.\n");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        else
        {
            head = read_csv_file(fp, tipo_leitura, cont_input);
            fclose(fp);
        }
    }
    return head;
}

/**
 * @brief lê o conteúdo de um ficheiro csv e cria uma lista com esses mesmos dados
 * 
 * @param fp FILE*: recebe o apontador para o ficheiro input
 * @param tipo_leitura leitura: tipo de leitura consoante o input do utilizador na linha de comando
 * @param cont_input char*: recebe o nome do continente da linha de comando
 * @return Lista_Princ* : cabeça de lista da lista criada
 */
Lista_Princ *read_csv_file(FILE *fp, leitura tipo_leitura, char *cont_input)
{
    char buffer[LINE_SIZE]; //le o conteudo da linha do ficheiro
    char *buffer_copy;
    int n_linhas, length;
    //variaveis relacionadas com os dados de listas
    char pais[STR_SIZE];
    char sigla[SGL_SIZE];
    char continente[STR_SIZE];
    int populacao = 0;
    char indicador[STR_SIZE];
    int contagem_semanal = 0;
    char ano_semana[DATA_SIZE];
    float racio = 0.0;
    int contagem_acumulativa = 0;

    Lista_Princ *head, *tail, *aux_ptr = NULL;
    Lista_Sec *aux_ptr_S = NULL;
    head = tail = NULL;

    n_linhas = 0;
    //Faz a leitura da linha
    while (fgets(buffer, LINE_SIZE, fp) != NULL)
    {
        length = strlen(buffer);
        if (buffer[length - 1] == '\n') //transforma o caracter de mudança de linha num terminador de string
            buffer[length - 1] = '\0';

        if (n_linhas == 0) //ignora o cabeçalho do ficheiro
            n_linhas++;
        else
        {
            buffer_copy = buffer;
            separa_buffer(fp, head, buffer_copy, pais, sigla, continente, &populacao, indicador, &contagem_semanal, ano_semana, &racio, &contagem_acumulativa);

            if (tipo_leitura == ALL || (tipo_leitura == CONTINENTE && strcmp(cont_input, continente) == 0)) //apenas lê caso a leitura seja integra ou o continente lido corresponda ao introduzido na linha de comandos
            {
                if ((aux_ptr = must_create_node(head, pais)) == NULL) //caso o pais ainda não exista na lista principal é criado um novo nó
                {
                    aux_ptr = gera_pais(pais, sigla, continente, populacao); //cria novo elemento na lista principal
                    if (aux_ptr == NULL)
                    {
                        printf("Erro na alocacao de memoria do elemento da lista principal.\n");
                        liberta_lista_pri(head);
                        fclose(fp);
                        exit(EXIT_FAILURE);
                    }
                    else
                        coloca_fim_Lpri(&head, &tail, aux_ptr);
                }
                aux_ptr_S = gera_lista_sec(indicador, contagem_semanal, ano_semana, racio, contagem_acumulativa); //cria novo elemento na lista secundária
                if (aux_ptr_S == NULL)
                {
                    printf("Erro na alocacao de memoria do elemento da lista secundária.\n");
                    liberta_lista_pri(head);
                    fclose(fp);
                    exit(EXIT_FAILURE);
                }
                coloca_fim_Lsec(&aux_ptr, aux_ptr_S);
                if (strcmp(indicador, "cases") == 0)
                    atualiza_ptr_dados_inf(&aux_ptr, aux_ptr_S);

                if (strcmp(indicador, "deaths") == 0)
                    atualiza_ptr_dados_dea(&aux_ptr, aux_ptr_S);
            }
            n_linhas++;
        }
    }
    return head;
}

/**
 * @brief Caso os dados sejam invalidos, liberta toda a memoria alocada e envia uma mensagem de erro para o stderr
 * 
 * @param fp FILE *: ponteiro para o ficheiro de entrada
 * @param Head Lista_Princ *: cabeça de lista da lista principal
 * \return void
 */
void dados_invalidos(FILE *fp, Lista_Princ *Head)
{
    printf("-1 Erro de leitura...\n");
    liberta_lista_pri(Head);
    fclose(fp);
    exit(EXIT_FAILURE);
}

/**
 * @brief Separa o buffer em virgulas e atribui o conteudo de cada palavra a cada uma das variaveis passadas por referência, faz também uma verificação da legibilidade desses mesmos dados
 * 
 * @param fp FILE*: apontador para o ficheiro de entrada
 * @param Head Lista_Princ* : cabeça de lista da lista principal,c aso o programa deva ser abortado liberta a lista
 * @param buffer_copy char *: copia do buffer que será separada por virgulas e analisada posteriormente
 * @param pais char *: nome do pais
 * @param sigla char *: sigla do pais
 * @param continente char *: continente onde o pais se localiza
 * @param populacao int *: populacao do pais
 * @param indicador char *: indicador 
 * @param contagem_semanal int *: contagem semanal
 * @param ano_semana char *: data a que correspondem os dados 
 * @param racio float *: valor da rate_14_day
 * @param contagem_acumulativa int *: contagem acumulativa
 * \return void
 */
void separa_buffer(FILE *fp, Lista_Princ *Head, char *buffer_copy, char *pais, char *sigla, char *continente, int *populacao, char *indicador, int *contagem_semanal, char *ano_semana, float *racio, int *contagem_acumulativa)
{
    char *token = NULL;
    int count_token = 1, length = strlen(buffer_copy);

    if (buffer_copy[length - 1] == '\n')
        buffer_copy[length - 1] = '\0';

    while ((token = strsep(&buffer_copy, ",")) != NULL) //percorre o buffer, separando as diferentes palavras por virgulas
    {
        switch (count_token)
        {
        case 1:
            if (verifica_strings(token) == 1)
                dados_invalidos(fp, Head);
            strcpy(pais, token);
            break;
        case 2:
            if (verifica_strings(token) == 1)
                dados_invalidos(fp, Head);
            strcpy(sigla, token);
            break;
        case 3:
            if (valida_cont_in(token) == false)
                dados_invalidos(fp, Head);
            strcpy(continente, token);
            break;
        case 4:
            if (verifica_inteiros(token) == 1)
                dados_invalidos(fp, Head);
            sscanf(token, "%d", &*populacao);
            break;
        case 5:
            if (verifica_strings(token) == 1)
                dados_invalidos(fp, Head);
            strcpy(indicador, token);
            break;
        case 6:
            if (verifica_inteiros(token) == 1)
                dados_invalidos(fp, Head);

            sscanf(token, "%d", &*contagem_semanal);
            break;
        case 7:
            if (verifica_data(token) == 1)
                dados_invalidos(fp, Head);
            strcpy(ano_semana, token);
            break;
        case 8:
            if (verifica_float(token) == 1)
                dados_invalidos(fp, Head);
            sscanf(token, "%f", &*racio);
            if (strlen(token) == 0) //assegura que caso não leia o valor do racio esteja fica inicializado a 0.0
                *racio = 0.0;
            break;
        case 9:
            if (sscanf(token, "%d", &*contagem_acumulativa) != 1)
                dados_invalidos(fp, Head);
            break;
        }
        count_token++;
    }
    if (count_token != 10) //Termina o programa caso o numero de colunas no ficheiro não seja o correto
        dados_invalidos(fp, Head);
}

/**
 * @brief conta o numero de nós da lista principal
 * 
 * @param Head Lista_Princ *: cabeça de lista da lista principal
 * @return int : retorna o numero de paises contados da lista
 */
int contagem_paises(Lista_Princ *Head)
{
    Lista_Princ *Aux = Head;
    int n_paises = 0;
    while (Aux != NULL)
    {
        n_paises++;
        Aux = Aux->next;
    }
    return n_paises;
}

/**
 * @brief conta o numero de nós da lista secundária
 * 
 * @param Head Lista_Princ *: ponteiro para o país atual
 * @return int : numero de elementos da lista secundária
 */
int contagem_semanas(Lista_Princ *ptr)
{
    Lista_Sec *Aux_S = ptr->LSec_Head;
    int n_elementos = 0;
    while (Aux_S != NULL)
    {
        n_elementos++;
        Aux_S = Aux_S->next;
    }
    return n_elementos;
}

/**
 * @brief lê os dados contidos no ficheiro binário e cria uma lista com esses mesmos dados
 * 
 * @param fp FILE *:apontador para o ficheiro de entrada
 * @return Lista_Princ*: retorna a cabeça de lista da lista principal criada
 */
Lista_Princ *read_binary_file(FILE *fp)
{
    //numero de elementos
    int n_paises, n_elementos_sec, i, j;
    //variaveis relacionadas com os dados da lista
    char pais[STR_SIZE];
    char sigla[SGL_SIZE];
    char continente[STR_SIZE];
    int populacao = 0;
    char indicador[STR_SIZE];
    int contagem_semanal = 0;
    char ano_semana[STR_SIZE];
    float racio = 0.0;
    int contagem_acumulativa = 0;

    Lista_Princ *head, *tail, *aux_ptr = NULL;
    Lista_Sec *aux_ptr_S = NULL;
    head = tail = NULL;

    fread(&n_paises, sizeof(int), 1, fp); //lê o numero de nós da lista principal
    for (i = 0; i < n_paises; i++)        //lê a informação respetiva ao numero de nós da lista principal
    {
        n_elementos_sec = 0;
        fread(pais, STR_SIZE, 1, fp);
        fread(sigla, SGL_SIZE, 1, fp);
        fread(continente, STR_SIZE, 1, fp);
        fread(&populacao, sizeof(int), 1, fp);
        fread(&n_elementos_sec, sizeof(int), 1, fp);
        for (j = 0; j < n_elementos_sec; j++)
        {
            fread(indicador, STR_SIZE, 1, fp);
            fread(&contagem_semanal, sizeof(int), 1, fp);
            fread(ano_semana, DATA_SIZE, 1, fp);
            fread(&racio, sizeof(float), 1, fp);
            fread(&contagem_acumulativa, sizeof(int), 1, fp);

            if ((aux_ptr = must_create_node(head, pais)) == NULL) //caso o pais ainda não exista na lista principal é criado um novo nó
            {
                aux_ptr = gera_pais(pais, sigla, continente, populacao);
                if (aux_ptr == NULL)
                {
                    printf("Erro na alocacao de memoria do elemento da lista principal.\n");
                    liberta_lista_pri(head);
                    fclose(fp);
                    exit(EXIT_FAILURE);
                }
                else
                    coloca_fim_Lpri(&head, &tail, aux_ptr);
            }

            aux_ptr_S = gera_lista_sec(indicador, contagem_semanal, ano_semana, racio, contagem_acumulativa);
            if (aux_ptr_S == NULL)
            {
                printf("Erro na alocacao de memoria do elemento da lista secundária.\n");
                liberta_lista_pri(head);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            coloca_fim_Lsec(&aux_ptr, aux_ptr_S); //coloca o novo elemento no fim da lista secundária
        }
    }
    return head;
}

/**
 * @brief escolhe o tipo de escrita consoante o nome do ficheiro introduzido na linha de comando
 * 
 * @param output_file char *: recebe o nome do ficheiro de saída
 * @param Head Lista_Princ*: recebe o cabeça de lista da lista principal
 * \return void 
 */
void write_data(char *output_file, Lista_Princ *Head)
{
    FILE *fp = NULL;
    tipo_ficheiro f_type = file_type(output_file);

    if (f_type == BINARIO)
    {
        if ((fp = fopen(output_file, "wb")) == NULL) //inicio da escrita do ficheiro
        {
            printf("erro na leitura.");
            exit(EXIT_FAILURE);
        }
        else
        {
            print_output_binary(fp, Head);
            fclose(fp);
        }
    }
    else
    {
        if ((fp = fopen(output_file, "w")) == NULL) //inicio da escrita do ficheiro
        {
            printf("erro na leitura.");
            exit(EXIT_FAILURE);
        }
        else
        {
            print_output_csv(fp, Head);
            fclose(fp);
        }
    }
}

/**
 * @brief Escreve o cabeçalho e a lista em formato de texto no ficheiro de saída 
 * 
 * @param fp FILE*: recebe o apontador para o ficheiro onde serão escritos os dados
 * @param Head Lista_Princ *: recebe o cabeça de lista da lista principal
 * \return void
 */
void print_output_csv(FILE *fp, Lista_Princ *Head)
{
    Lista_Princ *Aux = NULL;
    Lista_Sec *Aux_S = NULL;
    fprintf(fp, "%s\n", CABECALHO); //imprime o cabeçalho no ficheiro de saída
    for (Aux = Head; Aux != NULL; Aux = Aux->next)
    {
        for (Aux_S = Aux->LSec_Head; Aux_S != NULL; Aux_S = Aux_S->next)
            fprintf(fp, "%s,%s,%s,%d,%s,%d,%s,%.4f,%d\n", Aux->country, Aux->country_code, Aux->continent, Aux->population, Aux_S->indicator, Aux_S->weekly_count, Aux_S->year_week, Aux_S->rate_14_Day, Aux_S->cumulative_count);
    }
}

/**
 * @brief Escreve a lista em formato binário no ficheiro de saída 
 * 
 * @param fp FILE*: recebe o apontador para o ficheiro onde será escrita a lista
 * @param Head Lista_Princ *: recebe o cabeça de lista da lista principal
 * \return void
 */
void print_output_binary(FILE *fp, Lista_Princ *Head) //recebe ainda mais um parametro que indica o numero de países presentes na lista
{
    Lista_Princ *Aux = Head;
    Lista_Sec *Aux_S = NULL;
    int n_paises = contagem_paises(Head);
    fwrite(&n_paises, sizeof(int), 1, fp); //escreve o numero de nós da lista principal

    while (Aux != NULL)
    {

        fwrite(Aux->country, STR_SIZE, 1, fp);
        fwrite(Aux->country_code, SGL_SIZE, 1, fp);
        fwrite(Aux->continent, STR_SIZE, 1, fp);
        fwrite(&(Aux->population), sizeof(int), 1, fp);
        Aux->n_elementos_LS = contagem_semanas(Aux);
        fwrite(&(Aux->n_elementos_LS), sizeof(int), 1, fp);
        Aux_S = Aux->LSec_Head;
        while (Aux_S != NULL)
        {
            fwrite(Aux_S->indicator, STR_SIZE, 1, fp);
            fwrite(&(Aux_S->weekly_count), sizeof(int), 1, fp);
            fwrite(Aux_S->year_week, DATA_SIZE, 1, fp);
            fwrite(&(Aux_S->rate_14_Day), sizeof(float), 1, fp);
            fwrite(&(Aux_S->cumulative_count), sizeof(int), 1, fp);
            Aux_S = Aux_S->next;
        }
        Aux = Aux->next;
    }
}

/**
 * @brief Valida o nome do continente introduzido na linha de comandos
 * 
 * @param cont_input char *: nome do continente importado da linha de comandos
 * @return true , caso o nome do continente seja válido
 * @return false , caso o nome do continente não seja válido
 */
bool valida_cont_in(char *cont_input)
{
    char cont[5][STR_SIZE] = {"Africa", "Asia", "Europe", "America", "Oceania"};
    int i;
    for (i = 0; i < 5; i++)
    {
        if (strcmp(cont[i], cont_input) == 0)
            return true;
    }

    return false;
}

/**
 * @brief verifica a validade da string de entrada que descreve uma data do tipo yyyy-ww
 * 
 * @param data char *: string a ser analisada
 * @return int 0 caso o dado seja válido, 1 caso seja inválido
 */
int verifica_data(char *data)
{
    int c, length = strlen(data);

    for (c = 0; c < length; c++)
        if ((c != (length - 3) && isdigit(data[c]) == 0) || (data[length - 3] != '-') || (length != 7))
            return 1;
    return 0;
}

/**
 * @brief verifica se existem caractéres dentro da string
 * 
 * @param token char *: string onde vai ser lida um dado do tipo int
 * @return int : 0 caso seja válido, 1 caso seja inválido
 */
int verifica_inteiros(char *token)
{
    int c, length = strlen(token);

    for (c = 0; c < length; c++)
        if (isdigit(token[c]) == 0)
            return 1;

    return 0;
}

/**
 * @brief verifica se existem algarismos dentro da string
 * 
 * @param token char *:string a ser analisada
 * @return int :0 se vor válida, 1 caso seja inválida
 */
int verifica_strings(char *token)
{
    int c, length = strlen(token);

    for (c = 0; c < length; c++)
        if (isdigit(token[c]) > 0)
            return 1;
    return 0;
}

/**
 * @brief verifica se existem caracteres de texto diferentes de '.' dentro da string ou mais do que um caracter '.' na string
 * 
 * @param token char *: string onde se vai ler um dado do tipo float
 * @return int : retorna 0 se for válida, 1 caso seja inválida
 */
int verifica_float(char *token)
{
    int c, i = 0, length = strlen(token);
    for (c = 0; c < length; c++)
    {
        if ((isdigit(token[c]) == 0 && token[c] != '.') || (isdigit(token[c]) == 0 && i > 0)) //caso apareçam dois caracteres '.' ou caracteres que não sejam algarismos
            return 1;
        if (token[c] == '.')
            i++;
    }
    return 0;
}
/*************************************************************************************************************************************************************************************************************/