module Main where

import Data.Char
import Data.List
import Data.Function

count x y 
	|	length x > length y = LT
	|	otherwise = GT

listNub (x:xs) = (nub x):listNub xs
listNub [] = []

main = 
	do
		n <- readLn
		x <- readLn
		print(take n (listNub (sortBy count (group (sort (words(map toLower x)))))))