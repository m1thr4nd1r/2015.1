module Main where

import Data.Ix
import Data.Array
import Data.Maybe

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

mkGraph' :: (Ix n, Num w) => Bool -> (n,n) -> [(n,n,w)] -> (Graph'' n w)
mkGraph' dir bnds es = 
	accumArray (\xs x -> x:xs) [] bnds
				([(x1, (x2, w)) | (x1, x2, w) <- es] ++
					if dir then []
					else [(x2, (x1, w)) | (x1, x2, w) <- es, x1 /= x2])

adjacent g v = map fst (g!v)
-- Aplica fst a todos os elementos de indice v do grafo g

nodes g = indices g
-- Aplica indices, que retorna a lista de indices == vertices, no grafo g

edgeIn g (x,y) = elem y (adjacent g x)
-- Aplica a função elem, que retorna True se o elemento y esta na lista, a lista dos vertices adjacentes de x

weight x y g = head [c | (a,c) <- g ! x, (a == y)]
-- Cria uma lista com todos os c (pesos), onde (a,c) pertence ao elemento x (vertice) do grafo g, sendo a == y. retorna a cabeça dessa lista

edgesD g = [(v1,v2,w) | v1 <- nodes g ,(v2, w) <- g ! v1]
-- Retorna cada aresta (v1,v2,w), onde v1 pertence aos nós do grafo g, e (v2,w) pertence ao elemento v1 do grafo g

edgesU g = [(v1,v2,w) | v1 <- nodes g ,(v2, w) <- g ! v1, v1 < v2]
-- Retorna cada aresta (v1,v2,w), onde v1 pertence aos nós do grafo g, (v2,w) pertence ao elemento v1 do grafo g e v1 é menor que v2

-- FIM usando lista de adjacencia 

-- INI matrix de adjacencia

type Graph''' n w = Array (n, n) (Maybe w)

--mkGraph'' :: (Ix n, Num w) => Bool -> (n,n) -> [(n,n,w)] -> (Graph''' n w)
mkGraph'' dir bnds@(1,u) es
		= emptyArray // ([((x1,x2), Just w) | (x1,x2,w) <- es] ++
					     if dir then []
					     else [((x2,x1), Just w) | (x1,x2,w) <- es, x1/=x2])
		    where emptyArray
				 = array ((1,1),(u,u)) [((x1,x2),Nothing) |
									x1 <- range bnds,
									x2 <- range bnds]

adjacent' g1 v1 = [ v2 | v2 <- nodes' g1, (g1 ! (v1,v2)) /= Nothing ]
-- Retorna cada elemento v2, dado que v2 é um elemento [nó] do grafo g1, e que, no grafo, o elemento [aresta] (v1,v2) != Nothing

nodes' g1 = range (1,u) where ((1,_),(u,_)) = bounds g1
-- Retorna o range entre 1 e u, onde u é a cabeça do segundo elemento de bounds, que retorna os limites da matriz

edgeIn' g1 (x,y) = (g1!(x,y)) /= Nothing
-- Retorna True se o elemento [aresta] na posição (x,y), no grafo g1, é != Nothing

weight' x y g1 = w where (Just w) = g1!(x,y)
-- Retorna o valor do elemento [aresta] na posição (x,y), no grafo g1, se o mesmo não é Nothing

--edgesD' :: (Graph''' n w) => [(Ix a, Ix b)]
edgesD' g1 = [(v1,v2) | v1 <- nodes' g1, v2 <- nodes' g1, edgeIn' g1 (v1,v2)]

edgesU' g = [(v1,v2) | v1 <- nodes' g1, v2 <- range (v1,u), edgeIn' g1 (v1,v2)]
			where (_,(u,_)) = bounds g1

g = mkGraph' False (1, 5) [(1, 2, 12), (1, 3, 34), (1, 5, 78),
								   (2, 4, 55), (2, 5, 32), (3, 4, 61),
								   (3, 5, 44), (4, 5, 93)]
--graphAL' = g

g1 = mkGraph'' False (1, 5) [(1, 2, 12), (1, 3, 34), (1, 5, 78),
								   (2, 4, 55), (2, 5, 32), (3, 4, 61),
								   (3, 5, 44), (4, 5, 93)]

-- FIM matrix de adjacencia

-- Testes
a = listArray (1,3) [(2,12),(4,55),(5,44)]
e = [(1, 2, 12), (1, 3, 34), (1, 5, 78),
	   (2, 4, 55), (2, 5, 32), (3, 4, 61),
	   (3, 5, 44), (4, 5, 93)]
t xs x = x:xs
u v1 v2 = fromJust (g1!(v1,v2))
x v1 v2 = unwrap(g1!(v1,v2))
		   where unwrap (Just w) = w
-- ------------

main =
	do 
		putStrLn("Loaded")
		--print( show graphAL' )
