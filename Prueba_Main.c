#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char * name; //Nombre de la Canción.
	int year; //Año de la Canción.
	char * genre; //Género de la canción.
	char * artist; //Nombre del Artista.
	char * repList; //Lista de Reproducción.
} Cancion;

typedef struct {
	int cant; //Cantidad de Canciones.
	Cancion * arrCanciones; //Lista de Canciones.
	char * listName; //Nombre de la Lista.jj
} ListaCancion;


/* Función Mostrar Interfaz */
/*Se encarga de mostrar la interfaz inicial con el que el usuario
interactúa. Retorna un entero para evaluar que acción quiere el
usuario que el programa realice.*/

int mostrarInterfaz (int num) {

	printf("¡Bienvenido al Programa!\n");
	printf("¿Qué acción desea implementar hoy?\n");
	printf("\n(1) Ingresar una Canción\n");
	printf("(2) Buscar Canción\n\n");

	printf("Ingrese su opción: ");
	scanf("%d", &num);
	return num;
}

//El main servirá para dirigir el programa y abrir la interfaz.
int main () {

	int op = 0; //La variable op servirá para evaluar que hacer.

	//Abrir Interfaz.
	op = mostrarInterfaz(op);

	//Ejecutar una acción según el resultado obtenido desde la interfaz.
	switch (op) {
		case 1:
			printf("1"); /* En estos espacios van las acciones (funciones) */
			break;
		case 2:
			printf("2");
			break;
		default:
			printf("Ok");
			break;
	}

	scanf("%d", &op); // COMENTARIO: Esto de acá es sólo para que no se me cierre el programa en 2 milisegundos :)
	return 0;
}