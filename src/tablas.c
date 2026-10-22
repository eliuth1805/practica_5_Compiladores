#include "tablas.h"

/* Tablas globales */
Accion tabla_action[NUM_ESTADOS][NUM_TERMINALES];
int tabla_goto[NUM_ESTADOS][NUM_NO_TERMINALES];
Produccion producciones[NUM_PRODUCCIONES];

/* Registrar un desplazamiento */
static void shift(int estado, Terminal token, int destino)
{
    tabla_action[estado][token] =
        (Accion){ACC_SHIFT, destino};
}

/* Registrar una reduccion */
static void reduce(int estado, Terminal token, int produccion)
{
    tabla_action[estado][token] =
        (Accion){ACC_REDUCE, produccion};
}

/* Registrar una transicion GOTO */
static void goto_estado(int estado, NoTerminal simbolo, int destino)
{
    tabla_goto[estado][simbolo] = destino;
}

/* Reducciones segun FOLLOW(Lista) y FOLLOW(Comando) */
static void reducir_comandos(int estado, int produccion)
{
    Terminal tokens[] = {
        TK_CREAR, TK_BORRAR, TK_COPIAR,
        TK_MOVER, TK_MOSTRAR, TK_BUSCAR,
        TK_RENOMBRAR, TK_LISTAR,
        TK_EJECUTAR, TK_FIN
    };

    for (int i = 0; i < 10; i++) {
        reduce(estado, tokens[i], produccion);
    }
}

/* Reducciones segun FOLLOW(Archivo) */
static void reducir_archivo(int estado, int produccion)
{
    reduce(estado, TK_ID, produccion);
    reduce(estado, TK_RUTA, produccion);
    reduce(estado, TK_NOMBRE, produccion);
    reduce(estado, TK_PUNTO_COMA, produccion);
}

/* Reducciones segun FOLLOW(Accion) y FOLLOW(Elemento) */
static void reducir_expresion(int estado, int produccion)
{
    reduce(estado, TK_MAS, produccion);
    reduce(estado, TK_PAR_DER, produccion);
    reduce(estado, TK_PUNTO_COMA, produccion);
}

/* Transiciones compartidas por varios estados */
static void inicio_comandos(int estado)
{
    shift(estado, TK_BORRAR, 4);
    shift(estado, TK_BUSCAR, 5);
    shift(estado, TK_COPIAR, 6);
    shift(estado, TK_CREAR, 7);
    shift(estado, TK_EJECUTAR, 8);
    shift(estado, TK_LISTAR, 9);
    shift(estado, TK_MOSTRAR, 10);
    shift(estado, TK_MOVER, 11);
    shift(estado, TK_RENOMBRAR, 12);
}

static void inicio_archivo(int estado)
{
    shift(estado, TK_ID, 15);
    shift(estado, TK_NOMBRE, 16);
    shift(estado, TK_RUTA, 17);
}

static void inicio_elemento(int estado)
{
    shift(estado, TK_PAR_IZQ, 21);
    shift(estado, TK_ID, 24);
    shift(estado, TK_NUM, 25);
}

/* Inicializar todas las tablas */
void inicializar_tablas(void)
{
    /* Inicializar ACTION con errores y GOTO con -1 */
    for (int i = 0; i < NUM_ESTADOS; i++) {

        for (int j = 0; j < NUM_TERMINALES; j++) {
            tabla_action[i][j] = (Accion){ACC_ERROR, -1};
        }

        for (int j = 0; j < NUM_NO_TERMINALES; j++) {
            tabla_goto[i][j] = -1;
        }
    }

    /* Producciones de la gramatica */

    producciones[0] = (Produccion){NT_PROGRAMA, 1};

    producciones[1] = (Produccion){NT_PROGRAMA, 1};

    producciones[2] = (Produccion){NT_LISTA, 2};
    producciones[3] = (Produccion){NT_LISTA, 1};

    producciones[4] = (Produccion){NT_COMANDO, 3};
    producciones[5] = (Produccion){NT_COMANDO, 3};
    producciones[6] = (Produccion){NT_COMANDO, 4};
    producciones[7] = (Produccion){NT_COMANDO, 4};
    producciones[8] = (Produccion){NT_COMANDO, 3};
    producciones[9] = (Produccion){NT_COMANDO, 3};
    producciones[10] = (Produccion){NT_COMANDO, 4};
    producciones[11] = (Produccion){NT_COMANDO, 2};
    producciones[12] = (Produccion){NT_COMANDO, 3};

    producciones[13] = (Produccion){NT_ACCION, 3};
    producciones[14] = (Produccion){NT_ACCION, 1};

    producciones[15] = (Produccion){NT_ELEMENTO, 1};
    producciones[16] = (Produccion){NT_ELEMENTO, 1};
    producciones[17] = (Produccion){NT_ELEMENTO, 3};

    producciones[18] = (Produccion){NT_ARCHIVO, 1};
    producciones[19] = (Produccion){NT_ARCHIVO, 1};
    producciones[20] = (Produccion){NT_ARCHIVO, 1};

    /* Estados I0 a I3 */

    inicio_comandos(0);

    goto_estado(0, NT_COMANDO, 1);
    goto_estado(0, NT_LISTA, 2);
    goto_estado(0, NT_PROGRAMA, 3);

    reducir_comandos(1, 3);

    inicio_comandos(2);
    reduce(2, TK_FIN, 1);
    goto_estado(2, NT_COMANDO, 13);

    tabla_action[3][TK_FIN] = (Accion){ACC_ACCEPT, 0};

    /* Estados I4 a I12 */

    inicio_archivo(4);
    goto_estado(4, NT_ARCHIVO, 14);

    inicio_archivo(5);
    goto_estado(5, NT_ARCHIVO, 18);

    inicio_archivo(6);
    goto_estado(6, NT_ARCHIVO, 19);

    inicio_archivo(7);
    goto_estado(7, NT_ARCHIVO, 20);

    inicio_elemento(8);
    goto_estado(8, NT_ACCION, 22);
    goto_estado(8, NT_ELEMENTO, 23);

    shift(9, TK_PUNTO_COMA, 26);

    inicio_archivo(10);
    goto_estado(10, NT_ARCHIVO, 27);

    inicio_archivo(11);
    goto_estado(11, NT_ARCHIVO, 28);

    inicio_archivo(12);
    goto_estado(12, NT_ARCHIVO, 29);

    /* Estados I13 a I20 */

    reducir_comandos(13, 2);

    shift(14, TK_PUNTO_COMA, 30);

    reducir_archivo(15, 18);
    reducir_archivo(16, 20);
    reducir_archivo(17, 19);

    shift(18, TK_PUNTO_COMA, 31);

    inicio_archivo(19);
    goto_estado(19, NT_ARCHIVO, 32);

    shift(20, TK_PUNTO_COMA, 33);

    /* Estados I21 a I29 */

    inicio_elemento(21);
    goto_estado(21, NT_ACCION, 34);
    goto_estado(21, NT_ELEMENTO, 23);

    shift(22, TK_MAS, 35);
    shift(22, TK_PUNTO_COMA, 36);

    reducir_expresion(23, 14);
    reducir_expresion(24, 15);
    reducir_expresion(25, 16);

    reducir_comandos(26, 11);

    shift(27, TK_PUNTO_COMA, 37);

    inicio_archivo(28);
    goto_estado(28, NT_ARCHIVO, 38);

    inicio_archivo(29);
    goto_estado(29, NT_ARCHIVO, 39);

    /* Estados I30 a I39 */

    reducir_comandos(30, 5);
    reducir_comandos(31, 9);

    shift(32, TK_PUNTO_COMA, 40);

    reducir_comandos(33, 4);

    shift(34, TK_PAR_DER, 41);
    shift(34, TK_MAS, 35);

    inicio_elemento(35);
    goto_estado(35, NT_ELEMENTO, 42);

    reducir_comandos(36, 12);
    reducir_comandos(37, 8);

    shift(38, TK_PUNTO_COMA, 43);
    shift(39, TK_PUNTO_COMA, 44);

    /* Estados I40 a I44 */

    reducir_comandos(40, 6);
    reducir_expresion(41, 17);
    reducir_expresion(42, 13);
    reducir_comandos(43, 7);
    reducir_comandos(44, 10);
}
