#!/bin/bash
echo "> start"
echo "$ compile"
make all || { printf "\e[1;31m: Compilation error.\n" && exit 1; }
echo "$ test"
counter=1
max=2
our_res=-1
bad_res=-1
while [ $counter -le $max ]
do
	./gen
	if [ $? ]
	then
		sleep .01
		./our_micro_paint example_ >ours.out.log 2>ours.err.log
		our_res=$?
		./micro_paint example_ >yours.out.log 2>yours.err.log
		bad_res=$?
		if [ $our_res -ne $bad_res ]
		then
			printf "\n: different return result, our \e[1;31m$our_res\e[0m and yours \e[1;32m$bad_res\e[0m !\n"
			exit 1
		fi
		diff -y --suppress-common-lines ours.out.log yours.out.log
		if [ $? -ne 0 ]
		then
			printf "\e[1;31m: difference in output, coutput is our, output yours and the example is in example_ !\e[0m\n"
			exit 2
		fi
		if [ $(( $counter % 50 )) = 0 ]
		then
			printf "\e[1;34m[$counter]\e[0m"
		fi
		if [ $our_res -ne 0 ]
		then
			printf "\e[1;33m$our_res\e[0m"
		else
			printf "\e[1;32m$our_res\e[0m"
		fi
	fi
	max=$((max + 1))
	counter=$((counter + 1))
done
rm -rf example_ *.out.log *.err.log
printf "\n> done"
