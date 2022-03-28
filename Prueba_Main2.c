#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "list.c"

typedef struct {
    char name[100]; //Nombre de la Canción.
    char artist[50]; //Nombre del Artista.
    char genre[50]; //Género de la canción.
	char year[4]; //Año de la Canción.    
    char repList[10]; //Lista de Reproducción.
} Cancion;

typedef struct {
    int cant; //Cantidad de Canciones.
    List * Canciones; //Lista de Canciones.
    char * listName; //Nombre de la Lista.jj
} ListaCancion;


/** mostrarInterfaz()
 * Se encarga de mostrar la interfaz inicial con el que el usuario
 * interactúa. Retorna un entero para evaluar que acción quiere el
 * usuario que el programa realice. **/

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

//Función para obtener un elemento del string.
const char * getField (char * tmp, int campo) {
    int booleanComillas = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int inicio = 0;
	int pos = 0;
    int final = 0;
    while(tmp[pos]!='\0'){
 
        if(tmp[pos]== '\"'){
            booleanComillas = 1-booleanComillas;
            if(booleanComillas) inicio = pos+1;
            pos++;
            continue;
        }

        if(booleanComillas || tmp[pos]!= ','){
            if(campo==final) ret[pos-inicio] = tmp[pos];
            pos++;
            continue;
        }

        if(tmp[pos]== ','){
            if(campo==final) {
               ret[pos-inicio] = 0;
               return ret;
            }
            final++; inicio = pos+1;
        }

        pos++;
    }

    if(campo==final) {
       ret[pos-inicio] = 0;
       return ret;
    }
    
    return NULL;
}

/** Función directora para importar el archivo .csv con las canciones. **/
ListaCancion importarCanciones () {
	
	//Definir Lista Global de Canciones.
	ListaCancion listaGlobal;
	
	//Abrir Archivo.
	FILE * fcsv;
	fcsv = fopen ("Canciones.csv", "r");
	if (fcsv == NULL) {
		printf("Error al abrir archivo.csv\n");
		return listaGlobal;
	}
	
	//Definir Lista de Canciones.
    List * songsList = createList();
    
    //Crear Datos de Lectura.
    char cadena[1024];
	int i; //Contador
	int contCanciones = 0;
	while (fgets(cadena, 1023, fcsv) != NULL) { //Leerá las cadenas una por una.
    	Cancion * auxSong = (Cancion*) malloc (sizeof(Cancion));
		for (i = 0; i < 5; i++) { //Itera los campos de la cadena.
            const char * aux = getField(cadena, i); // Se obtiene el nombre
			printf(" %s",aux);
            if (i == 0)	strcpy(auxSong->name, aux);
            if (i == 1) strcpy(auxSong->artist, aux);
            if (i == 2) strcpy(auxSong->genre, aux);
            if (i == 3) strcpy(auxSong->year, aux);
            if (i == 4) strcpy(auxSong->repList, aux);
        }
        pushBack(songsList, auxSong);
        contCanciones++;
	}
	printf("\n");
	fclose(fcsv); //Cerrar el Archivo.	
	
	//Inicializar la Lista Global.
	listaGlobal.Canciones = songsList;
	listaGlobal.cant = contCanciones;
	listaGlobal.listName = "Lista Global";
	
	system("pause");
	
	return listaGlobal;
}

List * crearListas (ListaCancion globalList){
	//Crear una Lista de listas de música.
	List * musicRepList = createList(); //Almacena tipo ListaCancion
	
	//Crear una Lista de canciones para asignar.
	List * auxSongList = createList();
	
	//Comprobar que hayan canciones
	if (globalList.cant == 0) return musicRepList;
	
	//Variable auxiliar para encontrar el nombre de la lista.
	Cancion * auxSong = NULL;
	auxSong = firstList(globalList.Canciones);
	printf("%s\n", auxSong->repList);
	char * nameList = auxSong->repList; //Buscar el nombre de la lista.
	pushBack(auxSongList, auxSong); //Agregar a la lista de canciones actual.
	
	int sumSong = 0; //Sumatoria de Canciones
	int i;
	int k = globalList.cant;
	while (sumSong <= k-1) {
		//Recorrer el arreglo en busca de las canciones que pertenecen a la lista.
		int contSongs = 1;
		for (i = 1; i < k; i++) {
			auxSong = nextList(globalList.Canciones);
			if ((strcmp(auxSong->repList, nameList)) == 0) {
				pushBack(auxSongList, auxSong);
				contSongs++;
			}
		}
		
		//Crear una lista de reproducción.
		ListaCancion * musicList = (ListaCancion *) malloc (sizeof(ListaCancion));
		musicList->Canciones = auxSongList;
		musicList->cant = contSongs;
		printf("%d", contSongs);
		musicList->listName = nameList;
		pushBack(musicRepList, musicList); //Agregar a la lista de listas de Reproducción.
		
		//Comprobar si hay más Listas.
		if (musicList->cant == globalList.cant) break;
		sumSong = sumSong + musicList->cant;
		
		//Recorrer lista global otra vez hasta hallar la otra lista.
		auxSong = firstList(globalList.Canciones);
		for (int i = 0; i < globalList.cant; i++) {
			auxSong = nextList(globalList.Canciones);
			if (strcmp(auxSong->repList, nameList) != 1) {
				nameList = auxSong->repList;
				auxSong = firstList(globalList.Canciones);
			}
		}
 	}
	system("pause");
	return musicRepList;
}


//El main servirá para dirigir el programa y abrir la interfaz.
int main () {
    int op = 0;
    ListaCancion listaGlobal; //Lista de Canciones.
    List * listasMusica = NULL; //Listas de reproducción actuales.
	//Ejecutar una acción según el resultado obtenido desde la interfaz.
    while (op != 9){
        op = mostrarInterfaz(op);
        switch (op) {
            case 1:
				listaGlobal = importarCanciones();
				listasMusica = crearListas(listaGlobal);
                break;
            case 2:
                printf("2\n");
                break;
            case 3:
				printf("3\n");
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
        system("cls");
    }
    system("pause");
    /* Se puede hacer lo mismo con system("pause");
    scanf("%d", &op); // COMENTARIO: Esto de acá es sólo para que no se me cierre el programa en 2 milisegundos :)
    */
    return 0;
}