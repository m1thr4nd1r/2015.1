module Factorial where

factorial :: Integer -> Integer
factorial x = if x > 0 then x * factorial (x - 1) else 1