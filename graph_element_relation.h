#include "element_relation.h"
#include <list>
#include <vector>
#include <memory>

template <typename t_element, typename t_relation, bool isDirected> 
class Graph {
  
  private:
  using node = element<t_element, t_relation>;
  using edge = relation<t_element, t_relation>;

  private:
  using ptr_n = std::shared_ptr<node>;
  using ptr_e = std::shared_ptr<edge>;

  private:
  using list_n = std::list<ptr_n>;
  using adj_list = std::vector<list_n>;
  using iterator = typename adj_list::iterator;
  
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
  Graph() = default;
	Graph(Graph& g) {

	}
	//Graph( load from vtk file )
	//Graph( reload saved graph )
	//Graph( Random )
	~Graph() {} //Destroy All Graph

	void saveGraphOnDisk() {
    //--Falta--
	}

  void printAdj_list() {
    for (auto lis : graph) {
      for (auto ptr : lis) {
        (*ptr).print_element();
        std::cout << "->";
      }
      std::cout << std::endl;
    }
  }

	void insertNode(const t_element& toInsertvalue) {
    list_n newList;
    newList.push_back( std::make_shared<node>(toInsertvalue) );
    graph.push_back(newList);
	}

	void insertEdge(const t_element& _A, const t_element& _B) {
    auto A_it = find(_A);
    if( A_it == end(graph) ) return;

    auto B_it = find(_B);
    if( B_it == end(graph) ) return;

    ptr_n _from = (*A_it).front();
    ptr_n _to = (*B_it).front();

    (*_from).establish_new_relationship(
      std::make_shared<edge>(_from, _to)
    );

    (*A_it).push_back(_to);

	}

	void deleteNode() {
    //--Falta--
	}

	void gradeOfANode() {
    //--Falta--
	}

	bool isConnected() {
    //--Falta--
	}

	bool isBipartited() {
    //--Falta--
	}

	bool isDense() {
    //--Falta--
	}

	void MST_Kruskal() {
    //--Falta--
	}
	
	void MST_Prim() {
    //--Falta--
	}

	void searchTwoNodes_BFS() {
    //--Falta--
	}

	void searchTwoNodes_DFS() {
    //--Falta--
	}

	void Neighborhood() {
    //--Falta--
	}
  
};
