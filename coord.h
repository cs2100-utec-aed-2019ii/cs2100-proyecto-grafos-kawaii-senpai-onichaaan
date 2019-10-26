#pragma once
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
/*
  Structs important characteristics:
    1.  operator ==
    2.  operator << std::ostream
*/
struct coord{
  float x;
  float y;
  coord() = default;
  coord(int _x, int _y) {
    x=_x; y=_y;
  }
  
  void operator= (const std::string& s) {
    std::vector<std::string> row;
    std::string word;
    std::stringstream ss(s);
      while (std::getline(ss, word, ' ')) { 
       row.push_back(word);
      }
    x = std::stof(row[0]);
    y = std::stof(row[1]);
  }

  bool operator== (const coord& c){
  return this->x == c.x && this->y == c.y;
  }

  static coord create_random() {
    coord temp(std::rand()%100,std::rand()%100);
    return temp;
  }

  static coord create(float _x, float _y){
    coord temp(_x, _y);
    return temp;
  }

  friend std::ostream& operator<<(std::ostream& os, const coord& crd);

};

std::ostream& operator<<(std::ostream& os, const coord& crd)
{
    os << '(' << crd.x << ',' << crd.y << ')';
    return os;
}



struct distance{
  float value_ = 0;
  distance() = default;
  distance(float _value) {
    value_=_value;
  } 

  void operator+= (const distance& d){
	  value_ += d.value_;
  }

  void operator= (const distance& d) {
	  value_ = d.value_;
  }

  distance operator/ (unsigned int _parts){
    float parts = _parts;
    distance new_distance(value_/parts);
    return new_distance;
  }

  bool operator<= (const distance& d){
    return value_ <= d.value_;
  }

  bool operator< (const distance& d){
    return value_ < d.value_;
  }

  bool operator> (const distance& d){
    return value_ > d.value_;
  }

  friend std::ostream& operator<<(std::ostream& os, const distance& dt);

};



std::ostream& operator<<(std::ostream& os, const distance& dt)
{
    os << dt.value_;
    return os;
}


