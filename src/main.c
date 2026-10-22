#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "tablas.h"

/* Convertir una palabra en un token */
int obtener_token(const char *palabra, Terminal *token)
{
    static const char *nombres[] = {
        "crear", "borrar", "copiar", "mover",
        "mostrar", "buscar", "renombrar",
        "listar", "ejecutar", "id", "ruta",
        "nombre", "num", "+", "(", ")", ";"
    };

    for (int i = 0; i < TK_FIN; i++) {
        if (strcmp(palabra, nombres[i]) == 0) {
            *token = (Terminal)i;
            return 1;
        }
    }

    return 0;
}

int main(void)
{
    char entrada[2048];
    Terminal tokens[MAX_TOKENS];

    printf("====================================\n");
    printf(" ANALIZADOR SINTACTICO SLR(1)\n");
    printf(" Mini-Comandos de Terminal\n");
    printf("====================================\n\n");

    inicializar_tablas();

    printf("Introduce una cadena de tokens:\n");
    printf("> ");

    if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
        printf("Error al leer la entrada.\n");
        return 1;
    }

    int cantidad = 0;

    char *palabra = strtok(entrada, " \t\n");

    while (palabra != NULL) {

        Terminal token;

        if (!obtener_token(palabra, &token)) {
            printf("Error: token desconocido '%s'\n", palabra);
            return 1;
        }

        if (cantidad >= MAX_TOKENS - 1) {
            printf("Error: demasiados tokens.\n");
            return 1;
        }

        tokens[cantidad++] = token;

        palabra = strtok(NULL, " \t\n");
    }

    /* Agregar el marcador de fin de entrada */
    tokens[cantidad++] = TK_FIN;

    printf("\nIniciando analisis sintactico...\n\n");

    ResultadoAnalisis resultado = analizar(tokens, cantidad);

    printf("\n====================================\n");
    printf(" RESULTADO DEL ANALISIS\n");
    printf("====================================\n");

    printf("Desplazamientos: %d\n", resultado.desplazamientos);
    printf("Reducciones: %d\n", resultado.reducciones);
    printf("Errores: %d\n", resultado.errores);

    if (resultado.aceptado) {
        printf("Resultado: CADENA VALIDA\n");
        return 0;
    }

    printf("Resultado: CADENA INVALIDA\n");
    return 1;
}
