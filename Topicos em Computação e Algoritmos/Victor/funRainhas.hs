module Main where

funRainhas = map length (map nRainhas [1..])

nRainhas :: Int -> [[(Int,Int)]]
nRainhas k = rainhas k 
 where 
   rainhas n
     | n == 0 = [[]]
     | otherwise = [posi ++ [(n,m)] | posi <- rainhas (n-1), m <- [1..k], seguro posi (n,m)]
 
seguro lista nova = and [not (cond rainha nova) | rainha <- lista]
cond (i,j) (m,n) = (j==n) || (m-i == n-j) || (i-m == n-j)

main =
	do
		print("Possiveis solucoes para o problema das 8 rainhas: " ++ show(length (nRainhas 8)))
		print("Possiveis solucoes para o problema das n rainhas: " ++ show(funRainhas))