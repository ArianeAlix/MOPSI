#include "Tree.h"
#include<Imagine/Graphics.h>
#include <map>
using namespace Imagine;
using namespace std;


void sortLevel(vector<int> sorted[256], byte* image, int w, int h) {
	// pixels sorting in linear time
	for (int i = 0; i < w*h - 1; i++) {
		sorted[int(image[i])].push_back(i);
	}
	return;
}


vector<Tree*> neighbourhood(vector<Tree*> nodes, Tree* p, int num, int w, int h){
	vector < Tree* > neighbours = {};
	// Borders are specials
	if (num%w != 0 && nodes[num - 1]->wasSeen()) {
		// Checking if not on the left border
		neighbours.push_back(nodes[num - 1]);
	}
	if (num%w != w-1 && nodes[num + 1]->wasSeen()) {
		// Checking if not on the right border
		neighbours.push_back(nodes[num + 1]);
	}
	if (num > w && nodes[num - w]->wasSeen()) {
		// Checking if not on the upper border
		neighbours.push_back(nodes[num - w]);
	}
	if (num < w*(h-1) && nodes[num + w]->wasSeen()) {
		// Checking if not on the lower border
		neighbours.push_back(nodes[num + w]);
	}
	return neighbours;
}


Tree* buildComponentTree(byte* image, int w, int h, vector<Tree*> nodes, map<Tree*, Tree*> M, map<Tree*, Tree*> lowestNode) {
	for (int i = 0; i < w*h; i++) {
		Tree* p = new Tree(i, int(image[i])); // node build with label=i= n° of pixel and level=image[i]=grey level
		p->makeSet1();
		p->makeSet2();
		nodes.push_back(p);
		lowestNode[p] = p;
	}
	cout << nodes.size();
	vector<int> sorted[256] = {};
	sortLevel(sorted, image, w, h);
	for (int i = 0; i < 256; i++) {
		if (sorted[i].size() > 0) {
			// if the vector is not empty
			for (int j = 0; j < sorted[i].size(); j++) {
				Tree* p = nodes[sorted[i][j]]; // sorted[i][j] is the label of the jth pixel of intensity i
				Tree* curTree = p->find2();
				Tree* curNode = lowestNode[curTree]->find1();

				// we search for the already processed neighbours with a higher intensity((second condition always true)
				vector<Tree*> neighbours = neighbourhood(nodes, p, sorted[i][j], w, h);
				for (int k = 0; k < neighbours.size(); k++) {
					Tree* q = neighbours[k];
					Tree* adjTree = q->find2();
					Tree* adjNode = lowestNode[adjTree]->find1();
					if (curNode->getLabel() != adjNode->getLabel()) { // If the nodes are different
						if (curNode->getLevel() == adjNode->getLevel()) {
							curNode = adjNode->merge(curNode);
						}
						else {
							curNode->addAsLastSon(adjNode);
							curNode->setArea(curNode->getArea() + adjNode->getArea());
							curNode->setHighest(max(curNode->getHighest(), adjNode->getHighest()));
						}
						curTree = adjTree->link(curTree);
						lowestNode[curTree]= curNode;
					}
				}
			}
		}
	}
	Tree* root = lowestNode[(nodes[0]->find1())->find2()];
	for (int i = 0; i < w*h; i++) {
		M[nodes[i]] = nodes[i]->find1();
	}
	return new Tree;
}



int main() {
	
	// Création d'un set et test
	Tree* x = new Tree(0, 12);
	Tree* y = new Tree(1, 14);
	Tree* z = new Tree(2, 9);
	Tree* w = new Tree(3, 7);
	x->makeSet1();
	y->makeSet1();
	x->makeSet2();
	y->makeSet2();
	y->addAsLastSon(new Tree(4, 10));
	z->makeSet1();
	w->makeSet1();
	z->makeSet2();
	w->makeSet2();

	x->link(y);
	z->link(w);

	x->find2()->display("* "); // Affiche le parent de x, qui est y depuis l'opération link
	/*y->find()->display("* "); // Doit afficher la meme chose (est son propre parent)*/
	cout << endl;

	z->find2()->display("* "); // Affiche w 
	cout << endl;

	Tree* v = y->merge(w);
	v->display("* ");
	cout << endl;


	// IMPORT d'IMAGE
	// récupérer le fichier
	const char *image_file =
		(argc > 1) ? argv[1] : srcPath("lenna.png");

	// Load image
	byte* image;
	int width, height;
	cout << "Loading image: " << image_file << endl;
	loadGreyImage(image_file, image, width, height);

	// Print statistics
	cout << "Image size: " << width << "x" << height << endl;
	cout << "Number of pixels: " << width*height << endl;

	// Display image
	Window window = openWindow(width, height);
	putGreyImage(IntPoint2(0, 0), image, width, height);


	/*TEST DU TRI DES PIXELS (temps linéaire)

	vector<int> sorted[256] = {};
	sortLevel(sorted,image, width, height);
	cout << sorted[100][0] << endl;
	*/


	//TEST du début de l'algo (itération et création de noeuds) -> ok
	vector<Tree*> nodes;
	map<Tree*, Tree*> M;
	map<Tree*, Tree*> lowestNode;
	Tree* test = buildComponentTree(image, width, height, nodes, M, lowestNode);


	click();
	delete image;
	return 0;
}