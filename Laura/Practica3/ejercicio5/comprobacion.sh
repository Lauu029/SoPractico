#!/bin/bash

count=0
total_runs=1000000

while [ $count -lt $total_runs ]
do
    output=$(./nombre_del_programa)
    expected_output="File contents are:\n00000111112222233333444445555566666777778888899999\n"

    if [ "$output" != "$expected_output" ]
    then
        echo "La salida no coincide en la ejecución $count"
        exit 1
    fi

    ((count++))
done

echo "Todas las ejecuciones produjeron la salida esperada"
