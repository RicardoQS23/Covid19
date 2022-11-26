#include "covid19.h"
/************************************************************************************* R E S T R I Ç Ã O  D E  D A D O S *************************************************************************************/
/**
 * @brief escolhe o tipo de restrição de dados consoante o opção introduzida na linha de comando
 * 
 * @param Head Lista_Princ *: cabeça de lista da lista principal
 * @param tipo_restricao restricao: tipo de restrição escolhida na linha de comando
 * @param n int: parâmetro da restrição segundo o numero de n mil habitantes introduzido na linha de comando
 * @param semana char *: data introduzida na linha de comando
 * @param semana1 char *: um dos limites do intervalo de datas introduzido na linha de comando
 * @param semana2  char *: um dos limites do intervalo de datas introduzido na linha de comando
 * @return Lista_Princ* cabeça de lista da lista principal
 */
Lista_Princ *restricao_dados(Lista_Princ *Head, restricao tipo_restricao, int n, char *semana, char *semana1, char *semana2)
{
    if (tipo_restricao == MIN)
        Head = restrict_min(Head, n);
    if (tipo_restricao == MAX)
        Head = restrict_max(Head, n);
    if (tipo_restricao == DATE)
        Head = restrict_data(Head, semana);
    if (tipo_restricao == DATES)
        Head = restrict_datas(Head, semana1, semana2);
    return Head;
}

/**
 * @brief restringe toda a informação de países com menos de n mil habitantes
 * 
 * @param head Lista_Princ *: cabeça de lista da lista principal
 * @param n int: n mil habitantes
 * @return Lista_Princ*: cabeça de lista da lista principal já alterada
 */
Lista_Princ *restrict_min(Lista_Princ *head, int n)
{
    Lista_Princ *Aux, *ptr;
    Lista_Sec *Aux_S, *Head_S;
    Aux = ptr = head;

    while (ptr != NULL)
    {
        Aux = ptr;       //começa na cabeça da lista principal
        ptr = ptr->next; //avança na lista principal

        Aux_S = Head_S = Aux->LSec_Head; //começa na cabeça da lista secundaria

        if ((Aux->population) < n * 1000)
        {
            //Liberta lista secundária
            while (Aux_S != NULL)
            {
                Head_S = Aux_S->next;
                free(Aux_S);
                Aux_S = Head_S;
            }
            head = remove_from_list_P(head, Aux); // remove o nó da lista principal e atualiza a cabeça da lista caso esta seja eliminada
        }
    }
    return head;
}
/**
 * @brief restringe toda a informação de países com mais de n mil habitantes
 * 
 * @param head Lista_Princ *: cabeça de lista da lista principal
 * @param n int: n mil habitantes
 * @return Lista_Princ*: cabeça de lista da lista principal já alterada
 */
Lista_Princ *restrict_max(Lista_Princ *head, int n)
{
    Lista_Princ *Aux, *ptr;
    Lista_Sec *Aux_S, *Head_S;
    Aux = ptr = head;

    while (ptr != NULL)
    {
        Aux = ptr;       //começa na cabeça da lista principal
        ptr = ptr->next; //avança na lista principal

        Aux_S = Head_S = Aux->LSec_Head; //começa na cabeça da lista secundaria

        if ((Aux->population) > n * 1000)
        {
            //Liberta lista secundária
            while (Aux_S != NULL)
            {
                Head_S = Aux_S->next;
                free(Aux_S);
                Aux_S = Head_S;
            }
            head = remove_from_list_P(head, Aux); // remove o nó da lista principal e atualiza a cabeça da lista caso esta seja eliminada
        }
    }
    return head;
}

/**
 * @brief restringe toda a informação que não corresponda a dados dessa semana
 * 
 * @param head Lista_Princ*:cabeça de lista da lista principal
 * @param Pdata char*: data de referência
 * @return Lista_Princ*:cabeça de lista da lista principal já alterada
 */
Lista_Princ *restrict_data(Lista_Princ *head, char *Pdata)
{
    Lista_Princ *Aux, *ptr;
    Lista_Sec *Aux_S, *Head_S;

    Aux = ptr = head;

    while (ptr != NULL)
    {
        Aux = ptr;
        ptr = ptr->next;
        Aux_S = Head_S = Aux->LSec_Head; //começa na cabeça da lista secundaria
        while (Aux_S != NULL)
        {
            Head_S = Head_S->next;
            if (strcmp(Aux_S->year_week, Pdata) != 0)
                Aux->LSec_Head = remove_from_list_S(Aux->LSec_Head, Aux_S);
            Aux_S = Head_S;
        }
        if (Aux->LSec_Head == NULL) //caso o elemento não apresente uma lista secundária significa que não possui dados respetivos à data estabelecida
            head = remove_from_list_P(head, Aux);
    }
    return head;
}

/**
 * @brief restringe toda a informação que não se insira no intervalo delimitado pelas semanas Pdata1 e Pdata2
 * 
 * @param head Lista_Princ *: cabeça de lista da lista principal
 * @param Pdata1 char *: um dos limites do intervalo de datas
 * @param Pdata2 char *: um dos limites do intervalo de datas
 * @return Lista_Princ*:cabeça de lista da lista principal já alterada
 */
Lista_Princ *restrict_datas(Lista_Princ *head, char *Pdata1, char *Pdata2)
{
    Lista_Princ *Aux, *ptr;
    Lista_Sec *Aux_S, *Head_S;

    max_data(Pdata1, Pdata2); //coloca a Pdata1 como a mais antiga e a Pdata2 como a mais recente

    Aux = ptr = head;

    while (ptr != NULL)
    {
        Aux = ptr;
        ptr = ptr->next;
        Aux_S = Head_S = Aux->LSec_Head; //começa na cabeça da lista secundaria
        while (Aux_S != NULL)
        {
            Head_S = Head_S->next;
            if (strcmp(Aux_S->year_week, Pdata1) < 0 || strcmp(Aux_S->year_week, Pdata2) > 0)
                Aux->LSec_Head = remove_from_list_S(Aux->LSec_Head, Aux_S);
            Aux_S = Head_S;
        }
        if (Aux->LSec_Head == NULL) //caso o elemento não apresente uma lista secundária significa que não possui dados no intervalo delimitado pelas datas estabelecidas
            head = remove_from_list_P(head, Aux);
    }
    return head;
}

/**
 * @brief Copia para data1 a data mais antiga e para data2 a data mais recente
 * 
 * @param data1 char *: primeira data
 * @param data2 char *: segunda data
 * @return void
 */
void max_data(char *data1, char *data2)
{
    char *temp = NULL;
    if (strcmp(data1, data2) > 0) //caso a data1 seja mais recente que a data2 faz a troca
    {
        strcpy(temp, data2);
        strcpy(data2, data1);
        strcpy(data1, temp);
    }
}

/** \brief apaga uma entrada da lista principal e atualiza a nova cabeça caso esta seja apagada
 *
 * \param head Lista_Princ*: cabeça de lista da lista principal.
 * \param ptr Lista_Princ*: apontador para a entrada a apagar
 * @return Lista_Princ*: novo cabeça de lista, que pode ter sido alterado
 *
 */
Lista_Princ *remove_from_list_P(Lista_Princ *head, Lista_Princ *ptr)
{
    if (ptr != NULL && head != NULL)
    {
        if (ptr == head) // primeiro da lista principal
        {
            head = head->next;
            if (head != NULL) // se a lista não ficou vazia
                head->prev = NULL;
        }
        else
        {
            ptr->prev->next = ptr->next;
            if (ptr->next != NULL)
                ptr->next->prev = ptr->prev;
        }
        free(ptr); // libertar o elemento apagado
    }
    return head;
}

/**
 * @brief apaga uma entrada da lista secundária
 * 
 * @param head Lista_Sec*: cabeça de lista da lista principal.
 * @param ptr Lista_Sec*: apontador para a entrada a apagar
 * @return Lista_Sec*: novo cabeça de lista, que pode ter sido alterado
 */
Lista_Sec *remove_from_list_S(Lista_Sec *head_S, Lista_Sec *ptr)
{
    if (ptr != NULL && head_S != NULL)
    {
        if (ptr == head_S) // primeiro da lista secundaria
        {
            head_S = head_S->next;
            if (head_S != NULL) // se a lista não ficou vazia
                head_S->prev = NULL;
        }
        else
        {
            ptr->prev->next = ptr->next;
            if (ptr->next != NULL)
                ptr->next->prev = ptr->prev;
        }
        free(ptr); // liberta a memoria alocada para o elemento apagado
    }
    return head_S;
}
/*************************************************************************************************************************************************************************************************************/