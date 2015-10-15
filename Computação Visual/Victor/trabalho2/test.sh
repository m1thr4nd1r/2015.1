# `make SRC=samples.cpp`

echo "Testando..."

for input in `ls ../../imgs/2015*r.jpg`
do
	$(./a.out ${input})
done

echo "\nFim do Teste"
