#include "covid19.h"
/********************************************************************Funcoes relacionadas com a lista principal********************************************************************/
/**
 * @brief Aloca memória para o novo elemento da lista principal e copia os dados fixos para esse mesmo elemento
 * 
 * @param pais char[STR_SIZE]: nome do país
 * @param sigla char[SGL_SIZE]: sigla do país
 * @param continente char[STR_SIZE]: continente onde se localiza o país
 * @param populacao int: numero de habitantes do país
 * @return Lista_Princ* : ponteiro para o elemento da lista principal criado, NULL se não houver memória
 */
Lista_Princ *gera_pais(char pais[STR_SIZE], char sigla[SGL_SIZE], char continente[STR_SIZE], int populacao)
{
    Lista_Princ *New_Ctr = NULL;

    New_Ctr = (Lista_Princ *)calloc(1, sizeof(Lista_Princ)); //aloca memória para o novo elemento e inicializa toda a estrutura a 0 e NULL

    if (New_Ctr == NULL)
    {
        printf("Calloc falhou. Ficheiro demasiado grande. Programa terminado.\n");
        return NULL;
    }
    //passagem dos dados fixos para o novo elemento da lista principal
    strcpy(New_Ctr->country, pais);
    strcpy(New_Ctr->country_code, sigla);
    strcpy(New_Ctr->continent, continente);
    New_Ctr->population = populacao;

    return New_Ctr;
}

/**
 * @brief Coloca o novo elemento criado no fim da lista principal
 * 
 * @param head Lista_Princ **: cabeça de lista da lista principal
 * @param tail Lista_Princ **: cauda de lista da lista principal
 * @param ptr Lista_Princ *: ponteiro para o novo elemento a inserir no fim da lista principal
 * \return void
 */
void coloca_fim_Lpri(Lista_Princ **head, Lista_Princ **tail, Lista_Princ *ptr)
{
    /* primeiro elemento da lista */
    if (*tail == NULL)
    {
        /* primeiro elemento da lista */
        *head = *tail = ptr;
        ptr->next = ptr->prev = NULL;
    }
    else
    {
        /* pôr no fim */
        ptr->next = NULL;
        ptr->prev = *tail;
        (*tail)->next = ptr;
        *tail = ptr;
    }
}

/**
 * @brief Verifica se o país já existe na lista principal
 * 
 * @param Head Lista_Princ *: cabeça de lista da lista principal
 * @param pais char*: ponteiro para o nome do país lido do buffer
 * @return Lista_Princ*: ponteiro para o pais já na lista principal, NULL caso o país ainda não exista na lista principal
 */

Lista_Princ *must_create_node(Lista_Princ *Head, char pais[STR_SIZE])
{
    Lista_Princ *Aux;
    Aux = Head;
    while (Head != NULL)
    {

        Aux = Head;
        Head = Head->next;
        if (strcmp(Aux->country, pais) == 0)
            return Aux;
    }
    return NULL;
}

/**
 * @brief liberta a memória alocada para toda a lista principal
 * 
 * @param Head Lista_Princ *: cabeça de lista da lista principal
 * \return void
 */
void liberta_lista_pri(Lista_Princ *Head)
{
    Lista_Princ *Aux;
    Aux = Head;

    while (Aux != NULL)//Percorre a lista principal
    {                      
        Head = Head->next;                 
        liberta_lista_sec(Aux->LSec_Head); //liberta toda a lista secundaria anexada ao no da lista principal
        free(Aux);
        Aux = Head;  
    }
}
/********************************************************************Funcoes relacionadas com a lista secundaria********************************************************************/
/**
 * @brief Aloca memória para o novo elemento da lista secundária e copia os dados variáveis para esse mesmo elemento
 * 
 * @param indicador char[STR_SIZE]: apontador para o indicador
 * @param contagem_semanal int: valor da contagem semanal
 * @param ano_semana char[DATA_SIZE]: apontador para a data
 * @param racio float: valor da rate_14_day nessa data
 * @param contagem_acumulativa int: valor da contagem acumulativa
 * @return Lista_Sec*: ponteiro para o elemento da lista secundaria criado, NULL se não houver memória
 */

Lista_Sec *gera_lista_sec(char indicador[STR_SIZE], int contagem_semanal, char ano_semana[DATA_SIZE], float racio, int contagem_acumulativa)
{
    Lista_Sec *New_line;

    New_line = (Lista_Sec *)calloc(1, sizeof(Lista_Sec)); // aloca memória para o novo elemento da lista secundária e inicializa toda a estrutura

    if (New_line == NULL)
    {
        printf("calloc falhou. ficheiro demasiado grande. programa terminado.\n");
        return NULL;
    }
    //passagem dos dados variaveis para o novo elemento da lista secundária
    strcpy(New_line->indicator, indicador);
    New_line->weekly_count = contagem_semanal;
    strcpy(New_line->year_week, ano_semana);
    New_line->rate_14_Day = racio;
    New_line->cumulative_count = contagem_acumulativa;

    return New_line;
}

/**
 * @brief Coloca o novo elemento no fim da lista secundária
 * 
 * @param ptr Lista_Princ **: nó da lista principal onde se irá adicionar o elemento da lista secundária
 * @param ptr_S Lista_Sec *: ponteiro para o novo elemento a inserir no fim da lista secundaria
 * \return void
 */
void coloca_fim_Lsec(Lista_Princ **ptr, Lista_Sec *ptr_S)
{
    if ((*ptr)->LSec_Tail == NULL) //atribui o ponteiro para o primeiro elemento da lista secundaria ao ponteiro da lista principal caso a lista secundaria esteja vazia
    {
        (*ptr)->LSec_Head = (*ptr)->LSec_Tail = ptr_S;
        ptr_S->next = ptr_S->prev = NULL;
    }
    else
    {
        // coloca no fim e atualiza o ponteiro para o final da lista secundaria
        ptr_S->next = NULL;
        ptr_S->prev = (*ptr)->LSec_Tail;
        (*ptr)->LSec_Tail->next = ptr_S;
        (*ptr)->LSec_Tail = ptr_S;
    }
}

/**
 * @brief liberta a memória alocada para toda a lista secundária
 * 
 * @param ptr_S Lista_Sec *: cabeça de lista da lista secundária
 * \return void
 */
void liberta_lista_sec(Lista_Sec *ptr_S)
{
    Lista_Sec *Aux_S, *Head_S;

    Aux_S = Head_S = ptr_S; //inicia na cabeça da lista secundaria
    //Liberta lista secundária
    while (Aux_S != NULL)
    {
        Head_S = Aux_S->next;
        free(Aux_S);
        Aux_S = Head_S;
    }
}
/*************************************************************************************************************************************************************************************************************/