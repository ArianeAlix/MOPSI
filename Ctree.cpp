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

Ctree::Ctree(int lvl, int area, int label){
    m_level = lvl;
    m_area  = area;
    m_highest = lvl;
    m_volume = area;
    m_label = label;
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

int Ctree::getVolume(){
    return m_volume;
}

int Ctree::getLabel(){
    return m_label;
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

void Ctree::setVolume(int volume){
    m_volume = volume;
}

void Ctree::setLabel(int label){
    m_label = label;
}

void Ctree::addAsLastSon(Ctree *newSon){
    m_sons.push_back(newSon);
    m_volume = m_volume + newSon->getVolume() + newSon->getArea()*(newSon->getLevel()-m_level-1);
}

void Ctree::adoptSons(Ctree* prevFather){
    //the sons of prevFather are added to the Ctree
    for (int i=0; i<prevFather->nbSons(); i++){
        addAsLastSon(prevFather->getSons()[i]);
    }
    //and prevFather forgets them, to avoid double deletion
    prevFather->m_sons={};
}

void Ctree::integrateData(Ctree *mergedNode){
    //cout<<"integrate used"<<endl;
    m_area += mergedNode->getArea();
    m_volume += mergedNode->getArea();
    m_highest = max(m_highest,mergedNode->getHighest());
}

// Other Methods
void Ctree::display(string prefix, string indent){
    cout << prefix << "[" << m_level  << " , " <<  m_label  << "] " << m_area << ", " << m_volume << endl;
    prefix += indent;
    for (vector<Ctree*>::iterator it = m_sons.begin(); it != m_sons.end(); ++it) {
        (*it)->display(prefix, indent);
    }
}