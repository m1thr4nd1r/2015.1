module Main where

vectProd :: [Int] -> [Int] -> Int
vectProd [] [] = 0
vectProd (x:xs) (y:ys) 
	| length xs == length ys = x * y + (vectProd xs ys)
	| otherwise = error "length not equals."

ultimo' :: [a] -> a
ultimo' (_:xs) = ultimo' xs

ultimo :: [a] -> a
ultimo (x:xs) = (!!) (x:xs) ((length (x:xs)) - 1)

media :: Fractional a => [a] -> a
media (x:xs) = ((sum (x:xs)) / (fromIntegral (length (x:xs))))

f :: Num a => a -> a -> a
f a b = a + b

g :: Num a => (a, a) -> a
g (a,b) = a + b

curry' :: ((t1, t2) -> t) -> t1 -> t2 -> t
curry' f x y = f (x, y)

uncurry' :: (a -> b -> t) -> (a, b) -> t
uncurry' f p = f (fst p) (snd p)

double :: Num a => a -> a
double x = 2 * x

elementAt :: Int -> [a] -> [a]
elementAt a (xs) = [((!!) xs a)]

inicial [] _ = True
inicial (x:xs) [] = False
inicial (x:xs) (y:ys) = (x == y) && inicial xs ys

listSoma :: [Integer] -> Integer
listSoma = foldr (+) 0 

listProduto :: [Integer] -> Integer
listProduto = foldr (*) 1

append :: [a] -> [a] -> [a]
append xs ys = foldr (:) ys xs

concat' :: [[a]] -> [a]
concat' = foldr append []

map' :: (a -> a1) -> [a] -> [a1]
map' f = foldr (\x ys -> (f x):ys) []

numerosPrimos :: [Integer]
numerosPrimos = coador [2 .. ]
	where coador (x:xs) = x:coador [n | n <- xs, mod n x > 0]

main = do
	putStrLn "1. VectProd."
	print (vectProd [1,3,5,7,9] [0,2,4,6,8])
	print (vectProd [1,2] [3,4])
	-- print (vectProd [1,2] [3]) -- ERROR
	putStrLn "-------------------"
	putStrLn "2. Ultimo."
	print (ultimo [1,3,5,7,9])
	print (ultimo [1,2])
	putStrLn "-------------------"
	putStrLn "3. O tipo de compose."
	putStrLn "compose :: (t1 -> t) -> (t2 -> t1) -> t2 -> t"
	putStrLn "compose faz uma composicao de 2 funcoes"
	putStrLn "t2 eh valor que vai ser aplicado a composicao de funcao"
	putStrLn "(t2 -> t1) a funcao pega algo do tipo t2 e retorna algo do tipo t1"
	putStrLn "(t1 -> t) a funcao pega algo do tipo t1 e retorna algo do tipo t"
	putStrLn "t eh tipo de retorno"
	putStrLn "-------------------"
	putStrLn "4. Media."
	print (media [10,10,10])
	print (media [1,2,3,4,5,6])
	putStrLn "-------------------"
	putStrLn "5. Curry e Uncurry"
	print (uncurry' (+) (3,4))
	print (curry' g 3 4)
	print ((+) 3 4)
	print (3 + 4)
	putStrLn "As funcoes curry e uncurry, neste caso, executam a acao de somar 2 numeros."
	putStrLn "Curry eh o processo de transformar uma funcao que pega varios elementos e funcao de apenas um argumentos. "
	putStrLn "Uncurry eh o processo inverso ao curry."
	putStrLn "-------------------"
	putStrLn "6. Double"
	putStrLn "O map farah com que a fn double seja aplicado a cada elemento da lista."
	print (map double [1,4,4,3]) --[2,8,8,6]
	print (map (double . double) [1,4,4,3]) --[4,16,16,12]
	print (map double []) --[]
	putStrLn "-------------------"
	putStrLn "7. ElementAt"
	print (elementAt 1 [1,2,3,4,5,6,7,8,9,10])
	print (elementAt 0 [3,4,5])
	putStrLn "-------------------"
	putStrLn "8. Inicial"
	putStrLn "Checa se uma lista eh prefixo da outra"
	print (inicial [1,2,3,4] [1,2,3,4,5,6,7,8,9,10])
	print (inicial [2,3,4] [3,4,5])
	print (inicial [2,3,5] [2,3,4,5])
	putStrLn "-------------------"
	putStrLn "9. Teste Funcoes"
	putStrLn ("listSoma    => retorna a soma dos elementos de uma lista. ex listSoma [1,2,3,4,5] = " ++ (show (listSoma [1,2,3,4,5]))) 
	putStrLn ("listProduto => retorna o produto dos elementos de uma lista. ex listProduto [1,2,3,4,5] = " ++ (show (listProduto [1,2,3,4,5]))) 
	putStrLn ("append      => adicionar uma lista no final de outra e retorna a nova lista. ex append [1,2] [3,4] = [1,2,3,4]")
	putStrLn ("concat'     => concatena os elementos de uma lista de lista retorna a nova lista. ex concat' [[1,2],[3,4]] = [1,2,3,4]")
	putStrLn ("map'        => aplica uma funcao em cada elemento da lista. ex map' (+ 2) [3,4] = [5,6]")
	putStrLn "-------------------"
	putStrLn "10. Primos infinito"
	putStrLn "coador [2 .. ] => retorna uma lista de tamanho infinito iniciando em 2."
	putStrLn "x:coador [n | n <- xs, mod n x > 0] => x eh o topo da lista, tal que n seja um elemento da lista e o mod x de n seja maior que zero."
	print (numerosPrimos)
