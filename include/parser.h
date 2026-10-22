#ifndef PARSER_H
#define PARSER_H

#include "tablas.h"

#define MAX_TOKENS 256
#define MAX_PILA 512
#define MAX_HIJOS 8

/* Nodo del arbol de sintaxis */
typedef struct NodoAST {
    char etiqueta[64];
    struct NodoAST *hijos[MAX_HIJOS];
    int cantidad_hijos;
} NodoAST;

/* Elemento de la pila del analizador */
typedef struct {
    int estado;
    NodoAST *nodo;
} ElementoPila;

/* Pila utilizada durante el analisis */
typedef struct {
    ElementoPila elementos[MAX_PILA];
    int cima;
} Pila;

/* Resultado del analisis sintactico */
typedef struct {
    int aceptado;
    int errores;
    int desplazamientos;
    int reducciones;
} ResultadoAnalisis;

/* Operaciones de la pila */
void inicializar_pila(Pila *pila);
int apilar(Pila *pila, int estado);
int desapilar(Pila *pila, int cantidad);
int estado_actual(const Pila *pila);

/* Funcion principal del analizador */
ResultadoAnalisis analizar(const Terminal tokens[], int cantidad);

/* Funciones auxiliares */
const char *nombre_terminal(Terminal token);
void mostrar_pila(const Pila *pila);

/* Funciones para el AST */
NodoAST *crear_nodo_ast(const char *etiqueta);
void agregar_hijo_ast(NodoAST *padre, NodoAST *hijo);
void mostrar_ast(const NodoAST *raiz, int nivel);
void mostrar_ast_simplificado(const NodoAST *raiz, int nivel);
void liberar_ast(NodoAST *raiz);

#endif
