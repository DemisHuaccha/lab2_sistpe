#!/bin/bash

#Fraccion de codigo por si se quiere que preprocess.sh convierta a iso8601 si es llamado con --iso8601
#ej: ./preprocess.sh --iso8601
convert_to_iso8601=false
if [[ "$1" == "--iso8601" ]]; then    
    convert_to_iso8601=true
fi


# Entradas: stdin (líneas de texto con formato de ps)
# Salidas: stdout (formato: timestamp pid uid comm pcpu pmem)
# Descripción: Lee líneas de stdin, valida tipos de datos y formatea la salida
# Procesar cada línea de entrada
while read -r line; do
    # Extraer campos directamente desde la linea con awk
    timestamp=$(echo "$line" | awk '{print $1}')
    pid=$(echo "$line" | awk '{print $2}')
    uid=$(echo "$line" | awk '{print $3}')
    pcpu=$(echo "$line" | awk '{print $(NF-1)}')
    pmem=$(echo "$line" | awk '{print $NF}')
    comm=$(echo "$line" | awk '{for(i=4;i<=NF-2;++i) printf $i (i<NF-2?" ":""); print ""}')

    # Validar tipos de datos pid y uid son enteros, pcpu y pmem son decimales
    if ! [[ "$pid" =~ ^[0-9]+$ && "$uid" =~ ^[0-9]+$ && "$pcpu" =~ ^[0-9]+(\.[0-9]+)?$ && "$pmem" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
        continue
    fi

    # Convertir timestamp a ISO8601 si se activó el flag
    # Si la conversión falla (fecha inválida), saltamos la línea
    if $convert_to_iso8601; then
        iso_ts=$(date -d "$timestamp" --iso-8601=seconds 2>/dev/null)
        if [[ -z "$iso_ts" ]]; then
            continue
        fi
        timestamp="$iso_ts"
    fi

    # Imprimir línea procesada
    echo "$timestamp $pid $uid $comm $pcpu $pmem"
done
