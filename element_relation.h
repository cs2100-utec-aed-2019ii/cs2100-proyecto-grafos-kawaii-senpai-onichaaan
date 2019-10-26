#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <stack>
#include "coord.h"

//-----------------------
//Overview (no definitions)
template<typename element_type, typename relation_type>
class relation;

template<typename element_type, typename relation_type>
class element;

//-----------------------
//Traits : Alias
struct space{
  using line = relation<coord, distance>;
  using dot = element<coord, distance>;
};

template<typename type>
using ptr = std::shared_ptr<type>;

//----------------------
//Traits : Specialization
template<typename entity>
struct in_space{
  static const bool value = false;
};

template<>
struct in_space<space::line>{
  static const bool value = true;
};

template<>
struct in_space<space::dot>{
  static const bool value = true;
};


//------------------------
//Methods specialization
template<typename relation, bool in_space>
struct relation_helper{
  void static 
  calculate_content(relation* r){
    std::cout << "Not specialized"<<std::endl;
  }
};

template<typename relation>
struct relation_helper<relation, true>{
  static void 
  calculate_content(relation* r){
    r->value = std::sqrt( pow((r->from->value.x)-(r->to->value.x),2) + pow((r->from->value.y)-(r->to->value.y),2));
    //r->value.print();
  }
};

//-----------------------
//Definitions
template<typename element_type, typename relation_type>
class relation{

  template<typename _type, bool _bool>
  friend struct relation_helper;

  template<typename element_t, typename relation_t>
  friend struct element;

  private:
    using _r = relation<element_type, relation_type>;

    using _e = element<element_type, relation_type>;

    using ptr_e = ptr<_e>;

private:
    ptr_e from;
    ptr_e to;
    relation_type value;

private:
  void calculate_content(){
    relation_helper<_r, in_space<_r>::value>::calculate_content(this);
  }

public:
  relation() = default;
  relation(ptr_e _from, ptr_e _to, relation_type _value) {
    from = _from;
    to = _to;
    value = _value;
  }
  relation(ptr_e _from, ptr_e _to){
    from = _from;
    to = _to;
    this->calculate_content();
  }
  relation_type get_value() {
    return value;
  }
  ptr_e get_to() {return to;}
  ptr_e get_from() {return from;}

};

template<typename element_type, typename relation_type>
class element{
  private:
  using _r = relation<element_type, relation_type>;
  using _e = element<element_type, relation_type>;
  using ptr_r = ptr<_r>;
  using ptr_e = ptr<_e>;

  private:
  template<typename _type, bool _bool>
  friend struct relation_helper;

  private:
  element_type value;
  std::vector<ptr_r> my_relations;

  public:
  bool isVisited = false;


  public:
    element(element_type value): value{value}{};
    

  public:
    
    void establish_new_relationship(ptr_r new_relationship) {
      my_relations.push_back(new_relationship);
    }

    bool operator== (const element_type& c){
      return value == c;
    }

    void breakup_relationship(const element_type& c) {
      typename std::vector<ptr_r>::iterator it = begin(my_relations);
      for (; it != end(my_relations); it++) {
        if ((*it)->to->value == c) {my_relations.erase(it); 
        break;}
      }
    }

    unsigned int popularity()const{
      return my_relations.size();
    }

    void get_sum_rel(relation_type& sum, unsigned int counter ){
      for(const auto& v: my_relations){
        sum + (*v).value;
        counter++;
      }
    }

    template<typename container>
    void my_fellows(const relation_type& avg, container& collection){
      for(const auto& v: my_relations){
        if((*v).value <= avg) collection.push_back((*v).to->value);
      }
    }

    template<typename container>
    void my_fellows(container& collection){
      for(const auto& v: my_relations){
        if ((*v).to->isVisited == false) {
          collection.push_back((*v).to);
          (*v).to->isVisited = true;
        }
      }
    }

    bool my_fellows_DFS(std::stack<ptr_e>& collection) {
      for (const auto& v: my_relations) {
        if ((*v).to->isVisited == false) {
          collection.push((*v).to);
          (*v).to->isVisited = true;
          return true;
        }
      }
      return false;
    }

    element_type get_value(){
      return value;
    }

    std::string get_value_to_Output() {
      std::string s;
      s = s + std::to_string(value.x) + ' ' + std::to_string(value.y);
      return s;
    }

    void get_all_relations(std::vector<ptr_r>& container){
      for(int i = 0; i < my_relations.size(); i++) {
        container.push_back(my_relations[i]);
      }
    }
};
