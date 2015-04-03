module Main where

vectProd :: [Int] -> [Int] -> Int
vectProd [] [] = 0
vectProd (x:xs) (y:ys) 
	| length xs == length ys = x * y + (vectProd xs ys)
	| otherwise = error "length not equals."

lasty :: [Int] -> Int
lasty [] = 0
lasty x = x!!abs (1 - length x)

compose :: (t1 -> t) -> (t2 -> t1) -> t2 -> t
compose f g x = f (g x)

--average :: [Int] -> Fractional]
average :: Fractional a => [a] -> a
average x = sum(x) / fromIntegral(length x)

double :: Int -> Int
double x = 2*x

elementAt :: Int -> [a] -> [a]
elementAt x n = [n!!x]

inicial :: [Int] -> [Int] -> Bool
inicial [] _ = True
inicial (x:xs) [] = False
inicial (x:xs) (y:ys) = (x == y) && inicial xs ys

main = 
	do
		print("1-vectProd")
		print("vectProd " ++ show([1,2,3,4,5]) ++ " " ++ show([6,7,8,9,10]) ++ " : " ++ show(vectProd [1,2,3,4,5] [6,7,8,9,10]))
		print("vectProd [] [] : " ++ show(vectProd [] []))
		print("-------------------------------------------")
		print("2-lasty")
		print("lasty " ++ show([1,2,3,4,5]) ++ " : " ++ show(lasty [1,2,3,4,5]))
		print("lasty [] : " ++ show(lasty []))
		print("-------------------------------------------")
		print("3-compose")
		print("compose :: (t1 -> t) -> (t2 -> t1) -> t2 -> t")
		print("Compose retorna o resultado de F(G(x)). Seu tipo sao duas funcoes que podem ser aplicadas ao mesmo tipo do elemento de resposta")
		print("-------------------------------------------")
		print("4-average")
		print("average " ++ show([1,2,3,4,5]) ++ " : " ++ show(average [1,2,3,4,5]))
		print("-------------------------------------------")
		print("5-currying")
		print("uncurry (+) (3,4) == 7. Pois a funcao uncurry ira aplicar o primeiro argumento (+) como funcao em ambos os elementos do segundo argumento (3,4)")
		print("curry g 3 4 == 7. Pois a funcao curry ira aplicar o primeiro argumento (g) como funcao, tendo os proximos elementos como seus argumentos. Como g(a,b) = a+b, entao g(3,4) = 7")
		print("(+) 3 4 == 7. Pois estamos usando a funcao (+) na notacao prefixa")
		print("3 + 4 == 7. Pois estamos usando a funcao (+) na notacao infixa")
		print("-------------------------------------------")
		print("6-double")
		print("map double " ++ show ([1,4,4,3]) ++ " : " ++ show(map double [1,4,4,3]))
		print("map (double . double) " ++ show([1,4,4,3]) ++ " : " ++ show(map (double . double) [1,4,4,3]))
		print("map double [] " ++ " : " ++ show(map double []))
		print("-------------------------------------------")
		print("7-elementAt")
		print("elementAt " ++ show(4) ++ " " ++ show([1,2,3,4,5]) ++ " : " ++ show(elementAt 4 [1,2,3,4,5]))
		print("-------------------------------------------")
		print("8-inicial")
		print("A funcao inicial verifica se os elementos da primeira lista sao os primeiros elementos da segunda lista.")
		print("inicial " ++ show([1,2,3]) ++ " " ++ show([1,2,3,4,5]) ++ " : " ++ show(inicial [1,2,3] [1,2,3,4,5]))
		print("inicial " ++ show([1,2,4]) ++ " " ++ show([1,2,3,4,5]) ++ " : " ++ show(inicial [1,2,4] [1,2,3,4,5]))
		print("-------------------------------------------")
		print("9- ++ / concat / map")
		print(show([1,2,3]) ++ " ++ " ++ show([4,5]) ++ " : " ++ show([1,2,3] ++ [4,5]) ++ ". Disso temos que ++ concatena duas listas")
		print("map (*2) " ++ show([1,2,3]) ++ " : " ++ show(map (*2) [1,2,3]) ++ ". Disso temos que map aplica o seu primeiro argumento como funcao para cada um dos elementos do seu segundo argumento, que no caso eh uma lista")
		print("concat " ++ show([[1],[2,3]]) ++ " : " ++ show(concat [[1],[2,3]]) ++ ". Disso temos que concat concatena os elementos do seu argumento, uma lista de listas, transformando-o numa unica lista")
		print("-------------------------------------------")
		print("10-numerosPrimos")
		print("numerosPrimos = coador [2 .. ]")
		print("where coador (x:xs) = x:coador [n | n <- xs, mod n x > 0]")
		print("A funcao numerosPrimos chama a funcao coador que por sua vez funciona da seguinte forma: ")
		print("O primeiro elemento da lista, passada por argumento, de coador eh adicionado a lista de numeros primos. Os outros elementos da lista serao eliminados se os mesmos forem multiplos de X. Essa nova lista filtrada eh passada como parametro para uma nova chamada da funcao coador.")
		print("Em resumo, na primeira chamada removemos os elementos multiplos exclusivos de 2, na segunda removemos os elementos multiplos exclusivos de 3, e assim por diante. Ao final desse processo so teremos numeros primos")
		print("-------------------------------------------")