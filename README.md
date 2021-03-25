# Parallel Breadth-First Search

Given a graph G = (V,E) with vertex set V = V(G) and edge set E = E(G), the BFS problem is to compute for each vertex v ∈ V the distance that v lies from a source vertex v0 ∈ V. We measure distance as the minimum number of edges on a path from v0 to v in G. BFS visits the graph in waves (levels). 

![BFS Levels](./assets/waves.jpg)

A level is all of the vertices equidistant to the source. The number of levels is bounded by the diameter of a graph. The order in which we visit vertices of any given level doesn't matter, so it can be done concurrently. This idea underlies the Parallel BFS (PBFS) algorithm.

PBFS represents levels using an unordered-set data structure, called a bag, which supports efficient parallel traversal over the elements in the set and provides the following operations:

• Creation of a new empty bag

• Insertion of an element into a bag

• Bag unioning

