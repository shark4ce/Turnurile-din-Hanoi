#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct celula
{
	void* info;
	struct celula *urm;
}TCelula,*TLista;

typedef struct 
{
	char nume_turn;
	TLista varf;
}Stiva,*TStiva;

typedef struct 
{
	TLista inceput;
}coada,*TCoada;

typedef struct
{
	char sursa,destinatie;
}tcc,*TCC;

typedef struct
{
	char *culoare;
	int numar_discuri;
	TStiva a,b,c;
	TCoada coada;
}SH,*TSH;

void free_cel_list (TLista *L){
	TLista aux = NULL;

	while(*L){
		aux = *L;
		*L = aux->urm;
		free(aux->info);
		free(aux);
	}
}

void free_stack(TStiva s){
	if( s == NULL) return ;
	free_cel_list(&s->varf);
	free(s);
}
void free_heap(TCoada c){
	if( c == NULL) return ;
	free_cel_list(&c->inceput);
	free(c);
}

void free_systemH(TSH sh){
	if( sh == NULL ) return ;
	free_stack(sh->a);
	free_stack(sh->b);
	free_stack(sh->c);
	free_heap(sh->coada);
	free(sh->culoare);
	free(sh);
}

void free_systems(TLista *L){
	TLista aux = NULL;

	while(*L){
		aux = *L;
		*L = aux->urm;
		free_systemH(((TSH)aux->info));
		free(aux);
	}
}

TLista Push(TLista *L, TLista aux){
	//Introducem elemente in lista, precum si stiva
		if(*L == NULL)
		{
			*L = aux;
			(*L)->urm = NULL;
		}else{ 
			aux->urm = *L;
			*L = aux;
		}
return *L;
}

int check_stack(TStiva s){
	//verificam daca stiva e goala sau nu
	if(s->varf == NULL) return 0;
	else return 1;
}
TLista Pop(TStiva s){
	//scoatem
 	if(check_stack(s) == 0) return NULL;	
	TLista temp = NULL;
	temp = s->varf;
	s->varf = s->varf->urm;
	return temp;
}

int check_heap(TCoada c){
	if(c->inceput == NULL) return 0;
	else return 1;
}

TLista alloc_cel_list(void* data){
	TLista aux = NULL;
	aux = (TLista)malloc(sizeof(TCelula));
	if(!aux) return NULL;
	aux->info = data;
	aux->urm = NULL;
	return aux;
}

TStiva allocM_stack(){
	TStiva aux = NULL;
	aux = (TStiva)malloc(sizeof(Stiva));
	if(!aux) return NULL;
	aux->varf = NULL;
	return aux;
}

TCoada allocM_heap(){
	TCoada aux = NULL;
	aux = (TCoada)malloc(sizeof(coada));
	if( !aux ) return NULL;
	aux->inceput = NULL;
	return aux;
}

TSH allocM_system(char* info_c){
	TSH aux = NULL;

	aux = (TSH)malloc(sizeof(SH));
	if( !aux ) return NULL;
	aux->culoare = (char*)malloc(30*sizeof(char));
	aux->culoare = info_c ;
	aux->numar_discuri = 0;
	aux->b = allocM_stack();
	aux->b->nume_turn = 'B';
	aux->c = allocM_stack();
	aux->c->nume_turn = 'C';
	aux->a = allocM_stack();
	aux->a->nume_turn = 'A';
	aux->coada = allocM_heap();
	return aux;
}

TStiva insert_ordonat(TStiva s, TLista aux){
	if(s->varf == NULL)
	{
		s->varf = aux;
		s->varf->urm = NULL;
	}else{
		if(*((int*)aux->info) < *((int*)s->varf->info))
		{
			aux->urm = s->varf;
			s->varf = aux;
		}else{
			TLista temp = NULL;
			TStiva aux1 = NULL;
			aux1 = allocM_stack();
			while(check_stack(s) != 0)
			{
				temp = Pop(s);

				if(*((int*)aux->info) < *((int*)temp->info))
				{
					Push(&s->varf,temp);
					Push(&s->varf,aux);
					break;	

				}else{					
					
					Push(&aux1->varf,temp);
					if(check_stack(s) == 0)
					{
						Push(&s->varf,aux);
						break;
					}
				}
			}
			while(check_stack(aux1) != 0)
			{
					temp = Pop(aux1);
					Push(&s->varf,temp);
			}
		}
	}
	return s;
}


TStiva init_stack(TStiva s,void* data){
	//introducem datale in lista
	TLista aux = NULL;
	aux = alloc_cel_list(data);
	s = insert_ordonat(s,aux);
return s;
}

void print_stack(TStiva s,FILE* fd){
	
	TStiva aux = NULL;
	TLista temp = NULL;
	aux = allocM_stack();
	while (check_stack(s) != 0){
		temp = Pop(s);
		Push(&aux->varf,temp);
	}
	//bagam elementele inapoi in lista
	while( check_stack(aux) != 0){
		temp = Pop(aux);
		fprintf(fd," %d", *((int*)temp->info));
		Push(&s->varf,temp);
	}
	//dezalocam stiva aux
	free_stack(aux);
}

TLista init_stack_cel_systemH(char* info_c){
	TLista aux = NULL;
	TSH sis = NULL;
	sis = allocM_system(info_c);
	aux = alloc_cel_list(sis);
	return aux;
}

TSH find_color(TLista *sisteme,char* info_c){
	//gasim sistemult potrivit dupa culoare
	TLista aux = NULL;
	for(aux = *sisteme; aux != NULL;aux = aux->urm ){
		if(strcmp(((TSH)aux->info)->culoare,info_c) == 0) return ((TSH)aux->info);
	}
	return NULL;
}


void show_culoare(TLista sisteme,char* info_c,FILE *fd ){
	TSH aux = NULL;
	aux = find_color(&sisteme,info_c);
	if(aux == NULL)
	{
		fprintf(fd,"A_%s:",info_c);
		fprintf(fd,"\n");
		fprintf(fd,"B_%s:",info_c);
		fprintf(fd,"\n");
		fprintf(fd,"C_%s:",info_c);
		fprintf(fd,"\n");
		return;
	} 
	fprintf(fd,"A_%s:",info_c);
	print_stack(aux->a,fd);
	fprintf(fd,"\n");

	fprintf(fd,"B_%s:",info_c);
	print_stack(aux->b,fd);
	fprintf(fd,"\n");

	fprintf(fd,"C_%s:",info_c);
	print_stack(aux->c,fd);
	fprintf(fd,"\n");
}

TLista add_new_systemH(TLista *sisteme,char *info_c,void* data){
	TSH aux = NULL;
	TLista temp = NULL, u =NULL, temp1 = NULL;
	if(*sisteme == NULL)
	{
		temp = init_stack_cel_systemH(info_c);
		((TSH)temp->info)->a = init_stack(((TSH)temp->info)->a,data);
		((TSH)temp->info)->numar_discuri++;
		*sisteme = temp;

	}else{
		aux = find_color(sisteme,info_c);
		if(aux != NULL)
		{
			((TSH)aux)->numar_discuri++;
			init_stack(aux->a,data);
		}else{
			for(temp1 = *sisteme; temp1 != NULL; u = temp1, temp1 = temp1->urm);
				temp = init_stack_cel_systemH(info_c);
				((TSH)temp->info)->numar_discuri++;
				((TSH)temp->info)->a = init_stack(((TSH)temp->info)->a,data);
				u->urm = temp;
		}
	}
	return *sisteme;
}

TCoada calcul_algTower(int a, char source, char destination , char auxiliar,TCoada lista_coada){
	//a-sursa , b-auxiliar, c=destinatie
	// calculare recursiv a miscarilor precum si introducerea acestora intr-o coada
	if ( 0 == a ) return NULL ;
	calcul_algTower(a-1,source,auxiliar,destination,lista_coada);
	
	TCC aux = NULL;
	aux = (TCC)malloc(sizeof(tcc));
	aux->sursa = source;
	aux->destinatie = destination;
	TLista temp = NULL,last = NULL,temp1 = NULL;
	temp = alloc_cel_list(aux);
	
	if(lista_coada->inceput == NULL)
	{
		lista_coada->inceput = temp;

	}else{
		for(temp1 = lista_coada->inceput; temp1 != NULL; last = temp1, temp1 = temp1->urm);
		last->urm = temp;
	}
	calcul_algTower(a-1,auxiliar,destination,source,lista_coada);
	return lista_coada;
}

TLista Extrq(TCoada c){
	//extragere element din coada
	if(check_heap(c) == 0) return NULL;
	TLista temp = NULL;
	temp = c->inceput;
	c->inceput = c->inceput->urm;
	return temp;
}

TCoada calculate_moves(TLista *sisteme, char *info_c){
	TSH aux = NULL;
	TCoada c = NULL;
	aux = find_color(sisteme,info_c);
	if( aux == NULL) return NULL;
	if( aux->coada->inceput != NULL ) return aux->coada;
	else if(aux->a->varf == NULL && aux->b->varf == NULL) return NULL;
	else c = calcul_algTower(aux->numar_discuri,aux->a->nume_turn,aux->c->nume_turn,aux->b->nume_turn,aux->coada);
	return c;
}

void print_moves(TCoada c,int nr_mutari,char* info_c,FILE* fd){
	TCoada aux;
	TLista temp = NULL;
	int i = 0;
	aux = allocM_heap();
	fprintf(fd,"M_%s:", info_c);
	if(c != NULL){
		while(c->inceput != NULL)
		{
			if(i == nr_mutari) break;
			temp = Extrq(c);
			fprintf(fd," %c->%c",((TCC)temp->info)->sursa,((TCC)temp->info)->destinatie);
			Push(&aux->inceput,temp);
			i++;
		}
		while(aux->inceput != NULL)
		{
			temp = Extrq(aux);
			Push(&c->inceput,temp);
		}
		free_heap(aux);
	}
	fprintf(fd,"\n");
}

void show_movess(TLista sisteme,char *info_c,int nr_mutari,FILE *fd){
	TCoada c = NULL;
	c = calculate_moves(&sisteme,info_c);
	print_moves(c,nr_mutari,info_c,fd);
}

void play(TLista *sisteme,char *info_c, int nr_mutari){
	TSH s = NULL;
	int i,nr_mutari_permise = 0;
	TCoada c = NULL;
	TLista aux = NULL,temp = NULL;
	s = find_color(sisteme,info_c);
	c = calculate_moves(sisteme,info_c);
	nr_mutari_permise = (2<<(s->numar_discuri-1)) - 1;
	if(nr_mutari > nr_mutari_permise)
	{
		nr_mutari = nr_mutari_permise;
	}  
	for(aux = c->inceput, i = 0; aux != NULL ; aux = aux->urm, i++){
		if( i == nr_mutari) break;
		if(((TCC)aux->info)->sursa == 'A' && ((TCC)aux->info)->destinatie == 'B'){
			temp = Pop(s->a);
			Push(&s->b->varf,temp);
		}else if(((TCC)aux->info)->sursa == 'A' && ((TCC)aux->info)->destinatie == 'C'){
			temp = Pop(s->a);
			Push(&s->c->varf,temp);
		}else if(((TCC)aux->info)->sursa == 'B' && ((TCC)aux->info)->destinatie == 'A'){
			temp = Pop(s->b);
			Push(&s->a->varf,temp);
		}else if(((TCC)aux->info)->sursa == 'B' && ((TCC)aux->info)->destinatie == 'C'){
			temp = Pop(s->b);
			Push(&s->c->varf,temp);
		}else if(((TCC)aux->info)->sursa == 'C' && ((TCC)aux->info)->destinatie == 'A'){
			temp = Pop(s->c);
			Push(&s->a->varf,temp);
		}else if(((TCC)aux->info)->sursa == 'C' && ((TCC)aux->info)->destinatie == 'B'){
			temp = Pop(s->c);
			Push(&s->b->varf,temp);
		}
	}
	for( i = 0; i < nr_mutari; i++){
		temp = Extrq(c);
		//eliberare elemente
	}
}

int main(int argc, char **argv)
{
	TLista sisteme = NULL;
	int nr_operatii,nr_mutari;
	char *cmd = NULL;
	int *data = NULL;
	FILE *ft;
	ft = fopen(argv[1],"r");
	FILE *fd;
	fd = fopen(argv[2],"a");
	while(fscanf(ft,"%d",&nr_operatii) != EOF)
	{
		cmd = (char*)malloc(30*sizeof(char));
		fscanf(ft,"%s",cmd);
		if( strcmp(cmd,"add") == 0)
		{
			fscanf(ft,"%s",cmd);
			data = (int*)malloc(sizeof(int));
			fscanf(ft,"%d",data);
			sisteme = add_new_systemH(&sisteme,cmd,data);
		}
		if (strcmp(cmd,"show") == 0)
		{
			fscanf(ft,"%s",cmd);
			show_culoare(sisteme,cmd,fd);
			fseek(fd,1,SEEK_END);
		}
		if(strcmp(cmd,"show_moves") == 0)
		{
			fscanf(ft,"%s",cmd);
			fscanf(ft,"%d",&nr_mutari);
			show_movess(sisteme,cmd,nr_mutari,fd);
			fseek(fd,1,SEEK_END);
		}
		if(strcmp(cmd,"play") == 0)
		{
			fscanf(ft,"%s",cmd);
			fscanf(ft,"%d",&nr_mutari);
			play(&sisteme,cmd,nr_mutari);
		}
	}
	fclose(fd);
	fclose(ft);
	free(cmd);
	free_systems(&sisteme);
return 0;
}