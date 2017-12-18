#include "Tree.h"


Tree::Tree() {
	int label;
	int level;
	area = 1;
	int highest;
	vector<Tree*> sons = {};
	bool seen = false;
};



Tree::Tree(int lbl, int lvl) {
	label = lbl;
	level = lvl;
	area = 1;
	highest = lvl;
	vector<Tree*> sons = {};
	bool seen = false;
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



void Tree::makeSet1() {
	par1 = this;
	rank1 = 0;
};


void Tree::makeSet2() {
	par2 = this;
	rank2 = 0;
};


Tree* Tree::find1() {
	if (par1 != this) {
		par1 = (*par1).find1();
	}
	return par1;
};


Tree* Tree::find2() {
	if (par2 != this) {
		par2 = (*par2).find1();
	}
	return par2;
};


Tree* Tree::link(Tree* y) {
	if (rank2 > y->rank2) {
		y-> par2 = this;
		return this;
	}
	if (rank2 == y->rank2) {
		y->rank2 += 1;
	}
	par2 = y;
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



int Tree::getLabel() {
	return label;
};

int Tree::getLevel() {
	return level;
};

int Tree::getArea() {
	return area;
};

int Tree::getHighest() {
	return highest;
};


void Tree::setLevel(int lvl) {
	level = lvl;
};


void Tree::setArea(int a) {
	area=a;
};


void Tree::setHighest(int h) {
	highest = h;;
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

bool Tree::wasSeen() {
	return seen;
};

<<<<<<< HEAD
void Tree::isSeen() {
	seen = true;
}
=======
>>>>>>> 25338465497b35ea6400f687cac3987b8dad85f7

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