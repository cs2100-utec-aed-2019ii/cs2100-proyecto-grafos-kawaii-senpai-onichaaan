# cs2100-proyecto-grafos

# CS2100-Proyecto
#CS2100 #CS2100/groups
  
### Criteria  
* Implementation by Adjacent List  
* Directed and non Directed Graph  
* No Loops!  
* Use of traits for node and vertices type.  
* Use of OpenGL for Visualization  
* Use of Github - Gitflow Metodoly
	* Branch per developer
	* Karma comits
		* Feat(DEVELOPER_NAME): text;
* Nodes => Cartesian Coordinates
* Edges => by parameter || Euclidean Distance of Nodes
  
  
## Features Parcial  
* Constructors  
	* By Default  
	* By Copy  
	* By Parameter  
		* Load data from vtkfile  
		* Reload save graph
		* Random
* Destructor  (destroy all gaph)
* Save graph on disk
* Insert /Remove for nodes and edges  
* Grade of  a node ( i.e. node x has grade of …)
* is Connected?  
	* G(n,e) is connected if for any nodes (u, v) in G, exists a path between them.
* is Bipartited?  
* Graph density calculus
	* G(n,e) is dense if its number of edges is near to maximun number of edges for G
* Minimum Spanning Tree  
	* Prim  
	* Kruskal  
* Search  two nodes (a -> b)
	* BFS (Breadth First Search)  
	* DFS (Depth First Search)  
* Neighborhood  
	* Show Neighbors  of a node
	* Validate Neighborhood of two nodes  
		* use of threshold based on media of all distances
* OpenGL  Interaction
	* Draw graph in 2d representation no depth
	* Selection of nodes using mouse
	* Selection of nodes using mouse for insertion and deletion
  
## Features Final  
* Implement a cursor
	* cursor is basically an iterator 
* Iterate 
	* minimum 
	* maxima
	* custom edge weight  
* Hamiltonian path calculus
	* Hamiltonian is a path that visits all vertinces once time
* Multiple cursors (iterators)
	* Draw Iterator (spider form) 
	* Minimun Search of two nodes in parallel  
	* Two cursors are neighbors
* Change search of two nodes (a -> b) using cursors:
	* A* 
	* DisjkTra
* Subgraph by path  of iteration
* Join subgraphs  of different cursors
* Minimum Spanning Tree  
	* of Subgraph  
	* of set of points pexFormat  automatic segmentation of graphs (list of subgraphs)
	* OpenGL  Interaction
	* Cursor interaction using keyboard
	* Selection of nodes using mouse

