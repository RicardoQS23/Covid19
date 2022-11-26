#include "covid19.h"
/********************************************************************Funcoes relacionadas com a ordenacao de dados********************************************************************/
/**
 * @brief Algoritmo de ordenação que recorre à recursividade, separa a lista em sublistas de menor dimensão até cada uma dessas sublistas ficar com 0 ou 1 elementos, ordenando esses elementos posteriormente
 * 
 * @param Head Lista_Princ **: cabeça de lista da lista principal
 * @param tipo_ordem ordenacao: tipo de ordenação escolhida pelo utilizador na linha de comando
 * @param semana char *: parâmetro de ordenação
 */
void mergesort(Lista_Princ **Head, ordenacao tipo_ordem, char *semana)
{
    Lista_Princ *Aux_head = *Head;
    Lista_Princ *frontRef, *backRef; //ponteiros auxiliares para sublistas

    if ((Aux_head == NULL) || (Aux_head->next == NULL)) //caso a lista tenha 0 ou 1 elementos retorna
    {
        return;
    }
    front_back_split(Aux_head, &frontRef, &backRef); //separa a lista em duas partes com tamanho semelhante, excepto quando a lista tem dimensão ímpar e a frontRef fica com +1 elemento do que a backRef
    mergesort(&frontRef, tipo_ordem, semana);        //invoca a mesma funçao, tenho como cabeça da lista a frontRef
    mergesort(&backRef, tipo_ordem, semana);         //invoca a mesma funçao, tenho como cabeça da lista a backRef

    if (tipo_ordem == ALFA)
        *Head = sortedmerge_alfa(frontRef, backRef);
    if (tipo_ordem == POPULACAO)
        *Head = sortedmerge_pop(frontRef, backRef);
    if (tipo_ordem == INFETADOS)
        *Head = sortedmerge_inf(frontRef, backRef, semana);
    if (tipo_ordem == MORTES)
        *Head = sortedmerge_dea(frontRef, backRef, semana);
}
/**
 * @brief Ordena as sublistas alfabeticamente, e vai juntando as varias sublistas, recorrendo à recursividade
 * 
 * @param frontRef Lista_Princ *: cabeça de uma das sublistas
 * @param backRef Lista_Princ *: cabeça de uma das sublistas
 * @return Lista_Princ* :retorna uma sublista com dimensao superior à anterior, ordenada alfabeticamente
 */
Lista_Princ *sortedmerge_alfa(Lista_Princ *frontRef, Lista_Princ *backRef)
{
    Lista_Princ *result = NULL;

    if (frontRef == NULL)
        return (backRef);
    else if (backRef == NULL)
        return (frontRef);

    if (strcmp(frontRef->country, backRef->country) < 0)
    {
        result = frontRef;
        result->next = sortedmerge_alfa(frontRef->next, backRef);
        result->next->prev = frontRef;
        result->prev = NULL;
    }
    else
    {
        result = backRef;
        result->next = sortedmerge_alfa(frontRef, backRef->next);
        result->next->prev = backRef;
        backRef->prev = NULL;
    }
    return (result);
}

/**
 * @brief Ordena as sublistas decrescentemente de população, e vai juntando as varias sublistas, recorrendo à recursividade
 * 
 * @param frontRef Lista_Princ *: cabeça de uma das sublistas
 * @param backRef Lista_Princ *: cabeça de uma das sublistas
 * @return Lista_Princ* :retorna uma sublista com dimensao superior à anterior, ordenada alfabeticamente
 */
Lista_Princ *sortedmerge_pop(Lista_Princ *frontRef, Lista_Princ *backRef)
{
    Lista_Princ *result = NULL;

    if (frontRef == NULL)
        return (backRef);
    else if (backRef == NULL)
        return (frontRef);
    //Caso o valor da populaçao for igual em ambos os casos, desempata com a ordenação alfabética
    if (frontRef->population > backRef->population || ((frontRef->population == backRef->population) && strcmp(frontRef->country, backRef->country) < 0))
    {
        result = frontRef;
        result->next = sortedmerge_pop(frontRef->next, backRef);
        result->next->prev = frontRef;
        result->prev = NULL;
    }
    else
    {
        result = backRef;
        result->next = sortedmerge_pop(frontRef, backRef->next);
        result->next->prev = backRef;
        backRef->prev = NULL;
    }
    return (result);
}

/**
 * @brief Ordena as sublistas decrescentemente de infetados numa determinda data, e vai juntando as varias sublistas, recorrendo à recursividade
 * 
 * @param frontRef Lista_Princ *: cabeça de uma das sublistas
 * @param backRef Lista_Princ *: cabeça de uma das sublistas
 * @param semana char*: data
 * @return Lista_Princ*: retorna uma sublista com dimensao superior à anterior, ordenada alfabeticamente
 */
Lista_Princ *sortedmerge_inf(Lista_Princ *frontRef, Lista_Princ *backRef, char *semana)
{
    Lista_Princ *result = NULL;
    Lista_Sec *Aux1, *Aux2;
    char *indicador = "cases";

    if (frontRef == NULL)
        return (backRef);
    else if (backRef == NULL)
        return (frontRef);

    Aux1 = find_entry(frontRef, indicador, semana);
    if (Aux1 == NULL) //caso não encontre a semana assume que é 0
    {
        Aux1 = frontRef->LSec_Head;
        Aux1->weekly_count = 0;
    }
    Aux2 = find_entry(backRef, indicador, semana);
    if (Aux2 == NULL) //caso não encontre a semana assume que é 0
    {
        Aux2 = frontRef->LSec_Head;
        Aux2->weekly_count = 0;
    }
    //ordena alfabeticamente caso os valores sejam iguais
    if (Aux1->weekly_count > Aux2->weekly_count || ((Aux1->weekly_count == Aux2->weekly_count) && strcmp(frontRef->country, backRef->country) < 0))
    {
        result = frontRef;
        result->next = sortedmerge_inf(frontRef->next, backRef, semana);
        result->next->prev = frontRef;
        result->prev = NULL;
    }
    else
    {
        result = backRef;
        result->next = sortedmerge_inf(frontRef, backRef->next, semana);
        result->next->prev = backRef;
        backRef->prev = NULL;
    }
    return (result);
}

/**
 * @brief Ordena as sublistas decrescentemente de mortes numa determinda data, e vai juntando as varias sublistas, recorrendo à recursividade
 * 
 * @param frontRef Lista_Princ *: cabeça de uma das sublistas
 * @param backRef Lista_Princ *: cabeça de uma das sublistas
 * @param semana char*: data
 * @return Lista_Princ* :retorna uma sublista com dimensao superior à anterior, ordenada alfabeticamente
 */
Lista_Princ *sortedmerge_dea(Lista_Princ *frontRef, Lista_Princ *backRef, char *semana)
{
    Lista_Princ *result = NULL;
    Lista_Sec *Aux1, *Aux2;
    char *indicador = "deaths";

    if (frontRef == NULL)
        return (backRef);
    else if (backRef == NULL)
        return (frontRef);

    Aux1 = find_entry(frontRef, indicador, semana);
    if (Aux1 == NULL) //caso não encontre a semana assume que é 0
    {
        Aux1 = frontRef->LSec_Head;
        Aux1->weekly_count = 0;
    }
    Aux2 = find_entry(backRef, indicador, semana);
    if (Aux2 == NULL) //caso não encontre a semana assume que é 0
    {
        Aux2 = frontRef->LSec_Head;
        Aux2->weekly_count = 0;
    }
    //ordena alfabeticamente caso os valores sejam iguais
    if (Aux1->weekly_count > Aux2->weekly_count || ((Aux1->weekly_count == Aux2->weekly_count) && strcmp(frontRef->country, backRef->country) < 0))
    {
        result = frontRef;
        result->next = sortedmerge_dea(frontRef->next, backRef, semana);
        result->next->prev = frontRef;
        result->prev = NULL;
    }
    else
    {
        result = backRef;
        result->next = sortedmerge_dea(frontRef, backRef->next, semana);
        result->next->prev = backRef;
        backRef->prev = NULL;
    }
    return (result);
}

/**
 * @brief separa a lista em duas sublistas com tamanho semelhante, o tamanho difere caso o número de elementos da lista seja ímpar e nesse caso a primeira sublista fica com mais 1 elemento do que a segunda
 * 
 * @param source Lista_Princ *: cabeça da lista inicial
 * @param frontRef Lista_Princ **:  cabeça da primeira sublista
 * @param backRef Lista_Princ **:   cabeça da segunda sublista
 * \return void
 */
void front_back_split(Lista_Princ *source, Lista_Princ **frontRef, Lista_Princ **backRef)
{
    Lista_Princ *fast; //percorre a lista mais rapido do que o slow
    Lista_Princ *slow;
    if (source == NULL || source->next == NULL) //caso a lista tenha 0 ou 1 elementos, atribui o apontador source à cabeça da primeira sublista(frontRef)
    {
        *frontRef = source;
        *backRef = NULL;
    }
    else
    {
        slow = source;
        fast = source->next;
        while (fast != NULL)
        {
            fast = fast->next;
            if (fast != NULL)
            {
                slow = slow->next;
                fast = fast->next;
            }
        }

        *frontRef = source;
        *backRef = slow->next;
        slow->next = NULL;
    }
}

/**
 * @brief Procura o elemento da lista com a data e o indicador especificados
 * 
 * @param node Lista_Principal *:
 * @param indic char *:indicador
 * @param data char *:data
 * @return Lista_Sec* : retorna o ponteiro com a data e o indicador especificados, retorna NULL caso não encontre o elemento da lista referente a essa semana
 */
Lista_Sec *find_entry(Lista_Princ *node, char *indic, char *data)
{
    Lista_Sec *ptr;

    for (ptr = node->LSec_Head; ptr != NULL; ptr = ptr->next)
        if ((strcmp(ptr->indicator, indic) == 0) && (strcmp(ptr->year_week, data) == 0))
            return ptr;
    /* não encontrou */
    return NULL;
}
/*************************************************************************************************************************************************************************************************************/