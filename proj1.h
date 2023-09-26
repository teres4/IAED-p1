#ifndef MAIN_H
#define MAIN_H

#define MAXCARREIRAS 200 /*numero maximo de carreiras*/
#define MAXPARAGENS 10000 /*numero maximo de paragens*/
#define MAXLIGACOES 30000 /*numero maximo de ligacoes*/

#define NOMECARREIRA 21 /*dimensao do nome da carreira*/
#define NOMEPARAGEM 51 /*dimensao do nome da paragem*/

#define TRUE 1 /*verdadeiro*/
#define FALSE 0 /*falso*/

#define ORIGEM 6 /*constantes auxiliares para definir casos*/
#define DESTINO 7
#define CICLO 8


typedef struct{
    char nome[NOMEPARAGEM];
    double latitude;
    double longitude;
} paragem;
        
typedef struct{
    char nome[NOMECARREIRA];
    paragem origem;
    paragem destino; 
} carreira;

typedef struct{
    char carreira[NOMECARREIRA];
    int pos_carreira;
    paragem origem;
    paragem destino;
    float custo;
    float duracao;
} ligacao;


int compara_paragens(paragem paragem1, paragem paragem2);
    
int getNextArg(char str[]);

int lookUpCarreira(char nome[]);

int lookUpParagem(char nome[]);

void criar_carreira(char carreira[]);

int comparacaoPercurso(int i, int j, int descricao);

void caracteristicas_carreira(int icarreira, int print);

void caracteristicas_inverso(int icarreira);

int IsInverse(char inverso[]);

int associacao_ligacao(char carreira[], int idestino, int iorigem);

void copia_paragem(int icarreira, int iparagem, int tipo);

void comand_carreiras();

void comand_ligacoes();

void cria_ligacao(float custo, float duracao, int icarreira, int iorigem, int 
idestino);

void caracteristicas_paragem(int iparagem);

void comand_paragens();

void selectionSort(int arr[], int n);

void interligacao(int iparagem);

void comand_interligacoes();

#endif

