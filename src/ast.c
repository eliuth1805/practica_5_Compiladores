#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

/*
 * Crear un nuevo nodo del arbol sintactico.
 */
NodoAST *crear_nodo_ast(const char *etiqueta)
{
    NodoAST *nuevo = malloc(sizeof(NodoAST));

    if (nuevo == NULL) {
        fprintf(stderr,
                "Error: no se pudo reservar memoria para el AST.\n");
        return NULL;
    }

    snprintf(nuevo->etiqueta,
             sizeof(nuevo->etiqueta),
             "%s",
             etiqueta);

    nuevo->cantidad_hijos = 0;

    for (int i = 0; i < MAX_HIJOS; i++) {
        nuevo->hijos[i] = NULL;
    }

    return nuevo;
}

/*
 * Agregar un hijo a un nodo.
 */
void agregar_hijo_ast(NodoAST *padre, NodoAST *hijo)
{
    if (padre == NULL || hijo == NULL) {
        return;
    }

    if (padre->cantidad_hijos >= MAX_HIJOS) {
        fprintf(stderr,
                "Error: numero maximo de hijos alcanzado.\n");
        return;
    }

    padre->hijos[padre->cantidad_hijos] = hijo;
    padre->cantidad_hijos++;
}

/*
 * Mostrar el arbol sintactico completo.
 */
void mostrar_ast(const NodoAST *raiz, int nivel)
{
    if (raiz == NULL) {
        return;
    }

    for (int i = 0; i < nivel; i++) {
        printf("    ");
    }

    printf("|-- %s\n", raiz->etiqueta);

    for (int i = 0; i < raiz->cantidad_hijos; i++) {
        mostrar_ast(raiz->hijos[i], nivel + 1);
    }
}

/*
 * Liberar la memoria utilizada por el arbol.
 */
void liberar_ast(NodoAST *raiz)
{
    if (raiz == NULL) {
        return;
    }

    for (int i = 0; i < raiz->cantidad_hijos; i++) {
        liberar_ast(raiz->hijos[i]);
    }

    free(raiz);
}

/*
 * Imprimir espacios para organizar el arbol.
 */
static void imprimir_indentacion(int nivel)
{
    for (int i = 0; i < nivel; i++) {
        printf("    ");
    }
}

/*
 * Mostrar una etiqueta con su indentacion.
 */
static void imprimir_nodo(const char *etiqueta, int nivel)
{
    imprimir_indentacion(nivel);
    printf("|-- %s\n", etiqueta);
}

/*
 * Mostrar una expresion conservando
 * la estructura de las operaciones.
 *
 * Producciones:
 *
 * Accion -> Accion + Elemento
 * Accion -> Elemento
 *
 * Elemento -> id
 * Elemento -> num
 * Elemento -> ( Accion )
 */
static void mostrar_expresion(const NodoAST *nodo, int nivel)
{
    if (nodo == NULL) {
        return;
    }

    /*
     * Accion -> Accion + Elemento
     *
     * Se representa como una suma
     * con dos operandos.
     */
    if (strcmp(nodo->etiqueta, "Accion") == 0 &&
        nodo->cantidad_hijos == 3) {

        imprimir_nodo("Suma", nivel);

        /* Operando izquierdo */
        mostrar_expresion(nodo->hijos[0], nivel + 1);

        /* Operando derecho */
        mostrar_expresion(nodo->hijos[2], nivel + 1);

        return;
    }

    /*
     * Accion -> Elemento
     *
     * No necesitamos conservar este
     * nodo intermedio.
     */
    if (strcmp(nodo->etiqueta, "Accion") == 0 &&
        nodo->cantidad_hijos == 1) {

        mostrar_expresion(nodo->hijos[0], nivel);
        return;
    }

    /*
     * Elemento -> ( Accion )
     *
     * Los parentesis no se imprimen,
     * pero se conserva la estructura
     * de la expresion que contienen.
     */
    if (strcmp(nodo->etiqueta, "Elemento") == 0 &&
        nodo->cantidad_hijos == 3) {

        mostrar_expresion(nodo->hijos[1], nivel);
        return;
    }

    /*
     * Elemento -> id
     * Elemento -> num
     */
    if (strcmp(nodo->etiqueta, "Elemento") == 0 &&
        nodo->cantidad_hijos == 1) {

        mostrar_expresion(nodo->hijos[0], nivel);
        return;
    }

    /*
     * Mostrar identificadores y numeros.
     */
    imprimir_nodo(nodo->etiqueta, nivel);
}

/*
 * Mostrar una representacion simplificada
 * del arbol sintactico.
 */
void mostrar_ast_simplificado(const NodoAST *nodo, int nivel)
{
    if (nodo == NULL) {
        return;
    }

    const char *etiqueta = nodo->etiqueta;

    /*
     * Mostrar el nodo principal.
     */
    if (strcmp(etiqueta, "Programa") == 0) {

        imprimir_nodo("Programa", nivel);

        for (int i = 0; i < nodo->cantidad_hijos; i++) {
            mostrar_ast_simplificado(
                nodo->hijos[i],
                nivel + 1
            );
        }

        return;
    }

    /*
     * Omitir el nodo Lista.
     *
     * Esto permite mostrar todos los
     * comandos al mismo nivel.
     */
    if (strcmp(etiqueta, "Lista") == 0) {

        for (int i = 0; i < nodo->cantidad_hijos; i++) {
            mostrar_ast_simplificado(
                nodo->hijos[i],
                nivel
            );
        }

        return;
    }

    /*
     * Identificar los comandos.
     */
    if (strcmp(etiqueta, "Comando") == 0) {

        if (nodo->cantidad_hijos == 0 ||
            nodo->hijos[0] == NULL) {
            return;
        }

        const NodoAST *operacion = nodo->hijos[0];

        imprimir_nodo(operacion->etiqueta, nivel);

        /*
         * Mostrar los argumentos.
         */
        for (int i = 1; i < nodo->cantidad_hijos; i++) {

            const NodoAST *hijo = nodo->hijos[i];

            if (hijo == NULL) {
                continue;
            }

            /*
             * No mostrar el punto y coma.
             */
            if (strcmp(hijo->etiqueta, ";") == 0) {
                continue;
            }

            /*
             * Si el argumento es una expresion,
             * utilizar la funcion especializada.
             */
            if (strcmp(hijo->etiqueta, "Accion") == 0) {

                mostrar_expresion(hijo, nivel + 1);

            } else {

                mostrar_ast_simplificado(
                    hijo,
                    nivel + 1
                );
            }
        }

        return;
    }

    /*
     * Mostrar archivos como argumentos.
     */
    if (strcmp(etiqueta, "Archivo") == 0) {

        if (nodo->cantidad_hijos > 0 &&
            nodo->hijos[0] != NULL) {

            char texto[128];

            snprintf(texto,
                     sizeof(texto),
                     "Archivo: %s",
                     nodo->hijos[0]->etiqueta);

            imprimir_nodo(texto, nivel);
        }

        return;
    }

    /*
     * Omitir puntuacion.
     */
    if (strcmp(etiqueta, ";") == 0 ||
        strcmp(etiqueta, "(") == 0 ||
        strcmp(etiqueta, ")") == 0) {

        return;
    }

    /*
     * Mostrar cualquier otro nodo.
     */
    imprimir_nodo(etiqueta, nivel);

    for (int i = 0; i < nodo->cantidad_hijos; i++) {
        mostrar_ast_simplificado(
            nodo->hijos[i],
            nivel + 1
        );
    }
}
