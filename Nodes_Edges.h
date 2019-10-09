#pragma once
#include <cmath>
#include <vector>
using namespace std;

struct Coordinates {
	int x;
	int y;
	bool operator>(Coordinates _data) {
		if (x > _data.x) { return true; }
		if (y > _data.y) { return true; }
		return false;
	}
};

template <typename t_node, typename t_value>
struct Edge {
	t_value weight;
	t_node* from;
	t_node* to;
};

template <typename t_data, typename t_edgeval>
class Node {
	vector<Edge<Node, t_edgeval>> Edges;
	t_data data;
};

template <typename t_edgeval>
class Node<Coordinates, t_edgeval> {
	vector<Edge<Node<Coordinates, t_edgeval>, t_edgeval>> Edges;
	Coordinates data;

public:
	int distance_between_nodes(Node* n1) {
		if (data > n1->data) {
			return sqrt(pow(data.x - n1->data.x, 2) + pow(data.y - n1->data.y, 2));
		}
		else return sqrt(pow(n1->data.x - data.x, 2) + pow(n1->data.y - data.y, 2));
	}	
};
