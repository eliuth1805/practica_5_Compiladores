# Gramática del analizador SLR(1)

## Descripción

Para esta práctica se definió una gramática que permite
reconocer secuencias de mini-comandos de terminal.

La gramática incluye operaciones para crear, borrar,
copiar, mover, mostrar, buscar y renombrar archivos,
además de listar y ejecutar expresiones.

Se utilizaron 20 producciones y una producción aumentada.

## Producciones

```text
0.  S' → Programa
1.  Programa → Lista
2.  Lista → Lista Comando
3.  Lista → Comando
4.  Comando → crear Archivo ;
5.  Comando → borrar Archivo ;
6.  Comando → copiar Archivo Archivo ;
7.  Comando → mover Archivo Archivo ;
8.  Comando → mostrar Archivo ;
9.  Comando → buscar Archivo ;
10. Comando → renombrar Archivo Archivo ;
11. Comando → listar ;
12. Comando → ejecutar Accion ;
13. Accion → Accion + Elemento
14. Accion → Elemento
15. Elemento → id
16. Elemento → num
17. Elemento → ( Accion )
18. Archivo → id
19. Archivo → ruta
20. Archivo → nombre
```

## Símbolos no terminales

```text
Programa
Lista
Comando
Accion
Elemento
Archivo
```

La producción aumentada utiliza el símbolo S'.

## Símbolos terminales

```text
crear
borrar
copiar
mover
mostrar
buscar
renombrar
listar
ejecutar
id
ruta
nombre
num
+
(
)
;
```

También se utiliza `$` para representar el final de la entrada.

## Símbolo inicial

El símbolo inicial de la gramática es:

```text
Programa
```

Para construir el analizador se utiliza la producción
aumentada:

```text
S' → Programa
```

## Características de la gramática

La gramática permite reconocer uno o varios comandos
consecutivos.

Cada comando debe respetar su estructura y terminar
con punto y coma.

La producción:

```text
Accion → Accion + Elemento
```

introduce recursividad por la izquierda y permite
representar expresiones con varias sumas.

También se permiten expresiones agrupadas mediante
paréntesis.

## Ejemplos de cadenas válidas

```text
crear id ;

listar ;

copiar id ruta ;

ejecutar id + num ;

ejecutar ( id + num ) + id ;
```

## Ejemplos de cadenas inválidas

```text
crear id

copiar id ;

ejecutar id + ;

ejecutar ( id + num ;
```

Estas cadenas no cumplen con alguna de las producciones
de la gramática.

La entrada `eliminar id ;` también se rechaza porque
`eliminar` no pertenece al conjunto de terminales
reconocidos por el programa.
