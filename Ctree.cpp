#include <vector>
#include <cerrno>
#include <iostream>
#include <string>
#include <algorithm>
#include "Ctree.h"

using namespace std;


// Build and Destruction
Ctree::Ctree(){
    vector<Ctree*> m_sons;
}

Ctree::Ctree(int lvl, int area){
    m_level = lvl;
    m_area  = area;
    m_highest = lvl;
}

Ctree::~Ctree(){
    for (int i=0; i<m_sons.size(); i++){
        delete m_sons[i];
    }
}



// Data Access
int Ctree::getLevel(){
    return m_level;
}

int Ctree::getArea(){
    return m_area;
}

int Ctree::getHighest(){
    return m_highest;
}

Ctree* Ctree::getSon(int pos){
    return m_sons[pos];
}

vector<Ctree*> Ctree::getSons(){
    return m_sons;
}

int Ctree::nbSons(){
    return int(m_sons.size());
}


// Data Settings
void Ctree::setLevel(int level){
    m_level=level;
}

void Ctree::setArea(int area){
    m_area=area;
}

void Ctree::setHighest(int highest){
    m_highest = highest;
}

void Ctree::addAsLastSon(Ctree *newSon){
    m_sons.push_back(newSon);
}

void Ctree::adoptSons(Ctree* prevFather){
    //the sons of prevFather are added to the Ctree
    for (int i=0; i<prevFather->nbSons(); i++){
        m_sons.push_back(prevFather->getSons()[i]);
    }
    //and prevFather forgets them, to avoid double deletion
    prevFather->m_sons={};
}

void Ctree::integrateData(Ctree *mergedNode){
    //cout<<"integrate used"<<endl;
    m_area += mergedNode->getArea();
    m_highest = max(m_highest,mergedNode->getHighest());
}

// Other Methods
void Ctree::display(string prefix, string indent){
    cout << prefix << "[" << m_level  << "] " << m_area << endl;
    prefix += indent;
    for (vector<Ctree*>::iterator it = m_sons.begin(); it != m_sons.end(); ++it) {
        (*it)->display(prefix, indent);
    }
}