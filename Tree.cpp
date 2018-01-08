#include "Tree.h"
#include <iostream>

using namespace std;

Tree::Tree() {
    m_seen = false;
    m_parent = this;
    m_rank = 0;
};

Tree::Tree(int label){
    m_seen = false;
    m_parent = this;
    m_rank = 0;
    m_label = label;
}


Tree::~Tree(){};

void Tree::makeSet() {
    m_parent = this;
    m_rank = 0;
};

Tree* Tree::find() {
    if (m_parent != this) {
        //cout<<"modif";
        m_parent = m_parent->find();
    }
    return m_parent;
};

//Attention ne change pas les rangs des parents!
//->Toujours l'appeler sur des elements canoniques
Tree* Tree::link(Tree* y) {
    if (m_rank > y->m_rank) {
        y->m_parent = this;
        return this;
    }
    if (m_rank == y->m_rank) {
        y->m_rank += 1;
    }
    m_parent = y;
    return y;
};

// Attnetion the tree is only PARTIALLY displayed
void Tree::pdisplay(string* prefixe){
    if (m_parent != this){
        m_parent->pdisplay(prefixe);
    }
    *prefixe = "  "+*prefixe;
    cout<<*prefixe<<" "<<m_rank<<endl;
}


// Access and Modification of Data
int Tree::getLabel(){
    return m_label;
}

int Tree::getRank(){
    return m_rank;
}
void Tree::setRank(int rank){
    m_rank = rank;
}

Tree* Tree::getParent(){
    return m_parent;
}
void Tree::setParent(Tree* parent){
    m_parent = parent;
}

bool Tree::wasSeen() {
    return m_seen;
};

void Tree::isSeen() {
    m_seen = true;
}

