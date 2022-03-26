#include <stdio.h>
#include <stdlib.h>
#include "list.h"

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
                printf("1\n") ;
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