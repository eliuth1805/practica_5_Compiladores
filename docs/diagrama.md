# Diagrama de arquitectura del analizador SLR(1)

El siguiente diagrama representa la organización y el
funcionamiento general del analizador sintáctico.

```mermaid
flowchart TD
    A["Usuario"] --> B["Entrada de tokens"]
    B --> C["main.c"]

    C --> D{"¿Tokens reconocidos?"}

    D -->|No| E["Error: token desconocido"]
    D -->|Sí| F["parser.c"]

    F --> G["Pila de estados"]
    F --> H["tablas.c"]

    H --> I["Tabla ACTION"]
    H --> J["Tabla GOTO"]
    H --> K["Producciones"]

    I --> L{"Acción"}
    J --> L
    K --> L
    G --> L

    L -->|SHIFT| M["Desplazar token"]
    L -->|REDUCE| N["Aplicar producción"]
    L -->|ACCEPT| O["Cadena aceptada"]
    L -->|Error| P["Error sintáctico"]

    M --> F
    N --> Q["Actualizar pila y árbol"]
    Q --> F

    O --> R["ast.c"]
    R --> S["Árbol sintáctico completo"]
    R --> T["Vista simplificada del árbol"]

    E --> U["Fin"]
    P --> U
    S --> U
    T --> U
```

## Descripción

El programa comienza cuando el usuario introduce una
secuencia de tokens.

El archivo main.c identifica los tokens y los envía
al analizador implementado en parser.c.

El analizador utiliza una pila de estados y consulta
las tablas ACTION y GOTO definidas en tablas.c.

Dependiendo de la acción encontrada, realiza un
desplazamiento, una reducción, acepta la cadena
o informa de un error sintáctico.

Durante las reducciones se construye el árbol sintáctico.

Cuando la cadena es aceptada, el módulo ast.c permite
mostrar tanto el árbol completo como una representación
simplificada de su estructura.
