# Arquitectura del analizador SLR(1)

## Descripción general

El proyecto consiste en un analizador sintáctico SLR(1)
desarrollado en lenguaje C para reconocer una gramática
de mini-comandos de terminal.

El programa recibe una secuencia de tokens y determina
si cumple con las reglas de la gramática definida.

Durante el análisis muestra los estados de la pila,
las operaciones SHIFT y REDUCE y el resultado final.

Cuando la cadena es aceptada, también muestra el árbol
sintáctico completo y una representación simplificada.

## Organización del proyecto

### src/main.c

Es el punto de entrada del programa.

Se encarga de recibir la cadena de tokens, reconocer
los símbolos de entrada y solicitar el análisis sintáctico.

### src/parser.c

Contiene la implementación del analizador SLR(1).

Administra la pila de estados y realiza las operaciones
SHIFT, REDUCE y ACCEPT utilizando las tablas del parser.

También detecta los errores sintácticos.

### src/tablas.c

Contiene las tablas ACTION y GOTO, así como la
información de las producciones de la gramática.

Estas tablas están definidas directamente en el código.

### src/ast.c

Contiene las funciones necesarias para construir,
mostrar y liberar los nodos del árbol sintáctico.

También permite mostrar una vista simplificada que
elimina símbolos innecesarios y representa las sumas
mediante nodos llamados Suma.

### include/

Contiene los archivos de cabecera utilizados para
compartir estructuras, constantes y declaraciones
entre los distintos módulos.

### pruebas/

Contiene un script que ejecuta diez casos de prueba:
cinco cadenas válidas y cinco entradas inválidas.

Las salidas completas se guardan en pruebas/resultados/.

## Funcionamiento general

1. El usuario introduce una secuencia de tokens.
2. El programa identifica los tokens de entrada.
3. El analizador inicializa la pila con el estado 0.
4. Consulta la tabla ACTION utilizando el estado actual
   y el siguiente token.
5. Ejecuta desplazamientos o reducciones según corresponda.
6. Utiliza la tabla GOTO después de cada reducción.
7. Si alcanza ACCEPT, acepta la cadena y muestra los árboles.
8. Si encuentra una acción no definida, reporta un error.

## Resultados de las pruebas

Se realizaron diez pruebas automáticas.

- Cinco cadenas válidas fueron aceptadas.
- Cuatro cadenas con errores sintácticos fueron rechazadas.
- Una cadena con un token desconocido fue rechazada.

Las diez pruebas produjeron el resultado esperado.
