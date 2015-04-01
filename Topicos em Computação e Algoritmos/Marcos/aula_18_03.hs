module Main where

area r = pi * r * r

e = 2.7177281828

volEsfera r = 4 / 3 * pi * (cube r)
	where cube x = x * x * x

vol r = let
			cube x = x * x * x
		in
			4.0 / 3.0 * pi * (cube r)

fact n  = if (n == 0)
			then 1
		  	else n * fact (n-1)

fact2 n
	|	n == 0		= 1
	| 	otherwise 	= n * fact (n-1)

fact3 0 = 1
fact3 n = n * fact3(n-1) 

f x y z = x * y + z -- infixa

f2 x y z = (+) ((*) x y) z -- prefixa

g x y = (mod x y) + (div x y)

--mod x y
--equivale a
--x 'mod' y

--div x y
--equivale a
--x 'div' y

--Integers(tamanho limitado) = Int (42)
--Integers(tamanho ilimitado) = Integer (73227891243)
--Reals = Float (3.1415927)
--Booleans = Bool (False)
--Characters = Char ('a')
--Strings = String ("hello")

t1 = (3,True)

roots (a,b,c) = (r1,r2)
	where
	r1 = (-b + r) / f
	r2 = (-b - r) / f
	f = 2 * a
	r 
		| d >= 0 	= sqrt d
		| d < 0		= error "imaginary roots"
	d 				= b * b - 4 * a * c

length 

last [x] = x
last (_:xs) = last (xs)
last [] = error "ta vazia"

--(x:_) !! 0 = x
--(_:y) !! n = y !! (n-1)
main = do 
 		z <- readLn
 		i <- readLn
 		j <- readLn
 		n <- readLn
 		putStrLn( show ( volEsfera z ) )
 		putStrLn( show ( vol z) )
 		print ( ( ( fact z == fact2 z ) && (fact z == fact3 z ) ) )
 		putStrLn(show (roots (i,j,n) ) )
 