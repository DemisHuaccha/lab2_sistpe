#!/bin/bash

# Verificar argumento de salida
while getopts 'o:' OPTION; do
    case "$OPTION" in
        o)
            output_file="$OPTARG"
            ;;
        ?)
            echo "Uso: $0 -o <archivo_salida.csv/tsv>"
            exit 1
            ;;
    esac
done

# Validar que se haya proporcionado archivo de salida
if [[ -z "$output_file" ]]; then
    echo "Error: Debes especificar el archivo de salida con -o"
    exit 1
fi

# Obtener metadatos
fecha=$(date --iso-8601=seconds)
usuario=$(whoami)
host=$(hostname)

# Escribir metadatos al archivo
{
    echo "# Fecha de generación: $fecha"
    echo "# Usuario: $usuario"
    echo "# Host: $host"
    echo ""
} > "$output_file"

# Añadir contenido procesado desde stdin
cat >> "$output_file"
