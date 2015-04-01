module Main where


mult' a 0 = 0
mult' 0 b = 0
mult' a b = (+ a) (mult' a (b-1))
mult a (b:xs) = if a == 0 then [] else (b:xs) ++ (mult (a-1) (b:xs))

main = do 
		putStrLn ( show (mult' 2 3) ) 
		print ( mult 2 ['a','b'] ) 
		-- print (  mult)
