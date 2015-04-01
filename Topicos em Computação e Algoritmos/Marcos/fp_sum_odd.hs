-- f [] = [0]
-- f arr 
--     | odd ( head arr ) = sum (head arr) (f (tail arr))
--     | otherwise = f (tail arr)

f arr = sum (filter odd arr)

main = do
   inputdata <- getContents
   putStrLn $ show $ f $ map (read :: String -> Int) $ lines inputdata
