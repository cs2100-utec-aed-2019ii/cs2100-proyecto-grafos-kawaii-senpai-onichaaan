#include <iostream>
#include "graph_element_relation.h"

int main() {
  Graph<coord, distance, true> g1;

  g1.insertNode(coord::create(10, 20));
  g1.insertNode(coord::create(20, 30));
  g1.insertEdge(coord::create(10,20), coord::create(20, 30));
  g1.printAdj_list();
}