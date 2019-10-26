#pragma once
#include "openGL_react_graph.h"
#include "graph_element_relation.h"
#include "order.h"
#include "print.h"

class fake_example_graph { //The graph must follow this structure
	using edge = int;
	using vortex = float; //fake types
public:
	using container_edge = std::vector<edge>;
	using container_vortex = std::vector<vortex>;
	using ptr_c_e = std::shared_ptr<container_edge>;
	using ptr_c_v = std::shared_ptr<container_vortex>; //important alias
public:
	ptr_c_e edges = std::make_shared<container_edge>();
	ptr_c_v vortexs = std::make_shared<container_vortex>(); //important pointers
	//this pointers will replace the containers atributes
	//example:		(*edges).push_back(...);
};

int main(int argc, char* argv[]) {

	using graph = Graph<coord, distance, false>;
	using graph_g = graph_graphic<graph>;

	order::section("Creation");
	graph g1; //Test Small and Big Test
	//Graph<coord, distance, false> g1("ToReadFile.vtk"); //Test Load From VTK
	//Graph<coord, distance, false> g1(5, 3); //Test Random Constructor
	//Graph<coord, distance, false> g1("OutputFile.vtk"); //Test Load From Backup
	order::note("A not directed graph is created");

	order::section("Insertion");
	order::note("Multiple nodes and edges are inserted");

	/* // --Small Test--
	g1.insertNode(coord::create(10, 20));
	g1.insertNode(coord::create(20, 30));
	g1.insertEdge(coord::create(10,20), coord::create(20, 30));
	g1.insertNode(coord::create(30, 40));
	g1.insertNode(coord::create(40, 50));
	g1.insertEdge(coord::create(10,20), coord::create(30, 40));
	g1.insertEdge(coord::create(30,40), coord::create(40,50));
	*/ // --Big Test---
	g1.insertNode(coord::create(10, 20)); //--1
	g1.insertNode(coord::create(20, 30)); //--2
	g1.insertNode(coord::create(30, 40)); //--3
	g1.insertNode(coord::create(40, 50)); //--4
	g1.insertNode(coord::create(50, 60)); //--5
	g1.insertNode(coord::create(60, 70)); //--6
	g1.insertEdge(coord::create(10, 20), coord::create(20, 30));  //--1--2
	g1.insertEdge(coord::create(10, 20), coord::create(30, 40));  //--1--3
	g1.insertEdge(coord::create(20, 30), coord::create(40, 50));  //--2--4
	g1.insertEdge(coord::create(20, 30), coord::create(50, 60));  //--2--5
	g1.insertEdge(coord::create(30, 40), coord::create(50, 60));  //--3--5
	g1.insertEdge(coord::create(40, 50), coord::create(50, 60));  //--4--5
	g1.insertEdge(coord::create(40, 50), coord::create(60, 70));  //--4--6
	g1.insertEdge(coord::create(50, 60), coord::create(60, 70));  //--5--6

	order::section("Printing");
	order::note("Print all the nodes in adj_list by their relations");
	std::vector<std::vector<coord>> adjL;
	g1.get_Adj_list(adjL);
	print(adjL);

	order::section("Delete");
	order::note("A node is deleted");
	g1.deleteNode(coord::create(20, 30));
	g1.get_Adj_list(adjL);
	print(adjL);

	order::section("Degree");
	order::note("A node degree");
	print(g1.degreeOfANode(coord::create(40, 50)));

	order::section("Neighborhood");
	order::note("A node Neighborhood");
	std::vector<coord> nb;
	g1.Neighborhood(coord::create(50, 60), nb);
	print(nb);

	order::section("BFS from a to b");
	order::note("BFS from a to b");
	std::vector<coord> bfs;
	g1.searchTwoNodes_BFS(coord::create(10, 20), coord::create(50, 60), bfs);
	print(bfs);

	order::section("DFS from a to b");
	order::note("DFS from a to b");
	std::vector<coord> dfs;
	g1.searchTwoNodes_DFS(coord::create(10, 20), coord::create(50, 60), dfs);
	print(dfs);

	order::section("Is Connected");
	order::note("Is the graph connected?");
	print(g1.isConnected());

	order::section("Save Graph");
	order::note("Saving graph to a vtk file");
	g1.saveGraphOnDisk("OutputFile.vtk");

	order::section("Create Graph from Copy");
	order::note("Created a graph from an existing graph");
	order::note("Printing new graph");
	Graph<coord, distance, false> g2(g1);
	std::vector<std::vector<coord>> adjL_1;
	g2.get_Adj_list(adjL_1);
	print(adjL_1);

	order::section("Is Dense");
	order::note("Is the graph dense?");
	print(g1.isDense());

	order::section("Prim");
	order::note("Creating the Minimun Spanning Tree using Prim");
	std::vector<std::vector<coord>> MST;
	g1.MST_Prim(coord::create(10, 20), MST);
	print(MST);

	order::section("Kruskal");
	order::note("Creating the Minimun Spanning Tree using Kruskal");
	std::vector<std::vector<coord>> _MST;
	g1.MST_Kruskal(_MST);
	print(_MST);

	manager::universe.attach("my_graph_A", std::make_shared<graph_g>(&g1));
	manager::set_camera(600, 600);
	manager::set_limits(600, 600, 600);
	manager::set_tick_cycle(10);
	manager::set_bg_color(0, 0, 0, 1);
	manager::set_title("Grafos");
	manager::play(argc, argv);
}