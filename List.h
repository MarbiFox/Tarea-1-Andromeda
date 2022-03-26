#ifndef LIST_H
#define LIST_H

typedef struct Node Node; //Definir un Nodo.
typedef struct List List; //Definir una Lista.

/** Crear una Nodo nuevo y retornar un puntero a aquel. **/
Node * createNode (void * data);

/** Crear una Lista nueva y retorna un puntero a ella. **/
List * createList();

/** Devuelve un puntero al primer dato de la Lista, si es que existe.
 * Si ese no es el caso, devuelve NULL. **/
void * firstList (List * L);

/** Devuelve un puntero al siguiente dato de la Lista, si es que hay alguno
 * después del actual. Si ese no es el caso, devuelve NULL. **/
void * nextList (List * L);

/** Devuelve un puntero al último dato de la Lista, si es que existe.
 * Si ese no es el caso, devuelve NULL. **/
void * lastList (List * L);

/** Devuelve un puntero al dato anterior de la Lista, si es que hay alguno
 * antes del actual. Si ese no es el caso, devuelve NULL. **/
void * prevList (List * L);

/** Crea un nodo nuevo y le asigna un dato, después la función coloca
 * al nodo en la primera posición de la lista. **/
void * pushFront (List * L);

/** Crea un nodo nuevo y le asigna un dato, después la función coloca
 * al nodo en la última posición de la lista. **/
void * pushBack (List * L);

/** Crea un nodo nuevo y le asigna un dato, después la función coloca
 * al nodo en la posición posterior al dato actual en la lista. **/
void * pushCurrent (List * L);

/** Elimina el nodo y dato asignado ubicado en la primera posición de
 * la Lista. Retorna el dato que estaba en el nodo eliminado. **/
void * popFront (List * L);

/** Elimina el nodo y dato asignado ubicado en la última posición de
 * la Lista. Retorna el dato que estaba en el nodo eliminado. **/
void * popBack (List * L);

/** Elimina el nodo y dato asignado ubicado en la posición actual de
 * la Lista. Retorna el dato que estaba en el nodo eliminado. **/
void * popCurrent (List * L);

/** Elimina todos los nodos y datos de la Lista uno por uno. **/
void * cleanList (List * L);

#endif