-- Created by Marcos Silva

module Main where

--foldr f b [] = b
--foldr f b (x:xs) = f x (foldr f b xs)
import Data.List;

ou xs = foldr (||) False xs

e xs = foldr (&&) True xs

mais xs = foldr (+) 0 xs

produto xs = foldr (*) 1 xs

listDiff xs1 xs2 = foldl deleti xs1 xs2

deleti [] _ = []
deleti (x:xs) y
	|	x == y	= xs
	|	otherwise	= x : (deleti xs y)

--foldr1 f [x] = x
--foldr1 f (x:xs) = x 'f' (foldr1 f xs)

--foldl1 f (x:xs) = foldl1 f x xs

maximo xs = foldl1 max xs

minimo xs = foldl1 min xs

filtro x = [n |n <- x, even n]

any' p = ou.map p

all' p = e.map p

elem' n xs = any (\x -> x == n) xs

takeWhile' _ [] = []
takeWhile' p (x:xs)
	|	p x 	= x : (takeWhile p xs)
	|	otherwise	= []

dropWhile' _ [] = []
dropWhile' p xs@(x:xs')
	|	p x 	= dropWhile p xs1
	|	otherwise	= xs

main = do
		print(ou [False,False])
		print(ou [True,False])
		print(e [True, True])
		print(e [True, False])
		print(mais [2,3,4])
		print(produto [2,3,4])
		print(listDiff [1,2,4,3] [1,2,5,3])
		print([1,2,4,3]\\[1,2,5,3])
		print(maximo [1,2,3,4])
		print(minimo [5,6,7,4,3])
		print(any' even [1,2,3])
		print(all' even [1,2,3])
		print(elem' 2 [1,3,5])