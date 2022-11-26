#include "covid19.h"
/*************************************************************************************** S E L E Ç Ã O  D E  D A D O S ***************************************************************************************/
/**
 * @brief escolhe o tipo de seleção de dados consoante o opção introduzida na linha de comando
 * 
 * @param Head Lista_Princ *: cabeça de lista da lista principal
 * @param tipo_selecao selecao: contem o tipo de selecao introduzida na linha de comando
 * @return void
 */
void selecao_dados(Lista_Princ *Head, selecao tipo_selecao)
{
    if (tipo_selecao == INF)
        select_inf(Head);
    if (tipo_selecao == DEA)
        select_dea(Head);
    if (tipo_selecao == RACIOINF)
        select_racioinf(Head);
    if (tipo_selecao == RACIODEA)
        select_raciodea(Head);
    return;
}

/**
 * @brief Percorre a lista principal e altera a lista secundária de cada país, eliminando todos os elementos da lista secundária que não correspondam ao elemento com maior contagem de infetados
 * 
 * @param head Lista_Princ *:cabeça de lista da lista principal
 * @return void
 */
void select_inf(Lista_Princ *head)
{
    Lista_Princ *Aux;

    while (head != NULL)
    {
        Aux = head;
        head = head->next;
        if (Aux->Mais_inf != NULL)
        {
            free_lsec_wo_selected(Aux, Aux->Mais_inf);          
            Aux->LSec_Head = Aux->Mais_inf;//Coloca o elemento seleccionado na cabeça da lista secundária
            Aux->LSec_Head->next = Aux->LSec_Head->prev = NULL;
        }
    }
}
/**
 * @brief Percorre a lista principal e altera a lista secundária de cada país, eliminando todos os elementos da lista secundária que não correspondam ao elemento com maior contagem de mortes
 * 
 * @param head Lista_Princ *:cabeça de lista da lista principal
 * @return void
 */
void select_dea(Lista_Princ *head)
{
    Lista_Princ *Aux;

    while (head != NULL)
    {
        Aux = head;
        head = head->next;

        if (Aux->Mais_dea != NULL)
        {
            free_lsec_wo_selected(Aux, Aux->Mais_dea);
            Aux->LSec_Head = Aux->Mais_dea;//Coloca o elemento seleccionado na cabeça da lista secundária
            Aux->LSec_Head->next = Aux->LSec_Head->prev = NULL;
        }
    }
}
/**
 * @brief Percorre a lista principal e altera a lista secundária de cada país, eliminando todos os elementos da lista secundária que não correspondam ao elemento com maior racio de infetados
 * 
 * @param head Lista_Princ *:cabeça de lista da lista principal
 * @return void
 */
void select_racioinf(Lista_Princ *head)
{
    Lista_Princ *Aux;

    while (head != NULL)
    {
        Aux = head;
        head = head->next;
        if (Aux->Mais_racioinf != NULL)
        {
            free_lsec_wo_selected(Aux, Aux->Mais_racioinf);
            Aux->LSec_Head = Aux->Mais_racioinf;//Coloca o elemento seleccionado na cabeça da lista secundária
            Aux->LSec_Head->next = Aux->LSec_Head->prev = NULL;
        }
    }
}
/**
 * @brief Percorre a lista principal e altera a lista secundária de cada país, eliminando todos os elementos da lista secundária que não correspondam ao elemento com maior racio de mortes
 * 
 * @param head Lista_Princ *:cabeça de lista da lista principal
 * @return void
 */
void select_raciodea(Lista_Princ *head)
{
    Lista_Princ *Aux;

    while (head != NULL)
    {
        Aux = head;
        head = head->next;
        if (Aux->Mais_raciodea != NULL)
        {
            free_lsec_wo_selected(Aux, Aux->Mais_raciodea);
            Aux->LSec_Head = Aux->Mais_raciodea;//Coloca o elemento seleccionado na cabeça da lista secundária
            Aux->LSec_Head->next = Aux->LSec_Head->prev = NULL;
        }
    }
}

/**
 * @brief Compara o valor de contagem entre ambos os elementos da lista secundaria
 * 
 * @param special_node Lista_Sec *: elemento da lista secundária seleccionado
 * @param ptr Lista_Sec *: novo elemento da lista secundaria
 * @return Lista_Sec*: retorna ptr caso o seu valor de contagem seja superior ao do special_node, caso contrario retorna special_node
 */
Lista_Sec *max_num(Lista_Sec *special_node, Lista_Sec *ptr)
{   //caso o valor de ptr seja superior retorna ptr, no caso de serem iguais retorna a data mais recente
    if ((special_node->weekly_count < ptr->weekly_count) || (special_node->weekly_count == ptr->weekly_count && (strcmp(ptr->year_week, special_node->year_week) > 0)))
        return ptr;

    return special_node;
}

/**
 * @brief Compara o valor de racio entre ambos os elementos da lista secundaria
 * 
 * @param special_node Lista_Sec *: elemento da lista secundária seleccionado com o valor superior de racio
 * @param ptr Lista_Sec *: novo elemento da lista secundaria
 * @return Lista_Sec*: retorna ptr caso o seu valor de racio seja superior ao do special_node, caso contrario retorna special_node
 */
Lista_Sec *max_racio(Lista_Sec *special_node, Lista_Sec *ptr)
{   //caso o valor de ptr seja superior retorna ptr, no caso de serem iguais retorna a data mais recente
    if ((special_node->rate_14_Day < ptr->rate_14_Day) || (special_node->rate_14_Day == ptr->rate_14_Day && (strcmp(ptr->year_week, special_node->year_week) > 0)))
        return ptr;

    return special_node;
}

/**
 * @brief Atualiza os ponteiros para os elementos com os valores superiores de contagem de infetados e de racio de infetados do país
 * 
 * @param ptr Lista_Princ **: nó da lista principal
 * @param ptr1 Lista_Sec *: novo elemento da lista secundária
 * @return void
 */
void atualiza_ptr_dados_inf(Lista_Princ **ptr, Lista_Sec *ptr1)
{
    //contagem de infetados
    if ((*ptr)->Mais_inf == NULL)
        (*ptr)->Mais_inf = ptr1;
    else
        (*ptr)->Mais_inf = max_num((*ptr)->Mais_inf, ptr1);

    //racio de infetados
    if ((*ptr)->Mais_racioinf == NULL)
        (*ptr)->Mais_racioinf = ptr1;
    else
        (*ptr)->Mais_racioinf = max_racio((*ptr)->Mais_racioinf, ptr1);
}

/**
 * @brief Atualiza os ponteiros para os elementos com os valores superiores de contagem de mortes e de racio de mortes do país
 * 
 * @param ptr Lista_Princ **: nó da lista principal
 * @param ptr1  Lista_Sec *: novo elemento da lista secundária
 * @return void
 */
void atualiza_ptr_dados_dea(Lista_Princ **ptr, Lista_Sec *ptr1)
{
    //contagem de mortes
    if ((*ptr)->Mais_dea == NULL)//primeiro elemento da lista
        (*ptr)->Mais_dea = ptr1;
    else
        (*ptr)->Mais_dea = max_num((*ptr)->Mais_dea, ptr1);//compara o ultimo elemento colocado na lista secundária com o elemento seleccionado

    //racio de mortes
    if ((*ptr)->Mais_raciodea == NULL)//primeiro elemento da lista
        (*ptr)->Mais_raciodea = ptr1;
    else
        (*ptr)->Mais_raciodea = max_racio((*ptr)->Mais_raciodea, ptr1);//compara o ultimo elemento colocado na lista secundária com o elemento seleccionado
}

/**
 * @brief Liberta toda a lista secundária do país excepto o elemento seleccionado
 * 
 * @param Aux Lista_Princ *: nó da lista principal
 * @param special_node Lista_Sec *: elemento seleccionado
 * @return void
 */
void free_lsec_wo_selected(Lista_Princ *Aux, Lista_Sec *special_node)
{
    Lista_Sec *Aux_S, *Head_S;
    Aux_S = Head_S = Aux->LSec_Head;//começa na cabeça da lista secundária
    
    while (Aux_S != NULL)
    {
        Head_S = Aux_S->next;
        if (Aux_S != special_node)
            free(Aux_S);
        Aux_S = Head_S;
    }
}
/*************************************************************************************************************************************************************************************************************/
