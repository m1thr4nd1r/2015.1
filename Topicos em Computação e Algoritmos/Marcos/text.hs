module Main where

import Data.Char
import Data.List
import Data.Function

type Text = [Char]
type Word = [Char]

-- qsort [] = []
-- qsort (x@(_, b):xs) = let m1 = filter (\(z1,z2) -> z2 >= b) xs
--                           m2 = filter (\(z1,z2) -> z2 < b) xs
--                      in (qsort m1) ++ [x] ++ (qsort m2)
sortGT (a1, b1) (a2, b2)
  | b1 < b2 = GT
  | b1 > b2 = LT
  | b1 == b2 = compare a1 a2

sortG a b
  | length a < length b = GT
  | length a > length b = LT
  | length a == length b = compare a b

agrupar w = map (\l@(x:xs) -> (x,length l)) ( w )

printf (a, b) = a ++ ": " ++ ( show b )

countWords n x = intercalate "\n" (take n (map printf ( sortBy sortGT ( sort ( agrupar ( group (sort (words (map toLower x) ) ) ) ) ) ) ) )
countWords2 x = intercalate "\n" (map printf ( agrupar ( sortBy sortG ( group (sort (words (map toLower x) ) ) ) ) ) ) 

main = 
	do
		n <- readLn
		x <- readLn
		putStrLn ( countWords n x )
		-- print ( sortBy sortG ( group (sort (words (map toLower x) ) ) ) ) 
		-- print ( sortBy sortGT ( sort ( map (\l@(x:xs) -> (x,length l)) ( group (sort (words (map toLower x) ) ) ) ) ) )
		-- print ( sortBy sortGT ( sort ( agrupar ( group (sort (words (map toLower x) ) ) ) ) ) )
