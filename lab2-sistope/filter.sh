#!/bin/bash
cpu_min=0
mem_min=0
regex=".*"

# Procesar argumentos de filtrado:
# -c: uso mínimo de CPU
# -m: uso mínimo de memoria
# -r: expresión regular para filtrar por nombre de comando
while getopts 'c:m:r:' OPTION; do
	case "$OPTION" in 
	c)
		cpu_min="$OPTARG"
		;;

	m)
		mem_min="$OPTARG"
		;;

	r)
        regex="$OPTARG"
    ;;

	?)
		exit 1
	;;

	esac
done

# Validar que cpu_min contenga decimales válidos
if ! [[ "$cpu_min" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
    echo "Error: numeros de filtrado de CPU invalidos"
    exit 1
fi
# Validar que mem_min contenga decimales válidos
if ! [[ "$mem_min" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
    echo "Error: numeros de filtrado de memoria invalidos"
    exit 1
fi


# Entradas: stdin (líneas preprocessadas)
# Salidas: stdout (líneas filtradas)
# Descripción: Filtra las líneas según umbrales de CPU, Memoria y Regex de nombre
# Procesar cada línea de entrada
while read -r line; do
	# Extraer campos directamente desde la linea con awk
    timestamp=$(echo "$line" | awk '{print $1}')
    pid=$(echo "$line" | awk '{print $2}')
    uid=$(echo "$line" | awk '{print $3}')
    pcpu=$(echo "$line" | awk '{print $(NF-1)}')
    pmem=$(echo "$line" | awk '{print $NF}')
    comm=$(echo "$line" | awk '{for(i=4;i<=NF-2;++i) printf $i (i<NF-2?" ":""); print ""}')

    # Validar que pcpu y pmem sean decimales válidos
    if ! [[ "$pcpu" =~ ^[0-9]+(\.[0-9]+)?$ && "$pmem" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
        continue
    fi

    # Filtrar por CPU y Memoria usando bc para comparación de flotantes
    # Si pasa el filtro de recursos, verificamos el nombre del comando con regex
    if (( $(echo "$pcpu >= $cpu_min" | bc) )) && (( $(echo "$pmem >= $mem_min" | bc) )); then
        if [[ $comm =~ $regex ]]; then
            # Si cumple todas las condiciones, imprimimos la línea
            echo "$timestamp $pid $uid $comm $pcpu $pmem"
        fi
    fi

done
