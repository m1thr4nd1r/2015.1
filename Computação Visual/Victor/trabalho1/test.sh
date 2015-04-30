make test SRC=src.cpp CODE=stereo.cpp OUT=b.out

echo "Testando..."

for input in `ls -d */`
do
	out=`basename ${input}`.sol
	exec=`./b.out ${input} 1 > ${out}`
	res=$?
	echo ${exec}	
	if [ $res -ne 0 ]; then
		echo $exec "\n"
	fi
		
	saida=`basename ${input}`.out
	exec=`./a.out ${input} 1 > ${saida}`
	res=$?
	if [ $res -ne 0 ]; then
		echo $exec "\n"
	fi
	
	if [ -e $out ]; then
		if [ -e $saida]; then
			echo $out " | " $saida
		else
			echo "Falta " $saida "\n"
		fi
	else
		echo "Falta " $out "\n"
	fi
done
