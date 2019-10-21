struct coord{
  float x;
  float y;
  coord(int _x, int _y) {
    x=_x; y=_y;
  }
  void print() {
    std::cout << x << ',' << y; 
  }
  bool operator== (const coord& c){
  return this->x == c.x && this->y == c.y;
  }

  static coord create(float _x, float _y){
    coord temp(_x, _y);
    return temp;
  }

};

struct distance{
  float value;
  distance() = default;
  distance(float _value) {
    value=_value;
  } 
  void print() {
    std::cout << value;
  }
};
