#!/bin/bash

# Pruebas automaticas del analizador SLR(1)

set -u

mkdir -p pruebas/resultados

ejecutar_prueba() {
    numero="$1"
    tipo="$2"
    cadena="$3"

    archivo="pruebas/resultados/prueba_${numero}.txt"

    echo "===================================="
    echo " PRUEBA $numero - $tipo"
    echo "===================================="
    echo "Entrada: $cadena"

    printf '%s\n' "$cadena" | ./analizador > "$archivo"

    if grep -q "Resultado: CADENA VALIDA" "$archivo"; then
        resultado="VALIDA"
    elif grep -q "Resultado: CADENA INVALIDA" "$archivo"; then
        resultado="INVALIDA"
    elif grep -q "Error: token desconocido" "$archivo"; then
        resultado="INVALIDA"
    else
        resultado="DESCONOCIDO"
    fi

    echo "Resultado obtenido: $resultado"

    if [ "$resultado" = "$tipo" ]; then
        echo "Estado: CORRECTO"
    else
        echo "Estado: REVISAR"
    fi

    echo "Evidencia: $archivo"
    echo
}

echo "===================================="
echo " PRUEBAS DEL ANALIZADOR SLR(1)"
echo "===================================="
echo

# Cadenas validas
ejecutar_prueba 01 VALIDA "crear id ;"
ejecutar_prueba 02 VALIDA "crear id ; borrar ruta ; listar ;"
ejecutar_prueba 03 VALIDA "copiar id ruta ; mover nombre id ;"
ejecutar_prueba 04 VALIDA "ejecutar id + num + id ;"
ejecutar_prueba 05 VALIDA "ejecutar ( id + num ) + id ;"

# Cadenas invalidas
ejecutar_prueba 06 INVALIDA "crear id"
ejecutar_prueba 07 INVALIDA "copiar id ;"
ejecutar_prueba 08 INVALIDA "ejecutar id + ;"
ejecutar_prueba 09 INVALIDA "ejecutar ( id + num ;"
ejecutar_prueba 10 INVALIDA "eliminar id ;"

echo "===================================="
echo " PRUEBAS FINALIZADAS"
echo "===================================="
