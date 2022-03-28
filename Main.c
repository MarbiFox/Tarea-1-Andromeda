#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct {
    char name[100]; //Nombre de la Canción.
    char artist[100]; //Nombre del Artista.
    char genre[100]; //Género de la canción.
    int year; //Año de la Canción.
    char repList[100]; //Lista de Reproducción.
} Cancion;

typedef struct {
    int cant; //Cantidad de Canciones.
    Cancion * arrCanciones; //Lista de Canciones.
    char * listName; //Nombre de la Lista.jj
} ListaCancion;

//Funcion para leer el k-esimo elemento de un string (separado por comas)
const char *getField (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }
    return NULL;
}


int agregarCancion(){
    //ABRIR ARCHIVO
    FILE* playList = fopen("Canciones.csv","r");
    if(playList == NULL){
        printf("No se ha podido leer el archivo\n");
        return EXIT_FAILURE;
    }
    char linea[1024];
    int i;

    List *Musica = createList();
    Cancion *ptr = (Cancion*)malloc(sizeof(Cancion));

    while(fgets(linea,1023,playList) != NULL){
        for (i = 0 ; i < 5 ; i++)
        {
            const char *aux = getField(linea,i);
            if (i == 0){
                strcpy(ptr->name,aux);
            }
            if (i == 1){
                strcpy(ptr->artist,aux);
            }
            if (i == 2){
                strcpy(ptr->genre,aux);
            }
            if (i == 3){
                ptr->year = atoi(aux);
            }
            if (i == 4){
                strcpy(ptr->repList,aux);
            }
            //printf("%s ",aux);

        }
        pushBack(Musica,ptr);
        ptr = (Cancion*)malloc(sizeof(Cancion));
        printf("\n");
    }

    printf("------LEER CANCIONES Y SUS DATOS RESPECTIVOS------\n\n");
    Cancion *auxiliar = firstList(Musica);
    while(auxiliar != NULL){
        printf("%s %s %s %d %s\n",auxiliar->name,auxiliar->artist,auxiliar->genre,auxiliar->year,auxiliar->repList);
        auxiliar = nextList(Musica);
    }

    printf("\n-----PROBANDO LISTA-----\n");
    printf("[ENUMERAR CANCIONES]\n");
    auxiliar = firstList(Musica);

    int k = 1;
    while (auxiliar != NULL)
    {
        printf("Cancion numero %d: %s\n",k,auxiliar->name);
        auxiliar = nextList(Musica);
        k++;
    }

    return 0;
}


/* Función Mostrar Interfaz */
/*Se encarga de mostrar la interfaz inicial con el que el usuario
interactúa. Retorna un entero para evaluar que acción quiere el
usuario que el programa realice.*/

int mostrarInterfaz (int num) {

    printf(" ¡Bienvenido al Programa!\n");
    printf(" ¿Que accion desea implementar hoy?\n\n");
    printf("(1) Importar canciones\n");
    printf("(2) Exportar canciones\n");
    printf("(3) Agregar cancion\n");
    printf("(4) Buscar cancion\n");
    printf("(5) Eliminar cancion\n");
    printf("(6) Mostrar listas de reproducciones\n");
    printf("(7) Mostrar una lista de reproduccion\n");
    printf("(8) Mostrar todas las canciones\n");
    printf("(9) Finalizar programas\n\n");

    printf("Ingrese su opcion: ");
    scanf("%d", &num);
    return num;
}

//El main servirá para dirigir el programa y abrir la interfaz.
int main () {
    int op = 0;
    //Ejecutar una acción según el resultado obtenido desde la interfaz.
    while (op != 9){
        op = mostrarInterfaz(op);
        switch (op) {
            case 1:
                printf("1\n");
                break;
            case 2:
                printf("2\n");
                break;
            case 3:
                agregarCancion();
                break;
            case 4:
                printf("4\n");
                break;
            case 5:
                printf("5\n");
                break;
            case 6:
                printf("6\n");
                break;
            case 7:
                printf("7\n");
                break;
            case 8:
                printf("8\n");
                break;
            case 9:
                printf("Gracias por usar nuestro programa, recuerda no piratear ;)\n");
                break;
            default:
                printf("Tecla ingresada no valida\n");
                break;
        }
    }
   
    return 0;
}