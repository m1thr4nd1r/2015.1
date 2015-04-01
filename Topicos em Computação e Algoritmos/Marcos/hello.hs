module Main where

main =
    do putStrLn "What is your name?"
       name <- getLine
       putStrLn ("Hello, " ++ name ++ 
                 ". I think you will really like Haskell!")