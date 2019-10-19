#pragma once
struct example {
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
			learn("onTick", [this](void* args = nullptr) {this->apply("rotate"); });
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
			learn("onTick", [this](void* args = nullptr) {this->apply("rotate"); this->apply("spin"); });
		}
	};
};