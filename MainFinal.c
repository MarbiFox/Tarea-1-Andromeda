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
    char * listName; //Nombre de la Lista.
} ListaCancion;

/*******************************Funciones generales:*************************************/

//Imprime la informacion en las busquedas
void informacion(Cancion *);

//Elimina los espacios y cambia las letras de mayuscula a minuscula para hacer una mejor
//mejor comparacion en las busquedas
char * eliminarEspacios(char *);

//Cambia los caracteres de mayusculas a minusculas
int myTolower(int);
/****************************************************************************************/

/** mostrarInterfaz()
 * Se encarga de mostrar la interfaz inicial con el que el usuario
 * interactúa. Retorna un entero para evaluar que acción quiere el
 * usuario que el programa realice. **/

int mostrarInterfaz (int num) {

    printf("===========================================\n");
	printf("---------- Bienvenido al Programa ---------\n");
	printf("===========================================\n\n");
    printf("Por favor, eliga la operacion a realizar...\n\n");
    printf("(1) Importar canciones\n");
    printf("(2) Exportar canciones\n");
    printf("(3) Agregar cancion\n");
    printf("(4) Buscar cancion\n");
    printf("(5) Eliminar cancion\n");
    printf("(6) Mostrar listas de reproducciones\n");
    printf("(7) Mostrar una lista de reproduccion\n");
    printf("(8) Mostrar todas las canciones\n");
    printf("(9) Finalizar programas\n\n");
    printf("===========================================\n\n");
    printf("Ingrese su opcion: ");
    scanf("%d", &num);
    return num;
}

//Función para obtener un elemento del string.
char * getField (char * tmp, int campo) {
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

        if(tmp[pos] == ','){
            if(campo==final) {
               ret[pos-inicio] = 0;
               return ret;
            }
            final++;
			inicio = pos+1;
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
	
	//Especificar Nombre del Archivo.
	char fichero[50] = {};
	printf("Ingrese el nombre del archivo (NO INCLUYA .CSV en el nombre): ");
	getchar();
	scanf("%[^\n]s", fichero);
	strcat(fichero, ".csv"); 
	
	//Abrir Archivo.
	FILE * fcsv;
	fcsv = fopen (fichero, "r");
	if (fcsv == NULL) {
		printf("Error al abrir archivo.csv\n");
		exit(1);
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
			char * aux = getField(cadena, i); // Se obtiene el nombre
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

	//Mensaje de Salida
	printf("Se ha importado el archivo con exito!\n");
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
	int contSongs = 1; //Caso inicial.
	while (sumSong < k) {
		//Recorrer el arreglo en busca de las canciones que pertenecen a la lista.
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
		musicList->listName = nameList;
		pushBack(musicRepList, musicList); //Agregar a la lista de listas de Reproducción.

		//Limpiar Lista.
        contSongs = 0;

		//Comprobar si hay más Listas.
		if (musicList->cant == globalList.cant) break;
        if (sumSong == 0) sumSong++; //Contar la Primera Canción.
		sumSong = sumSong + musicList->cant;

		//Recorrer lista global otra vez hasta hallar la otra lista.
		auxSong = firstList(globalList.Canciones);
		for (int i = 0; i < k; i++) {
			auxSong = nextList(globalList.Canciones);
			if (strcmp(auxSong->repList, nameList) != 0) {
				nameList = auxSong->repList;
				auxSong = firstList(globalList.Canciones);

                break;
			}
		}
		auxSongList = createList();
 	}

	//system("pause");
	return musicRepList;
}

void exportarCanciones (ListaCancion listaGlobal) {
	if (listGlobal.cant == 0){
		printf("No se encontraron datos para exportar :( \n\n");
		system("pause");
        return;
    } 
	
	//Especificar Nombre del Archivo.
	char fichero[50] = {};
	printf("Ingrese el nombre del archivo (NO INCLUYA .CSV en el nombre): ");
	getchar();
	scanf("%[^\n]s", fichero);
	strcat(fichero, ".csv"); 
	
	//Abrir o Crear el archivo para exportar
	FILE * fout;
	fout = fopen(fichero, "w");
	if (fout == NULL) {
		printf("Error al abrir archivo.csv\n");
		exit(1);
	}

	//Colocar las canciones de la lista Global dentro del archivo.
	int contCanciones = 0;
    Cancion * auxSong = (Cancion*) malloc (sizeof(Cancion));
    while (contCanciones < listaGlobal.cant) {

        if (contCanciones == 0) {
            auxSong = firstList(listaGlobal.Canciones);
        }
        else {
            auxSong = nextList(listaGlobal.Canciones);
        }

        //Colocar el string en el archivo csv.
        if (strchr(auxSong->genre, ',') != 0){
        	fprintf(fout, "%s,%s,\"%s\",%s,%s\n", auxSong->name, auxSong->artist, auxSong->genre, auxSong->year, auxSong->repList);
		}
		else {
			fprintf(fout, "%s,%s,%s,%s,%s\n", auxSong->name, auxSong->artist, auxSong->genre, auxSong->year, auxSong->repList);
		}

        //Reiniciar ciclo
        contCanciones++;
    }
    
    printf("\nSe ha exportado el archivo con exito!\n");
    
    system("pause");

    fclose(fout);

}

void agregarCancion(ListaCancion *listaGlobal, List *ListasMusica, int cont){
    
	printf("\n===========================================\n");
	
	//Crear una canción.
	Cancion *cancion_agregar = (Cancion*)malloc(sizeof(Cancion));
	
    char name[40];
    printf("INGRESE NOMBRE: ");
    scanf(" %[^\n]s",name);
    strcpy(cancion_agregar->name,name);

    char artist[40];
    printf("INGRESE ARTISTA: ");
    scanf(" %[^\n]s",artist);
    strcpy(cancion_agregar->artist,artist);

    char genre[40];
    printf("INGRESE GENERO: ");
    scanf(" %[^\n]s",genre);
    strcpy(cancion_agregar->genre,genre);

    char year[40];
    printf("INGRESE ANO: ");
    scanf("%s",year);
    strcpy(cancion_agregar->year,year);

    char repList[40];
    printf("INGRESE LISTA DE REPRODUCCION: ");
    scanf(" %[^\n]s",repList);
    strcpy(cancion_agregar->repList,repList);
        
	//Verificar si se han importando listas de reproducción.
	if (cont == 1){

        ListaCancion *auxList = firstList(ListasMusica);
        Cancion *aux = firstList(auxList->Canciones);
        
        while (auxList != NULL){
            if (strcmp (auxList->listName, repList) == 0){
            	//Búsqueda con Bool
            	int var = 0;
                while (aux != NULL){
					if (strcmp(aux->name, name) == 0) {
                    	if (strcmp(eliminarEspacios(aux->artist), eliminarEspacios(artist)) == 0) {
                    		if (strcmp(eliminarEspacios(aux->genre), eliminarEspacios(genre)) == 0) {
                    			if (strcmp(eliminarEspacios(aux->year), eliminarEspacios(year)) == 0) {
                    				printf("\n---------[LA CANCION YA EXISTE EN ESTA LISTA]----------\n");
                    				system("pause");
                    				return;
								}
							}
						}
                    }
                    else {
                    	aux = nextList(auxList->Canciones); 
                    }
                }
                pushBack(auxList->Canciones,cancion_agregar);
                auxList->cant++;
                pushBack(listaGlobal->Canciones,cancion_agregar);
                listaGlobal->cant++;
                printf("\n-------[Cancion agregada]--------\n");
                system("pause");
                return;
            }
            auxList = nextList(ListasMusica);
        }
    }
    else {
    	//Crear una lista de reproducción nueva cuando no hayan canciones.
		ListaCancion * newSongList = (ListaCancion *) malloc (sizeof(ListaCancion));
		List * newList = createList();
		
		pushBack(newList, cancion_agregar);
		
		newSongList->Canciones = newList;
		newSongList->cant++;
		newSongList->listName = repList;

		pushBack(ListasMusica, newSongList);

		listaGlobal->Canciones = newList;
		listaGlobal->cant++;
		listaGlobal->listName = repList;
		
		system("pause");
		return;
    }
}

//Buscar apariciones del genero ingresado en los generos de las canciones
char * busquedaEspecial(char *listGenre, char *genre){
    char *lisAux = eliminarEspacios(listGenre);
    return strstr(lisAux,genre);
}

//Recorre las canciones y busca las coincidencias para mostrarlas en pantalla
void buscarPorGenero(ListaCancion listGlobal){
    char artistGenre[41] = {};
    printf("\nIngrese el genero de la cancion:\n");
    getchar();
    scanf("%[0-9a-zA-Z ,-]s",artistGenre);

    int cont;
    int i = 0;
    Cancion * aux;

    aux = firstList(listGlobal.Canciones);
    char * artistAux = eliminarEspacios(artistGenre);
    for (cont = 0 ; cont < listGlobal.cant ; cont++){
        if (busquedaEspecial(aux->genre, artistAux) != NULL){
            informacion(aux);
            i++;
        }
        aux = nextList(listGlobal.Canciones);
    }
    if (i == 0){
        printf("\nLo sentimos, no encontramos ninguna cancion con ese nombre en nuestra base :(\n");
        printf("\n===========================================\n");
    }

}

//Buscar coincidencias por artistas en la lista
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
        printf("\n===========================================\n");
    }

}

//Buscar coincidencias en los nombres al recorrer la lista y haciendo comparaciones
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
        printf("\n===========================================\n");
    }
}

//Menú de busqueda por nombre, artista y género
void buscarCancion(ListaCancion listGlobal){
    if (listGlobal.cant == 0){
		printf("No hay canciones para poder realizar una busqueda :( \n\n");
		system("pause");
        return;
    } 
    system("cls");
	
    int op;
    while (1){
    	printf("===========================================\n");
        printf("\n1. Buscar por nombre de la cancion\n");
        printf("2. Buscar por artista de la cancion\n");
        printf("3. Buscar por genero\n");
        printf("4. Volver al menu\n");
        printf("\n===========================================\n");
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
                buscarPorGenero(listGlobal);
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

ListaCancion EliminarCancion (ListaCancion listaGlobal) {
    if (listGlobal.cant == 0){
		printf("No se encontraron canciones :( \n\n");
		system("pause");
        return;
    } 

    char yearCmp[5];//año de la cancion que queremos eliminar
    char nameCmp[100];//nombre de la cancion que queremos eliminar
    char artCmp[50];//nombre del aritista de la cancion que vamos a eleminar

	//intuducimos los datos de la cancion a eliminar
	
	printf("\n===========================================\n");
    getchar();
    printf("\nEscriba el nombre de la cancion: ");
    scanf("%100[^\n]s",nameCmp);

    getchar();
    printf("\nEscriba el nombre del el/la artista: ");
    scanf("%50[^\n]s",artCmp);

    getchar();
    printf("\nEscriba el ano de la cancion: ");
    scanf("%5[^\n]s",yearCmp);

    int cantidad=listaGlobal.cant;//cantidad de canciones en la lista

    List * songsList = createList();// lista auxiliar la cual luego vamos a asignar a listaGlobal.canciones
    Cancion * auxSong = (Cancion*) malloc (sizeof(Cancion));//auxiliar para buscar la cancion que queremos eliminar
    auxSong = firstList(listaGlobal.Canciones); //Dirigirse a la primera canción de la lista.
    int eliminacion=0;// dato verificador para eliminar la cancion
    int push=1;// dato para evitar meter la cancion que queremos eliminar a songList

    for(int i=0;i<cantidad;i++){
        push=1;// iniciamos en cada ciclo push = 1

        if(strcmp(eliminarEspacios(auxSong->name),eliminarEspacios(nameCmp)) == 0){//comparamos el nombre

            if(strcmp(eliminarEspacios(auxSong->artist),eliminarEspacios(artCmp)) == 0){//comparamos el arista

                if(strcmp(eliminarEspacios(auxSong->year),eliminarEspacios(yearCmp)) == 0){//comparamos el año

                    //si todo se cumple encontraremos la cancion a eliminar
                    printf("\n-------Se elimino la cancion-------\n");

                    listaGlobal.cant--;//se descuenta de la cantidad de canciones

                    eliminacion=1;//verificamos que se encontro

                    push=0;//evitamos que se agrege a la lista nueva
                }
            }
        }

        if (push==1){//si push es 1 significa que no es la cancion que estabamos buscando asi que se agrega a la lista nueva
            pushBack(songsList,auxSong);
        }

        //next para pasar de cancion en cancion
        auxSong=nextList(listaGlobal.Canciones);
    }

    //asignamos al nueva lista sin la cancion (siempre cuando se alla encontrado)
    if(eliminacion==1) listaGlobal.Canciones=songsList;

    //si no se encontro la cancion se muestra un mensaje
    //si "eliminacion" es 0 significa que no se encontro
    if (eliminacion==0) printf("\n------No se encontro la cancion------\n");

	printf("===========================================\n");
    system("pause");

    //se retorna la lista
    return listaGlobal;
}

void mostrarNombresListas (List * listasMusica) {
	if (listGlobal.cant == 0){
		printf("No se encontraron listas :( \n\n");
		system("pause");
        return;
    } 
	
	//Acceder a la lista.
	ListaCancion * auxList = (ListaCancion *) malloc (sizeof(ListaCancion));
	auxList = firstList(listasMusica);

	//Mostrar Nombres y Cantidad de Canciones.
	printf("\n===========================================\n");
	printf("LISTAS DE REPRODUCCION DISPONIBLES:\n");
	while (auxList != NULL) {
		printf("\n--- %s --- %d Canciones.\n", auxList->listName, auxList->cant);
		auxList = nextList(listasMusica);
	}
	printf("\n");
	system("pause");
}

void mostrarListaRep (List * listasMusica) {
	if (listGlobal.cant == 0){
		printf("No se encontraron listas :( \n\n");
		system("pause");
        return;
    } 
	
	//Preguntar por la lista a buscar.
	ListaCancion * auxList = (ListaCancion *) malloc (sizeof(ListaCancion));
	auxList = firstList(listasMusica);
	char nameAux[11];
	printf("Ingrese lista: ");
	scanf(" %[^\n]s", nameAux);
    int cont = 0;

	//Buscar la lista de reproducción.
	while (auxList != NULL) {
        if (strcmp(auxList->listName,nameAux) == 0){
            cont = 1;
            break;
        }

        auxList = nextList(listasMusica);

        if (auxList == NULL && cont == 0){
            printf("NO EXISTE LA LISTA\n");
            system("pause");
            return;
        }
	}

    Cancion *auxSong = (Cancion*)malloc(sizeof(Cancion));
    auxSong = firstList(auxList->Canciones);


	int cantSongs = auxList->cant;
    printf("\n------[%s]-----[%d canciones]------\n\n",auxList->listName,auxList->cant);
    int contador = 1;
	for (int i = 1; i <= cantSongs; i++) {
		printf("%d.-%s -",contador,auxSong->name);
        printf(" %s -", auxSong->artist);
        printf(" %s -", auxSong->genre);
		printf(" %s\n", auxSong->year);
		auxSong = nextList(auxList->Canciones);
		contador++;
	}
    contador = 1;
    printf("\n");
	system("pause");

}

void mostrarCanciones (ListaCancion listaGlobal) {
	if (listGlobal.cant == 0){
		printf("No se encontraron canciones :( \n\n");
		system("pause");
        return;
    } 
	
    //Crear Interfaz.
    printf("\n===========================================\n");
    printf("MOSTRANDO TODAS LAS CANCIONES...\n\n");

    //Verificar si hay más canciones.
    if (listaGlobal.cant == 0) printf("No se encontraron canciones :( \n\n");

    //Mostrar cada una de las canciones.
    Cancion * auxSong = (Cancion*) malloc (sizeof(Cancion));
    auxSong = firstList(listaGlobal.Canciones); //Dirigirse a la primera canción de la lista.
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
    if (listaGlobal.cant != 1) printf("\nHay un total de %d canciones...\n", listaGlobal.cant);
    else printf("\nHay solo una cancion...\n", listaGlobal.cant);
    system("pause");
}

//El main servirá para dirigir el programa y abrir la interfaz.
int main () {
    int op = 0;
    ListaCancion listaGlobal; //Lista de Canciones.
    listaGlobal.cant = 0;
    List * listasMusica = createList(); //Listas de reproducción actuales.
	//Ejecutar una acción según el resultado obtenido desde la interfaz.}
	int cont = 0;
    while (op != 9){
        op = mostrarInterfaz(op);
        switch (op) {
            case 1:
				listaGlobal = importarCanciones();
				listasMusica = crearListas(listaGlobal);
				cont = 1;
                break;
            case 2:
                exportarCanciones(listaGlobal);
                break;
            case 3:
				agregarCancion(&listaGlobal,listasMusica,cont);
				cont = 1;
                break;
            case 4:
                buscarCancion(listaGlobal);
                break;
            case 5:
			    listaGlobal = EliminarCancion(listaGlobal);
			    listasMusica = crearListas(listaGlobal);
                break;
            case 6:
                mostrarNombresListas(listasMusica);
                break;
            case 7:
                mostrarListaRep(listasMusica);
                break;
            case 8:
                mostrarCanciones(listaGlobal);
                break;
            case 9:
                system("cls");
				printf("Gracias por usar nuestro programa, recuerda no piratear ;)\n");
                system("pause");
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

//Funcion que cambia de mayúscula a minúscula
int myTolower(int caracter){
    if(caracter > 64 && caracter < 91) return caracter + 32;
    return caracter;
}

//Elimina los espacios y cambia los caracteres de mayúscula a minuscula
char * eliminarEspacios(char  *cadena){
    char *aux = (char *) calloc(31,sizeof(char));
    int k = 0;
    int h = 0;
    while (*(cadena + k) != '\0'){
        if(*(cadena + k) != ' '){
            *(aux + h) = (char) myTolower(*(cadena + k));
            h++;
        }
        k++;
    }
    return aux;
}

//Funcion que imprime toda la informacion por cancion
void informacion(Cancion * aux){
    printf("\n-------------------------------------\n");
    printf("Nombre: %s\n",aux->name);
    printf("Artista: %s\n",aux->artist);
    printf("Genero: %s\n",aux->genre);
    printf("Ano: %s\n",aux->year);
    printf("Lista de Reproduccion: %s\n",aux->repList);
    printf("-------------------------------------\n");
}