#!/bin/bash

# Asociar arrays para métricas por comando
declare -A count
declare -A sum_cpu
declare -A max_cpu
declare -A sum_mem
declare -A max_mem

# Leer línea por línea
while read -r line; do
    # Extraer los últimos tres campos: comm, pcpu, pmem
    comm=$(echo "$line" | awk '{for(i=4;i<=NF-2;++i) printf $i (i<NF-2?" ":""); print ""}')
    pcpu=$(echo "$line" | awk '{print $(NF-1)}')
    pmem=$(echo "$line" | awk '{print $NF}')

    # Validar que pcpu y pmem sean decimales válidos
    if ! [[ "$pcpu" =~ ^[0-9]+(\.[0-9]+)?$ && "$pmem" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
        continue
    fi

    # Inicializar si es la primera vez
    if [[ -z "${count[$comm]}" ]]; then
        count[$comm]=0
        sum_cpu[$comm]=0
        max_cpu[$comm]=0
        sum_mem[$comm]=0
        max_mem[$comm]=0
    fi

    # Acumular métricas
    ((count[$comm]++))
    sum_cpu[$comm]=$(echo "${sum_cpu[$comm]} + $pcpu" | bc)
    sum_mem[$comm]=$(echo "${sum_mem[$comm]} + $pmem" | bc)

    # Actualizar máximos
    if (( $(echo "$pcpu > ${max_cpu[$comm]}" | bc -l) )); then
        max_cpu[$comm]=$pcpu
    fi
    if (( $(echo "$pmem > ${max_mem[$comm]}" | bc -l) )); then
        max_mem[$comm]=$pmem
    fi
done

# Imprimir encabezado
echo -e "comm\tcount\tcpu_avg\tcpu_max\tmem_avg\tmem_max"

# Calcular promedios y mostrar resultados
for comm in "${!count[@]}"; do
    cpu_avg=$(echo "scale=2; ${sum_cpu[$comm]} / ${count[$comm]}" | bc)
    mem_avg=$(echo "scale=2; ${sum_mem[$comm]} / ${count[$comm]}" | bc)
    echo -e "$comm\t${count[$comm]}\t$cpu_avg\t${max_cpu[$comm]}\t$mem_avg\t${max_mem[$comm]}"
done
