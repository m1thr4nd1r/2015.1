module Main where

import Data.Ix
import Data.Array

-- Representando usando tuplas
type Graph n w = [(n,[(n, w)])]

-- Representando usando ponteiro
data Graph' n w = Vertex n [((Graph' n w), w)]

graphPTR = v1
	where
		v1 = Vertex 1 [(v2, 12),(v3, 34),(v5, 78)]
		v2 = Vertex 2 [(v1, 12),(v4, 55),(v5, 32)]
		v3 = Vertex 3 [(v1, 34),(v4, 61),(v5, 44)]
		v4 = Vertex 4 [(v2, 55),(v3, 61),(v5, 93)]
		v5 = Vertex 5 [(v1, 78),(v2, 32),(v3, 44),(v4,93)]

-- Representando usando lista de adjacencia
type Graph'' n w = Array n [(n, w)]

graphAL = array (1, 5) [(1, [(2, 12), (3, 34), (5, 78)]),
						(2, [(1, 12), (4, 55), (5, 32)]),
						(3, [(1, 34), (4, 61), (5, 44)]), 
						(4, [(2, 55), (3, 61), (5, 93)]),
						(5, [(1, 78), (2, 32), (3, 44), (4, 93)])]

mkGraph'' :: (Ix n, Num w) => Bool -> (n,n) -> [(n,n,w)] -> (Graph'' n w)
mkGraph'' dir bnds es = 
	accumArray (\xs x -> x:xs) [] bnds
				([(x1, (x2, w)) | (x1, x2, w) <- es] ++
					if dir then []
					else [(x2, (x1, w)) | (x1, x2, w) <- es, x1 /= x2])

graphAL' = mkGraph'' False (1, 5) [(1, 2, 12), (1, 3, 34), (1, 5, 78),
								   (2, 4, 55), (2, 5, 32), (3, 4, 61),
								   (3, 5, 44), (4, 5, 93)]


main =
	do 
		putStrLn("Marcos")
		--print( show graphPTR )
