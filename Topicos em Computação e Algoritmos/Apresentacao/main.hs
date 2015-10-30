-- ghci --make nomedoArquivo 

module Main where

import Queue
import Data.Ix
import Data.Array
import Data.Maybe
import Data.List

-- Representando usando tuplas
type Graph n w = [(n,[(n, w)])]

-- Representando usando ponteiro
data GraphPonteiro n w = Vertex n [((GraphPonteiro n w), w)]

graphPTR = v1
	where
		v1 = Vertex 1 [(v2, 12),(v3, 34),(v5, 78)]
		v2 = Vertex 2 [(v1, 12),(v4, 55),(v5, 32)]
		v3 = Vertex 3 [(v1, 34),(v4, 61),(v5, 44)]
		v4 = Vertex 4 [(v2, 55),(v3, 61),(v5, 93)]
		v5 = Vertex 5 [(v1, 78),(v2, 32),(v3, 44),(v4,93)]

-- Representando usando lista de adjacencia
type GraphListaAdjacencia n w = Array n [(n, w)]

-- CRIANDO DIRETO
graphAL' = array (1, 5) [(1, [(2, 12), (3, 34), (5, 78)]),
						(2, [(1, 12), (4, 55), (5, 32)]),
						(3, [(1, 34), (4, 61), (5, 44)]), 
						(4, [(2, 55), (3, 61), (5, 93)]),
						(5, [(1, 78), (2, 32), (3, 44), (4, 93)])]

mkGraphAL :: (Ix n, Num w) => Bool -> (n,n) -> [(n,n,w)] -> (GraphListaAdjacencia n w)
mkGraphAL dir bnds es = 
	accumArray (\xs x -> x:xs) [] bnds
				([(x1, (x2, w)) | (x1, x2, w) <- es] ++
					if dir then []
					else [(x2, (x1, w)) | (x1, x2, w) <- es, x1 /= x2])

-- USANDO MKGRAPH

graphAL = mkGraphAL False (1, 5) [(1, 2, 12), (1, 3, 34), (1, 5, 78),
								   (2, 4, 55), (2, 5, 32), (3, 4, 61),
								   (3, 5, 44), (4, 5, 93)]

graphALT = mkGraphAL True (1, 5) [(1, 2, 12), (1, 3, 34), (1, 5, 78),
								   (2, 4, 55), (2, 5, 32), (3, 4, 61),
								   (3, 5, 44), (4, 5, 93)]

---

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
-- complexidade de tempo O(|V|^2)

type GraphMatrixAdjacencia n w = Array (n, n) (Maybe w)

mkGraphMatrixAdjacencia :: (Ix n, Num w) => Bool -> (n,n) -> [(n,n,w)] -> (GraphMatrixAdjacencia n w)
mkGraphMatrixAdjacencia dir bnds@(i,u) es
		= emptyArray // ([((x1,x2), Just w) | (x1,x2,w) <- es] ++
					     if dir then []
					     else [((x2,x1), Just w) | (x1,x2,w) <- es, x1/=x2])
		    where emptyArray
				 = array ((i,i),(u,u)) [((x1,x2), Nothing) | x1 <- range bnds, x2 <- range bnds]

graphAM = mkGraphMatrixAdjacencia False (1, 5) [(1, 2, 12), (1, 3, 34), (1, 5, 78),
								   (2, 4, 55), (2, 5, 32), (3, 4, 61),
								   (3, 5, 44), (4, 5, 93)]

graphAMT = mkGraphMatrixAdjacencia True (1, 5) [(1, 2, 12), (1, 3, 34), (1, 5, 78),
								   (2, 4, 55), (2, 5, 32), (3, 4, 61),
								   (3, 5, 44), (4, 5, 93)]

----

adjacent' graphAM v1 = [ v2 | v2 <- nodes' graphAM, (graphAM ! (v1,v2)) /= Nothing ]
-- Retorna cada elemento v2, dado que v2 é um elemento [nó] do grafo graphAM, e que, no grafo, o elemento [aresta] (v1,v2) != Nothing

nodes' graphAM = range (1,u) where ((1,_),(u,_)) = bounds graphAM
-- Retorna o range entre 1 e u, onde u é a cabeça do segundo elemento de bounds, que retorna os limites da matriz

edgeIn' graphAM (x,y) = (graphAM!(x,y)) /= Nothing
-- Retorna True se o elemento [aresta] na posição (x,y), no grafo graphAM, é != Nothing

weight' x y graphAM = w where (Just w) = graphAM!(x,y)
-- Retorna o valor do elemento [aresta] na posição (x,y), no grafo graphAM, se o mesmo não é Nothing

edgesD' graphAM = [(v1,v2) | v1 <- nodes' graphAM, v2 <- nodes' graphAM, edgeIn' graphAM (v1,v2)]

edgesU' g = [(v1,v2) | v1 <- nodes' graphAM, v2 <- range (v1,u), edgeIn' graphAM (v1,v2)]
			where (_,(u,_)) = bounds graphAM

-- FIM matrix de adjacencia

-- Testes
a' = array (1,4) [(3,"c"),(2,"a"),(1,"f"),(4,"e")]
m = array ((1,1),(2,3)) [((i,j), (i*j)) | i <-[1..2] , j <- [1..3]]
a'' = listArray (1,4) "face"
b = listArray (1,3) [(2,12),(4,55),(5,44)]
l' = [(1,2,3),(2,3,4),(1,4,5)]
l'' = [(x1,(x2,w)) | (x1,x2,w) <- l']
l''' = accumArray (\xs x -> x : xs) [] (1,2) l'' 
l'''' = [(\xs x -> x : xs) l'']
m' = [Nothing,Just(2),Nothing,Just(3)]
m'' = (`lookup` [(1,11),(2,22),(3,33)])
-- ------------

------------------------------------
-- BUSCAS
------------------------------------

-- complexidade de tempo: O(|V|+|A|)

depthFirstSearch start g = dfs [start] []
	where
		dfs [] vis = vis
		dfs (x:xs) vis
			| elem x vis = dfs xs vis
			| otherwise = dfs ((adjacent g x) ++ xs) (vis ++ [x])

depthFirstSearch' :: Ix a => a -> GraphListaAdjacencia a w -> [a] 
depthFirstSearch' start g = dfs [start] []
	where
		dfs [] vis = vis
		dfs (x:xs) vis
			| elem x vis = dfs xs vis
			| otherwise = dfs ((adjacent g x) ++ xs) (vis ++ [x])

mapT x [] = []
mapT x (y:xs) = (x, y) : mapT x xs

depthFirstSearch'' :: (Eq w, Num a, Ix a) => a -> (GraphMatrixAdjacencia a w) -> [(a,a)] 
depthFirstSearch'' start g = dfs [(start, start)] []
	where
		dfs [] vis = vis
		dfs (x:xs) vis
			| elem x vis = dfs xs vis
			| otherwise = dfs ((mapT (fst x) (adjacent' g (snd x))) ++ xs) (vis ++ [x])

-- complexidade de tempo: O(|V|+|A|)
breadthFirstSearch' start g
					= reverse (bfs (enqueue start empty) [])
	where
		bfs q vis
			| (isEmpty q) = vis
			| elem (front q) vis
							= bfs (dequeue q) vis
			| otherwise 	= let c = front q
							  in
							  	bfs (foldr enqueue
							  				(dequeue q)
							  				(adjacent' g c))
									(c:vis)	

breadthFirstSearch :: (Ix a, Num a, Eq w) => a -> GraphListaAdjacencia a w -> [a]
breadthFirstSearch start g
					= reverse (bfs (enqueue start empty) [])
	where
		bfs q vis
			| (isEmpty q) = vis
			| elem (front q) vis
							= bfs (dequeue q) vis
			| otherwise 	= let c = front q
							  in
							  	bfs (foldr enqueue
							  				(dequeue q)
							  				(adjacent g c))
									(c:vis)	

graphD = mkGraphAL True (1,6) [(1,2,0),(1,3,0),(1,4,0),(3,6,0),(5,4,0),(6,2,0),(6,5,0)]

-- USO:
--
-- depthFirstSearch 1 graphD
-- depthFirstSearch 1 g
-- depthFirstSearch' 1 g
-- depthFirstSearch'' 1 graphD

-----
-- INI: Ordenacao topologica
-----

inDegree g n  = length [ t | v<-nodes g, t<-adjacent g v, (n==t)]

topologicalSort g = tsort [n | n<-nodes g, (inDegree g n == 0)]
                          []
                    where 
                    	tsort [] r   = r
                    	tsort (c:cs) vis
                    	 | elem c vis = tsort cs vis
                    	 | otherwise  = tsort cs (c:(tsort (adjacent g c) vis))

-- Indica ordem
data Courses = Maths | Theory | Languages | Programming | Concurrency | Architecture | Parallelism 
				deriving (Eq, Ord, Enum, Ix, Show)

cg = mkGraphAL True (Maths, Parallelism) 
		[(Maths, Theory, 1),(Languages,Theory,1),
		 (Programming,Languages,1),(Programming,Concurrency,1),
		 (Concurrency,Parallelism,1),(Architecture,Parallelism,1)]

-- USO:
--
-- topologicalSort g
-- topologicalSort cg

-----
-- FIM: Ordenacao topologica
-----

-----
-- INI : Prim
-----

prim g = 
	prim' [n] ns []
	where 
		(n:ns) = nodes g
		es = edgesU g
		prim' t [] mst = mst
		prim' t r mst 
			= let e@(c,u',v') = minimum [(c,u,v) | (u,v,c) <- es, elem u t, elem v r] 
			  in prim' (v':t) (delete v' r) (e:mst)

-----
-- FIM : Prim
-----

main =
	do 
		putStrLn("m -> \n" ++ show(m))
		putStrLn("m!(1,2) -> \n" ++ show(m!(1,2)))
		putStrLn("bounds m -> \n" ++ show(bounds m))
		putStrLn("indices m -> \n" ++ show(indices m))
		putStrLn("elems m -> \n" ++ show(elems m))
		putStrLn("assocs m -> \n" ++ show(assocs m))
		putStrLn("m//[((1,1),4)] -> \n" ++ show(m//[((1,1),4)]))
		--print( show graphAL' )
