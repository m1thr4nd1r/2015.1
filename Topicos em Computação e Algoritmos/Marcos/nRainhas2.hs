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
 
-- checks if queens are on the same diagonal
-- with [0..] we place each queen on her own row
check f = length . nub . zipWith f [0..]
 
-- filters out results where 2 or more queens are on the same diagonal
-- with [0..n-1] we place each queeen on her own column
generate n = filter (\x -> check (+) x == n && check (-) x == n) $ permutations [0..n-1]

main = do
		putStrLn("Possiveis solucoes para o problema das 8 rainhas: " ++ show(length (nRainhas 8)))
		putStrLn(show(nRainhas 8))
		putStrLn("")
		--putStrLn("Possiveis solucoes para o problema das 9 rainhas: " ++ show(length (nRainhas 9)))
		--putStrLn(show(nRainhas 9))
		--putStrLn("")
		--putStrLn("Possiveis solucoes para o problema das 10 rainhas: " ++ show(length (nRainhas 10)))
		--putStrLn(show(nRainhas 10))
		--putStrLn("")
		--putStrLn("Possiveis solucoes para o problema das 11 rainhas: " ++ show(length (nRainhas 11)))
		--putStrLn(show(nRainhas 11))
		--putStrLn("")
		print $ generate 8
		--print("Possiveis solucoes para o problema das n rainhas: " ++ show(map nRainhas [1..]))
