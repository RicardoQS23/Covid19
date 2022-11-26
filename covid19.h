#ifndef COVID19_H_INCLUDED
#define COVID19_H_INCLUDED
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <ctype.h>

#define STR_SIZE 28
#define SGL_SIZE 4
#define DATA_SIZE 8
#define LINE_SIZE 256
#define LISTA_OPCOES "i:L:S:D:P:o:"
#define CABECALHO "country,country_code,continent,population,indicator,weekly_count,year_week,rate_14_day,cumulative_count"
/******************************************************************************************** E S T R U T U R A S ********************************************************************************************/
typedef struct Lista_Sec
{ //Dados variáveis
    char indicator[STR_SIZE];
    int weekly_count;
    char year_week[DATA_SIZE];
    float rate_14_Day;
    int cumulative_count;
    struct Lista_Sec *next,
        *prev;
} Lista_Sec;

typedef struct Lista_Princ
{ //Dados fixos
    char country[STR_SIZE];
    char country_code[SGL_SIZE];
    char continent[STR_SIZE];
    int population;
    //numero de nós da lista secundária do país
    int n_elementos_LS;
    Lista_Sec *LSec_Head, //apontador para a cabeça da lista secundária
        *LSec_Tail;       //apontador para o fim da lista secundária
    //Apontadores para dados de selecao
    Lista_Sec *Mais_inf, //apontador para o elemento da lista secundaria com o maior numero de infetados
        *Mais_dea,       //apontador para o elemento da lista secundaria com o maior numero de mortes
        *Mais_racioinf,  //apontador para o elemento da lista secundaria com o maior racio de infetados
        *Mais_raciodea;  //apontador para o elemento da lista secundaria com o maior racio de mortes
    //apontadores para elementos da lista principal
    struct Lista_Princ *prev, //apontador para o elemento anterior da lista principal
        *next;                //apontador para o proximo elemento da lista principal

} Lista_Princ;

typedef enum tipo_ficheiro
{
    CSV,
    BINARIO
} tipo_ficheiro;

typedef enum _leitura
{
    ALL,
    CONTINENTE
} leitura;

typedef enum _ordenacao
{
    ALFA,
    POPULACAO,
    INFETADOS,
    MORTES
} ordenacao;

typedef enum _selecao
{
    NENHUMA_SEL,
    INF,
    DEA,
    RACIOINF,
    RACIODEA
} selecao;

typedef enum _restricao
{
    NENHUMA_RES,
    MIN,
    MAX,
    DATE,
    DATES
} restricao;

/********************************************************************************************* C A B E Ç A L H O *********************************************************************************************/
void usage(char *programa);
/*Funcoes para leitura de input e escrita de output*/
tipo_ficheiro file_type(char *ficheiro_input);
Lista_Princ *read_data(char *input_file, leitura tipo_leitura, char *cont_input);
Lista_Princ *read_csv_file(FILE *fp, leitura tipo_leitura, char *cont_input);
void dados_invalidos(FILE *fp, Lista_Princ *Head);
void separa_buffer(FILE *fp, Lista_Princ *head, char *buffer_copy, char *pais, char *sigla, char *continente, int *populacao, char *indicador, int *contagem_semanal, char *ano_semana, float *racio, int *contagem_acumulativa);
int contagem_paises(Lista_Princ *Head);
int contagem_semanas(Lista_Princ *ptr);
Lista_Princ *read_binary_file(FILE *fp);
void write_data(char *output_file, Lista_Princ *Head);
void print_output_csv(FILE *fp, Lista_Princ *Head);
void print_output_binary(FILE *fp, Lista_Princ *Head);
//Funcoes de validação de input
bool valida_cont_in(char *cont_input);
int verifica_data(char *data);
int verifica_inteiros(char *token);
int verifica_strings(char *token);
int verifica_float(char *token);
/*Funcoes relacionadas com a manipulacao e des/alocacao de memoria de listas*/
//Funcoes relacionadas com a lista principal
Lista_Princ *gera_pais(char pais[STR_SIZE], char sigla[SGL_SIZE], char continente[STR_SIZE], int populacao);
void coloca_fim_Lpri(Lista_Princ **head, Lista_Princ **tail, Lista_Princ *ptr);
Lista_Princ *must_create_node(Lista_Princ *Head, char pais[STR_SIZE]);
void liberta_lista_pri(Lista_Princ *Head);
//Funcoes relacionadas com a lista secundária
Lista_Sec *gera_lista_sec(char indicador[STR_SIZE], int contagem_semanal, char ano_semana[DATA_SIZE], float racio, int contagem_acumulativa);
void coloca_fim_Lsec(Lista_Princ **ptr, Lista_Sec *ptr_S);
void liberta_lista_sec(Lista_Sec *ptr_S);
/*Funcoes relacionadas com a selecao de dados*/
void selecao_dados(Lista_Princ *Head, selecao tipo_selecao);
void select_inf(Lista_Princ *head);
void select_dea(Lista_Princ *head);
void select_racioinf(Lista_Princ *head);
void select_raciodea(Lista_Princ *head);
Lista_Sec *max_num(Lista_Sec *special_node, Lista_Sec *ptr);
Lista_Sec *max_racio(Lista_Sec *special_node, Lista_Sec *ptr);
void atualiza_ptr_dados_inf(Lista_Princ **ptr, Lista_Sec *ptr1);
void atualiza_ptr_dados_dea(Lista_Princ **ptr, Lista_Sec *ptr1);
void free_lsec_wo_selected(Lista_Princ *Aux, Lista_Sec *special_node);
/*Funcoes relacionadas com a restricao de dados*/
Lista_Princ *restricao_dados(Lista_Princ *Head, restricao tipo_restricao, int n, char *semana, char *semana1, char *semana2);
Lista_Princ *restrict_min(Lista_Princ *head, int n);
Lista_Princ *restrict_max(Lista_Princ *head, int n);
Lista_Princ *restrict_data(Lista_Princ *head, char *Pdata);
Lista_Princ *restrict_datas(Lista_Princ *head, char *Pdata1, char *Pdata2);
void max_data(char *data1, char *data2);
Lista_Princ *remove_from_list_P(Lista_Princ *head, Lista_Princ *ptr);
Lista_Sec *remove_from_list_S(Lista_Sec *head, Lista_Sec *ptr);
/*Funcoes relacionadas com a ordenacao de dados*/
void mergesort(Lista_Princ **Head, ordenacao tipo_ordem, char *semana);
Lista_Princ *sortedmerge_alfa(Lista_Princ *frontRef, Lista_Princ *backRef);
Lista_Princ *sortedmerge_pop(Lista_Princ *frontRef, Lista_Princ *backRef);
Lista_Princ *sortedmerge_inf(Lista_Princ *frontRef, Lista_Princ *backRef, char *semana);
Lista_Princ *sortedmerge_dea(Lista_Princ *frontRef, Lista_Princ *backRef, char *semana);
void front_back_split(Lista_Princ *source, Lista_Princ **frontRef, Lista_Princ **backRef);
Lista_Sec *find_entry(Lista_Princ *node, char *indic, char *data);
#endif
/*************************************************************************************************************************************************************************************************************/