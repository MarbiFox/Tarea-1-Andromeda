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

        if(tmp[pos] == ','){
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
		
		//Limpiar Lista.
        cleanList(auxSongList);
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
				printf(" %s\n", auxSong->repList);
				auxSong = firstList(globalList.Canciones);
				
                break;
			}
		}
		
        system("pause");
 	}
    
	//system("pause");
	return musicRepList;
}

void exportarCanciones (ListaCancion listaGlobal) {
	
	//Abrir o Crear el archivo para exportar
	FILE * fout;
	fout = fopen("Can.csv", "w");
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
    system("pause");
    
    fclose(fout);
	       
}    

void buscarPorNombre(ListaCancion listGlobal){
    char nombreCancion[81] = {};
    printf("\nIngrese el nombre de la cancion:\n");
    getchar();
    scanf("%81s[^\n]",nombreCancion);
    printf("%s\n",nombreCancion);

    int cont;
    int i = 0;
    Cancion * aux;
    aux = firstList(listGlobal.Canciones);
    for (cont = 0 ; cont < listGlobal.cant ; cont++){
    //while(aux!=NULL){
        if (strcmp(aux->name,nombreCancion) == 0){
            printf("\n-------------------------------------\n");
            printf("Nombre: %s\n",aux->name);
            printf("Artista: %s\n",aux->artist);
            printf("Genero: %s\n",aux->genre);
            printf("Ano: %s\n",aux->year);
            printf("Lista de Reproduccion: %s\n",aux->repList);
            printf("-------------------------------------\n");
            i++;
        }
        aux = nextList(listGlobal.Canciones);
    }
    if (i == 0){
        printf("\nLo sentimos, no encontramos ninguna cancion con ese nombre en nuestra base :(\n");
    }
}

void buscarPorArtista(ListaCancion listGlobal){
    char artistSong[21] = {};
    printf("\nIngrese el artista de la cancion:\n");
    getchar();
    scanf("%21[^\n]s",artistSong);
    puts(artistSong);

    int cont;
    int i = 0;
    Cancion * aux;
    aux = firstList(listGlobal.Canciones);
    for (cont = 0 ; cont < listGlobal.cant ; cont++){
        if (strcmp(aux->artist,artistSong) == 0){
            printf("\n-------------------------------------\n");
            printf("Nombre: %s\n",aux->name);
            printf("Artista: %s\n",aux->artist);
            printf("Genero: %s\n",aux->genre);
            printf("Ano: %s\n",aux->year);
            printf("Lista de Reproduccion: %s\n",aux->repList);
            printf("-------------------------------------\n");
            i++;
        }
        aux = nextList(listGlobal.Canciones);
    }
    if (i == 0){
        printf("\nLo sentimos, no encontramos ninguna cancion con ese nombre en nuestra base :(\n");
    }

}

void buscarPorGenero(ListaCancion listGlobal){
    char artistGenre[21] = {};
    printf("\nIngrese el genero de la cancion:\n");
    getchar();
    scanf("%[0-9a-zA-Z ,-]",artistGenre);
    puts(artistGenre);

    int cont;
    int i = 0;
    Cancion * aux;
    aux = firstList(listGlobal.Canciones);
    for (cont = 0 ; cont < listGlobal.cant ; cont++){
        if (strcmp(aux->genre,artistGenre) == 0){
            printf("\n-------------------------------------\n");
            printf("Nombre: %s\n",aux->name);
            printf("Artista: %s\n",aux->artist);
            printf("Genero: %s\n",aux->genre);
            printf("Ano: %s\n",aux->year);
            printf("Lista de Reproduccion: %s\n",aux->repList);
            printf("-------------------------------------\n");
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

    printf("\n1. Buscar por nombre de la cancion\n");
    printf("2. Buscar por artista de la cancion\n");
    printf("3. Buscar por genero\n");
    printf("4. Volver al menu\n\n");

    int op=0;
    while (op!=4){
        printf("Ingrese su opcion: ");
        scanf("%i", &op);
        if (op == 1){
           buscarPorNombre(listGlobal);
        }
        if (op == 2){
           buscarPorArtista(listGlobal);
        }
        if (op== 3){
           buscarPorGenero(listGlobal);
        }
        if (op == 4){
           break;
        }
    }
    system("pause");
}

void EliminarCancion(ListaCancion listaGlobal) {
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

ListaCancion EliminarCancion(ListaCancion listaGlobal) {
    if (listaGlobal.cant == 0) printf("No se encontraron canciones :( \n\n");

    char yearCmp[5];//año de la cancion que queremos eliminar
    char nameCmp[100];//nombre de la cancion que queremos eliminar
    char artCmp[50];//nombre del aritista de la cancion que vamos a eleminar
    
	//intuducimos los datps de la cancion a eliminar
    
    getchar();
    printf("\n escriba el nombre de la cancion:");
    scanf("%100[^\n]s",nameCmp);

    getchar();
    printf("\n escriba el nombre del el/la artista:");
    scanf("%50[^\n]s",artCmp);
    
    getchar();
    printf("\n escriba el año de la cancion:");
    scanf("%5[^\n]s",yearCmp);

    int cantidad=listaGlobal.cant;//cantidad de canciones en la lista
    
    List * songsList = createList();// lista auxiliar la cual luego vamos a asignar a listaGlobal.canciones
    Cancion * auxSong = (Cancion*) malloc (sizeof(Cancion));//auxiliar para buscar la cancion que queremos eliminar
    auxSong = firstList(listaGlobal.Canciones); //Dirigirse a la primera canción de la lista.
    int eliminacion=0;// dato verificador para eliminar la cancion
    int push=1;// dato para evitar meter la cancion que queremos eliminar a songList 

    for(int i=0;i<cantidad;i++){
        push=1;// iniciamos en cada ciclo push = 1

        if(strcmp(auxSong->name,nameCmp) == 0){//comparamos el nombre
            
            if(strcmp(auxSong->artist,artCmp) == 0){//comparamos el arista
                
                if(strcmp(auxSong->year,yearCmp) == 0){//comparamos el año
                    
                    //si todo se cumple encontraremos la cancion a eliminar
                    printf("\n--------Se elimino la cancion--------\n");

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
    if (eliminacion==0) printf("\n--------No se encontro la cancion--------\n");

    system("pause");

    //se retorna la lista
    return listaGlobal;
}

void mostrarNombresListas (List * listasMusica) {
	
	//Acceder a la lista.
	ListaCancion * auxList = (ListaCancion *) malloc (sizeof(ListaCancion));
	auxList = firstList(listasMusica);
	
	//Mostrar Nombres y Cantidad de Canciones.
	while (1) {
		printf("--- %s --- %d Canciones.\n", auxList->listName, auxList->cant);
		if (listasMusica->current->next != NULL) {
			auxList = nextList(listasMusica);
		}
		else {
			break;
		}
	}
	
	system("pause");
}

void mostrarListaRep (List * listasMusica) {
	
	//Preguntar por la lista a buscar.
	ListaCancion * auxList = (ListaCancion *) malloc (sizeof(ListaCancion));
	auxList = firstList(listasMusica);  
	char nameAux[11] = {};          
	getchar();                                    
	scanf("%[0-9a-zA-Z ,-]", nameAux);
	
	//Buscar la lista de reproducción.
	while (1) {
		
		printf("--- %s\n", auxList->listName);
		if (strcmp(auxList->listName, nameAux) == 0) break;
		printf("--- %s\n", auxList->listName);
		
		if (listasMusica->current->next != NULL) {
			auxList = nextList(listasMusica);                  
		} 
		else {
			printf("No se encontró la Lista :( \n)");
			break;
		}
	}
	
	system("pause");
	
	
	//Imprimir los Datos de Cada canción.
	Cancion * auxSong = (Cancion *) malloc (sizeof(Cancion));
	auxSong = firstList(auxList->Canciones);
	printf("%s -", auxSong->name);
	/*
	printf("Pront\n");
	int cantSongs = auxList->cant;
	
	for (int i = 1; i < cantSongs; i++) {
		printf("%s -", auxSong->name);
        printf(" %s -", auxSong->artist);
        printf(" %s -", auxSong->genre);
		printf(" %s -", auxSong->year);
		printf(" %s\n", auxSong->repList);
		auxSong = nextList(auxList->Canciones);
	}*/
}

void mostrarCanciones (ListaCancion listaGlobal) {
	
    //Crear Interfaz.
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
    if (listaGlobal.cant != 1) printf("Hay un total de %d canciones...", listaGlobal.cant);
    else printf("Hay sólo una canción...", listaGlobal.cant);
    system("pause");
}

//El main servirá para dirigir el programa y abrir la interfaz.
int main () {
    int op = 0;
    ListaCancion listaGlobal; //Lista de Canciones.
    List * listasMusica = createList(); //Listas de reproducción actuales.
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
				printf("3\n");
                break;
            case 4:
                buscarCancion(listaGlobal);
                break;
            case 5:
				EliminarCancion(listaGlobal);
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
    //system("pause");
    /* Se puede hacer lo mismo con system("pause");
    scanf("%d", &op); // COMENTARIO: Esto de acá es sólo para que no se me cierre el programa en 2 milisegundos :)
    */
    return 0;
}
