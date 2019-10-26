#pragma once
#include "element_relation.h"
#include <list>
#include <stack>
#include <string>
#include <fstream>

template <typename t_element, typename t_relation, bool isDirected> 
class Graph {
  
  private:
  using node = element<t_element, t_relation>;
  using edge = relation<t_element, t_relation>;

  public:
  using ptr_n = std::shared_ptr<node>;
  using ptr_e = std::shared_ptr<edge>;

  private:
  using list_n = std::list<ptr_n>;
  using adj_list = std::vector<list_n>;
  using iterator = typename adj_list::iterator;
  using list_iterator = typename list_n::iterator;
  
  private:
  adj_list graph;

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
  //--CONSTRUCTORS--
  Graph() = default;
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
		auto A_it = find(toInsertvalue);
		if (A_it != end(graph)) return;

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
      if (*(begin(v)) == *(begin(*wanted))) {continue;}
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
    //--We Tried Hard and With Many Hours of Sweat and Tears and Ochinan--
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
		for (const auto& v : graph) {
			(*(v.begin()))->get_all_relations(All_relations);
		}

		auto really_all_relations = All_relations;

		t_relation shortestDistance;
		unsigned int index = 0;
		shortestDistance = All_relations[0]->get_value();
		for (int i = 0; i < All_relations.size(); i++) {
			if (shortestDistance > All_relations[i]->get_value()) { shortestDistance = All_relations[i]->get_value(); index = i; }
		}
		ptr_e shortestEdge = All_relations[index];
		All_relations.erase(begin(All_relations) + index);
		
		MST_Prim(shortestEdge->get_from()->get_value(), _mst);

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
		  for (int i = 0; i < posible_paths.size();) {
          if(vis == (posible_paths[i])->get_to()){
            posible_paths.erase(begin(posible_paths) + i);
          }else{
            i++;
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
	  if (BFS.front()->get_value() == endCoord) {
		  for (iterator it = begin(graph); it != end(graph); it++) {
			  (*(begin(*it)))->isVisited = false;
		  }
		  return;
	  }
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
		if (DFS.top()->get_value() == endCoord) {
			for (iterator it = begin(graph); it != end(graph); it++) {
				(*(begin(*it)))->isVisited = false;
			}
			return;
		}
        continue;
      }
      DFS.pop();
    }
	for (iterator it = begin(graph); it != end(graph); it++) {
		(*(begin(*it)))->isVisited = false;
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
};
