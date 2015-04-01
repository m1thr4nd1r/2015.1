module Main where

fact :: Integer -> Integer
fact3 x = if x > 0 then x * fact (x - 1) else 1

fact n
	| n == 0 = 1
	| n > 0 = n * fact2(n-1)

sumInt n
	| n == 0 = 0
	| n > 0 = n + sumInt(n-1)

sumSqr n
	| n == 0 = 0
	| n > 0 = n * n + sumSqr(n-1)

induction base comb n
	| n == 0 = base
	| n > 0 = comb n (induction base comb (n-1))

fact2 = induction 1 (*)
sumInt2 = induction 0 (+)
sumSqr2 = induction 0 ( \ x y -> x * x + y ) 

main = 
	do 
		x <- readLn
		putStrLn ( "SumInt: " ++ show ( sumInt x ) ) 
		putStrLn ( "sumSqr: " ++ show ( sumSqr x ) ) 
		putStrLn ( "Factorial: " ++ show ( fact x ) ) 
		putStrLn " :: INDUCTION :: "
		putStrLn ( "SumInt: " ++ show ( sumInt2 x ) ) 
		putStrLn ( "sumSqr: " ++ show ( sumSqr2 x ) ) 
		putStrLn ( "Factorial: " ++ show ( fact2 x ) ) 
		