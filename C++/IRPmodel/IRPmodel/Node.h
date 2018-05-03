#pragma once
#include <vector>;


using namespace std;
class Node
{

public:
	class Edge {
	public:
		//Edge(Edge& edge);
		Edge(Node& endNode, double value);
		//Declared virtual to invoke destructor of any derived classes, avoids memory leak
		virtual ~Edge();
		double getValue();
		void setValue(double);
		Node *getEndNode();

	private:
		double Value;
		Node &EndNode;
	};


	//Constructores
	Node(int id, vector<Edge*> edges);
	Node(const Node&);
	Node(int id);

	virtual Node& operator =(const Node&);
	//Destructor
	virtual ~Node();

	//get functions
	Edge * getEdge(Node & n);
	int getnEdges();
	//Returns the first edge
	Edge * getEdge();
	vector <Edge*> getEdges();
	int getState();
	int getId() const;

	//modifier functions
	void setId(int id);
	bool isDepot();
	void setState(int s);
	Edge* addEdge(double value, Node * child);
	Edge* addEdge(Edge *);
	bool hasEdge(Edge *);
	Edge* addEdge(Node * child);
	void removeEdge(Node &child);
	void deleteEdges();
	void deleteEdge(Node *);
	void removeEdges();
	
	//Operator overloading
	bool operator==(const Node &) const;
	bool operator!=(const Node &node) const
	{
		return !(*this == node); // invokes Array::operator==
	}
	//Possible state constants
	static const int TABU_EDGE = 31;
	static const int REMOVE = 32;

private:
	int NodeID;
	int State;

	vector <Edge*> Edges;
};

