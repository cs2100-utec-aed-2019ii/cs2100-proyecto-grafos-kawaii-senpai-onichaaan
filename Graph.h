#pragma once
#include "Nodes_Edges.h"

template <bool isDirected, typename t_node>
class Graph {
	vector<vector<t_node>> list;

public:
	Graph() = default;
	Graph(Graph& g) {

	}
	//Graph( load from vtk file )
	//Graph( reload saved graph )
	//Graph( Random )
	~Graph() {} //Destroy All Graph

	void saveGraphOnDisk() {

	}

	void insertNode() {

	}

	void insertEdge() {

	}

	void deleteNode() {

	}

	void gradeOfANode() {

	}

	bool isConnected() {

	}

	bool isBipartited() {

	}

	bool isDense() {

	}

	void MST_Kruskal() {

	}
	
	void MST_Prim() {

	}

	void searchTwoNodes_BFS() {

	}

	void searchTwoNodes_DFS() {

	}

	void Neighborhood() {

	}

	// OpenGL Interaction	-Draw Graph
	//						-Select Nodes using Mouse
	//						-Selection Nodes for insertion / deletion
	//						-Selection of Two Nodes to insert Edge
};