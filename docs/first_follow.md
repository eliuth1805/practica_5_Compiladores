# Conjuntos FIRST y FOLLOW

## Introducción

Para construir el analizador SLR(1) se calcularon los
conjuntos FIRST y FOLLOW de los símbolos no terminales.

El conjunto FIRST indica los terminales con los que
puede comenzar una cadena derivada de un símbolo.

El conjunto FOLLOW contiene los terminales que pueden
aparecer inmediatamente después de un no terminal
en alguna derivación de la gramática.

Estos conjuntos ayudan a determinar las acciones de
reducción y a resolver los conflictos presentes
en la construcción LR(0).

## Conjuntos FIRST

| No terminal | FIRST |
|---|---|
| Programa | {crear, borrar, copiar, mover, mostrar, buscar, renombrar, listar, ejecutar} |
| Lista | {crear, borrar, copiar, mover, mostrar, buscar, renombrar, listar, ejecutar} |
| Comando | {crear, borrar, copiar, mover, mostrar, buscar, renombrar, listar, ejecutar} |
| Accion | {id, num, (} |
| Elemento | {id, num, (} |
| Archivo | {id, ruta, nombre} |

## Justificación de FIRST

### Programa, Lista y Comando

La producción:

    Programa → Lista

indica que FIRST(Programa) depende de FIRST(Lista).

A su vez, Lista puede derivar uno o varios comandos:

    Lista → Lista Comando
    Lista → Comando

Por ello, los tres conjuntos contienen los terminales
que pueden comenzar un comando.

### Archivo

Las producciones:

    Archivo → id
    Archivo → ruta
    Archivo → nombre

permiten identificar directamente sus tres posibles
símbolos iniciales.

### Accion y Elemento

Las producciones:

    Accion → Accion + Elemento
    Accion → Elemento

indican que una acción comienza con un elemento.

Como:

    Elemento → id
    Elemento → num
    Elemento → ( Accion )

ambos conjuntos contienen id, num y el paréntesis
de apertura.

## Conjuntos FOLLOW

| No terminal | FOLLOW |
|---|---|
| Programa | {$} |
| Lista | {$, crear, borrar, copiar, mover, mostrar, buscar, renombrar, listar, ejecutar} |
| Comando | {$, crear, borrar, copiar, mover, mostrar, buscar, renombrar, listar, ejecutar} |
| Accion | {+, ), ;} |
| Elemento | {+, ), ;} |
| Archivo | {id, ruta, nombre, ;} |

## Justificación de FOLLOW

### Programa

Programa es el símbolo inicial, por lo que su
conjunto FOLLOW contiene el marcador de fin:

    FOLLOW(Programa) = {$}

### Lista y Comando

En:

    Lista → Lista Comando

después de Lista aparece Comando, por lo que se
agregan los terminales de FIRST(Comando).

Además, una lista puede terminar al final de
la entrada, lo que incorpora el símbolo $.

Como Comando aparece al final de las producciones
de Lista, hereda los símbolos de FOLLOW(Lista).

### Archivo

Archivo aparece en comandos que reciben uno
o dos argumentos.

Cuando aparece un segundo Archivo, el primero
puede ir seguido de id, ruta o nombre.

Cuando Archivo es el último argumento, debe
ir seguido del punto y coma.

Por eso:

    FOLLOW(Archivo) = {id, ruta, nombre, ;}

### Accion y Elemento

En:

    Accion → Accion + Elemento

después de Accion aparece el operador +.

En:

    Elemento → ( Accion )

después de Accion aparece el paréntesis de cierre.

En:

    Comando → ejecutar Accion ;

después de Accion aparece el punto y coma.

Como Elemento aparece al final de las producciones
de Accion, hereda los mismos símbolos FOLLOW.

Por lo tanto:

    FOLLOW(Accion) = {+, ), ;}
    FOLLOW(Elemento) = {+, ), ;}

## Uso de FOLLOW en SLR(1)

En un analizador LR(0), una reducción puede
considerarse para todos los terminales, lo que
puede producir conflictos.

En SLR(1), cuando un estado contiene un elemento
completo de la forma:

    A → α .

la reducción correspondiente se coloca únicamente
en las columnas de ACTION cuyos terminales
pertenecen a FOLLOW(A).

Esto permite restringir las reducciones y evitar
conflictos que aparecerían en la tabla LR(0).

Los conjuntos FIRST y FOLLOW se utilizan como
parte de la justificación de las tablas SLR(1)
implementadas en el proyecto.
