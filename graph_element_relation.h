#pragma once
#include "element_relation.h"
#include "trituple.h"
#include "fourtuple.h"
#include <random>
#include <list>
#include <stack>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include <cmath>

#define directed true
#define not_directed false

template <typename t_element, typename t_relation, bool isDirected> 
class Graph {
  
  private:
  using node = element<t_element, t_relation>;
  using edge = relation<t_element, t_relation>;

  public:
  using ptr_n = std::shared_ptr<node>;
  using ptr_e = std::shared_ptr<edge>;

  public:
  class Iterator {
    private:
    ptr_n pointer;

    public:
    Iterator() = default;
		Iterator(ptr_n pointer): pointer{pointer} {}
		
    Iterator operator++(int){
      pointer = pointer->best_friends_forever();
      return *this;
    };

    Iterator operator--(int){
      pointer = pointer->nemesis();
      return *this;
    };

	t_element& operator* (void) {
		return pointer->get_value_access();
	}

	std::vector<t_element> getNeighborhood() {
		std::vector<ptr_n> neighbours;
		
		pointer->get_all_relations_to(neighbours);

		std::vector<t_element> last;
		for (const auto& neighbour : neighbours) {
			last.push_back(pointer->get_value());
			last.push_back(neighbour->get_value());
		}
		return last;
	}

  };

  private:
  using list_n = std::list<ptr_n>;
  using adj_list = std::vector<list_n>;
  using iterator = typename adj_list::iterator;
  using list_iterator = typename list_n::iterator;
  
  private:
  adj_list graph;

	public:
	bool bool_find(const t_element& wanted) {
		iterator it;
		for (it = begin(graph); it != end(graph); it++) {
			if (*((*it).front()) == wanted) return true;
		}
		return false;
	}
  
	private:
	iterator find(const t_element& wanted){
		iterator it;
		for (it = begin(graph); it != end(graph); it++ ){
			if(*((*it).front()) == wanted) break;
		}
			return it;
	}

	public:
	void update(std::vector<ptr_n>& vptr_n, std::vector<ptr_e>& vptr_e) {
		vptr_n.clear();
		vptr_e.clear();
		for (const auto& list : graph) {
			vptr_n.push_back((*begin(list)));
			(*(list.begin()))->get_all_relations(vptr_e);
		}
	}

	t_element return_first_element() {
		return (*(begin(graph[0])))->get_value();
	}

  public:
	//--Iterator Functions--
	Iterator Begin() {
		std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,graph.size()-1);
   	int index = dist(rng);
		Iterator it( *(graph[index].begin()) );
		return it;
	}

	Iterator End() {
		Iterator it(nullptr);
		return it;
	}

  //--CONSTRUCTORS--
	Graph() = default;

	template <typename container_t>
	Graph(const container_t& path) {
		for (const auto& it : path) {
			insertNode(it);
		}
		if (path.size() != 0) {
			for (int i = 0; i < path.size() - 1; i++) {
				insertEdge(path[i], path[i + 1]);
			}
		}
	}

	Graph(Graph& g) {
    for (auto it : g.graph) {
      insertNode((it.front())->get_value());
    }
    for (auto list : g.graph) {
      for (auto _node : list) {
        if (_node == list.front()) {continue;}
        insertEdge((list.front())->get_value(), _node->get_value()); 
      }
    }
	}

  //VTK File
	Graph(const char* s) {
    std::string line;
    std::ifstream File;
    File.open(s);
    if (!File.is_open()) {
      std::cout << "Cant open File" << std::endl;
    }
    for (int i = 0; i < 6; i++) {
      std::getline(File,line);
    }

    std::vector<std::string> row;
    std::string word;
    std::stringstream ss(line);
    
    while (std::getline(ss, word, ' ')) {
      row.push_back(word);
    }
    int num_nodes = std::stoi(row[1]);
    for (int i = 0; i < num_nodes; i++) {
      std::getline(File,line);
      t_element new_element;
      new_element = line;
      insertNode(new_element);
    }
    row.clear();
    for (int i = 0; i < 2; i++) {
      std::getline(File,line);
    }
    std::stringstream ss1(line); 
    while (std::getline(ss1, word, ' ')) {
      row.push_back(word);
    }
    unsigned int num_edges = std::stoi(row[1]);
    for (int i = 0; i < num_edges; i++) {
      std::getline (File,line);
      std::stringstream ss2(line); 
      row.clear();
      while (std::getline(ss2, word, ' ')) {
        row.push_back(word);
      }
      
      int temp = std::stoi(row[0]);
      if (temp == 1) {
        insertEdge((*begin(graph[(stoi(row[1]))]))->get_value(), (*begin(graph[(stoi(row[2]))]))->get_value());
      }
      else {
        for (int j = 0; j < temp; j++) {
        insertEdge((*begin(graph[(stoi(row[j+1]))]))->get_value(), (*begin(graph[(stoi(row[((j+1)%temp) +1]))]))->get_value());
      }
      }
      
    }
    File.close();
  }
  //Leer archivo saveado - Usar funcion de leer VTK
  //Random
	Graph(int num_nodes, int num_edges) {
    for (int i = 0; i < num_nodes; i++) {
      insertNode(t_element::create_random());
    }
    for (int i = 0; i < num_edges; i++) {
      int node_1 = 0 + ( std::rand() % ( (num_nodes-1) - 0 + 1 ) );
      int node_2 = 0 + ( std::rand() % ( (num_nodes-1) - 0 + 1 ) );
      insertEdge(((graph[node_1].front())->get_value()), ((graph[node_2].front())->get_value()));
    }
  }

  //--DESTROYER--
	/*
	~Graph() {
    ptr_n deleteNode;
    while(!graph.empty()) {
      deleteNode = ((graph.front()).front());
      if (deleteNode == nullptr) return;
      iterator i = find(deleteNode->get_value());
      graph.erase(i);
      for(auto& v: graph){
        list_iterator it = begin(v);
        for(; it != end(v); it++){
          if(*(*it) == deleteNode->get_value()){ 
            (*(*begin(v))).breakup_relationship(deleteNode->get_value());
            break;
            }
        }
        if(it != end(v)) v.erase(it);
      }
    }
  }
  */

  //--FUNCTIONS--
  int get_num_edges() {
    int num_edges = 0;
    for (auto list : graph) {
      for (auto _node : list) {
        if (_node == list.front()) {continue;}
        num_edges++;
      }
    }
    return num_edges;
  }

  int get_index_in_adj_list(t_element toFindElement) {
    for (int i = 0; i < graph.size(); i++) {
      if ( (*(graph[i].front())).get_value() == toFindElement ) {return i;}
    }
    return 0;
  }

	void saveGraphOnDisk(const char* s) {
    std::ofstream File;
    File.open(s);
    if(!File.is_open()) {
      std::cout << "Cant open file" << std::endl;
      return;
    }
    
    File << "# vtk DataFile Version 2.0\n";
    File << "grid description\n";
    File << "ASCII\n";
    File << "DATASET UNSTRUCTURED_GRID\n";
    File << "\n";
    File << "POINTS " << graph.size() << " float\n";
    for (int i = 0; i < graph.size(); i++) {
      File << (graph[i].front())->get_value_to_Output() << "\n";
    }
    File << "\n";
    File << "CELLS " << get_num_edges() << "\n";
    for (int i = 0; i < graph.size(); i++) {
      for (auto _node : graph[i]) {
        if (_node == graph[i].front()) {continue;}
        File << "1 " << i << " " << get_index_in_adj_list(_node->get_value()) << "\n";
      }
    }
	}

  template<template<typename...> class cont_cont, typename cont, typename... args>
  void get_Adj_list(cont_cont<cont, args...>& collection) {
    collection.clear();
    for (const auto& lis : graph) {
      cont _temp;
      for (const auto& ptr : lis) {
        _temp.push_back((*ptr).get_value());
      }
      if(_temp.size()>0){
        collection.push_back(_temp);
      }
    }
  }

	void insertNode(const t_element& toInsertvalue) {
    list_n newList;
    newList.push_back( std::make_shared<node>(toInsertvalue) );
    graph.push_back(newList);
	}

	void insertEdge(const t_element& _A, const t_element& _B) {
    //--DIRECTED
    if (isDirected == true) {
      auto A_it = find(_A);
      if( A_it == end(graph) ) return;

      auto B_it = find(_B);
      if( B_it == end(graph) ) return;

      ptr_n _from = (*A_it).front();
      ptr_n _to = (*B_it).front();

      if (_from->get_value() == _to->get_value()) {return;}

      for (auto check : *A_it) {
        if (check->get_value() == _from->get_value()) {continue;}
        if (check->get_value() == _to->get_value()) {return;}
      }

      (*_from).establish_new_relationship(
        std::make_shared<edge>(_from, _to)
      );

      (*A_it).push_back(_to);
    }

    //--NOT DIRECTED
    else if (isDirected == false) {
      auto A_it = find(_A);
      if( A_it == end(graph) ) return;

      auto B_it = find(_B);
      if( B_it == end(graph) ) return;

      ptr_n _from = (*A_it).front();
      ptr_n _to = (*B_it).front();

      if (_from->get_value() == _to->get_value()) {return;}

      for (auto check : *A_it) {
        if (check->get_value() == _from->get_value()) {continue;}
        if (check->get_value() == _to->get_value()) {return;}
      }

      (*_from).establish_new_relationship(
        std::make_shared<edge>(_from, _to)
      );
      (*_to).establish_new_relationship(
        std::make_shared<edge>(_to, _from)
      );

      (*A_it).push_back(_to);
      (*B_it).push_back(_from);
    }
	}


	void deleteNode(const t_element& deleteValue) {
    auto deleteNode = find(deleteValue);
    if (deleteNode == end(graph)) return;
    
    graph.erase(deleteNode);

    for(auto& v: graph){
      list_iterator it = begin(v);
      for(; it != end(v); it++){
        if(*(*it) == deleteValue){ 
          (*(*begin(v))).breakup_relationship(deleteValue);
          break;
          }
      }
      if(it != end(v)) v.erase(it);
    }
	}

	unsigned int degreeOfANode(const t_element& wantedNode) {
    unsigned int degree = 0;
    auto wanted = find(wantedNode);
    if( wanted == end(graph) ) return false;
    degree += (*(*(begin(*wanted)))).popularity();
    for(auto& v: graph){
      if (begin(v) == begin(*wanted)) {continue;}
      list_iterator it = begin(v);
      for(; it != end(v); it++){
        if(*(*it) == wantedNode){ 
          degree++;
          break;
        }
      }
    }
    return degree;
	}
  
	bool isConnected() {
    bool is = true;
    std::vector<t_element> _bfs;
    
    auto _value__ = (*(begin(graph[0])))->get_value();
    this->BFS(_value__, _bfs);

    for(const auto& v: graph){
      
      auto _value = (*(begin(v)))->get_value();
      this->BFS(_value, _bfs);

        for(const auto& v: graph){
          t_element _value_ = (*(begin(v)))->get_value();

          bool notFinded = true;
          for(auto& b : _bfs){
            if(b == _value_) {
              notFinded = false;
              break;
            }
          }

          if(notFinded){
            is = false;
            break;
          }

        }
        if(!is) break;
    }
    return is;
	}

	bool isBipartited() {
    //--Falta--
	}

	bool isDense() {
    unsigned int max_relations = graph.size()*(graph.size()-1);
    unsigned int num_relations = 0;
    for(const auto& v : graph){
      num_relations += (*(begin(v)))->popularity();
    }
    if(max_relations - num_relations < 3) {
      return true;
    }
    return false;
	}


  template<template<typename...> class container, typename... args>
	void MST_Kruskal(container<std::vector<t_element>, args...>& _mst) {
   
   std::vector<ptr_e> All_relations;
    for(const auto& v : graph) {
      (*(v.begin()))->get_all_relations(All_relations);
    }

    auto really_all_relations = All_relations;

    std::vector<ptr_n> visited;
    
    t_relation shortestDistance;
    unsigned int index = 0;
    shortestDistance = All_relations[0]->get_value();
    for(int i = 0; i < All_relations.size(); i++){
      if(shortestDistance > All_relations[i]->get_value()) {shortestDistance = All_relations[i]->get_value(); index = i;}
    }
    ptr_e shortestEdge = All_relations[index];
    All_relations.erase(begin(All_relations) + index);
    visited.push_back(shortestEdge->get_from());
    t_element _from__ = shortestEdge->get_from()->get_value();
    t_element _to__ = shortestEdge->get_to()->get_value();
    std::vector<t_element> _arr_temp = {_from__, _to__};   
    _mst.push_back(_arr_temp);
    
    bool notFinished = true;
    while (notFinished) {
      if (All_relations.empty()) {notFinished = false; break;}

      index = 0;
      shortestDistance = All_relations[0]->get_value();
      for(int i = 0; i < All_relations.size(); i++){
        if(shortestDistance > All_relations[i]->get_value()) {shortestDistance = All_relations[i]->get_value(); index = i;}
      }

      ptr_e shortestEdge = All_relations[index];

      
      All_relations.erase(begin(All_relations) + index);
      
      bool not_to = true;
      
      for(auto& vis : visited){
        if(vis == shortestEdge->get_to()){
            not_to = false;
            break;
        }
      }

      if(not_to){
        t_element _from_ = shortestEdge->get_from()->get_value();
        t_element _to_ = shortestEdge->get_to()->get_value();
        std::vector<t_element> arr_temp = {_from_, _to_};   
        _mst.push_back(arr_temp);
        
         visited.push_back(shortestEdge->get_from());
      }
 
    }
	}
	
  template<template<typename...> class container, typename... args>
	void MST_Prim(const t_element& InitialCoord, container<std::vector<t_element>, args...>& _mst) {

    iterator InitialIterator = find(InitialCoord);

    ptr_n InitialNode = *(begin(*InitialIterator));
    
    std::vector<ptr_n> visited;
    visited.push_back(InitialNode);

    bool notFinished = true;
    while (notFinished) {
      std::vector<ptr_e> posible_paths;

      for(const auto& vis : visited){
        vis->get_all_relations(posible_paths);
      }

      for(const auto& vis : visited){
        for(auto it = begin(posible_paths); it != end(posible_paths);){
          if(vis == (*it)->get_to()){
            posible_paths.erase(it);
          }else{
            it++;
          }
        }
      }
      
      if (posible_paths.empty()) {notFinished = false; break;}
      t_relation shortestDistance;
      unsigned int index = 0;
      shortestDistance = posible_paths[0]->get_value();
      for(int i = 0; i < posible_paths.size(); i++){
        if(shortestDistance > posible_paths[i]->get_value()) {shortestDistance = posible_paths[i]->get_value(); index = i;}
      }

      visited.push_back(posible_paths[index]->get_to());

      t_element _from_ = ((posible_paths[index])->get_from())->get_value();

      t_element _to_ = ((posible_paths[index])->get_to())->get_value();

      std::vector<t_element> arr_temp = {_from_, _to_};
    
      _mst.push_back(arr_temp);
    }
	}

  template<typename container>
	void BFS(const t_element& initCoord, container& collection) {
    collection.clear();
    std::list<ptr_n> BFS;
    auto initIterator = find(initCoord);
    ptr_n initNode = *(begin(*initIterator));
    initNode->isVisited = true;
    BFS.push_back(initNode);
    
    while(!BFS.empty()) {
      collection.push_back(BFS.front()->get_value());
      (BFS.front())->my_fellows(BFS);
      BFS.pop_front();
    }
    
    for (iterator it = begin(graph); it != end(graph); it++ ) {
      (*(begin(*it)))->isVisited = false;
    }
	}

  template<typename container>
	void searchTwoNodes_BFS(const t_element& initCoord, const t_element& endCoord, container& collection) {
    std::list<ptr_n> BFS;
    auto initIterator = find(initCoord);
    if (initIterator == end(graph)) {return;}
    ptr_n initNode = *(begin(*initIterator));
    initNode->isVisited = true;
    BFS.push_back(initNode);
    
    while(!BFS.empty()) {
      collection.push_back(BFS.front()->get_value());
      (BFS.front())->my_fellows(BFS);
      BFS.pop_front();
    }
    
    for (iterator it = begin(graph); it != end(graph); it++ ) {
      (*(begin(*it)))->isVisited = false;
    }
	}
	
	template<typename container>
	void DFS(const t_element& initCoord, container& collection) {
		std::stack<ptr_n> DFS;
		auto initIterator = find(initCoord);
		if (initIterator == end(graph)) { return; }
		ptr_n initNode = *(begin(*initIterator));
		initNode->isVisited = true;
		DFS.push(initNode);
		collection.push_back(DFS.top()->get_value());

		while (!DFS.empty()) {
			if ((DFS.top())->my_fellows_DFS(DFS)) {
				collection.push_back(DFS.top()->get_value());
				continue;
			}
			DFS.pop();
		}
		for (iterator it = begin(graph); it != end(graph); it++) {
			(*(begin(*it)))->isVisited = false;
		}

	}

  template<typename container>
	void searchTwoNodes_DFS(const t_element& initCoord, const t_element& endCoord, container& collection) {
    std::stack<ptr_n> DFS;
    auto initIterator = find(initCoord);
    if (initIterator == end(graph)) {return;}
    ptr_n initNode = *(begin(*initIterator));
    initNode->isVisited = true;
    DFS.push(initNode);
    collection.push_back(DFS.top()->get_value());

    while(!DFS.empty()) {
      if ((DFS.top())->my_fellows_DFS(DFS)) {
        collection.push_back(DFS.top()->get_value());
        continue;
      }
      DFS.pop();
    }

	}

  template<typename container>
  void Neighborhood(const t_element& wantedNode, container& collection) {
    
    auto it = find(wantedNode);
    if(it == end(graph) ) return;
    
    
    t_relation sum;
    unsigned int i = 0;

    for(const auto& v: graph){
      (*(*(begin(v)))).get_sum_rel(sum, i);
    }
    t_relation avg = sum / i;

    (*(*(begin(*it)))).my_fellows(avg, collection);
	}
	
	template <typename container>
	void Dijkstra(const t_element& initCoord, const t_element& endCoord, container& minimunpath) {
		auto it_init = find(initCoord);
		if (it_init == end(graph)) return;
		auto it_end = find(endCoord);
		if (it_end == end(graph)) return;

		ptr_n node_init = *(begin(*it_init)); //--Declarar nodo inicial
		ptr_n node_end = *(begin(*it_end));	//--Declarar nodo final

		std::vector<ptr_n> unvisited_nodes; //--Declarar vector de nodos no visitados
		std::vector<trituple<ptr_n, distance, ptr_n>> info;	//--Declarar vector de informacion de cada nodo
		std::vector<ptr_n> visited_nodes;	//--Declarar vector de nodos visitaddos

		for (const auto& node : graph) {	//--Añadir todos los vertices al vector nodos no visitados
			unvisited_nodes.push_back(*(node.begin()));	
		}

		info.emplace_back(node_init, 0, nullptr); //--Añadir nodo inicial al vector de informacion con valor de g = 0

		for (const auto& node : unvisited_nodes) { //--Añadir el resto de nodos al vector de informacion
			if (node == node_init) { continue; }
			info.emplace_back(node, INFINITY, nullptr);
		}
	  
		while (!unvisited_nodes.empty()) { //--Mientras que todavia hayan nodos no visitados
			ptr_n temp_node = unvisited_nodes[0];
			trituple<ptr_n, distance, ptr_n> current_node;
			for (const auto& tupla : info) { //--Conseguimos un vertice cualquiera no visitado
				if (tupla.first == temp_node) { current_node = tupla; break; }
			}

			auto current_node_iter = unvisited_nodes.begin();

			for (auto& tupla : info) { //--Tomando en cuenta le nodo declarado anterior buscamos el nodo no visitado con menor valor g, y tomamos ese como nodo actual
				if (tupla.second < current_node.second) {
					bool found = false;
					auto temp_iter = unvisited_nodes.begin();
					for (auto iter_node = unvisited_nodes.begin(); iter_node != unvisited_nodes.end(); iter_node++) {
						if (*iter_node == tupla.first) { temp_iter = iter_node; found = true; break; }
					}
					if (found) {
						current_node = tupla;
						current_node_iter = temp_iter;
					}
				}
			}
			
			std::vector<ptr_e> relations;
			current_node.first->get_all_relations(relations); //--Conseguimos vecinidad del nodo actual
			
			int i = relations.size();
			while (i > 0) { //--Eliminamos de la vecinidad nodos que ya hemos visitado
				bool found = false;
				auto rel_iter = relations.begin();
				for (; rel_iter != relations.end(); rel_iter++) {
					for (auto node : visited_nodes) {
						if ((*rel_iter)->get_to() == node) { found = true; break; }
					}
					if (found) { relations.erase(rel_iter); break; i--; }
				}
				i--;
			}
		  
			for (const auto& node : relations) { //--Para cada nodo de la vecinidad no visitado calculamos nuevo valor g y comparamos si es menor al que ya esta declarado en la informacion,  si lo es lo cambiamos sino no
				typename std::vector<trituple<ptr_n, distance, ptr_n>>::iterator temp_tri;
				for (auto tuple = info.begin(); tuple != info.end(); tuple++) {
					if ((*tuple).first == node->get_to()) { temp_tri = tuple; break; }
				}
				float currfloat = current_node.second.value_ + node->get_value().value_;
				distance currdistance(currfloat);
				if (currfloat < (*temp_tri).second.value_) {
					(*temp_tri).second = currdistance;
					(*temp_tri).third = current_node.first;
				}
			}
		  
			visited_nodes.push_back(current_node.first);	//--Añadimos nodo inicial a los nodos ya visitados
			unvisited_nodes.erase(current_node_iter); //--Eliminamos nodo inicial de los nodos no visitados
		}

		trituple<ptr_n, distance, ptr_n> final_node; //--Declaramos vector de camino
		for (const auto& tupla : info) { //-Conseguimos informacion del nodo final
			if (tupla.first == node_end) { final_node = tupla; break; }
		}

		minimunpath.push_back(final_node.first->get_value()); //--Añadimos nodo final al camino

		while (final_node.first != node_init) { //--Añadimos los valores de los nodos con los que llegamos al nodo actual al vector del camino
			minimunpath.push_back(final_node.third->get_value());
			for (const auto& tupla : info) {
				if (tupla.first == final_node.third) { final_node = tupla; break; }
			}
		}
	
	}

	template <typename container>
  void A_star(const t_element& initCoord, const t_element& endCoord, container& minimunpath) {
	  auto it_init = find(initCoord);
	  if (it_init == end(graph)) return;
	  auto it_end = find(endCoord);
	  if (it_end == end(graph)) return;

	  ptr_n node_init = *(begin(*it_init));	//--Conseguir Nodo Inicial
	  ptr_n node_end = *(begin(*it_end));	//--Conseguir Nodo Final

	  std::vector<ptr_n> open;	//--Declarar el vector de Open Vertices
	  std::vector<ptr_n> closed;	//--Declarar el vector de Closed Vertices
	  std::vector<fourtuple<ptr_n, distance, distance, ptr_n>> info;	//--Declarar el vector de informacion de los vertices

	  float _h = std::sqrt(pow((node_init->get_value().x) - (node_end->get_value().x), 2) + pow((node_init->get_value().y) - (node_end->get_value().y), 2));	//--Calcular heuristica del nodo inicial
	  distance first_f(_h);	//--Castear el valor de la heuristica al tipo de distance
	  info.emplace_back(node_init, 0, first_f, nullptr);	//--emplace_back al vector de informacion	el nodo inicial con valor g = 0

	  for (const auto& list : graph) { 	//--Añadir el resto de vertices al vector de informacion
		  if (*(list.begin()) == node_init) { continue; }
		  info.emplace_back(*(list.begin()), INFINITY, INFINITY, nullptr);
	  }

	  open.push_back(node_init);	//--Añadir el vertice inicial al vector de open vertices
	  ptr_n current_node;
	  current_node = node_init; //--Declarar como nodo actual el nodo inicial

	  while (current_node != node_end) { //--Mientras el nodo actual no sea igual al nodo final
		  std::vector<ptr_e> relationships;
		  current_node->get_all_relations(relationships); //--Conseguir la vecindad del nodo actual
		  
		  int i = relationships.size();
		  while (i > 0) { //--Verificar si los nodos vecinos ya estan visitados, si es que si eliminarlos
			  bool found = false;
			  auto rel_iter = relationships.begin();
			  for (; rel_iter != relationships.end(); rel_iter++) {
				  for (const auto& node : closed) {
					  if ((*rel_iter)->get_to() == node) { found = true; break; }
				  }
				  if (found) { relationships.erase(rel_iter); break; i--; }
			  }
			  i--;
		  }

		  for (const auto& rel : relationships) { //--Verificar si los nodos no visitados estan en el vector de open vertices sino añadirlos
			  bool found = false;
			  for (const auto& node : open) {
				  if (rel->get_to() == node) { found = true; break; }
			  }
			  if (found == false) { open.push_back(rel->get_to()); }
		  }

		  typename std::vector<fourtuple<ptr_n, distance, distance, ptr_n>>::iterator temp_current_node; //--Conseguir iterador del vector de infromacion del nodo actual correspondiente
		  for (auto tupla = info.begin(); tupla != info.end(); tupla++) {
			  if ((*tupla).first == current_node) { temp_current_node = tupla; }
		  }

		  for (const auto& rel : relationships) { //--Updatear la info de los vertices vecinos
			  typename std::vector<fourtuple<ptr_n, distance, distance, ptr_n>>::iterator temp_four;
			  for (auto tupla = info.begin(); tupla != info.end(); tupla++) { //--Buscar el vector actual
				  if ((*tupla).first == rel->get_to()) { temp_four = tupla; }
			  }

			  float g = (*temp_current_node).second.value_ + rel->get_value().value_;
			  float h = std::sqrt(pow(((rel->get_to())->get_value().x) - (node_end->get_value().x), 2) + pow(((rel->get_to())->get_value().y) - (node_end->get_value().y), 2));
			  float f = g + h; //--Calcular el valor de f sumando g = la distancia entre el nodo inicial y el nodo que se esta calculando y h = la distancia aprox. del nodo que se esta calculando al nodo final

			  if (f < (*temp_four).third.value_) { //--Si el valor de f es menor al valor de f, ya en la tabla cambiarlo
				  (*temp_four).second = distance(g);
				  (*temp_four).third = distance(f);
				  (*temp_four).fourth = current_node;
			  }

		  }
		  
		  closed.push_back(current_node);	//--Añadir nodo actual al vector de closed vertices
		  auto itr = open.begin();
		  for (; itr != open.end(); itr++) {
			  if (*itr == current_node) { break; }
		  }
		  open.erase(itr); //--Eliminar nodo actual del vector de open vertices
		  
		  fourtuple<ptr_n, distance, distance, ptr_n> temp_four;
		  ptr_n temp_open = (*open.begin());
		  for (const auto& tupla : info) {
			  if (tupla.first == temp_open) { temp_four = tupla; break; }
		  }
			
		  for (const auto& node : open) {	//--Conseguir el siguiente nodo en el vector de open vertices con el menor valor de f
			  fourtuple<ptr_n, distance, distance, ptr_n> temp;
			  for (const auto& tupla : info) {
				  if (tupla.first == node) { temp = tupla; break; }
			  }
			  if (temp.third < temp_four.third) { temp_four = temp; }
		  }
		  	current_node = temp_four.first; //--Declarar el nodo actual como el nodo del vector de open vertices con menor valor f
	  	}
	  
	  fourtuple<ptr_n, distance, distance, ptr_n> final_node; //--Declarar vector de nodos del camino
	  for (const auto& tupla : info) {
		  if (tupla.first == node_end) { final_node = tupla; break; }
	  }

	  minimunpath.push_back(final_node.first->get_value()); //--Añadir nodo final
	  
	  while (final_node.first != node_init) { //--Añadir el valor del nodo al que llegamos al nodo actual de forma iterativa hasta llegar al nodo inicial
		  minimunpath.push_back(final_node.fourth->get_value());
		  for (const auto& tupla : info) {
			  if (tupla.first == final_node.fourth) { final_node = tupla; break; }
		  }
	  }
  }

};
