import Numeric.LinearAlgebra
-- import Data.Tree

-- data TreeUp a = Leaf m rhs| Branch (Tree a) (Tree a) deriving (Eq,Show)

-- t = Branch (Branch (Leaf 1) (Leaf 2)) (Leaf 3)


data Tree = Leaf Int Int 
        | Node Tree Tree ((Double, Double, Double, Double), (Double, Double))
        | Branch Tree Tree
        | Branch2 Tree Tree ((Double, Double, Double), Double)
        deriving (Eq,Show)


gen_tree :: Int -> Int -> Tree
gen_tree offset level
    -- | level == 0     = (Leaf offset (offset+1)) 
    | level == 1     = (Node (Leaf offset (offset+1)) (Leaf (offset+1) (offset+2)) (m' offset))
    | otherwise      = (Branch (gen_tree offset (level-1)) (gen_tree (offset+2^(level-1)) (level-1)))


tuplify4 [a,b,c,d] = (a,b,c,d)
tuplify3 [a,b,c]   = (a,b,c)
tuplify2 [a,b]     = (a,b)


l = 3
n = 2^l
h = 1.0/(fromIntegral n)

m' :: Int -> ((Double, Double, Double, Double), (Double, Double))
m' idx
    | idx == 0           = ((-1.0/h**2, 0.0     , 1.0/h**2, -1.0/h**2), (0.0,  0.0))
    | idx == (2^(l-1))   = ((-1.0/h**2, 1.0/h**2, 0.0     ,  1.0/h**2), (0.0,  0.0))
    | otherwise          = ((-1.0/h**2, 1.0/h**2, 1.0/h**2, -1.0/h**2), (0.0, 20.0))


t' :: ((Double, Double, Double, Double), (Double, Double)) -> ((Double, Double, Double, Double), (Double, Double)) -> (((Double, Double, Double), Double), ((Double, Double, Double, Double), (Double, Double)) )
t' ((a, b, c, d), (e, f)) ((g, h, i, j), (k, l)) = 
    let sf = f+k in
    let r1  = 4 |> [f+k, d+g, c, h] in
    let r2  = 4 |> [e, b, a, 0.0] in
    let r3  = 4 |> [l, i, 0.0, j] in     
    let r2' = r2 - (scale (sf/b) r1) in
    let r3' = r3 - (scale (sf/i) r1) in
    let rhs:lhs = (toList r1) in 
    let local = ((tuplify3 lhs), rhs) in
    let [a',b',c',d'] = (toList r2') in 
    let [a'',b'',c'',d''] = (toList r2') in 
    let export = ((c',d',c'',d''), (a',a'')) in
    do 
        print r1
        (local, export)
    

up :: Tree -> (Tree, ((Double, Double, Double, Double), (Double, Double)))
up (Node l r m) = ((Node l r m), m)
up (Branch l r) = 
    let (ln, lm) = (up l) in 
    let (rn, rm) = (up r) in 
    let (local, m) = (t' lm rm) in
    ((Branch2 ln rn local), m)

--     
--     w
--   
-- 
-- m :: Int -> Matrix Double
-- m idx
--     | idx == 0    = (2><2)[-1.0/h**2,  0.0      
--                           , 1.0/h**2, -1.0/h**2]
--     | idx == n-1  = (2><2)[-1.0/h**2,  1.0/h**2
--                           , 0.0     ,  1.0/h**2]
--     | otherwise   = (2><2)[-1.0/h**2,  1.0/h**2
--                           , 1.0/h**2, -1.0/h**2]
-- rhs :: Int -> Vector Double
-- rhs idx
--     | idx == 0    = 2 |> [0.0,  0.0]
--     | idx == n-1  = 2 |> [0.0, 20.0]
--     | otherwise   = 2 |> [0.0,  0.0]
-- 
-- 

main = do
    let t = (gen_tree 0 3)
    -- print t
    print (up t)

-- wlet h = 4
-- 
-- let m (Leaf 0.0) = (2><2)[-1/h**2, 0, 1/h**2, -1/h**2]
-- let m (Leaf _)   = (2><2)[-1/h**2, 1/h**2, 1/h**2, -1/h**2]
-- let m (Leaf 1.0) = (2><2)[-1/h**2, 1/h**2, 0, 1/h**2]
-- 
-- 
-- let rhs (Leaf 0.0) = (2><1)[0.0, 0.0]
-- let rhs (Leaf _) = (2><1)[0.0, 0.0]
-- let rhs (Leaf 1.0 ) = (2><1)[20.0, 0.0]