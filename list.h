#ifndef LIST_H
#define LIST_H

typedef struct Node Node;

typedef struct List List;

/*Crear nodo*/
Node * createNode(void * data);

/*Crear lista*/
List * createList();

/*Primer elemento de la Lista*/
void * firstList(List * list);

/*Siguiente elemento de la Lista*/
void * nextList(List * list);

/*Ultimo elemento de la Lista*/
void * lastList(List * list);

/*Elemento anterior de la Lista*/
void * prevList(List * list);

/*Insertar al inicio de la Lista*/
void pushFront(List * list, void * data);

/*Insertar al final de la Lista*/
void pushBack(List * list, void * data);

/*Insertar en medio de la Lista*/
void pushCurrent(List * list, void * data);

/*Eliminar al inicio de la Lista*/
void * popFront(List * list);

/*Eliminar al final de la Lista*/
void * popBack(List * list);

/*Eliminar en medio de la Lista*/
void * popCurrent(List * list);

/*Limpiar lista*/
void cleanList(List * list);


#endif
