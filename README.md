# Parallel Breadth-First Search

Given a graph G = (V,E) with vertex set V = V(G) and edge set E = E(G), the BFS problem is to compute for each vertex v ∈ V the distance that v lies from a source vertex v0 ∈ V. We measure distance as the minimum number of edges on a path from v0 to v in G. BFS visits the graph in waves (levels). 

![BFS Levels](./assets/waves.jpg)

A level is all of the vertices equidistant to the source. The number of levels is bounded by the diameter of a graph. The order in which we visit vertices of any given level doesn't matter, so it can be done concurrently. This idea underlies the Parallel BFS (PBFS) algorithm.

PBFS represents levels using an unordered-set data structure, called a bag, which supports efficient parallel traversal over the elements in the set and provides the following operations:

• Creation of a new empty bag

• Insertion of an element into a bag

• Bag unioning

• Bag splitting

A bag is a collection of pennants, no two of which have the same size. A pennant is a tree of 2<sup>k</sup> nodes, where k is a nonnegative integer. Each node x in this tree contains two pointers (left and right) to its children. The root of the tree has only a left child, which is a complete binary tree of the remaining elements. Two pennants x and y of size 2<sup>k</sup> can be combined to form a pennant of size 2<sup>k+1</sup> in O(1) time. It is also possible to split a pennant of at least two elements into two parts in O(1) time. 

![Pennant unioning](./assets/pennant_unioning.jpg)

A bag stores pennants in an array S, called the backbone. Each entry S[k] in the backbone contains either a null pointer or a pointer to a pennant of size 2<sup>k</sup>.

![Backbone](./assets/backbone.jpg)

Insertion of an element into a bag employs an algorithm similar to that of incrementing a binary counter. First we package the given element as a pennant of size 1. Then we combine it with all pennants in the backbone starting from the smallest one until a free entry is found. Finally, the pennant is placed into a free entry and all previous entries are set to a null pointer. 

Since pennant unioning takes constant time, worst-case time to insert a pennant into a bag of n elements is O(log(n)).

Bag unioning uses an algorithm similar to ripple-carry addition of two binary counters. Given three pennants x, y, and z, where each either has size 2<sup>k</sup> or is empty, we can merge them to produce a pair of pennants (s, c), where s has size 2<sup>k</sup> or is empty, and c has size 2<sup>k+1</sup> or is empty. With this idea in mind, we can union two bags by merging corresponding pennants of their backbones and the "carry bit" from the previous step. To compute all entries in the backbone of the resulting bag takes O(log(n)) time.

Splitting a bag operates like an arithmetic right shift: we divide each pennant into halves. One half is kept in the original bag and the other one is placed at the same position in the new bag. If there is a remainder (the least significant element), it is inserted into the original bag. 

To split a pennant A, we first set the root of a new pennant B to be the root of A. Then we set the root of A to be the only child of B, set the only child of B to be the second child of A, and remove the second child of A. Each of the pennants A and B now contain half the elements. Like pennant unioning, splitting a pennant also operates in O(1) time. Consequently, the asymptotic runtime of bag splitting is O(log(n)).