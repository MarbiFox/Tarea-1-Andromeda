#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct Node Node;

struct Node {
    void * data;
    Node * next;
    Node * prev;
};

struct List {
    Node * head;
    Node * tail;
    Node * current;
};

typedef List List;

Node * createNode(void * data) {
    Node * new = (Node *)malloc(sizeof(Node));
    assert(new != NULL);
    new->data = data;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

List * createList() {
    List * lista = (List *) malloc (sizeof(List));
    lista->head = NULL;
    lista->tail = NULL;
    lista->current = NULL;
    return lista;
}

void * firstList(List * list) {
    if (list->head != NULL) {
        list->current = list->head;
        return list->current->data;
    }
    return NULL;
}

void * nextList(List * list) {
    if (list->current != NULL && list->current->next != NULL) {
        list->current = list->current->next;
        return list->current->data;
    }
    return NULL;
}

void * lastList(List * list) {
    if (list->tail != NULL) {
        list->current = list->tail;
        return list->current->data;
    }
    return NULL;
}

void * prevList(List * list) {
    if (list->current != NULL && list->current->prev != NULL) {
        list->current = list->current->prev;
        return list->current->data;
    }
    return NULL;
}

void pushFront(List * list, void * data) {
    Node * n = createNode(data);
    n->next = list->head; //Conectar el nodo a la lista.
    if (list->head != NULL) {
        list->head->prev = n; //Si hay elementos en la lista, conectar el primero al nodo nuevo.
    }
    else {
        list->tail = n; //Si no hay head, no hay elementos y el nuevo nodo será el último.
    }
    list->head = n; //Declarar n como el nuevo head.
    n->prev = NULL; //Declarar su puntero hacia el antecesor como NULL;
}

void pushBack(List * list, void * data) {
    list->current = list->tail;
    pushCurrent(list,data);
}

void pushCurrent(List * list, void * data) {
    Node * n = createNode(data);
    if (list->current != NULL) {
        n->prev = list->current; //Declarar el nodo current como el antecesor de n.
        if (list->current != list->tail) {
            list->current->next->prev = n; //Conectar el puntero prev del sucesor de current a n.
            n->next = list->current->next; //Conectar el n con su sucesor.
        }
        else {
            list->tail = n; //En caso de que el current sea el tail, sólo reemplazar tail con el nodo nuevo.
        }
        list->current->next = n; //Conectar el nodo current al n.
    }
    list->current = n; //Declarar n como el nuevo current.
}

void * popFront(List * list) {
    list->current = list->head;
    return popCurrent(list);
}

void * popBack(List * list) {
    list->current = list->tail;
    return popCurrent(list);
}

void * popCurrent(List * list) {
    if (list->current->prev != NULL) {
        list->current->prev->next = list->current->next; //Conectar el antecesor de current al sucesor de current.
    }
    else {
        list->head = list->current->next; //Dejar head como el sucesor de current.
    }
    if (list->current->next != NULL) {
        list->current->next->prev = list->current->prev; //Conectar el sucesor de current con el antecesor de current.
    }
    else {
        list->tail = list->current->prev; //Dejar tail como el antecesor de current.
    }
    Node * nodoEliminado = list->current; //Guardar un puntero a la dirección del nodo a eliminar.
    list->current = list->current->next; //Dejar apuntado el siguiente nodo.
    free(nodoEliminado); //Borrar el nodo.
}

void cleanList(List * list) {
    while (list->head != NULL) {
        popFront(list);
    }
}