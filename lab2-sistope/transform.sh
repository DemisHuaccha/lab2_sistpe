#!/bin/bash

# Verificar flag para anonimizar el UID
anon_uid=false # por defecto, desactivado
if [[ "$1" == "--anon-uid" ]]; then
    anon_uid=true
fi

# Entrada: recibe el uid del proceso (numero entero)
# Salida: hash del uid en formato SHA256zoom (retorno de echo)
# Descripción: Esta función anonimiza el UID del proceso usando SHA256 y lo trunca a sus primeros 8 caracteres
anonimizar_uid() {
    echo -n "$1" | sha256sum | awk '{print substr($1,1,8)}'
}

# Procesar cada línea de entrada
while read -r line; do
    # Extraer campos
    timestamp=$(echo "$line" | awk '{print $1}')
    pid=$(echo "$line" | awk '{print $2}')
    uid=$(echo "$line" | awk '{print $3}')
    pcpu=$(echo "$line" | awk '{print $(NF-1)}')
    pmem=$(echo "$line" | awk '{print $NF}')
    comm=$(echo "$line" | awk '{for(i=4;i<=NF-2;++i) printf $i (i<NF-2?" ":""); print ""}')

    # Anonimizar UID si se indicó
    if $anon_uid; then
        uid=$(anonimizar_uid "$uid")
    fi

    # Imprimir línea transformada
    echo "$timestamp $pid $uid $comm $pcpu $pmem"
done
