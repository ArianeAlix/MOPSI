#include "Tree.h"
#include<Imagine/Graphics.h>
using namespace Imagine;



int main(){
	Tree* root = new Tree(0,12);
	root->addAsLastSon(new Tree(1,8));
	root->getSon(0)->addAsLastSon(new Tree(2,4));
	root->getSon(0)->addAsLastSon(new Tree(3,9));
	root->addAsLastSon(new Tree(4,23));
	root->getSon(1)->addAsLastSon(new Tree(5,17));
	root->getSon(1)	->getSon(0)->addAsLastSon(new Tree(6,15));

	//Test: afficher 15
	//cout << root->getSon(1)->getSon(0)->getSon(0)->getData() << endl;

	//Affichage de l'arbre avec parcours en profondeur
	root->display("* ");
	cout << endl;

	//Test de l'erreur possible de getSon()
	cout << "**Test d'erreur pour getSon() \n" << endl;
	cout << root->getSon(3)<<endl;
	cout << endl;



	// Création d'un set et test
	Tree* x= new Tree(0, 12);
	Tree* y = new Tree(1, 14);
	Tree* z = new Tree(2, 9);
	Tree* w = new Tree(3, 7);
	x->makeSet();
	y->makeSet();
	z->makeSet();
	w->makeSet();

	x->link(y);
	z->link(w); 

	x->find()->display("* "); // Affiche le parent de x, qui est y depuis l'opération link
	/*y->find()->display("* "); // Doit afficher la meme chose (est son propre parent)*/
	cout << endl;

	z->find()->display("* "); // Affiche w 
	cout << endl;

	y=y->merge(w);
	y->display("* ");
	cout << endl;


	click();
	return 0;
}