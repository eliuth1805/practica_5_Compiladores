#ifndef TABLAS_H
#define TABLAS_H

#define NUM_ESTADOS 45
#define NUM_PRODUCCIONES 21

/* Tokens terminales */
typedef enum {
    TK_CREAR,
    TK_BORRAR,
    TK_COPIAR,
    TK_MOVER,
    TK_MOSTRAR,
    TK_BUSCAR,
    TK_RENOMBRAR,
    TK_LISTAR,
    TK_EJECUTAR,
    TK_ID,
    TK_RUTA,
    TK_NOMBRE,
    TK_NUM,
    TK_MAS,
    TK_PAR_IZQ,
    TK_PAR_DER,
    TK_PUNTO_COMA,
    TK_FIN,
    NUM_TERMINALES
} Terminal;

/* Simbolos no terminales */
typedef enum {
    NT_PROGRAMA,
    NT_LISTA,
    NT_COMANDO,
    NT_ACCION,
    NT_ELEMENTO,
    NT_ARCHIVO,
    NUM_NO_TERMINALES
} NoTerminal;

/* Tipos de acciones del analizador */
typedef enum {
    ACC_ERROR,
    ACC_SHIFT,
    ACC_REDUCE,
    ACC_ACCEPT
} TipoAccion;

/* Representacion de una accion */
typedef struct {
    TipoAccion tipo;
    int valor;
} Accion;

/* Representacion de una produccion */
typedef struct {
    int lado_izquierdo;
    int longitud;
} Produccion;

/* Tablas del analizador */
extern Accion tabla_action[NUM_ESTADOS][NUM_TERMINALES];
extern int tabla_goto[NUM_ESTADOS][NUM_NO_TERMINALES];
extern Produccion producciones[NUM_PRODUCCIONES];

/* Inicializacion de las tablas */
void inicializar_tablas(void);

#endif
