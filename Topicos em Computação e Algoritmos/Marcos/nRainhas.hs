module Main where

import Data.List (nub, permutations)

nRainhas :: Int -> [[(Int,Int)]]
nRainhas k = rainhas k
	where 
	rainhas n
		| n == 0 = [[]]
		| otherwise = [posi ++ [(n,m)] | posi <- rainhas (n-1), m <- [1..k], seguro posi (n,m)]
 
seguro lista nova = and [not (cond rainha nova) | rainha <- lista]
cond (i,j) (m,n) = (j==n) || (m-i == n-j) || (i-m == n-j)

main = do
		--putStrLn("Quantidade de solucoes para 8 rainhas: " ++ show(length (nRainhas 8)))
		--putStrLn(show(nRainhas 8))
		--putStrLn("")
		--putStrLn("Quantidade de solucoes para 9 rainhas: " ++ show(length (nRainhas 9)))
		--putStrLn(show(nRainhas 9))
		--putStrLn("")
		--putStrLn("Quantidade de solucoes para 10 rainhas: " ++ show(length (nRainhas 10)))
		--putStrLn(show(nRainhas 10))
		--putStrLn("")
		--putStrLn("Quantidade de solucoes para 11 rainhas: " ++ show(length (nRainhas 11)))
		--putStrLn(show(nRainhas 11))
		--putStrLn("")
		print("Quantidade de solucoes para n rainhas: " ++ show(map length (map nRainhas [1..100])))
