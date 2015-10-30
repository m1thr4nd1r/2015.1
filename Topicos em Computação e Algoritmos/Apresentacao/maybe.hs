module Main where

	import Data.Maybe

	-- Usando MONAD
	-- Para MONAD, a operação de ligação passa através Just, enquanto Nothing vai forçar o resultado seja sempre Nothing.
	f::Int -> Maybe Int
	f 0 = Nothing
	f x = Just x
	 
	g :: Int -> Maybe Int
	g 100 = Nothing
	g x = Just x

	h ::Int -> Maybe Int
	h x = case f x of
	     Just n -> g n
	     Nothing -> Nothing

	-- Usando FUNCTOR
	-- Para Functor, a função fmap se move dentro do construtor Just e é a identidade do construtor Nothing.
	h' :: Int -> Maybe Int
	h' x = do 
			n <- f x
			g n

	main = 
		do 	
			let h'0 = h' 0
			let g10 = g 10
			putStrLn $ "f 0: " ++ (show (f 0))
			putStrLn $ "f 100: " ++ (show (f 100))
			putStrLn $ "g 0: " ++ (show (g 0))
			putStrLn $ "g 100: " ++ (show (g 100))
			putStrLn $ "h 0: " ++ (show (h 0))
			putStrLn $ "h 100: " ++ (show (h 100))
			putStrLn $ "h' 0: " ++ (show h'0)
			putStrLn $ "h' 100: " ++ (show (h' 100))
			putStrLn $ "f 0 = g 100 = h 0 = h = 100 = h' 0 = h' 100 = Nothing"
			putStrLn $ "f x = g x = h x = h' x = Just x"
			putStrLn $ "g 10: " ++ (show g10)
			putStrLn $ "isJust g 10: " ++ (show (isJust g10))
			putStrLn $ "isNothing g 10: " ++ (show (isNothing g10))
			putStrLn $ "maybeToList g 10 " ++ (show (maybeToList g10))
			putStrLn $ "maybeToList h' 0 " ++ (show (maybeToList h'0))
			putStrLn $ "Map Maybe G  [1..101]: " ++ (show (mapMaybe g  [0..101]))
			putStrLn $ "Map Maybe H' [1..101]: " ++ (show (mapMaybe h' [0..101]))
