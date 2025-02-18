#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

#define TAM 20

struct lista{
    int caminho[TAM];
    int info;
    struct lista* prox;
};
typedef struct lista Lista;

//Inicializa a Lista com NULL
Lista* inicializa(){
    return NULL;
}

//Verifica se a lista está vazia
int vazia(Lista* recebida){
    if(recebida == NULL)
        return 1;
    else   
        return 0;
}

//Remove o primeiro nó da Lista 
Lista* removePrimeiro(Lista* recebida,int *valor,int *retirado){
    Lista *aux;
    *valor = recebida->info;
    for(int i=0;recebida->caminho[i]!=0;i++)
        retirado[i] = recebida->caminho[i];
    aux = recebida;
    recebida = recebida->prox;
    free(aux);

    return recebida;
}

//Insere um nó na lista de forma ordenada considerando seu custo
Lista* insereListaOrdenada( Lista *recebida, int valor, int *caminho){
    Lista *ordenada;
    Lista *aux=NULL;
    Lista *aux1;
    
    
    ordenada = (Lista*)malloc(sizeof(Lista));
    ordenada->info = valor;
    for(int i=0;i<TAM;i++)
        ordenada->caminho[i] = caminho[i];
    ordenada->prox = NULL;

    aux1 = recebida;

    while((aux1 != NULL)&&(aux1->info<valor)){
        aux = aux1;
        aux1 = aux1->prox;    
    }
    if(aux == NULL){
        ordenada->prox=recebida;
        recebida=ordenada;
    }
    else{
        aux->prox = ordenada;
        ordenada->prox = aux1;
    }
    
    
    return recebida;
}

//Libera todos os nós da lista
void libera(Lista* recebida){
    Lista* aux;
    while(recebida != NULL){
        aux = recebida;
        recebida = recebida->prox;
        free(aux);
    }
    printf("\nElementos da lista liberados\n");

}

//função para inserir elemento no topo da pilha
void push(int p[],int num,int *i){
    if(*i >= TAM)
        printf("Nao pode inserir! A pilha esta cheia\n");
    else{
        p[*i] = num;
        (*i)++;
        
    }
}
//função para retirar elemento do topo da pilha
int pop(int p[],int *i){
    if(*i == 0)
        printf("Nao pode desempilhar! A pilha esta vazia\n");
    else
        return p[--(*i)];
}

//função para verificar se um valor pertence a uma matriz
int pertence(char *valor,char (*cidades)[TAM]){
    for(int i=0;i<TAM;i++){
        if(strcmp(valor,cidades[i])==0){
            return 1;
        }
            
    }
    return 0;
}

//função para acrescentar um inteiro no final de um vetor
void colocaVertice(int *caminhos,int vertice){
    for(int i=0;i<TAM;i++)
        if(caminhos[i]==0){
            caminhos[i] = vertice;
            return;
        }
}

//Função que retorna o ultimo elemento de um vetor
int ultimoElemento(int *caminho){
    for(int i=0;i<TAM;i++){
        if(caminho[i+1]==0)
            return caminho[i];   
    }
}

//Função para atribuir zero a todas posições de um vetor
void zeraVetor(int *caminho){
    for(int i=0;i<TAM;i++)
        caminho[i]=0;
}

//Função para calcular o custo de um caminho feito usando a busca em profundidade
int custoProfundidade(int (*mat)[TAM], char **caminho, char (*cidades)[TAM]){
    int custo=0;
    int ant=0;
    int prox=0;
    for(int i=0;caminho[i+1][0]!=0;i++){
        for(int j=0;j<TAM; j++){
            if(strcmp(caminho[i],cidades[j])==0)
                ant=j;
            if(strcmp(caminho[i+1],cidades[j])==0)
                prox=j;
        }
        custo+=mat[ant][prox];
    }
    return custo;

}

//Busca em profundidade (Depth-first search)
char** dfs(int (*mat)[TAM], char *origem, char *destino,char (*cidades)[TAM]){
    int visitado[TAM] = {};              //vetor para marcar indice dos vertices visitados
    char **caminho;                      //caminho 
    int pilha[TAM],pilhaAux[TAM];        //pilha é a borda, pilhaAux usada para ajudar na construção do caminho
    int start,end,j=0;                   //j é o indice do nosso vetor de strings caminho
    int p=0,pAux=0,aux,aux2;             //pAux e p variaveis de controle de posição das pilhas, aux e aux2 resultado dos pops das pilhas
    int numAdj=0;                        //usado como numero de adjacências de um nó

    caminho = (char**)calloc(TAM,sizeof(char*));
    for(int i=0;i<TAM;i++){
        caminho[i] = (char*)calloc(20,sizeof(char));
    }


    //Pega os índices da origem e do destino
    for(int i = 0;i<TAM;i++){
        if(strcmp(origem,cidades[i])==0)
            start = i;
        if(strcmp(destino,cidades[i])==0)
            end = i;
    }

    
    //Coloca no topo da pilha o nosso nó de origem
    push(pilha,start,&p);

    //Expansão dos nós da pilha e geração do caminho até a pilha ficar vazia ou acharmos o caminho
    while(p!=0 && strcmp(caminho[j],cidades[end])!=0){

        //Pega o no do topo da pilha
        aux = pop(pilha,&p);
        numAdj=0;

        //Marca o nó atual como visitado se ele ainda não foi e também acrescenta ele no caminho
        if(!visitado[aux]){
            visitado[aux]=1;
            strcpy(caminho[j++], cidades[aux]);
        }

        //Faz a expansão dos nós do grafo a partir da profundidade usando uma pilha
        for(int i = 0;i<TAM;i++){
            //printf("mat[%d][%d]!=%d == %d  && !visitado[i]%d\n", start,i,mat[start][i],pilha[p-1],visitado[i]);
            //verifica se o nó atual possui adjacências que ainda não foram visitadas
            //Se sim salva no topo da pilha e se uma das adjacências for o destino, salva no caminho
            if(mat[aux][i]!=0  && !visitado[i]){
                numAdj++;
                if(i == end){
                    strcpy(caminho[j], cidades[i]);
                    break;
                }
                push(pilha,i,&p);
            }
        }

        //Colocamos em uma pilha auxiliar qual nó que possui mais de 1 filho
        //em caso de traceback quando não achar a solução, ele volta nosso caminho até ele
        if(numAdj>=2){
            for(int i=0;i<numAdj;i++)
                push(pilhaAux,aux,&pAux);
        }

        //se o nó que estiver no topo da pilha for uma folha
        //Se chegou aqui significa que não encontrou a solução ainda mas ja explorou um ramo por completo
        //Então concluimos que esse ramo, não faz parte do caminho então tiramos ele do caminho
        if(numAdj==0){
            aux2 = pop(pilhaAux,&pAux);
            //Removemos o nó desse ramo do vetor de caminho, até chegar na bifurcação que gerou esse ramo
            while(strcmp(caminho[j-1],cidades[aux2])!=0){
                caminho[--j][0]=0;
            }
            //verificar se todas as bifurcações daquele nó forem exploradas
            //Se sim, tirar aquele nó do caminho pois não faz parte do caminho solução
            if(strcmp(caminho[j-1],cidades[pilhaAux[pAux-1]])!=0){
                caminho[j--][0]=0;
            }
            
        }


    }
    return caminho;
}

//Busca de Custo Uniforme(Uniform Cost Search)
char** ucs(int (*mat)[TAM], char *origem, char *destino, char (*cidades)[TAM],int *custoFinal){
    //Cria uma lista para armazenar todos os caminhos durante a expansão
    Lista *caminhos = inicializa();
    char **caminhoFinal;
    int visitado[TAM] = {};
    int inicio[1] = {};
    int novoCaminho[TAM] = {};
    int retirado[TAM] = {};
    int custo=0,novoCusto=0;
    int start,end;
    int indice=0;
    int vertice;

    caminhoFinal = (char**)calloc(TAM,sizeof(char*));
    for(int i=0;i<TAM;i++){
        caminhoFinal[i] = (char*)calloc(20,sizeof(char));
    }

    //Pega os índices da origem e do destino
    for(int i = 0;i<TAM;i++){
        if(strcmp(origem,cidades[i])==0)
            start = i;
        if(strcmp(destino,cidades[i])==0)
            end = i;
    }
    
    //Pega o índice da origem e adiciona em um vetor e depois coloca esse caminho inicial na lista de caminhos
    //Sempre salvamos o indice de um vértice somando 1 para nosso vetor nunca possuir um zero nele(nesse caso Arad seria o índice zero)
    //Sendo assim, podemos inicializar o vetor com zeros e sempre sabemos que um zero representa seu fim
    colocaVertice(inicio,start+1);
    caminhos = insereListaOrdenada(caminhos,0,inicio);

    //Faz a expansão dos nós até chegar na solução final
    while(!vazia(caminhos)){
        zeraVetor(retirado);

        //Remove o primeiro caminho da lista de caminhos e salva o seu custo e o caminho retirado nas variaveis custo e retirado
        caminhos = removePrimeiro(caminhos,&custo,retirado);

        //Pega o índice do ultimo vertice do caminho retirado para posteriormente usar na expansão
        vertice = ultimoElemento(retirado);
        indice = vertice-1;

        //Verifica se o indice do ultimo vertice é o destino final
        //Se sim, salva esse caminho em uma variavel alocada dinamicamente e retorna ela para a main
        if(indice==end){
            *custoFinal = custo;
            for(int i=0;retirado[i]!=0;i++)
                strcpy(caminhoFinal[i],cidades[retirado[i]-1]);

            return caminhoFinal;
        }
        
        //Faz a expansão dos nós, sempre pegando os adjacentes a partir do índice que pegamos
        //Usa o caminho que esta no inicio da nossa lista(retirado) e também o último vértice dele(indice)
        //Verifica os nós adjacentes da variável índice
        //Faz a soma dos custos do caminho atual com o da adjacência
        //Cria um novo caminho usando o vetor novoCaminho que é composto pelos vertices da variavel retirado mais o vértice adjacente encontrado
        //Coloca esse caminho na nossa Lista de caminhos de forma ordenada considerando o custo
        //O processo é repetido de modo a criar os diferentes caminhos com todas as adjacências

        for(int i=0;i<TAM;i++){
            if(mat[indice][i]!=0 && !visitado[i]){
                zeraVetor(novoCaminho);
                novoCusto = mat[indice][i] + custo;
                for(int j=0;retirado[j]!=0;j++)
                    colocaVertice(novoCaminho,retirado[j]);

                colocaVertice(novoCaminho,i+1);
                
                caminhos = insereListaOrdenada(caminhos,novoCusto,novoCaminho);

            }

        }
        //Depois da expansõa, marca o vértice expandido como visitado
        visitado[indice] = 1;

    }

    libera(caminhos);
}

int main(){
    //Matriz de adjacências das cidades. (OBS: não está em ordem alfabetica)
    int mat[TAM][TAM] = {
        /*v1*/ 0,75,0,140,118,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        /*v2*/ 75,0,71,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        /*v3*/ 0,71,0,151,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        /*v4*/ 140,0,151,0,0,0,0,0,80,0,99,0,0,0,0,0,0,0,0,0,
        /*v5*/ 118,0,0,0,0,111,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        /*v6*/ 0,0,0,0,111,0,70,0,0,0,0,0,0,0,0,0,0,0,0,0,
        /*v7*/ 0,0,0,0,0,70,0,75,0,0,0,0,0,0,0,0,0,0,0,0,
        /*v8*/ 0,0,0,0,0,0,75,0,0,120,0,0,0,0,0,0,0,0,0,0,
        /*v9*/ 0,0,0,80,0,0,0,0,0,146,0,97,0,0,0,0,0,0,0,0,
        /*v10*/ 0,0,0,0,0,0,0,120,146,0,0,138,0,0,0,0,0,0,0,0,
        /*v11*/ 0,0,0,99,0,0,0,0,0,0,0,0,211,0,0,0,0,0,0,0,
        /*v12*/ 0,0,0,0,0,0,0,0,97,138,0,0,101,0,0,0,0,0,0,0,
        /*v13*/ 0,0,0,0,0,0,0,0,0,0,211,101,0,90,85,0,0,0,0,0,
        /*v14*/ 0,0,0,0,0,0,0,0,0,0,0,0,90,0,0,0,0,0,0,0,
        /*v15*/ 0,0,0,0,0,0,0,0,0,0,0,0,85,0,0,142,0,0,98,0,
        /*v16*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,142,0,92,0,0,0,
        /*v17*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,92,0,87,0,0,
        /*v18*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,87,0,0,0,
        /*v19*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,98,0,0,0,0,86,
        /*v20*/ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,86,0
    };
    char cidades[TAM][20] = {
        "Arad","Zerind","Oradea","Sibiu","Timisoara","Lugoj","Mehadia","Dobreta","RV","Craiova",
        "Fagaras","Pitesti","Bucharest","Giurgiu","Urziceni","Vaslui","Iasi","Neamt","Hirsova","Eforie"
    };
    char **caminho;
    int custo;
    int indice;
    int numNoVisitados;
    char origem[TAM];
    char destino[TAM];
    char buffer[20];
   
    
    while(1){
        printf("\n-----------CIDADES DISPONIVEIS-----------\n");
            for(int i=0;i<TAM;i++)
                printf("\n%s", cidades[i]);

        while(1){
            printf("\n\nInsira a cidade de origem: ");
            fgets(origem, sizeof(origem), stdin);
            printf("\n\nInsira a cidade de destino: ");
            fgets(destino, sizeof(destino), stdin);
     		
     		       
            origem[strcspn(origem,"\n")] = 0;
            destino[strcspn(destino,"\n")] = 0;

            if((pertence(origem,cidades)*pertence(destino,cidades)) && strcmp(origem,destino)!=0){
                break;
            }
            if(strcmp(origem,destino)==0){
                printf("\nOrigem e destino nao podem ser iguais");
            }
            else
                printf("\nUm das cidades nao existe! Certifique-se de inserir como esta na tabela");
        }
        do{
        	printf("\nDigite 1 para Busca em Profundidade e 2 para Busca de Custo Uniforme: ");
        	fgets(buffer, sizeof(buffer), stdin);
    		sscanf(buffer, "%d", &indice);
        }while(indice<1 || indice>2);
        
        if(indice==1){
            caminho = dfs(mat,origem,destino,cidades);
            printf("\n-----CAMINHO USANDO BUSCA EM PROFUNDIDADE-----\n");
            printf("\nCaminho Final: ");
            numNoVisitados=0;
            for(int i = 0;caminho[i][0]!=0;i++){
                numNoVisitados++;
                printf("%s ", caminho[i]);
            }
            custo = custoProfundidade(mat,caminho,cidades);
            printf("\nCusto do caminho: %d", custo);
            printf("\nNumero de nos visitados: %d", numNoVisitados-1);
        }

        if(indice==2){
            caminho = ucs(mat,origem,destino,cidades,&custo);
            printf("\n-----CAMINHO USANDO BUSCA DE CUSTO UNIFORME-----\n");
            printf("\nCaminho Final: ");
            for(int i=0;caminho[i][0]!=0;i++)
                printf("%s ", caminho[i]);
            printf("\nCusto do caminho: %d", custo);
        }
        
        printf("\n\nPressione 0 para sair ou qualquer outro valor para repetir a busca: ");
        fgets(buffer, sizeof(buffer), stdin);  

        if (strcmp(buffer, "0\n") == 0) {  // Compara diretamente
            break;
        }
        
        
    }

    for(int i = 0; i<TAM; i++)
        free(caminho[i]);
    free(caminho);


    return 0;
}
