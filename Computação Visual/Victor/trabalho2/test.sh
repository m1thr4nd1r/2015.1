# `make SRC=samples.cpp`

echo "Testando..."
# `make SRC=sample.cpp`

for input in `ls ../../imgs/2015*.jpg`
do
	echo ${input}
	$(./a.out ${input})
done

echo "\nFim do Teste"
