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
	coord selectedCoord;
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
			selectedCoord = *pts;

		});
		this->learn("onMiddleUp", [this](void* args = nullptr) {

			coord* pts = (coord*)args;
			my_graph->insertEdge(selectedCoord, *pts);
			my_graph->update(vortex, edges);

		});
		this->learn("onKeyPressed", [this](void* args = nullptr) {
			std::vector<coord> traversal;
			std::vector<std::vector<coord>> MST;
			unsigned char* key = (unsigned char*)args;
			switch (*key) {
			case '1':
				my_graph->BFS(my_graph->return_first_element(), traversal);
				order::section("BFS of all graph");
				print(traversal);
				break;

			case '2':
				my_graph->DFS(my_graph->return_first_element(), traversal);
				order::section("DFS of all graph");
				print(traversal);
				break;

			case '3':
				order::section("Is Connected");
				order::note("Is the graph connected?");
				print(my_graph->isConnected());
				break;

			case '4':
				order::section("Save Graph");
				order::note("Saved graph to a vtk file");
				my_graph->saveGraphOnDisk("OutputFile.vtk");
				break;

			case '5':
				order::section("Is Dense");
				order::note("Is the graph dense?");
				print(my_graph->isDense());
				break;

			case '6':
				order::section("Prim");
				order::note("Creating the Minimun Spanning Tree using Prim from first value of graph");
				my_graph->MST_Prim(my_graph->return_first_element(), MST);
				print(MST);
				break;

			case '7':
				order::section("Kruskal");
				order::note("Creating the Minimun Spanning Tree using Kruskal");
				my_graph->MST_Kruskal(MST);
				print(MST);
				break;
			}
		});
	}
};