#pragma once
#include <vector>
#include <cerrno>
#include <iostream>
#include <string>
using namespace std;

// Node of a tree containing some data at each node
class Tree {
private:
    // Node information
	int label;
    int area;
	int level;
	int highest;
	bool seen;// to know if we have already processed it
    // Sequence of sons (empty if none)
    vector<Tree*> sons;
	// Info on its set
	Tree* par1;
	Tree* par2;
	int rank1;
	int rank2;

public:
	//BUILD
	Tree();

    // Create a node with given information
    Tree(int lbl, int lvl);

    // Destruct a node and all its descendants
	void delProf(Tree arb);
    ~Tree();


	//ITS SET
	// Create the set of a node 
	void makeSet1();
	void makeSet2();// considering p as a tree

	// Find the canonical node of the set
	Tree* find1();
	Tree* find2();// considering p as a tree

	// Link two sets
	Tree* link(Tree* y);

	//Merge two nodes
	Tree* merge(Tree* y);


	// INFOS ACCESS AND MODIFICATION
    // Return information of this node
	int getLabel();
    int getLevel();
	int getArea();
	int getHighest();
    // Set information of this node
    void setLevel(int lvl);
	void setArea(int a);
	void setHighest(int h);
    // Return the number of sons of this node
    int nbSons();

    // Return the son at position pos, if any (considering left-most son is at position 0)
	// Return an error if no son exists at the given position without exiting 
    Tree* getSon(int pos);

	// Check is the node was already processed
	bool wasSeen();


    // Add newSon as supplementary right-most son of this node
    void addAsLastSon(Tree* newSon);

	// Displaying the tree with a depth-first search
	void display(string prefix="", string indent="  ");
};