module Main where

main =
    do putStrLn "Do you like Haskell? [yes/no]"
       answer <- getLine
       case answer of
         "yes" -> putStrLn "yay!"
         "no"  -> putStrLn "I am sorry to hear that :("
         _     -> putStrLn "say what???"