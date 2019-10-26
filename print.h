#include <iostream>

//Library by José Ignacio Huby Ochoa
//Lima, Perú
//CS, UTEC
//2019

namespace{
  
  template<typename type>
  void print(const type& _value){
    std::cout<<_value;
  }
  
  template<template<typename...>class container, typename type, typename... args>
  void print(const container<type, args...>& collection){
    
    for(const auto& c : collection){
      print(c);
      print("\t");
    }
    

  }
  
  
  template<template<typename...>class container, typename... args, template<typename...>class subcont, typename... subargs>
  void print(const container<subcont<subargs...>, args...>& collection){
    print('\n');
    print(">>>");
    print('\n');
    for(const auto& c: collection){
      print(c);
      print('\n');
    }
     print('\n');
    print("<<<");
    print('\n');
}
}
