#include "openGL_react.h"

//By José Ignacio Huby Ochoa
// Lima, Perú
// CS - UTEC


/*	
	--------------------------------------
	1.Create classes that derive from observer and use "learn" 
	method to implement functions in the constructor of the class
	(more info in reactive.h and openGL_react: basics)
	--------------------------------------
*/
// For 1. example see openGL_react:basics


int main(int argc, char* argv[]) {

	/*
	--------------------------------------
	2.Create and add objects to manager::universe
	(this are examples, they do not need to have the same 
	constructure arguments)
	--------------------------------------
*/
	manager::universe.attach("Sol", std::make_shared<basic::body>(

		basic::dot::create(300, 300, 1), //Ubication
		basic::dot::create(100, 0, 0), //Size (sphere only x)
		basic::dot::create(0.2, 0.9, 0.7), //Rotation
		10, //Rotation angle
		1 //Rotation velocity

		));

	manager::universe.attach("Tierra", std::make_shared<basic::satelite>(

		basic::dot::create(300, 300, 1), //Ubication
		basic::dot::create(50, 0, 0), //Size (sphere only x)
		basic::dot::create(-1, -0.6, -0.8), //Rotation
		0.1, //Rotation angle
		2, //Rotation velocity
		std::static_pointer_cast<basic::body>(manager::universe["Sol"]),
		200,
		0.01
		));



	/*
	--------------------------------------
	3.Set up, initialize and execute graphic window 
	--------------------------------------
*/
	manager::set_bg_color(0, 0, 0, 1);
	manager::set_camera(600, 600);
	manager::set_limits(600, 600, 100);
	manager::set_tick_cycle(10);
	manager::set_title("Hola");
	manager::play(argc, argv);


	return true;
}