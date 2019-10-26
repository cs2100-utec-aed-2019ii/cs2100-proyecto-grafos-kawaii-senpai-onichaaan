#include <iostream>
#include <string>

//Library by José Ignacio Huby Ochoa
//Lima, Perú
//CS, UTEC
//2019

namespace order{
  template<typename type>
  void print(type text){
    std::cout<<text;
  }

  void line(){
    print("---------\n");
  }

  void section(std::string title){
    print("\n\n\n");
    line();
    print("SECTION: ");
    print(title);
    print("\n");
    line();
    print("\n");
  }

  void note(std::string text){
    print("\n");
    print("[");
    print(text);
    print("]");
    print("\n");
    print("\n");
  }

}