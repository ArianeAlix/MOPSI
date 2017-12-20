#include "Tree.h"


Tree::Tree() {
	int label;
	int level;
	area = 1;
	int highest;
	vector<Tree*> sons = {};
	bool seen = false;
	Tree* parent;
};



Tree::Tree(int lbl, int lvl) {
	label = lbl;
	level = lvl;
	area = 1;
	highest = lvl;
	vector<Tree*> sons = {};
	bool seen = false;
	Tree* parent;
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
		par1 = par1->find1();
	}
	return par1;
};


Tree* Tree::find2() {
	if (par2 != this) {
		par2 = par2->find2();
	}
	return par2;
};


Tree* Tree::link1(Tree* y) {
	if (rank1 > y->rank1) {
		y->par1 = this;
		return this;
	}
	if (rank1 == y->rank1) {
		y->rank1 += 1;
	}
	par1 = y;
	return y;
};

Tree* Tree::link2(Tree* y) {
	if (rank2 > y->rank2) {
		y->par2 = this;
		return this;
	}
	if (rank2 == y->rank2) {
		y->rank2 += 1;
	}
	par2 = y;
	return y;
};


Tree* Tree::merge(Tree* y) {
	Tree* temp = link2(y);
	Tree* temp2;
	if (temp->label == y->label) {
		for (vector<Tree*>::iterator it = sons.begin(); it != sons.end(); ++it) {
			y->addAsLastSon(*it);
		}
		temp2 = this;
	}
	else{
		for (vector<Tree*>::iterator it = y->sons.begin(); it != y->sons.end(); ++it) {
			addAsLastSon(*it);
		}
		temp2 = y;
	}
	temp->area += temp2->area;
	temp->highest =max(temp->highest, temp2->highest);
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

int Tree::getMark() {
	return mark;
};

Tree* Tree::getParent() {
	return parent;
};



void Tree::setLevel(int lvl) {
	level = lvl;
};


void Tree::setArea(int a) {
	area=a;
};


void Tree::setHighest(int h) {
	highest = h;
};

void Tree::setMark(int m) {
	mark=m;
};

void Tree::setParent(Tree* padre) {
	parent=padre;
};


int Tree::nbSons() {
	return sons.size();
};

int Tree::nbLeaves() {
	if (nbSons() > 0) {
		int sum=0;
		for (vector<Tree*>::iterator it = sons.begin(); it != sons.end(); ++it) {
			sum+= (*it)->nbLeaves();
		}
		return sum;
	}
	else
		return 1;
}


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

void Tree::isSeen() {
	seen = true;
}

void Tree::addAsLastSon(Tree* newSon) {
	sons.push_back(newSon);
	newSon->setParent(this);
};



void Tree::display(string prefix, string indent){
	cout << prefix << "[" << label  << "] "  << level << endl;
	prefix += indent;
	for (vector<Tree*>::iterator it = sons.begin(); it != sons.end(); ++it) {
		(*it)->display(prefix, indent);
	}
};