#!/usr/bin/env python

import numpy
import scipy
import scipy.linalg

L = 3
N = 2**L
h = 1./N
hh = h*h
ihh = 1./hh

class Node:
    left = None
    right = None
    matrix = numpy.zeros(shape=(3,3))
    matrix_rhs = numpy.zeros(shape=3)
    name = "#"

    def m(self):
        return numpy.matrix([[self.matrix[1,1], self.matrix[1,2]], [self.matrix[2,1], self.matrix[2,2]]])

    def rhs(self):
        return numpy.array([self.matrix_rhs[1], self.matrix_rhs[2]])

    def merge(self):
        a = self.left.m()
        ar = self.left.rhs()
        b = self.right.m()
        br = self.right.rhs()
        self.matrix = numpy.matrix([
                             [a[1,1]+b[0,0], a[1,0], b[0,1] ],
                             [a[0,1]       , a[0,0], 0      ],
                             [b[1,0]       , 0     , b[1,1] ]
                            ])
        self.matrix_rhs = numpy.array([ar[1] + br[0], ar[0], br[1]])        

    def up(self):
        self.left.up()
        self.right.up()
        
        self.merge()

        if abs(self.matrix[1,0]) > 1e-6:
            self.matrix_rhs[1] = self.matrix_rhs[1] * self.matrix[1,0]/self.matrix[0,0]
            self.matrix[1] = self.matrix[1] - self.matrix[0] * self.matrix[1,0]/self.matrix[0,0]
        if abs(self.matrix[2,0]) > 1e-6:
            self.matrix_rhs[2] = self.matrix_rhs[2] * self.matrix[2,0]/self.matrix[0,0]            
            self.matrix[2] = self.matrix[2] - self.matrix[0] * self.matrix[2,0]/self.matrix[0,0]
        
    def down(self, root):
        self.matrix[1] = numpy.array([0.,1.,0.])
        self.matrix[2] = numpy.array([0.,0.,1.])
        
        if self == root.left: 
            self.matrix_rhs[1] = root.matrix_rhs[1]
            self.matrix_rhs[2] = root.matrix_rhs[0]
        else:
            self.matrix_rhs[1] = root.matrix_rhs[0]
            self.matrix_rhs[2] = root.matrix_rhs[2]
        
        self.matrix_rhs = numpy.linalg.solve(self.matrix, self.matrix_rhs)
        self.matrix = numpy.zeros((3,3))
        numpy.fill_diagonal(self.matrix, 1)
        
        self.left.down(self);
        self.right.down(self);
        
class Leaf:
    name = "Leaf"
    matrix = numpy.matrix([[-ihh, ihh], [ihh, -ihh]])
    matrix_rhs = numpy.array([0., 0.])
    
    def m(self):
        return self.matrix

    def rhs(self):
        return self.matrix_rhs
    
    def up(self): 
        pass
        
    def down(self, root):
        self.matrix = numpy.matrix([[1., 0], [0., 1.]])
        if self == root.left: 
            self.matrix_rhs = numpy.array([root.matrix_rhs[1], root.matrix_rhs[0]])
        else:
            self.matrix_rhs = numpy.array([root.matrix_rhs[0], root.matrix_rhs[2]])

class Root(Node):
    
    def up(self): 
        self.left.up()
        self.right.up()

        self.merge()
        
    def down(self):
        self.matrix_rhs = numpy.linalg.solve(self.matrix, self.matrix_rhs)
        self.matrix = numpy.zeros((3,3))
        numpy.fill_diagonal(self.matrix, 1)
        
        self.left.down(self);
        self.right.down(self);
        


def gen_tree_1(root, levels):
    if levels == 0:
        root.left = Leaf()
        root.right = Leaf()
    else:
        root.left = Node()
        root.right = Node()
        gen_tree_1(root.left, levels-1)
        gen_tree_1(root.right, levels-1)

def gen_tree(levels):
    root = Root()
    gen_tree_1(root, levels-1)
    
    ln = root.left
    while isinstance(ln, Node):
        ln = ln.left
    ln.matrix = numpy.matrix([[1.0,0.0],[ihh, -ihh]])

    rn = root.right
    while isinstance(rn, Node):
        rn = rn.right

    rn.matrix = numpy.matrix([[-ihh,ihh],[0,1.0]]);
    rn.matrix_rhs = numpy.array([0., 20.0])
    
    return root


def get_variables(root):
    if isinstance(root.left, Leaf) and not isinstance(root.right, Leaf):
        return [root.left] + get_variables(root.right)
    elif not isinstance(root.left, Leaf) and isinstance(root.right, Leaf):
        return get_variables(root.left) + [root.right]
    elif isinstance(root.left, Leaf) and isinstance(root.right, Leaf):
        return [root.left, root.right]
    else:
        return get_variables(root.left) + get_variables(root.right)

def get_solution(root):
    rn = root.right
    while isinstance(rn, Node):
        rn = rn.right
    
    return map(lambda x: x.matrix_rhs[0], get_variables(root)) + [rn.matrix_rhs[1]]
def print_tree(g, root):
    g.write("%s[label=\"%s\\n%s\"];\n" % (id(root), str(root.matrix).encode('string_escape'), str(root.matrix_rhs).encode('string_escape')))
    if isinstance(root, Node):
        g.write("%s -> %s;\n" % (id(root), id(root.left)))
        g.write("%s -> %s;\n" % (id(root), id(root.right)))
        print_tree(g,root.left)
        print_tree(g,root.right)    

t = gen_tree(L)    



t.up()

g = open('test1.dot', 'w')
g.write("digraph g {\n")
print_tree(g, t)
g.write("}\n")
g.close()


t.down()
g = open('test2.dot', 'w')
g.write("digraph g {\n")
print_tree(g, t)
g.write("}\n")
g.close()



print "done"
print t.matrix
print t.matrix_rhs
print get_solution(t)