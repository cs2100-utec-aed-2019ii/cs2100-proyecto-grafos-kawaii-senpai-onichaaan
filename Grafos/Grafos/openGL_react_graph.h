#pragma once
#include "openGL_react.h" //Also by J. Huby Ochoa

//By José Ignacio Huby Ochoa
// Lima, Perú
// CS - UTEC

template<typename ptr_c_e, typename ptr_c_v >
class graph_graphic : public observer{

private:
	ptr_c_e edges;
	ptr_c_v vortex;
public:
	graph_graphic(ptr_c_e edges, ptr_c_v vortex): edges{ edges }, vortex{ vortex } {
		this->learn("draw_vortex", [this](void* args = nullptr) {
			for (auto v : *(this->vortex)) {
				glPushMatrix();
				//glTranslatef(v->x, v->y, v->z);
				//glutSolidSphere(v->rad, 10, 10);
				glPopMatrix();
			}
		});
		this->learn("draw_edges", [this](void* args = nullptr) {
			for (auto e : *(this->edges)) {
				glPushMatrix();
				//glBegin(GL_LINES);
				//glVertex3f(e->opening->x, e->opening->y, e->opening->z);
				//glVertex3f(e->ending->x, e->ending->y, e->ending->z);
				//glEnd();
				glPopMatrix();
			}
		});
		this->learn("draw", [this](void* args = nullptr) {

			this->apply("draw_vortex");
			this->apply("draw_edges");

		});
	}
};