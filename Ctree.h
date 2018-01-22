#pragma once
#include <vector>
#include <cerrno>
#include <iostream>
#include <string>
#include <algorithm>


// Class to represent a Component Tree
class Ctree{

private:
    int m_level;
    int m_area;
    int m_highest;
    int m_volume;
    int m_label;

public:
    std::vector<Ctree*> m_sons;
    // Build and Destruction
    Ctree();
    Ctree(int lvl, int area, int label);
    ~Ctree();

    // Data access
    int getLevel();
    int getArea();
    int getHighest();
    int getVolume();
    int getLabel();
    // Return the son at position pos, if any (considering left-most son is at position 0)
    // Return an error if no son exists at the given position without exiting
    Ctree* getSon(int pos);
    std::vector<Ctree*> getSons();
    // Return the number of sons of this node
    int nbSons();

    // Data Setting
    void setLevel(int level);
    void setVolume(int volume);
    void setArea(int area);
    void setHighest(int highest);
    void setLabel(int label);
    // Add newSon as supplementary right-most son of this node
    void addAsLastSon(Ctree* newSon);
    // Add prevFather.getSons as supplementary right-most sons of this node, and deals with deletion issues
    void adoptSons(Ctree* prevFather);
    // Adds the data of another node which is integrated
    void integrateData(Ctree* mergedNode);

    // Other methods
    // Displaying the tree with a depth-first search
    void display(std::string prefix="", std::string indent="  ");
};