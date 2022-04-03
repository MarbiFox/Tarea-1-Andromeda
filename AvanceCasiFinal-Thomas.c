#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


typedef struct {
    char name[100]; //Nombre de la Canción.
    char artist[50]; //Nombre del Artista.
    char genre[50]; //Género de la canción.
	char year[5]; //Año de la Canción.    
    char repList[10]; //Lista de Reproducción.
} Cancion;

typedef struct {
    int cant; //Cantidad de Canciones.
    List * Canciones; //Lista de Canciones.
    char * listName; //Nombre de la Lista.jj
} ListaCancion;

//Funciones generales: 

//Imprime la informacion en las busquedas
void informacion(Cancion *);

//Elimina los espacios y cambia las letras de mayuscula a minuscula para hacer una mejor
//mejor comparacion en las busquedas
char * eliminarEspacios(char *);

//Cambia los caracteres de mayusculas a minusculas
int myTolower(int);


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
    while(tmp[pos] != '\0'){
        
        if(tmp[pos] == '\n'){
            tmp[strlen(tmp) - 1] = '\0';
        }
        
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

    //ingresar nombre archivo
    
    printf("\nIngrese el nombre del archivo y su extension (nombre.tipo)\n");
    char archivo[31] = {};
    scanf("%s", archivo);
	
	//Abrir Archivo.
	FILE * fcsv;
	fcsv = fopen (archivo, "r");
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
			printf(" %s -", aux);
            if (i == 0)	strcpy(auxSong->name, aux);
            if (i == 1) strcpy(auxSong->artist, aux);
            if (i == 2) strcpy(auxSong->genre, aux);
            if (i == 3) strcpy(auxSong->year, aux);
            if (i == 4) {
                printf("\n",aux);
                strcpy(auxSong->repList, aux);
            }
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
	char * nameList = auxSong->repList; //Buscar el nombre de la lista.
	pushBack(auxSongList, auxSong); //Agregar a la lista de canciones actual.
	
	int sumSong = 0; //Sumatoria de Canciones
	int i;
	int k = globalList.cant;
	while (sumSong < k) {
		//Recorrer el arreglo en busca de las canciones que pertenecen a la lista.
		int contSongs = 0;
		for (i = 1; i < k; i++) {
			auxSong = nextList(globalList.Canciones);
			if ((strcmp(auxSong->repList, nameList)) == 0) {
				pushBack(auxSongList, auxSong);
				contSongs++;
				//printf("\n-----%s-----", auxSong->name);
			}
		}
		
		//printf("\n-----%d-----", contSongs);
		//printf("\n-----%s-----\n\n", nameList);
		
		//Crear una lista de reproducción.
		ListaCancion * musicList = (ListaCancion *) malloc (sizeof(ListaCancion));
		musicList->Canciones = auxSongList;
		musicList->cant = contSongs;
		musicList->listName = nameList;
		pushBack(musicRepList, musicList); //Agregar a la lista de listas de Reproducción.
		
        //BORRAR
        int cont = 0;
        auxSong = firstList(auxSongList);
        while (auxSong->name != NULL)
        {
            cont++;
            printf("%d------- %s\n", cont, auxSong->name);
            auxSong = nextList(auxSongList); //BORRAR
        }
        cont = 0;

        cleanList(auxSongList);

		//Comprobar si hay más Listas.
		if (musicList->cant == globalList.cant) break;
        if (sumSong == 0) sumSong++; //Contar la Primera Canción.
		sumSong = sumSong + musicList->cant;
		//printf("\n--assa---%d---sasa--", sumSong); BORRAR.

		//Recorrer lista global otra vez hasta hallar la otra lista.
		auxSong = firstList(globalList.Canciones);
		for (int i = 0; i < k; i++) {
			auxSong = nextList(globalList.Canciones);
            //printf("\n\n----%s---%s\n", auxSong->name, auxSong->repList);
			if (strcmp(auxSong->repList, nameList) != 0) {
				strcpy(nameList,auxSong->repList);
				auxSong = firstList(globalList.Canciones);
                break;
			}
		}

        system("pause");
 	}

    //BORRAR 
    /*auxSong = lastList(globalList.Canciones);
    printf("\n----%s---%s", auxSong->name, auxSong->repList);
    auxSong = firstList(globalList.Canciones);
    printf("\n----%s---%s", auxSong->name, auxSong->repList);
    int co = strcmp(auxSong->repList, "Lista 1\n");
    printf("   %d", co);*/
    
	system("pause");
	return musicRepList;
}

void exportarCanciones (ListaCancion listaGlobal) {
	
	//Abrir o Crear el archivo para exportar
	FILE * fout;
	fout = fopen("Canciones.txt", "w+");
	if (fout == NULL) {
		printf("Error al abrir archivo.csv\n");
		exit(1);
	}
	
	//Ver que tiene adentro.
	char * cadena = NULL;
	fgets(cadena, 1023, fout);
	printf("---%s---", cadena);
	
}

void mostrarCanciones (ListaCancion listaGlobal){
	
    //Crear Interfaz.
    printf("MOSTRANDO TODAS LAS CANCIONES...\n\n");
    
    //Verificar si hay más canciones.
    if ( listaGlobal.cant == 0) printf("No se encontraron canciones :( \n\n");
    
    //Mostrar cada una de las canciones.
    Cancion * auxSong = (Cancion*) malloc (sizeof(Cancion));
    auxSong = firstList(listaGlobal.Canciones); //Dirigirse a la primera canción de la lista.
    int cantCampos = 5;
    int cantSongs = listaGlobal.cant;
    for (cantSongs; cantSongs != 0; cantSongs--) {
        //Mostrar los datos de las Canciones.
		printf("%s -", auxSong->name);
        printf(" %s -", auxSong->artist);
        printf(" %s -", auxSong->genre);
		printf(" %s -", auxSong->year);
		printf(" %s\n", auxSong->repList);
		//Moverse a la siguiente canción.
        auxSong = nextList(listaGlobal.Canciones);
    }
    
	//Mensaje de Salida
    if (listaGlobal.cant != 1) printf("Hay un total de %d canciones...", listaGlobal.cant);
    else printf("Hay sólo una canción...", listaGlobal.cant);
    system("pause");
}

/*//Separar y buscar los generos en una cancion para hacer la comparacion
int busquedaEspecial(char *ListaDeGeneros, char *genero){
    int i = 0;
    char *copiaAux = (char *) calloc(41,sizeof(char));

    while (*(ListaDeGeneros + i) != '\0'){
        if()
    }
}

void buscarPorGenero(ListaCancion listGlobal){
    char artistGenre[41] = {};
    printf("\nIngrese el genero de la cancion:\n");
    getchar();
    scanf("%[0-9a-zA-Z ,-]s",artistGenre);

    int cont;
    int i = 0;
    Cancion * aux;

    aux = firstList(listGlobal.Canciones);
    for (cont = 0 ; cont < listGlobal.cant ; cont++){
        if (busquedaEspecial(aux->genre, artistGenre) == 0){
            informacion(aux);
            i++;
        }
        aux = nextList(listGlobal.Canciones);
    }
    if (i == 0){
        printf("\nLo sentimos, no encontramos ninguna cancion con ese nombre en nuestra base :(\n");
    }

}*/

void buscarPorArtista(ListaCancion listGlobal){
    char artistSong[21] = {};
    printf("\nIngrese el artista de la cancion:\n");
    getchar();
    scanf("%[0-9a-zA-Z ,-]s",artistSong);

    int cont;
    int i = 0;
    Cancion * aux = firstList(listGlobal.Canciones);
    char *nameAux = eliminarEspacios(artistSong); 

    for (cont = 0 ; cont < listGlobal.cant ; cont++){
        char *nameArtistAux = eliminarEspacios(aux->artist);
        if (strcmp(nameAux,nameArtistAux) == 0){
            informacion(aux);
            i++;
        }
        aux = nextList(listGlobal.Canciones);
    }
    if (i == 0){
        printf("\nLo sentimos, no encontramos ninguna cancion con ese nombre en nuestra base :(\n");
    }

}

void buscarPorNombre(ListaCancion listGlobal){ 
    char nameSong[31] = {};
    printf("\nIngrese el nombre de la cancion:\n");
    getchar();
    scanf("%[0-9a-zA-Z ,-]s",nameSong);

    int cont;
    int i = 0;

    Cancion * aux = firstList(listGlobal.Canciones);
    char *nameAux = eliminarEspacios(nameSong); 

    for (cont = 0 ; cont < listGlobal.cant ; cont++){
        char *nameArtistAux = eliminarEspacios(aux->name);
        if (strcmp(nameAux,nameArtistAux) == 0){
            informacion(aux);
            i++;
        }
        aux = nextList(listGlobal.Canciones);
    }
    if (i == 0){
        printf("\nLo sentimos, no encontramos ninguna cancion con ese nombre en nuestra base :(\n");
    }
}

void buscarCancion(ListaCancion listGlobal){
    if (listGlobal.cant == 0){
        printf("No hay canciones para poder realizar una busqueda :( \n\n");
        return;
    } 
    system("cls");

    int op;
    while (1){
        printf("\n1. Buscar por nombre de la cancion\n");
        printf("2. Buscar por artista de la cancion\n");
        printf("3. Buscar por genero\n");
        printf("4. Volver al menu\n\n");
        printf("Ingrese su opcion: ");
        scanf("%i", &op);
        switch (op){
            case 1:
                buscarPorNombre(listGlobal);
                break;
            case 2:
                buscarPorArtista(listGlobal);
                break;
            case 3:
                //buscarPorGenero(listGlobal);
            case 4:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opcion ingresada no valida, por favor elija de nuevo\n");
                break;
        }
        if (op == 4) break;
        system("pause");
        system("cls");
    }
}


void EliminarCancion(ListaCancion listaGlobal){
    if (listaGlobal.cant == 0) printf("No se encontraron canciones :( \n\n");

    char yearCmp[5];
    char nameCmp[100];
    char artCmp[50];
    
	//areglar entrada
    printf("\n escriba el nombre de la cancion:");
    scanf("%100s",&nameCmp);


    printf("\n escriba el nombre del el/la artista:");
    scanf("%s",&artCmp);
    puts("");

    printf("\n escriba el año de la cancion:");
    scanf("%d",&yearCmp);
    puts("");

    int cantidad=listaGlobal.cant;
    printf("\n status cantidad:si");

    Cancion * auxSong = (Cancion*) malloc (sizeof(Cancion));
    printf("\n status malloc:si");
    auxSong = firstList(listaGlobal.Canciones); //Dirigirse a la primera canción de la lista.
    printf("\n status first:si");
    
    int eliminacion=0;

    for(int i=0;i<cantidad;i++){
        if(strcmp(auxSong->name,nameCmp) == 0){  
            if(strcmp(auxSong->artist,artCmp) == 0){
                if(strcmp(auxSong->year,yearCmp) == 0){
                    printf("\n--------Se elimino la cancion--------\n");
                    popCurrent(listaGlobal.Canciones);
                    eliminacion=1;
                    break;
                }
            }
        }
        auxSong=nextList(listaGlobal.Canciones);
    }
    if (eliminacion==0) printf("\n--------No se encontro la cancion--------\n");
    system("pause");
}

//El main servirá para dirigir el programa y abrir la interfaz.
int main (){
    int op = 0;
    ListaCancion listaGlobal; //Lista de Canciones.
    listaGlobal.cant = 0;
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
                exportarCanciones(listaGlobal);
                break;
            case 3:
                break;
            case 4:
                buscarCancion(listaGlobal);
                system("pause");
                system("cls");
                break;
            case 5:
		        //EliminarCanciones(listaGlobal);
                break;
            case 6:
                printf("6\n");
                break;
            case 7:
                printf("7\n");
                break;
            case 8:
                EliminarCancion(listaGlobal);
                break;
            case 9:
                printf("Gracias por usar nuestro programa, recuerda no piratear ;)\n");
                system("pause");
                system("cls");
                break;
            default:
				system("cls");
				printf("Tecla ingresada no valida\n");
				system("pause");
                break;
        }
        system("cls");
    }
    system("pause");
    return 0;
}

void informacion(Cancion * aux){
    printf("\n-------------------------------------\n");
    printf("Nombre: %s\n",aux->name);
    printf("Artista: %s\n",aux->artist);
    printf("Genero: %s\n",aux->genre);
    printf("Ano: %s\n",aux->year);
    printf("Lista de Reproduccion: %s\n",aux->repList);
    printf("-------------------------------------\n");
}

int myTolower(int caracter){
    if(caracter > 64 && caracter < 91) return caracter + 32;
    return caracter;
}

char * eliminarEspacios(char  *cadena){
    char *aux = (char *) calloc(31,sizeof(char));
    int k = 0;
    int h = 0;
    while (*(cadena + k) != '\0'){
        if(*(cadena + k) != ' '){
            *(cadena + k) = myTolower(*(cadena + k));
            *(aux + h) = (char) *(cadena + k);
            h++;
        }
        k++;
    }
    return aux;
}