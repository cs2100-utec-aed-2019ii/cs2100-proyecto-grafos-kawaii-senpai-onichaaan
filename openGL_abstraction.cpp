#include "openGL_react_graph.h" //Also by J. Huby Ochoa

//By José Ignacio Huby Ochoa
// Lima, Perú
// CS - UTEC

/*
--------------------------------
1. Go to openGL_react_graph.h
and replace the commented section
in "draw_vortex" and "draw_edge"
with the values that correspond to
your implementation.
-------------------------------
*/


/*
--------------------------------
2. Read the following fake_example_
graph and make sure you have this
alias and atributes.
-------------------------------
*/

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


/*
--------------------------------
3. Replace "fake_example_graph"
alias with your "real graph name"
and initialize your graph like
"g" if you haven't already.
-------------------------------
*/

int main(int argc, char* argv[]) {
	using graph = fake_example_graph; //replace this for the real graph name
	using graph_g = graph_graphic< graph::ptr_c_e, graph::ptr_c_v>;

	graph g;

	manager::universe.attach("my_graph_A", std::make_shared<graph_g>(

		g.edges,
		g.vortexs

		));
	manager::set_camera(600, 600);
	manager::set_limits(600, 600, 100);
	manager::set_tick_cycle(10);
	manager::set_bg_color(0, 0, 0, 1);
	manager::set_title("Grafos");
	manager::play(argc, argv);
}