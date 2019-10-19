#pragma once
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "reactive.h" //By J. Huby Ochoa as well
#define ECHAP 27

//By José Ignacio Huby Ochoa
// Lima, Perú
// CS - UTEC
using subject = rct::subject;
using observer = rct::observer;


struct manager {
public:
	using time = float;
	using numeric = float;
	using spectrum = float;
	using text = const char*;
	

public:
	struct space {
		numeric x;
		numeric y;
		numeric z;
		void define(numeric _x, numeric _y, numeric _z) {
			x = _x;
			y = _y;
			z = _z;
		}
	};

	struct color {
		spectrum red;
		spectrum green;
		spectrum blue;
		spectrum alpha;
		void define(spectrum _red, spectrum _green, spectrum _blue, spectrum _alpha) {
			red = _red;
			green = _green;
			blue = _blue;
			alpha = _alpha;
		}
	};

public:
	static space limits;
	static space camera;
	static time tick_cycle;
	static color bg_color;
	static text title;
	static subject universe;

public:
	static void set_limits(numeric x, numeric y, numeric z) {
		limits.define(x, y, z);
	}
	static void set_camera(numeric x, numeric y) {
		camera.define(x, y, 0);
	}
	static void set_tick_cycle(time cycle) {
		tick_cycle = cycle;
	}
	static void set_bg_color(spectrum red, spectrum green, spectrum blue, spectrum alpha) {
		bg_color.define(red, green, blue, alpha);
	}
	static void set_title(text _title) {
		title = _title;
	}

private:

	static GLvoid initGL() {
		glClearColor(bg_color.red, bg_color.green, bg_color.blue, bg_color.alpha);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
	}

	static GLvoid window_display() {
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0.0f, limits.x, 0.0f, limits.y, 0.0f, limits.z);

		universe.trigger("draw");

		glutSwapBuffers();
		glFlush();
	}

	static GLvoid window_reshape(GLsizei width, GLsizei height) {
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, limits.x, 0.0f, limits.y, 0.0f, limits.z);
		glMatrixMode(GL_MODELVIEW);
		//	gluLookAt(0,50, 100, 0,0,0, 0, 1, 0);

	}

	static GLvoid window_key(unsigned char key, int x, int y) {
		switch (key) {
		case ECHAP:
			exit(1);
			break;

		default:
			break;
		}
	}

	static GLvoid callback_mouse(int button, int state, int x, int y) {
		if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {

		}
	}

	static void Timer(int value) {
		glutPostRedisplay();
		universe.trigger("update");
		glutTimerFunc(tick_cycle, Timer, 10);
	}

public:
	static void play(int argc, char* argv[]) {
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitWindowSize(camera.x, camera.y);
		glutInitWindowPosition(100, 100);
		glutCreateWindow(title);
		initGL();
		glEnable(GL_TEXTURE_2D);
		glutDisplayFunc(&window_display);
		glutReshapeFunc(&window_reshape);
		glutKeyboardFunc(&window_key);
		glutTimerFunc(tick_cycle, &Timer, 1);
		glutMouseFunc(&callback_mouse);
		glutMainLoop();
	}

};

manager::space manager::limits;
manager::space manager::camera;
manager::color manager::bg_color;
manager::time manager::tick_cycle;
manager::text manager::title;
subject manager::universe;



struct basic {
	struct dot {
		float x;
		float y;
		float z;

		dot(float x, float y, float z) : x{ x }, y{ y }, z{ z }{}

		static dot create(float _x, float _y, float _z) {
			dot new_dot(_x, _y, _z);
			return new_dot;
		}
	};

	struct body : public rct::observer {
		dot ubication;
		dot size;
		dot rotation;
		float k_rotation;
		float v_rotation;
		body(dot ubication, dot size, dot rotation, float k_rotation, float v_rotation) : ubication{ ubication }, size{ size }, rotation{ rotation }, k_rotation{ k_rotation }, v_rotation{ v_rotation }
		{
			learn("draw", [this](void* args = nullptr) {
				glPushMatrix();
				glTranslatef(this->ubication.x, this->ubication.y, this->ubication.z);
				glRotatef(this->k_rotation, this->rotation.x, this->rotation.y, this->rotation.z);
				glutWireSphere(this->size.x, 25, 25);

				glPopMatrix();
			});
			learn("rotate", [this](void* args = nullptr) {
				this->k_rotation += this->v_rotation;
			});
			learn("update", [this](void* args = nullptr) {this->apply("rotate"); });
		}
	};

	struct satelite : public body {
		using ptr_c = std::shared_ptr<body>;
		ptr_c c_spin;
		float v_spin;
		float g_spin = 0;
		float r_spin;
		satelite(dot ubication, dot size, dot rotation, float k_rotation, float v_rotation, ptr_c c_spin, float r_spin, float v_spin) :
			body{ ubication, size, rotation,  k_rotation, v_rotation }, c_spin{ c_spin }, r_spin{ r_spin }, v_spin{ v_spin }
		{
			learn("spin", [this](void* args = nullptr) {
				this->g_spin += this->v_spin;
				this->ubication.x = (this->r_spin * cos(this->g_spin)) + this->c_spin->ubication.x;
				this->ubication.y = (this->r_spin * sin(this->g_spin)) + this->c_spin->ubication.y;
			});
			learn("update", [this](void* args = nullptr) {this->apply("rotate"); this->apply("spin"); });
		}
	};
};