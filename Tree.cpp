#include "Tree.h"


Tree::Tree(int lbl, int lvl) {
	label = lbl;
	level = lvl;
	area = 1;
	highest = lvl;
	vector<Tree*> sons = {};
};

void Tree::delProf(Tree arb) {
	//Détruit l'arbre en remontant à partir de la racine "arb"
	for (vector<Tree*>::iterator it = arb.sons.begin(); it != arb.sons.end(); ++it) {
		delProf(**it);
		delete *it;
	}
}

Tree::~Tree() {
	delProf(*this);
};



void Tree::makeSet() {
	par = this;
	rank = 0;
};


Tree* Tree::find() {
	if (par != this) {
		par = (*par).find();
	}
	return par;
};


Tree* Tree::link(Tree* y) {
	if (rank > y->rank) {
		y-> par = this;
		return this;
	}
	if (rank == y->rank) {
		y->rank += 1;
	}
	par = y;
	return y;
};





int Tree::getLevel() {
	return level;
};

void Tree::setLevel(int lvl) {
	level = lvl;
};

int Tree::nbSons() {
	return sons.size();
};

Tree* Tree::getSon(int pos) {
	try {
		return sons.at(pos);
	}
	catch (out_of_range &exn) {
		cerr << "Out of range :" << exn.what() << endl;
	}
};


void Tree::addAsLastSon(Tree* newSon) {
	sons.push_back(newSon);
};




void Tree::display(string prefix, string indent){
	cout << prefix << "[" << label  << "] "  << level << endl;
	prefix += indent;
	for (vector<Tree*>::iterator it = sons.begin(); it != sons.end(); ++it) {
		(*it)->display(prefix, indent);
	}
};