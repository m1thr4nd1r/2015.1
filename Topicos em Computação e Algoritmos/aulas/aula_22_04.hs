module Main where

fact 0 = 1
fact n = n * fact(n-1)

elemento :: Eq a => a -> [a] -> Bool
elemento _ [] = False;
elemento a (x : xs) 
	| a == x = True
	| otherwise = elemento a xs

minimo [x] = x
minimo xs = min ( minimo a ) ( minimo b )
	where (a, b) = splitAt ( length xs `div` 2 ) xs

delete _ [] = []
delete y (x:xs)
	|	x == y	= xs
	|	otherwise	= x : (delete y xs)

sSort :: (Ord a) => [a] -> [a]
sSort [] = []
sSort xs = m : sSort ( delete m xs)
	where m = minimum xs

inserir x [] = [x]
inserir x (y : ys)
	| x <= y = x : y : ys
	| otherwise = y : inserir x ys

iSort :: (Ord a) => [a] -> [a]
iSort [] = []
iSort (x : xs) = inserir x (iSort xs)

bubble [] = []
bubble [x] = [x]
bubble (x : y : xs)
	| x > y = y : bubble (x:xs)
	| otherwise = x : bubble (y : xs)

bSort [] = []
bSort xs = (bSort (init ys)) ++ (last ys)
	where ys = bubble xs

main = do
	putStrLn("Complexidade das funcoes acima.")
	print(minimo [6.0,2,3,4,5])
	print(sSort [5,4,3,2,1])
	print(iSort [5,4,3,2,1])
	print(iSort [5,4,3,2,1])
