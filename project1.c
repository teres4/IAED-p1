/* iaed-23 - ist1106009 - project2 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "structs.h"

ligacao _ligacoes[MAXLIGACOES] = {0}; /*vetor de ligações*/
int _n_ligacoes = 0; /*número de ligações existentes*/

paragem _paragens[MAXPARAGENS] = {0}; /*vetor de paragens*/
int _n_paragens = 0; /*número de paragens existentes*/

carreira __carreiras[MAXCARREIRAS] = {0}; /*vetor de carreiras*/
int _n_carreiras = 0; /*número de carreiras existentes*/

int main(){
    int c;
    while ((c = getchar()) != EOF){
        switch(c){
            case 'q':
                return 0;
            case 'c':
                comand_carreiras();
                break;
            case 'p':
                comand_paragens();
                break;
            case 'l':
                comand_ligacoes();
                break;
            case 'i':
                comand_interligacoes();
                break;
        }
    }
    return 0;
}


/*compara duas paragens, 
devolve 1 se forem iguas, 0 caso contrário*/
int compara_paragens(paragem paragem1, paragem paragem2){
    return (paragem1.latitude == paragem2.latitude
    && paragem1.longitude == paragem2.longitude);
}

/*preenche a string dada com input do stdin, ignora espaços brancos no início e retira aspas, devolve 1 se o último carácter for \n, 0 caso contrário*/
int getNextArg(char str[]) {
	int i = 0;
    char c = getchar();
    if(c == '"'){
        c = getchar();
        do{
		str[i++] = c;
		c = getchar();}
        while (c != '"');
        c = getchar();
        str[i] = '\0';
        return (c == '\n');
	}
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
	return (c == '\n');
}

/*pesquisa por uma carreira no vector de carreiras, devolve o seu indice + 1 se for encontrada, 0 caso contrário*/
int lookUpCarreira(char nome[]){
    int i;
    for(i = 0; i < _n_carreiras; i++){
        if(!strcmp(nome, __carreiras[i].nome))
            return i + 1;
    }
    return 0;
}

/*pesquisa por uma paragem no vector de paragens, devolve o seu indice + 1 se for encontrada, 0 caso contrário*/
int lookUpParagem(char nome[]){
    int i;
    for(i = 0; i < _n_paragens; i++){
        if(!strcmp(nome, _paragens[i].nome))
            return i + 1;
    }
    return 0;
}


/*dependendo da descrição que recebe, a função compara paragens de ligações e carreiras*/
int comparacaoPercurso(int i, int j, int descricao){

    /*i é a ligação e j é a carreira ou
    i é a ligação antiga e j é a ligação nova*/

    if(descricao == CICLO)
        return((!strcmp(_ligacoes[i].carreira, _ligacoes[j].
        carreira)) && 
        (compara_paragens(_ligacoes[i].destino, _ligacoes[j].
        origem)));
    if(descricao == ORIGEM)
        return((!strcmp(_ligacoes[i].carreira, __carreiras[j].
        nome)) && 
        (compara_paragens(_ligacoes[i].origem, __carreiras[j].
        origem)));
    if(descricao == DESTINO)
        return((!strcmp(_ligacoes[i].carreira, __carreiras[j].
        nome)) && 
        (compara_paragens(_ligacoes[i].destino, __carreiras[j].
        destino)));
    return FALSE;
}

/*recebe um índice de uma carreira e percorre o seu percurso de ligações, dependendo da descrição imprime as paragens ou as características da carreira*/
void caracteristicas_carreira(int icarreira, int print){

    int  numparagens = 0, fim = TRUE;
    float custo = 0, duracao = 0;
    int i, iorigem = 0;

    for(i = 0; i < _n_ligacoes; i++){
        if(comparacaoPercurso(i, icarreira, ORIGEM)){ 
            /*compara origem de ligações com a origem da carreira e carreira 
            associada */
            if(print){
                printf("%s, ", __carreiras[icarreira].origem.nome);
            }
            iorigem = i; /*nova ligação origem para itenerar*/
            custo += _ligacoes[i].custo;
            duracao += _ligacoes[i].duracao;
            numparagens = 1;
            break;
        }
    }
    while(numparagens && fim){
        for(i = 0; i < _n_ligacoes; i++){
            if(comparacaoPercurso(iorigem, icarreira, DESTINO)){
                /*compara o destino da ligação antiga com o destino da carreira 
                e a carreira da ligação com a carreira atual, indica que 
                se chegou ao fim do percurso*/
                if(print)
                    printf("%s\n", __carreiras[icarreira].destino.nome);
                numparagens++;
                fim = FALSE;
                break;
            }else if(comparacaoPercurso(iorigem, i, CICLO)){
                /*compara o destino da ligação antiga com a origem da ligação 
                nova e a carreira de cada ligação*/
                if(print)
                    printf("%s, ", _ligacoes[i].origem.nome);
                iorigem = i;
                custo += _ligacoes[i].custo;
                duracao += _ligacoes[i].duracao; 
                numparagens++;
            }
        }
    }
        if(!print){
            if(!numparagens){
                printf("%s %d %.2f %.2f\n",__carreiras[icarreira].nome, 
                numparagens, custo, duracao);
            }else{
                printf("%s %s %s %d %.2f %.2f\n", __carreiras[icarreira].
                nome, __carreiras[icarreira].origem.nome, 
                __carreiras[icarreira].destino.nome, numparagens, custo, 
                duracao);
            }
        }
    return;
}

/*devolve 1 se o argumento dado corresponde a uma variação de "inverso", senão devolve 0*/
int IsInverse(char inverso[]){
    int i, len = strlen(inverso);
    for(i = 0; i < len; i++)
        if((inverso[i] != "inverso"[i]) || (len < 3) || (len > 8))
            return FALSE;
    return TRUE;
}


/*recebe um índice de uma carreira no vector e percorre o seu percurso de ligaçṍes na ordem inversa*/
void caracteristicas_inverso(int icarreira){

    int  fim = FALSE;
    int i, iorigem = 0;

    for(i = 0; i < _n_ligacoes; i++){
            if(comparacaoPercurso(i, icarreira, DESTINO)){ 
                /*compara destino de ligações com o destino da carreira e 
                carreira associada*/
                printf("%s, ", __carreiras[icarreira].destino.nome);
                iorigem = i; /*nova ligação para se itenerar*/
                fim = TRUE;
                break;
            }
        }
        while(fim){
            for(i = 0; i < _n_ligacoes; i++){
                if(comparacaoPercurso(iorigem, icarreira, ORIGEM)){
                    /*compara a origem da ligacao antiga com a origem da 
                    carreira e a carreira da ligacao com a carreira atual, 
                    indica que se chegou ao fim do percurso*/
                    printf("%s\n", __carreiras[icarreira].origem.nome);
                    fim = FALSE;
                    break;
                }else if(comparacaoPercurso(i, iorigem, CICLO)){
                    /*compara origem da ligacao antiga com destino da ligacao 
                    nova e a carreira associada a cada*/
                    printf("%s, ", _ligacoes[i].destino.nome);
                    iorigem = i;
                }
            }
        }
    return;
}


/*função que lida com o comando 'c'*/
void comand_carreiras(){

    char carreira[NOMECARREIRA], inverso[8];
    char a;
    int i, icarreira, lastarg;

    a = getchar();

    if ((a == '\n') || (a == EOF) || (a == 0)){ 
        for(i = 0; i < _n_carreiras; i++){
            caracteristicas_carreira(i, FALSE);
        }
        return;
    }
    lastarg = getNextArg(carreira);
    icarreira = lookUpCarreira(carreira);
    if(icarreira--){
        if(lastarg){
            caracteristicas_carreira(icarreira, TRUE);
            return; 
        }else{
            getNextArg(inverso);
            if(IsInverse(inverso)){
                caracteristicas_inverso(icarreira);
                return;
            }else{
                printf("incorrect sort option.\n");
                return;
            }
        }
    }else{
        strcpy(__carreiras[_n_carreiras].nome, carreira);
        _n_carreiras++;
        return;
    }

}

/*devolve descrição do tipo de inserção de paragens numa carreira que é feito quando se cria uma ligação*/
int associacao_ligacao(char carreira[], int idestino, int iorigem){

    int icarreira = lookUpCarreira(carreira);
    --icarreira;
    --idestino;
    --iorigem;

    if (__carreiras[icarreira].origem.nome[0] == '\0' && __carreiras
    [icarreira].destino.nome[0] == '\0')
        return TRUE;
    /*carreira sem paragens*/
        
    if((compara_paragens(__carreiras[icarreira].destino, _paragens
    [iorigem])) && 
    compara_paragens(__carreiras[icarreira].origem, _paragens[idestino]))
    /*carreira circular*/
        return CICLO;
    
    else if(compara_paragens(__carreiras[icarreira].origem, 
    _paragens[idestino]))
    /*ligação adicionada a partir do início*/
        return ORIGEM;
    
    else if(compara_paragens(__carreiras[icarreira].destino, 
    _paragens[iorigem]))
    /*ligação adicionada a partir do fim*/
        return DESTINO;

    return FALSE;
}


/*copia uma paragem para uma carreira de acordo com o tipo recebido*/
void copia_paragem(int icarreira, int iparagem, int tipo){

    if(tipo == ORIGEM){
        strcpy(__carreiras[icarreira].origem.nome, _paragens[iparagem].nome);
        __carreiras[icarreira].origem.latitude = _paragens[iparagem].latitude;
        __carreiras[icarreira].origem.longitude = _paragens[iparagem].longitude;
    }
    if(tipo == DESTINO){
        strcpy(__carreiras[icarreira].destino.nome, _paragens[iparagem].nome);
        __carreiras[icarreira].destino.latitude = _paragens[iparagem].latitude;
        __carreiras[icarreira].destino.longitude = _paragens[iparagem].longitude;
    }
}

/*função que lida com o comando 'l'*/
void comand_ligacoes(){

    char carreira[NOMECARREIRA], origem[NOMEPARAGEM], destino[NOMEPARAGEM];
    float custo, duracao;
    int icarreira, iorigem, idestino, mudanca;

    getNextArg(carreira);
    getNextArg(origem);
    getNextArg(destino);
    if(!scanf("%f %f", &custo, &duracao))
        return;
    if(!(icarreira = lookUpCarreira(carreira))){
        printf("%s: no such line.\n", carreira);
        return;
    }else if(!(iorigem = lookUpParagem(origem))){
        printf("%s: no such stop.\n", origem);
        return;
    }else if(!(idestino = lookUpParagem(destino))){
        printf("%s: no such stop.\n", destino);
        return;
    }else if(!(mudanca = associacao_ligacao(carreira, idestino, iorigem))){
        printf("link cannot be associated with bus line.\n");
        return; 
    }else if((custo < 0) || (duracao < 0)){
        printf("negative cost or duration.\n");
        return;
    }

    cria_ligacao(custo, duracao, --icarreira, --iorigem, --idestino);

    switch(mudanca){
        case(TRUE):
            copia_paragem(icarreira, iorigem, ORIGEM);
            copia_paragem(icarreira, idestino, DESTINO);
            break;
        case(DESTINO):
            copia_paragem(icarreira, idestino, DESTINO);
            break;
        case(ORIGEM):
            copia_paragem(icarreira, iorigem, ORIGEM);
            break;
        case(CICLO):
            copia_paragem(icarreira, idestino, ORIGEM);
            copia_paragem(icarreira, idestino, DESTINO);
            break;
    }
    return;
}


/*cria uma ligação*/
void cria_ligacao(float custo, float duracao, int icarreira, int iorigem, int idestino){

    strcpy(_ligacoes[_n_ligacoes].carreira, __carreiras[icarreira].nome);

    strcpy(_ligacoes[_n_ligacoes].origem.nome, _paragens[iorigem].nome);
    _ligacoes[_n_ligacoes].origem.latitude =  _paragens[iorigem].latitude;
    _ligacoes[_n_ligacoes].origem.longitude =  _paragens[iorigem].longitude;

    strcpy(_ligacoes[_n_ligacoes].destino.nome, _paragens[idestino].nome);
    _ligacoes[_n_ligacoes].destino.latitude =  _paragens[idestino].latitude;
    _ligacoes[_n_ligacoes].destino.longitude =  _paragens[idestino].longitude;
    
    _ligacoes[_n_ligacoes].custo = custo;
    _ligacoes[_n_ligacoes].duracao = duracao;
    
    _ligacoes[_n_ligacoes].pos_carreira = icarreira;

    _n_ligacoes++;
}


/*percorre as ligações e carreiras para determinar o número de carreiras que passam numa paragem e imprime as caracteristicas desta*/
void caracteristicas_paragem(int iparagem){

    int iligacao = 0, ncarreiras = 0, icarreira;

    for(iligacao = 0; iligacao < _n_ligacoes; iligacao++)
        if(compara_paragens(_ligacoes[iligacao].destino, 
        _paragens[iparagem]))
        /*compara destinos de ligações com paragem*/
            ncarreiras++;
    for(icarreira = 0; icarreira < _n_carreiras; icarreira++)
        if((compara_paragens(__carreiras[icarreira].origem, 
        _paragens[iparagem])) && !(compara_paragens(__carreiras
        [icarreira].destino, _paragens[iparagem])))
        /*compara origens de carreiras com paragem*/
            ncarreiras++;
    
    printf("%s: %16.12f %16.12f %d\n",
    _paragens[iparagem].nome, _paragens[iparagem].latitude,
    _paragens[iparagem].longitude, ncarreiras);    
    
}

/*função que lida com o comando 'p'*/
void comand_paragens(){

    int i, iparagem;
    char paragem[NOMEPARAGEM], c;

    double latitude, longitude;

    if (((c = getchar()) == '\n') || (c == EOF)){
        /*lista as paragens*/
        for(i = 0;i < _n_paragens;i++)
            caracteristicas_paragem(i);
        return;
    }
    if(getNextArg(paragem)){
        if((iparagem = lookUpParagem(paragem))){
            iparagem--;
            printf("%16.12f %16.12f\n", _paragens[iparagem].latitude, _paragens[iparagem].longitude);
        }else{
            printf("%s: no such stop.\n", paragem);
        }
    }else{
        if(lookUpParagem(paragem))
            printf("%s: stop already exists.\n", paragem);
        else{
            if(!scanf("%lf %lf", &latitude, &longitude))
                return;
            /*cria paragem*/
            strcpy(_paragens[_n_paragens].nome,paragem);
            _paragens[_n_paragens].latitude = latitude;
            _paragens[_n_paragens].longitude = longitude;
            _n_paragens++;
        }
    }
}


/*função que troca dois elementos num vector*/
void swap(int vec[], int i, int j) {
    int temp = vec[i];

    vec[i] = vec[j];
    vec[j] = temp;
}

/*algoritmo de ordenação selection sort ajustado para ordenar os nomes das carreias a partir dos indices*/
void selectionsort(int vec[], int n) {

    int i, j, imin;
    for (i = 0; i < n-1; i++) {
        imin = i;
        for (j = i+1; j < n; j++) {
            if (strcmp(__carreiras[vec[j]].nome , __carreiras[vec
            [imin]].nome) < 0) {
                imin = j;
            }
        }
        swap(vec, imin, i);
    }
}

/*recebe um indíce de uma paragem e adciona os indices das carreiras a qual passam à paragem a um vetor*/
void interligacao(int iparagem){

    int iligacao, icarreira, ncarreiras = 0, i;
    int carreiras[MAXCARREIRAS];

    for(iligacao = 0; iligacao < _n_ligacoes; iligacao++)
        if(compara_paragens(_ligacoes[iligacao].destino, 
        _paragens[iparagem])){
        /*compara destinos de ligações com paragem*/
            carreiras[ncarreiras] = _ligacoes[iligacao].pos_carreira;
            ncarreiras++;
        }
    for(icarreira = 0; icarreira < _n_carreiras; icarreira++)
        if((compara_paragens(__carreiras[icarreira].origem, 
        _paragens[iparagem])) && !(compara_paragens(__carreiras[icarreira].destino, _paragens[iparagem]))){
        /*compara origens de carreiras com paragem*/
            carreiras[ncarreiras] = icarreira;
            ncarreiras++;
        }
    if(ncarreiras >= 2){
        selectionsort(carreiras, ncarreiras);
        printf("%s %d: " ,_paragens[iparagem].nome, ncarreiras);
        for(i = 0; i < ncarreiras - 1; i++)
            printf("%s ", __carreiras[carreiras[i]].nome);
        printf("%s\n", __carreiras[carreiras[i]].nome);
    }   
}

/*função que lida com o comando 'i'*/
void comand_interligacoes(){ 
    int iparagem;
    for(iparagem = 0;iparagem < _n_paragens; iparagem++)
        interligacao(iparagem);
}

