#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct data{
	int dia, mes, ano;
};

struct artista{
	char nacionalidade[30],nome[30];
};

struct musica{
	char nome[30], genero[20];
	struct data lancamento;
	int duracao;
	struct artista artista;
};

void le_musica( struct musica *p, struct musica *v , int *n);
void mostra_musica( struct musica x );

void le_data( struct data *p );
void mostra_data (struct data x );

void mostra_vetor( struct musica *v, int n );
int busca_musica( struct musica *v, int n, char *nome );

void carrega_arquivo( struct musica **v, int *n, char *nome_arq );
void salva_arquivo( struct musica *v, int n, char *nome_arq );

void remove_registro(struct musica *v, int *n);
int ja_existe( struct musica *v, int n, char nome_da_musica[30]);

void ordena ( struct musica *v, int n);
void busca_artista(struct musica *v,int n, char artista[30]);

void busca_nacionalidade(struct musica *v,int n, char nacionalidade[30]);
void busca_genero(struct musica *v,int n, char genero[30]);

int main(int argc, char *argv[]) {
	int n = 0;  // Quantidade de musicas
	struct musica *v = NULL; // Ponteiro para o vetor de musicas
	char nome[30];
	int indice;
	int criterio;

    carrega_arquivo( &v, &n, "musicas.txt");

	int opcao;
	do{
		system("CLS");
		printf("1-Cadastrar\n2-Remover registros existentes \n3-Listar\n4-Consultar registros\n5-Ordenar \n6-Sair\n");
		scanf("%d", &opcao);

		switch( opcao ){
			case 1:
				n++;
				v = realloc( v, sizeof( struct musica ) * n );
				le_musica( &v[n-1],v,&n );
				system("PAUSE");
				break;

            case 2:
                remove_registro(v,&n);
                n--;
                system("PAUSE");
                break;

			case 3:
				mostra_vetor( v, n );
				system("PAUSE");
				break;

			case 4:
			    printf("\nConsultar por:\n");
			    criterio;
			    printf("1- Nome da musica \n2- Nome do artista \n3- Nacionalidade \n4- Genero\n");
			    scanf("%d",&criterio);

			    switch( criterio ){

                    case 1:

                        printf("Digite o nome da musica a ser buscada: ");
                        scanf(" %[^\n]", nome );

                        indice = busca_musica( v, n, nome );
                        if( indice == -1 )
                            printf("musica nao encotrada!\n");
                        else{
                            printf("%d : ", indice+1 );
                            mostra_musica( v[indice] );
                            }
                        break;

                    case 2:
                        printf("Digite o nome do artista: ");
                        scanf(" %[^\n]", nome );
                        busca_artista(v,n,nome);
                        break;

                    case 3:
                        printf("Digite a nacionalidade: ");
                        scanf(" %[^\n]", nome );
                        busca_nacionalidade(v,n,nome);
                        break;

                    case 4:
                        printf("Digite o genero da musica: ");
                        scanf(" %[^\n]", nome );
                        busca_genero(v,n,nome);
                        break;

			    }

				system("PAUSE");
				break;

            case 5:
                ordena(v,n);
                mostra_vetor(v,n);
                system("PAUSE");
                break;

		}

	}while( opcao != 6 );

    salva_arquivo( v, n, "musicas.txt");

	free( v );

	return 0;
}


void le_musica( struct musica *p, struct musica *v, int *n ){
	char nome_da_musica[30];
	printf("Digite o nome da musica: ");
	scanf(" %[^\n]",nome_da_musica);

	if(ja_existe(v,*n,nome_da_musica)==1){
        printf("Essa musica ja existe.\n");
        (*n)--;
        return;
	}

	strcpy(p->nome,nome_da_musica);
	printf("Digite a duracao da musica: ");
	scanf("%d",&(*p).duracao);
	printf("Digite o nome do artista: ");
	scanf(" %[^\n]",p->artista.nome);
	printf("Digite a nacionalidade do artista: ");
	scanf(" %[^\n]",p->artista.nacionalidade);
	printf("Digite o genero: ");
	scanf(" %[^\n]", p->genero);
	printf("Digite a data de lancamento:\n");
	le_data( &p->lancamento );
}

void mostra_musica( struct musica x ){
	printf("%s, %d, %s, %s, %s ", x.nome,x.duracao, x.artista.nome,x.artista.nacionalidade,x.genero);
	mostra_data( x.lancamento );
	printf("\n");
}

void le_data( struct data *p ){
	printf("Digite o dia: ");
	scanf("%d", &p->dia);
	printf("Digite o mes: ");
	scanf("%d", &p->mes);
	printf("Digite o ano: ");
	scanf("%d", &p->ano);
}

void mostra_data (struct data x ){
	printf("%d/%d/%d", x.dia, x.mes, x.ano);
}

void mostra_vetor( struct musica *v, int n ){
	if( n == 0 )
		printf("Nao ha musicas cadastradas!\n");
	else{
		int i;
		printf("Lista de musicas cadatradas:\n");
		for( i = 0 ; i < n ; i++ ){
			printf("%d : ", i+1 );
			mostra_musica( v[i] );
		}
	}
}

int busca_musica( struct musica *v, int n, char *nome ){
	int i;
	for( i = 0 ; i < n ; i++ )
		if( strcmpi( v[i].nome, nome ) == 0 )
			return i;

	return -1;
}

void carrega_arquivo( struct musica **v, int *n, char *nome_arq ){
	FILE *f = fopen( nome_arq, "rt" );
	if( f == NULL ){
		printf("Arquivo nao encontrado!\n");
		system("PAUSE");
		return;
	}
	fscanf( f, "%d", n );

	*v = malloc( sizeof( struct musica ) * *n );

	int i;
	for( i = 0 ; i < *n ; i++ ){
		fscanf( f, " %[^\n]", (*v)[i].nome );
		fscanf( f, "%d",&(*v)[i].duracao );
		fscanf( f, " %[^\n]", (*v)[i].artista.nome );
		fscanf( f, " %[^\n]", (*v)[i].artista.nacionalidade );
		fscanf( f, " %[^\n]", (*v)[i].genero );
		fscanf( f, "%d %d %d", &(*v)[i].lancamento.dia, &(*v)[i].lancamento.mes, &(*v)[i].lancamento.ano );
	}

	fclose( f );
	printf("%d registros carregados!\n", *n);
	system("PAUSE");
}

//transcreve todo o arquivo de texto
void salva_arquivo( struct musica *v, int n, char *nome_arq ){
	FILE *f = fopen( nome_arq, "wt" );

	fprintf(f, "%d\n", n );

	int i;
	for( i = 0 ; i < n ; i++ ){
		fprintf( f, "%s\n", v[i].nome);
		fprintf( f, "%d\n", v[i].duracao);
		fprintf( f, "%s\n", v[i].artista.nome);
		fprintf( f, "%s\n", v[i].artista.nacionalidade);
		fprintf( f, "%s\n", v[i].genero);
		fprintf( f, "%d %d %d\n", v[i].lancamento.dia, v[i].lancamento.mes, v[i].lancamento.ano);
	}
	fclose( f );
}

void remove_registro(struct musica *v, int *n){

    //recebe o nome da musica
    char nome_da_musica[30];
    printf("Qual o nome da musica: ");
    scanf(" %[^\n]",nome_da_musica);

    //verifica se a musica ja existe, se nao existir volta pro menu
    if(ja_existe(v,*n,nome_da_musica)==0){
        printf("\nMusica nao encontrada, tente novamente.\n\n");
        (*n)++;
        return;

    }

    //cria uma estrutura auxiliar e encontra a posicao do vetor a ser removido
    struct musica aux;
    int localizacao_no_vetor;
    for(int i = 0; i<*n;i++){
        if(strcmp(v[i].nome, nome_da_musica)== 0){
            localizacao_no_vetor = i;
        }
    }

    //faz o ultimo vetor trocar de posicao com o vetor a ser removido
    aux = v[localizacao_no_vetor];
    v[localizacao_no_vetor] = v[(*n)-1];
    v[(*n)-1] = aux;

    //realoca a estrutura de vetores, removendo o ultimo vetor, que eh a ultima musica
    v = realloc(v,sizeof(struct musica)*((*n)-1));

    printf("\nMusica removida com sucesso!\n\n");
}

int ja_existe( struct musica *v, int n, char nome_da_musica[30]){
    for(int i = 0; i<n;i++){
        if(strcmp(v[i].nome,nome_da_musica)== 0){
            return 1;
        }
    }

    return 0;
}

void ordena ( struct musica *v, int n){
    struct musica aux;

    //Ordena o vetor de forma ascendente (menor para o maior)
    for(int i = 0; i<n;i++){
        for(int j = 0; j<n;j++){
            if(strcmp(v[j].nome,v[i].nome)==1){
                aux = v[j]; //aux recebe o maior
                v[j] = v[i]; //a posicao do maior( q ta em primeiro) recebe o valor do menor(q ta em segundo)
                v[i] = aux; // a posicao do menor (q ta em segundo) recebe o maior

            }
        }
    }

}

void busca_artista(struct musica *v,int n, char artista[30]){
    printf("\n");
    for(int i = 0; i<n;i++){
        if(strcmp(v[i].artista.nome,artista)== 0){
            mostra_musica(v[i]);
        }
    }
}

void busca_nacionalidade(struct musica *v,int n, char nacionalidade[30]){
    printf("\n");
    for(int i = 0; i<n;i++){
        if(strcmp(v[i].artista.nacionalidade,nacionalidade)== 0){
            mostra_musica(v[i]);
        }
    }
}

void busca_genero(struct musica *v,int n, char genero[30]){
    printf("\n");
    for(int i = 0; i<n;i++){
        if(strcmp(v[i].genero,genero)== 0){
            mostra_musica(v[i]);
        }
    }
}

