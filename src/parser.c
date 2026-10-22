#include <stdio.h>
#include "parser.h"

/* Inicializar la pila con el estado 0 */
void inicializar_pila(Pila *pila)
{
    pila->cima = 0;
    pila->elementos[0].estado = 0;
    pila->elementos[0].nodo = NULL;
}

/* Agregar un estado a la pila */
int apilar(Pila *pila, int estado)
{
    if (pila->cima >= MAX_PILA - 1) {
        printf("Error: la pila esta llena.\n");
        return 0;
    }

    pila->cima++;
    pila->elementos[pila->cima].estado = estado;
    pila->elementos[pila->cima].nodo = NULL;

    return 1;
}

/* Retirar estados de la pila */
int desapilar(Pila *pila, int cantidad)
{
    if (cantidad < 0 || cantidad > pila->cima) {
        printf("Error: no se pueden retirar esos estados.\n");
        return 0;
    }

    pila->cima -= cantidad;
    return 1;
}

/* Consultar el estado actual */
int estado_actual(const Pila *pila)
{
    return pila->elementos[pila->cima].estado;
}

/* Mostrar el contenido de la pila */
void mostrar_pila(const Pila *pila)
{
    printf("Pila: ");

    for (int i = 0; i <= pila->cima; i++) {
        printf("%d ", pila->elementos[i].estado);
    }

    printf("\n");
}

/* Nombres de los terminales */
const char *nombre_terminal(Terminal token)
{
    static const char *nombres[] = {
        "crear", "borrar", "copiar", "mover",
        "mostrar", "buscar", "renombrar",
        "listar", "ejecutar", "id", "ruta",
        "nombre", "num", "+", "(", ")", ";", "$"
    };

    if (token < 0 || token >= NUM_TERMINALES) {
        return "desconocido";
    }

    return nombres[token];
}

/* Nombres de los no terminales */
static const char *nombre_no_terminal(NoTerminal simbolo)
{
    static const char *nombres[] = {
        "Programa",
        "Lista",
        "Comando",
        "Accion",
        "Elemento",
        "Archivo"
    };

    if (simbolo < 0 || simbolo >= NUM_NO_TERMINALES) {
        return "Desconocido";
    }

    return nombres[simbolo];
}

/* Liberar los arboles que permanecen en la pila */
static void limpiar_pila_ast(Pila *pila)
{
    for (int i = 0; i <= pila->cima; i++) {
        liberar_ast(pila->elementos[i].nodo);
        pila->elementos[i].nodo = NULL;
    }
}

/* Analizador sintactico SLR(1) */
ResultadoAnalisis analizar(const Terminal tokens[], int cantidad)
{
    ResultadoAnalisis resultado = {0, 0, 0, 0};
    Pila pila;

    inicializar_pila(&pila);

    if (tokens == NULL || cantidad <= 0 || cantidad > MAX_TOKENS) {
        printf("Error: entrada invalida.\n");
        resultado.errores++;
        return resultado;
    }

    int posicion = 0;

    while (posicion < cantidad) {

        int estado = estado_actual(&pila);
        Terminal token = tokens[posicion];

        if (token < 0 || token >= NUM_TERMINALES) {
            printf("Error: token desconocido.\n");
            resultado.errores++;
            limpiar_pila_ast(&pila);
            return resultado;
        }

        Accion accion = tabla_action[estado][token];

        mostrar_pila(&pila);
        printf("Token actual: %s\n", nombre_terminal(token));

        /*
         * SHIFT:
         * Desplazar el token y crear su nodo.
         */
        if (accion.tipo == ACC_SHIFT) {

            printf("SHIFT al estado %d\n\n", accion.valor);

            if (!apilar(&pila, accion.valor)) {
                resultado.errores++;
                limpiar_pila_ast(&pila);
                return resultado;
            }

            NodoAST *nodo = crear_nodo_ast(
                nombre_terminal(token)
            );

            if (nodo == NULL) {
                resultado.errores++;
                limpiar_pila_ast(&pila);
                return resultado;
            }

            pila.elementos[pila.cima].nodo = nodo;

            posicion++;
            resultado.desplazamientos++;

        /*
         * REDUCE:
         * Construir el arbol utilizando la produccion.
         */
        } else if (accion.tipo == ACC_REDUCE) {

            int numero = accion.valor;

            if (numero <= 0 || numero >= NUM_PRODUCCIONES) {
                printf("Error: produccion invalida.\n");
                resultado.errores++;
                limpiar_pila_ast(&pila);
                return resultado;
            }

            Produccion produccion = producciones[numero];

            printf("REDUCE con produccion %d\n\n", numero);

            if (produccion.longitud < 0 ||
                produccion.longitud > pila.cima ||
                produccion.longitud > MAX_HIJOS) {

                printf("Error: longitud de produccion invalida.\n");
                resultado.errores++;
                limpiar_pila_ast(&pila);
                return resultado;
            }

            /* Crear el nodo del no terminal */
            NodoAST *nuevo = crear_nodo_ast(
                nombre_no_terminal(produccion.lado_izquierdo)
            );

            if (nuevo == NULL) {
                resultado.errores++;
                limpiar_pila_ast(&pila);
                return resultado;
            }

            /*
             * Recuperar los hijos antes de retirar
             * los estados de la pila.
             */
            int inicio = pila.cima - produccion.longitud + 1;

            NodoAST *hijos[MAX_HIJOS];

            for (int i = 0; i < produccion.longitud; i++) {
                hijos[i] = pila.elementos[inicio + i].nodo;
            }

            /* Retirar los estados de la produccion */
            if (!desapilar(&pila, produccion.longitud)) {
                liberar_ast(nuevo);
                resultado.errores++;
                limpiar_pila_ast(&pila);
                return resultado;
            }

            estado = estado_actual(&pila);

            /* Consultar la tabla GOTO */
            int siguiente =
                tabla_goto[estado][produccion.lado_izquierdo];

            if (siguiente < 0 || siguiente >= NUM_ESTADOS) {

                printf("Error: transicion GOTO invalida.\n");

                for (int i = 0; i < produccion.longitud; i++) {
                    liberar_ast(hijos[i]);
                }

                liberar_ast(nuevo);
                resultado.errores++;
                limpiar_pila_ast(&pila);
                return resultado;
            }

            /* Agregar el nuevo estado */
            if (!apilar(&pila, siguiente)) {

                for (int i = 0; i < produccion.longitud; i++) {
                    liberar_ast(hijos[i]);
                }

                liberar_ast(nuevo);
                resultado.errores++;
                limpiar_pila_ast(&pila);
                return resultado;
            }

            /* Conectar los hijos al nuevo nodo */
            for (int i = 0; i < produccion.longitud; i++) {
                agregar_hijo_ast(nuevo, hijos[i]);
            }

            /* Asociar el arbol con el estado GOTO */
            pila.elementos[pila.cima].nodo = nuevo;

            resultado.reducciones++;

        /*
         * ACCEPT:
         * Mostrar el arbol completo y el simplificado.
         */
        } else if (accion.tipo == ACC_ACCEPT) {

            printf("CADENA ACEPTADA\n");

            resultado.aceptado = 1;

            NodoAST *raiz = pila.elementos[pila.cima].nodo;

            printf("\n");
            printf("====================================\n");
            printf(" ARBOL SINTACTICO\n");
            printf("====================================\n\n");

            if (raiz != NULL) {

                /* Mostrar el arbol sintactico completo */
                mostrar_ast(raiz, 0);

                printf("\n");
                printf("====================================\n");
                printf(" AST SIMPLIFICADO\n");
                printf("====================================\n\n");

                /* Mostrar el AST simplificado */
                mostrar_ast_simplificado(raiz, 0);

            } else {
                printf("No se pudo construir el arbol.\n");
            }

            printf("\n");

            /* Liberar la memoria utilizada */
            limpiar_pila_ast(&pila);

            return resultado;

        /*
         * ERROR:
         * El token no tiene una accion valida.
         */
        } else {

            printf("ERROR SINTACTICO\n");
            printf("Token inesperado: %s\n",
                   nombre_terminal(token));

            resultado.errores++;

            limpiar_pila_ast(&pila);

            return resultado;
        }
    }

    printf("Error: la entrada termino sin ser aceptada.\n");

    resultado.errores++;

    limpiar_pila_ast(&pila);

    return resultado;
}
