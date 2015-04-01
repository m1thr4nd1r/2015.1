f _ [] = []
f a lst
	| a `mod` 2 == 1 = [ ( head lst ) ] ++ ( f (a + 1) (tail lst) )
	| otherwise = f (a + 1) (tail lst)

-- This part deals with the Input and Output and can be used as it is. Do not modify it.
main = do
   inputdata <- getContents
   let 
        numbers = map read (lines inputdata) :: [Int] 
   mapM_ (putStrLn. show). f 0 $ numbers
