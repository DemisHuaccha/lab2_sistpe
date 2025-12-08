#!/bin/bash
#Para empezar a crear el script, se usa el shebang

#VALORES POR DEFECTO DE LAS OPCIONES
intervalo_muestreo=0 #muestreo lo más rapido posible
duracion_total=0 #duración indefinida, no se extraen muestras

# Entradas: sin entradas
# Salidas: sin valores de retorno
# Descripción: Esta función imprime el estado de los procesos en ejecución (separados por guiones)
print_ps(){
    #echo "- - - - - - - - - - - - - - - - - - -"
    ps -eo pid=,uid=,comm=,pcpu=,pmem= --sort=-%cpu | while read -r line; do
    	timestamp=$(date +"%Y-%m-%d %H:%M:%S")
    	iso_ts=$(date -d "$timestamp" --iso-8601=seconds 2>/dev/null)
        if [[ -z "$iso_ts" ]]; then
            continue
        fi
        timestamp="$iso_ts"
        echo "$timestamp $line"
    done
}

#getopts debe ser usado dentro de un ciclo while para que todas las opciones sean "analizadas"
#'it' define las opciones '-i' y '-t'
# cuando se usa ':' quiere decir que la opción requiere un argumento. --> 'i:t' --> i requiere argumento
while getopts 'i:t:' OPTION; do
	case "$OPTION" in
	i)
		intervalo_muestreo="$OPTARG"
		;;
	t)
		duracion_total="$OPTARG"
		;;
	?)
	    #echo "para usar el generador: generador.sh [-i (numero)] [-t (numero)]"
		exit 1
		;;
	esac
done

# Verificar entradas de flags
if ! [[ "$intervalo_muestreo" =~ ^[0-9]+([.][0-9]+)?$ ]] || ! [[ "$duracion_total" =~ ^[0-9]+([.][0-9]+)?$ ]]; then
    echo "Error: Los flag -i y -t deben ser números positivos (enteros o flotantes)."
    exit 1
fi

# Ciclo principal de muestreo de ps
inicio=$SECONDS
while (( $(echo "$SECONDS - $inicio < $duracion_total" | bc -l) )); do
    print_ps
    sleep $intervalo_muestreo
done
exit 1
