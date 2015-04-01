module Main where

import Data.List (delete)

foldt            :: (a -> a -> a) -> a -> [a] -> a
foldt f z []     = z
foldt f z [x]    = x
foldt f z xs     = foldt f z (pairs f xs)
 
foldi            :: (a -> a -> a) -> a -> [a] -> a
foldi f z []     = z
foldi f z (x:xs) = f x (foldi f z (pairs f xs))
 
pairs            :: (a -> a -> a) -> [a] -> [a]
pairs f (x:y:t)  = f x y : pairs f t
pairs f t        = t

perms [] = [[]]
perms xs = [ x : p | x <- xs, p <- perms(delete x xs)]

primes = sieve [2..] 
  where sieve (p:xs) = 
          p : sieve [x | x <- xs, x `mod` p /= 0]

fibs = 0:1:zipWith (+) fibs (tail fibs)

f 1 k = 1
f n k = ((k-1) + (f (n-1) k)) `mod` n + 1

main = do
	line <- getLine
	putStrLn line
	print (take 15 fibs)
	print (perms "123")
	print (map sum [[0,2,4,6,8,10],[1,3,5,7,9,11]])
	print (zip [0,2,4,6,8,10] [1,3,5,7,9,11])
	print (unzip [(0,1),(2,3),(4,5),(6,7),(8,9),(10,11)])
	print (foldr (\x y -> concat ["(",x,"+",y,")"]) "0" (map show [1..13]))
	print (foldl (\x y -> concat ["(",x,"+",y,")"]) "0" (map show [1..13]))
	print (foldt (\x y -> concat ["(",x,"+",y,")"]) "0" (map show [1..13]))
	print (foldi (\x y -> concat ["(",x,"+",y,")"]) "0" (map show [1..13]))
	print (map (subtract 1 . (*2)) [1..5])
	print (f 10 3)
	print (f 5 2)
	print (f 6 3)
	print (f 1234 233)