# 7: Graph Algorithms

## Goals

* Learn how to play with the graph ADT, particularly with vertices and
  edges

* Learn how to implement three graph algorithms: Dijkstra's shortest
  path, Prim's minimum spanning tree, topological sort.

### Table of contents
1. [Graph Algorithms](#Graph-Algorithms)
2. [Provided Graph ADT](#Provided-Graph-ADT)
3. [Tasks: Implement Three Graph Algorithms](#Tasks)

## Graph Algorithms

A graph is a set of vertices which are connected by edges. Graphs are
one of the main study subject in discrete mathematics, which also has
many interesting real-world applications---e.g., network flow
optimization, map navigation, etc.

This homework provides the minimum specification over the graph ADT,
and asks you to develop following three graph algorithms.

* Dijkstra's shortest path
* Prim's minimum spanning tree
* Topological sort

## Provided Graph ADT

Unlike previous homeworks that we provided concrete design
specifications of classes, this homework provides the minimum
specification. That means you may introduce your own changes as you
want---implementing your own member functions or variables. You will
still need to ensure that your constructor works properly, such that
we can run the test for grading. The provided test will check if the
constructor is correctly designed.

The followings list the key types to be used in the graph (see
`include/graph.hpp`).

* **Vertex**: A vertex is typed as `vertex_t`

* **Edge**: An edge is typed as `edge_t`, which is associated with a
source vertex, a destination vertex, and its weight. Here, the edge
weight is a `double` type.

* **A list of edges**: A list of edges is typed as `edges_t`, which is
a `vector` of `edge_t`.

* **A type of graph**: `GraphType` specifies the type of graph, either a
  undirected graph or a directed graph.


Using the above mentioned types, the `Graph` class provides the Graph
ADT. The constructor of `Graph` (see `include/graph.hpp`) takes
following three arguments.

* `size_t num_vertices` takes the number of vertices in the graph to be constructed

* `const edges_t &edges` takes a set of edges in the graph to be constructed

* `GraphType Type` takes the type of graph to be constructed, either a
  undirected or directed graph

While the specification of this constructor should not be changed for
testing, you may add any member functions or variables to the `Graph`
class to implement the Graph ADT---such as adding edges, removing
edges, returning the in/out degree of a certain vertex, etc.

Moreover, we assume that if `num_vertices` is n, a graph contains n vertices
from `0` to `n-1`.

## Tasks

Based on the Graph ADT, your task in this homework is to implement
following three different graph algorithms. Each graph algorithm has
its own function, which takes a graph as input and returns a result as
ouput, and you will need to complete the implementation of all
functions.  You should not change the input and ouput specification
(as it will be used for testing), but you can add any functions or
variables as you want.

### Dijkstra's Algorithm

Given a graph and a source vertex, the Dijkstra algorithm finds the
shortest path from the source vertex to all other vertices. Your task
is to implement the function `dijkstra_shortest_path()` in
`include/dijkstra_shortest_path.hpp`.

* This function takes an input, a graph `g` and a source vertex `src`.

* This function returns an unordered map, containing the shortest path
  information. The returned unordered map maps a vertex `v` to a tuple
  (`u`, `d`), where `u` is the previous vertex (in the path from `src`
  to `v`) and `d` is the distance `src` to `v`. `u` is `std::nullopt`
  if `u` is not reachable from `src`.

### Prim's Algorithm

Given a graph and a source vertex, the Prim's algorithm finds the
MST. Your task is to implement the function
`prim_minimum_spanning_tree()` in
`include/prim_minimum_spanning_tree.hpp`.

* This function takes an input, a graph `g` and a source vertex `src`.

* This function returns the verctor of edges in the MST. It returns
  `std::nullopt` if the MST does not exist.


### Topological Sort

Given a graph, topological sort finds the sorted sequence of vertices.
Your task is to implement the function `topological_sort()` in
`include/topological_sort.hpp`.

* This function takes an input, a graph `g`.

* This function returns a vector, having a sorted sequence of vertex
  ids.

## Reference

* Test Cases  
SNU Course "Introduction to Data Structures" (Crs. No. 430.217)  
 - Professor Byuongyoung Lee (https://lifeasageek.github.io/),  
 - TA(PhD Student) Sangyun Kim (ksu880@snu.ac.kr)
