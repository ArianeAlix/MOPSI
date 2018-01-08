#pragma once
#include <vector>
#include <cerrno>
#include <iostream>
#include <string>
#include <algorithm>

class Tree {
private:
    // Node information
    bool m_seen;// to know if we have already processed it
    Tree* m_parent;
    int m_rank;
    int m_label;

    //For the N-lobes filter
    //int mark;
public:
    //Build and Destruction
    Tree();
    Tree(int label);
    ~Tree();


    //Methods
    // Create the set of a node
    void makeSet();
    // Find the canonical node of the set
    Tree* find();
    // Link two sets
    Tree* link(Tree* y);
    // PARTIALLY Displays the tree for tests
    void pdisplay(std::string *prefixe);



    // DATA ACCESS AND MODIFICATION
    // Return information of this node
    int getRank();
    int getLabel();
    Tree* getParent();
    // Check is the node was already processed
    bool wasSeen();
    void setRank(int rank);
    void setParent(Tree* parent);
    // Tell the node has been seen
    void isSeen();
};