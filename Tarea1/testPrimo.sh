#!/bin/bash

p="primo"
primos=(2 3 4 3010349 3010351 2305843009213693951 137083915467899401)
expected=($p $p 2 $p 191 $p 370248451)
len=${#expected[@]}

for (( i=0; i<$len; i++ )); 
do
	echo
	echo "Running test number $( expr $i + 1)" ;
	echo "Your result: " ;
	./primo ${primos[$i]} 20;
	echo "Expected result: ${expected[$i]}"
	echo "============================================"
done

