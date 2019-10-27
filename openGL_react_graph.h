#pragma once
#include "openGL_react.h" //Also by J. Huby Ochoa
#include "order.h"
#include "print.h"

//By José Ignacio Huby Ochoa
// Lima, Perú
// CS - UTEC

template<typename graph>
class graph_graphic : public observer{
private:
	using ptr_e = typename graph::ptr_e;
	using ptr_n = typename graph::ptr_n;
private:
	std::vector<ptr_e> edges;
	std::vector<ptr_n> vortex;
	graph* my_graph;
	coord selectedCoord_1;
	coord selectedCoord_2;
public:
	graph_graphic(graph* g) : my_graph{g} {

		my_graph->update(vortex, edges);

		this->learn("draw_vortex", [this](void* args = nullptr) {
			for (const auto& v : this->vortex) {
				glPushMatrix();
				glTranslatef(v->get_value().x, v->get_value().y , 0);
				glutSolidSphere(5, 10, 10);
				glPopMatrix();
			}
		});
		this->learn("draw_edges", [this](void* args = nullptr) {
			for (const auto& e : this->edges) {
				glPushMatrix();
				glBegin(GL_LINES);
				glVertex3f(e->get_from()->get_value().x, e->get_from()->get_value().y , 0);
				glVertex3f(e->get_to()->get_value().x, e->get_to()->get_value().y, 0);
				glEnd();
				glPopMatrix();
			}
		});
		this->learn("draw", [this](void* args = nullptr) {

			this->apply("draw_vortex");
			this->apply("draw_edges");

		});
		this->learn("onLeftClick", [this](void* args = nullptr) {
			
			coord* pts = (coord*)args;
			my_graph->insertNode(*pts);
			my_graph->update(vortex, edges);

		});
		this->learn("onRightClick", [this](void* args = nullptr) {

			coord* pts = (coord*)args;
			my_graph->deleteNode(*pts);
			my_graph->update(vortex, edges);
			

		});
		this->learn("onMiddleDown", [this](void* args = nullptr) {

			coord* pts = (coord*)args;
			selectedCoord_1 = *pts;
			if (my_graph->bool_find(*pts)) std::cout << "Found" << std::endl;
			else { std::cout << "Not Found" << std::endl; }
			my_graph->update(vortex, edges);

		});
		this->learn("onMiddleUp", [this](void* args = nullptr) {

			coord* pts = (coord*)args;
			selectedCoord_2 = *pts;
			if (my_graph->bool_find(*pts)) std::cout << "Found" << std::endl;
			else { std::cout << "Not Found" << std::endl; }
			my_graph->update(vortex, edges);

		});
		this->learn("onKeyPressed", [this](void* args = nullptr) {
			std::vector<coord> traversal;
			std::vector<std::vector<coord>> MST;
			std::vector<coord> nb;
			std::vector<coord> bfs;
			unsigned char* key = (unsigned char*)args;
			switch (*key) {
			case '1':
				order::section("BFS of all graph");
				print("Loading...");
				my_graph->BFS(my_graph->return_first_element(), traversal);
				print("Done Loading");
				print(traversal);
				break;

			case '2':
				order::section("DFS of all graph");
				print("Loading...");
				my_graph->DFS(my_graph->return_first_element(), traversal);
				print("Done Loading");
				print(traversal);
				break;

			case '3':
				order::section("Is Connected");
				order::note("Is the graph connected?");
				print("Loading...");
				print(my_graph->isConnected());
				print("Done Loading");
				break;

			case '4':
				order::section("Save Graph");
				order::note("Saved graph to a vtk file");
				print("Loading...");
				my_graph->saveGraphOnDisk("OutputFile.vtk");
				print("Done Loading");
				break;

			case '5':
				order::section("Is Dense");
				order::note("Is the graph dense?");
				print("Loading...");
				print(my_graph->isDense());
				print("Done Loading");
				break;

			case '6':
				order::section("Prim");
				order::note("Creating the Minimun Spanning Tree using Prim from first value of graph");
				print("Loading...");
				my_graph->MST_Prim(my_graph->return_first_element(), MST);
				print("Done Loading");
				print(MST);
				break;

			case '7':
				order::section("Kruskal");
				order::note("Creating the Minimun Spanning Tree using Kruskal");
				print("Loading...");
				my_graph->MST_Kruskal(MST);
				print("Done Loading");
				print(MST);
				break;

			case '8':
				order::section("Degree");
				order::note("A node degree");
				print(my_graph->degreeOfANode(selectedCoord_1));
				break;

			case '9':
				order::section("Neighborhood");
				order::note("A node Neighborhood");
				print("Loading...");
				my_graph->Neighborhood(selectedCoord_1, nb);
				print("Done Loading"); std::cout << std::endl;
				print(nb);
				break;

			case '0':
				order::section("Prim of the selected node");
				order::note("Creating the Minimun Spanning Tree using Prim");
				print("Loading...");
				my_graph->MST_Prim(selectedCoord_1, MST);
				print("Done Loading");
				print(MST);
				break;

			case 'q':
				order::section("BFS from a to b");
				order::note("BFS from a to b");
				print("Loading...");
				my_graph->searchTwoNodes_BFS(selectedCoord_1, selectedCoord_2, bfs);
				print("Done Loading");
				print(bfs);
				break;

			case 'w':
				order::section("DFS from a to b");
				order::note("DFS from a to b");
				print("Loading...");
				my_graph->searchTwoNodes_DFS(selectedCoord_1, selectedCoord_2, bfs);
				print("Done Loading");
				print(bfs);
				break;

			case 'e':
				my_graph->insertEdge(selectedCoord_1, selectedCoord_2);
				break;
			}
		});
	}
};