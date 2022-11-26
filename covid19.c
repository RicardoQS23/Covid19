#include "covid19.h"
/************************************************************************************* P R O G R A M A  P R I N C I P A L *************************************************************************************/

int main(int argc, char *argv[])
{
    Lista_Princ *Head = NULL;
    leitura leitura = ALL;
    selecao selecao = NENHUMA_SEL;
    restricao restricao = NENHUMA_RES;
    ordenacao ordem = ALFA;
    char *Pdata = NULL, *Pdata1 = NULL, *Pdata2 = NULL, *cont_input = NULL, *Sdata = NULL, *ficheiro_in = NULL, *ficheiro_out = NULL;
    int index, n = 0, iflag = 0, oflag = 0, p1flag = 0, p2flag = 0, sflag = 0; //flags auxiliares para a validação dos argumentos da linha de comando

    int opt = 'h'; //inicializa a opção opt ao interface de ajuda caso não sejam inseridas quaisquer opções
    opterr = 0;    // evita a ocorrência de erros por opções inválidas por parte da função getopt()

    while ((opt = getopt(argc, argv, LISTA_OPCOES)) != -1) // processar as opções da linha de comando
    {
        switch (opt)
        {
        case 'i': /* nome do ficheiro de entrada */
            iflag = 1;
            ficheiro_in = optarg;
            break;

        case 'o': /* nome do ficheiro de saída */
            oflag = 1;
            ficheiro_out = optarg;
            break;

        case 'L': /* tipo de leitura*/
            if (strcmp(optarg, "all") == 0)
                leitura = ALL;
            else
            {
                cont_input = optarg;
                if (valida_cont_in(cont_input) == false)
                {
                    printf("opção de leitura \"%s\" desconhecida.\n", optarg);
                    usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                leitura = CONTINENTE;
            }
            break;

        case 'S': /* tipo de ordenação*/
            optind--;
            for (index = 0; optind < argc && *argv[optind] != '-'; index++, optind++)
            {
                switch (index)
                {
                case 0:
                    if (strcmp(optarg, "alfa") == 0)
                        ordem = ALFA;

                    else if (strcmp(optarg, "pop") == 0)
                        ordem = POPULACAO;

                    else if (strcmp(optarg, "inf") == 0)
                        ordem = INFETADOS;

                    else if (strcmp(optarg, "dea") == 0)
                        ordem = MORTES;
                    else
                    {
                        printf("opção de ordenação \"%s\" desconhecida.\n", optarg);
                        usage(argv[0]);
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 1:
                    sflag = 1;
                    if (verifica_data(argv[optind]) == 1)
                    {
                        printf("\nErro: argumento \"%s\" inválido.\n", argv[optind]);
                        usage(argv[0]);
                        exit(EXIT_FAILURE);
                    }
                    Sdata = argv[optind];
                    break;
                }
            }
            break;

        case 'D': /* tipo de seleção*/
            if (strcmp(optarg, "inf") == 0)
                selecao = INF;

            else if (strcmp(optarg, "dea") == 0)
                selecao = DEA;

            else if (strcmp(optarg, "racioinf") == 0)
                selecao = RACIOINF;

            else if (strcmp(optarg, "raciodea") == 0)
                selecao = RACIODEA;
            else
            {
                printf("opção de selecao \"%s\" desconhecida.\n", optarg);
                usage(argv[0]);
                exit(EXIT_FAILURE);
            }
            break;

        case 'P': /* tipo de restrição*/
            optind--;
            for (index = 0; optind < argc && *argv[optind] != '-'; index++, optind++)
            {
                switch (index)
                {
                case 0:
                    if (strcmp(argv[optind], "min") == 0)
                        restricao = MIN;

                    else if (strcmp(argv[optind], "max") == 0)
                        restricao = MAX;

                    else if (strcmp(argv[optind], "date") == 0)
                        restricao = DATE;

                    else if (strcmp(argv[optind], "dates") == 0)
                        restricao = DATES;
                    else
                    {
                        printf("\nopção de selecao \"%s\" desconhecida.\n", optarg);
                        usage(argv[0]);
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 1:
                    p1flag = 1;
                    if (restricao == MIN || restricao == MAX)
                    {
                        if (verifica_inteiros(argv[optind]) == 1)
                        {
                            printf("\nErro: argumento \"%s\" inválido.\n", argv[optind]);
                            usage(argv[0]);
                            exit(EXIT_FAILURE);
                        }
                        sscanf(argv[optind], "%d", &n);
                    }
                    if (restricao == DATE || restricao == DATES)
                    {
                        if (verifica_data(argv[optind]) == 1)
                        {
                            printf("\nErro: argumento \"%s\" inválido.\n", argv[optind]);
                            usage(argv[0]);
                            exit(EXIT_FAILURE);
                        }
                        Pdata = Pdata1 = argv[optind];
                    }
                    break;
                case 2:
                    p2flag = 1;
                    if (verifica_data(argv[optind]) == 1)
                    {
                        printf("\nErro: argumento \"%s\" inválido.\n", argv[optind]);
                        usage(argv[0]);
                        exit(EXIT_FAILURE);
                    }
                    Pdata2 = argv[optind];
                    break;
                }
            }
            break;
        default: //caso a opção não seja reconhecida
            printf("\nErro: opção '%c' desconhecida.\n", optopt);
        case 'h':
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    //caso as opções obrigatórias não sejam invocadas na linha de comandos ou não sejam introduzido argumentos necessários ao correto funcionamento do programa
    if ((iflag == 0 || oflag == 0) || ((ordem == INFETADOS || ordem == MORTES) && sflag == 0) || (restricao != NENHUMA_RES && p1flag == 0) || (restricao == DATES && p2flag == 0))
    {
        printf("\nAusência de argumentos obrigatórios.\n");
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    if (optind != argc)
    {
        printf("\nExistência de argumentos desnecessários.\n");
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    Head = read_data(ficheiro_in, leitura, cont_input); //leitura do ficheiro de entrada

    if (selecao != NENHUMA_SEL) //faz a selecao de dados
        selecao_dados(Head, selecao);

    if (restricao != NENHUMA_RES) //faz a restricao de dados
        Head = restricao_dados(Head, restricao, n, Pdata, Pdata1, Pdata2);

    if (file_type(ficheiro_in) == CSV)
        mergesort(&Head, ordem, Sdata); //faz a ordenacao de dados

    write_data(ficheiro_out, Head); //escrita da lista no ficheiro de saída
    liberta_lista_pri(Head);        //liberta a lista
    return EXIT_SUCCESS;
}
/***************************************************************************************** O U T R A S  F U N Ç Õ E S *****************************************************************************************/
/**
 * @brief Invoca a interface de ajuda com as instruções para o correto funcionamento do programa
 * 
 * @param programa char *: recebe o nome do programa
 * \return void
 */
void usage(char *programa)
{
    printf("Utilização: %s [opções]\n", programa);
    printf("opções válidas:\n");
    printf("[%s]\t\t   mostrar instruções\n", programa);
    printf("[-i ficheiro]\t   invoca o <ficheiro> de entrada, é uma opção obrigatória para o funcionamento do programa\n");
    printf("[-o ficheiro]\t   indica o <ficheiro> de saída, é uma opção obrigatória para o funcionamento do programa\n");
    printf("[-L argumento]\t   opcão de Leitura de dados:\n\t\t\t-caso o argumento = <all> o programa faz a leitura integral do ficheiro\n\t\t\t-caso o argumento= <nome_continente> o programa apenas lê dados respetivos ao continente introduzido na linha e comando, estes devem começar com letra maiúscula\n");
    printf("[-S argumento]\t   opcão de Ordenação de dados:\n\t\t\t-caso o argumento = <alfa> ordena alfabeticamente\n\t\t\t-caso argumento = <pop> ordena por ordem decrescente de população\n\t\t\t-caso o argumento = <inf yyyy-ww> ordena por ordem decrescente do numero total de infetados por países numa determinada data yyyy-ww\n\t\t\t-caso o argumento = <dea yyyy-ww>, ordena por ordem decrescente do numero total de mortes por países numa determinda data\n");
    printf("[-D argumento]\t   opcão de Seleção de dados:\n\t\t\t-caso o argumento = <inf> é seleccionada para cada país a semana com mais infetados\n\t\t\t-caso o argumento = <dea> é seleccionada para cada país a semana com mais mortes\n\t\t\t-caso o argumento = <racioinf> é seleccionada para cada país a semana com maior racio de infetados\n\t\t\t-caso o argumento = <raciodea> é seleccionada para cada país a semana com maior racio de mortes\n");
    printf("[-P argumento]\t   opcão de Restrição de dados:\n\t\t\t-caso o argumento = <min n> são eliminados da lista todos os países com população inferior a n mil habitantes\n\t\t\t-caso o argumento = <max n> são eliminados da lista todos os países com população supeior a n mil habitantes\n\t\t\t-caso o argumento = <date yyyy-ww> são eliminados da lista todos os dados que não são relativos à semana indicada\n\t\t\t-caso o argumento = <dates yyyy-ww yyyy-ww> são eliminados da lista todos os dados que não se encontrem no interior do intervalo limitados por essas semanas\n");
}
/*************************************************************************************************************************************************************************************************************/
