#pragma once
#include "openGL_react.h" //Also by J. Huby Ochoa
#include "order.h"
#include "print.h"
#include <memory>

//By José Ignacio Huby Ochoa
// Lima, Perú
// CS - UTEC

template<typename graph>
class graph_graphic : public observer{

private:

	using ptr_e = typename graph::ptr_e;
	using ptr_n = typename graph::ptr_n;
	using ptr_gg = std::shared_ptr<graph_graphic>;
	using ptr_g = std::shared_ptr<graph>;

private:

	std::vector<ptr_e> edges;
	std::vector<ptr_n> vortex;
	ptr_g my_graph;
	ptr_gg path;
	coord selectedCoord_1;
	coord selectedCoord_2;
	bool searchTwoNodes_Mode = false;
	bool iterator_Mode = false;
	typename graph::Iterator itr;
	std::vector<coord> iterator_traversal;

public:

	graph_graphic(graph* g) : my_graph{g} {

		my_graph->update(vortex, edges);

		this->learn("draw_vortex", [this](void* args = nullptr) {
			bool is_path = static_cast<bool>(args);
			for (const auto& v : this->vortex) {
				glPushMatrix();
				glTranslatef(v->get_value().x, v->get_value().y , 0);
				if (is_path) { glColor3f(0.0f, 0.0f, 1.0f); }
				else { glColor3f(1.0f, 1.0f, 1.0f); }
				glutSolidSphere(5, 10, 10);
				glPopMatrix();
			}
		});
		this->learn("draw_edges", [this](void* args = nullptr) {
			bool is_path = static_cast<bool>(args);
			for (const auto& e : this->edges) {
				glPushMatrix();
				glBegin(GL_LINES);
				if (is_path) { glColor3f(0.0f, 0.0f, 1.0f); }
				else { glColor3f(1.0f, 1.0f, 1.0f); }
				glVertex3f(e->get_from()->get_value().x, e->get_from()->get_value().y , 0);
				if (is_path) { glColor3f(0.0f, 0.0f, 1.0f); }
				else { glColor3f(1.0f, 1.0f, 1.0f); }
				glVertex3f(e->get_to()->get_value().x, e->get_to()->get_value().y, 0);
				glEnd();
				glPopMatrix();
			}
		});
		this->learn("draw", [this](void* args = nullptr) {

			this->apply("draw_vortex", (void*) false);
			this->apply("draw_edges", (void*) false);
			if (path) {
				path->apply("draw_vortex", (void*) true);
				path->apply("draw_edges", (void*) true);
			}

		});
		this->learn("onLeftClick", [this](void* args = nullptr) {
			
			if (searchTwoNodes_Mode) {
				coord* pts = (coord*)args;
				if (my_graph->bool_find(*pts)) {
					std::cout << "Found" << std::endl;
					selectedCoord_1 = *pts;
				}
				else {
					std::cout << "Not Found" << std::endl;
				}
				my_graph->update(vortex, edges);
			}
			else {
				coord* pts = (coord*)args;
				my_graph->insertNode(*pts);
				my_graph->update(vortex, edges);
			}

		});
		this->learn("onRightClick", [this](void* args = nullptr) {

			if (searchTwoNodes_Mode) {
				coord* pts = (coord*)args;
				if (my_graph->bool_find(*pts)) { 
					std::cout << "Found" << std::endl;
					selectedCoord_2 = *pts;
				}
				else { 
					std::cout << "Not Found" << std::endl;
				}
				my_graph->update(vortex, edges);
			}
			else {
				coord* pts = (coord*)args;
				my_graph->deleteNode(*pts);
				my_graph->update(vortex, edges);
			}	

		});
		this->learn("onMiddleDown", [this](void* args = nullptr) {

			coord* pts = (coord*)args;
			if (my_graph->bool_find(*pts)) {
				std::cout << "Found" << std::endl;
				selectedCoord_1 = *pts;
			}
			else {
				std::cout << "Not Found" << std::endl;
			}
			my_graph->update(vortex, edges);

		});
		this->learn("onMiddleUp", [this](void* args = nullptr) {

			coord* pts = (coord*)args;
			if (my_graph->bool_find(*pts)) {
				std::cout << "Found" << std::endl;
				selectedCoord_2 = *pts;
			}
			else {
				std::cout << "Not Found" << std::endl;
			}
			my_graph->update(vortex, edges);

		});
		this->learn("onKeyPressed", [this](void* args = nullptr) {
			std::vector<coord> traversal;
			std::vector<std::vector<coord>> MST;
			std::vector<coord> nb;
			std::vector<coord> bfs;
			unsigned char* key = (unsigned char*)args;
			switch (*key) {
			case 'i':
				if (iterator_Mode) {
					order::section("Iterator Mode - OFF");
					iterator_Mode = false;
					path = nullptr;
					iterator_traversal.clear();
				}
				else {
					order::section("Iterator Mode - ON");
					iterator_Mode = true;
					itr = my_graph->Begin();
					iterator_traversal.push_back(*itr);
					path = std::make_shared<graph_graphic>(new graph(iterator_traversal));
				}
				break;

			case GLUT_KEY_RIGHT:
				if (iterator_Mode) {
					itr++;
					iterator_traversal.push_back(*itr);
					path = std::make_shared<graph_graphic>(new graph(iterator_traversal));
				}
				break;

			case GLUT_KEY_LEFT:
				if (iterator_Mode) {
					itr--;
					iterator_traversal.push_back(*itr);
					path = std::make_shared<graph_graphic>(new graph(iterator_traversal));
				}
				break;

			case 'n':
				if (iterator_Mode) {
					path = std::make_shared<graph_graphic>(new graph(itr.getNeighborhood()));
					iterator_traversal.clear();
				}
				break;


			case 's':
				if (searchTwoNodes_Mode) {
					order::section("Search Two Nodes Mode - OFF");
					searchTwoNodes_Mode = false;
					path = nullptr;
				}
				else {
					order::section("Search Two Nodes Mode - ON");
					searchTwoNodes_Mode = true;
				}
				break;
				
			case 'A':
				if (searchTwoNodes_Mode) {
					order::section("A Star");
					print("Loading...");
					my_graph->A_star(selectedCoord_1, selectedCoord_2, traversal);
					path = std::make_shared<graph_graphic>(new graph(traversal));
					print("Done Loading");
					break;
				}
				else { break; }

			case 'D':
				if (searchTwoNodes_Mode) {
					order::section("DisjkTra");
					print("Loading...");
					my_graph->Dijkstra(selectedCoord_1, selectedCoord_2, traversal);
					path = std::make_shared<graph_graphic>(new graph(traversal));
					print("Done Loading");
					break;
				}
				else { break; }

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
				order::section("Dijkstra");
				order::note("");
				print("Loading...");
				my_graph->Dijkstra(selectedCoord_1, selectedCoord_2, traversal);
				print("Done Loading");
				print(traversal);
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
				my_graph->update(vortex, edges);
				break;
			}
		});
	}
};