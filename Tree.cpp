#include "Tree.h"


Tree::Tree() {
	int label;
	int level;
	area = 1;
	int highest;
	vector<Tree*> sons = {};
};



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
		this->addAsLastSon(y);
		return this;
	}
	if (rank == y->rank) {
		y->rank += 1;
	}
	par = y;
	y->addAsLastSon(this);
	return y;
};


Tree* Tree::merge(Tree* y) {
	Tree* temp = this->link(y);
	Tree* temp2;
	if (temp->label == y->label) {
		for (vector<Tree*>::iterator it = sons.begin(); it != sons.end(); ++it) {
			y->addAsLastSon(*it);
		}
		temp2 = this;
	}
	else{
		for (vector<Tree*>::iterator it = y->sons.begin(); it != y->sons.end(); ++it) {
			this->addAsLastSon(*it);
		}
		temp2 = y;
	}
	temp->area = temp->area + temp2->area;
	if (temp->highest < temp2->highest) {
		temp->highest = temp2->highest;
	}
	return temp;
}


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