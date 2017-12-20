#include "Tree.h"
#include<Imagine/Graphics.h>
#include <map>
#include <algorithm>
#include <ctime>

using namespace Imagine;
using namespace std;


void sortLevel(vector<int> sorted[256], byte* image, int w, int h) {
	// pixels sorting in linear time
	for (int i = 0; i < w*h; i++) {
		sorted[255 - int(image[i])].push_back(i);// We stock the labels of the pixels in a tab of vectors, with decreasing order oflevels
	}
	return;
}


vector<Tree*> neighbourhood(vector<Tree*> nodes, Tree* p, int num, int w, int h) {
	vector < Tree* > neighbours = {};
	// Borders are specials
	if (num%w != 0 && nodes[num - 1]->wasSeen()) {
		// Checking if not on the left border
		neighbours.push_back(nodes[num - 1]);
	}
	if (num%w != w - 1 && nodes[num + 1]->wasSeen()) {
		// Checking if not on the right border
		neighbours.push_back(nodes[num + 1]);
	}
	if (num > w && nodes[num - w]->wasSeen()) {
		// Checking if not on the upper border
		neighbours.push_back(nodes[num - w]);
	}
	if (num < w*(h - 1) && nodes[num + w]->wasSeen()) {
		// Checking if not on the lower border
		neighbours.push_back(nodes[num + w]);
	}
	return neighbours;
}


Tree* buildComponentTree(byte* image, int w, int h, vector<Tree*>& nodes, map<Tree*, Tree*>& M, map<Tree*, Tree*>& lowestNode) {
	for (int i = 0; i < w*h; i++) {
		Tree* p = new Tree(i, int(image[i])); // node build with label=i= n� of pixel and level=image[i]=grey level
		p->setParent(p);
		p->makeSet1();
		p->makeSet2();
		nodes.push_back(p);
		lowestNode[p] = p;
	}
	vector<int> sorted[256] = {}; // Tab of vectors, tab[i] is the vector of the labels of pixels with level 255-i
	sortLevel(sorted, image, w, h);

	for (int i = 0; i < 256; i++) {
		if (sorted[i].size() > 0) {
			// if the vector is not empty
			for (int j = 0; j < sorted[i].size(); j++) {
				Tree* p = nodes[sorted[i][j]]; // sorted[i][j] is the label of the jth pixel of intensity 255-i
				p->isSeen(); // We indicate that the node has been processed
				Tree* curTree = p->find1();
				Tree* curNode = lowestNode[curTree]->find2();

				// we search for the already processed neighbours with a higher intensity (second condition always true)
				vector<Tree*> neighbours = neighbourhood(nodes, p, sorted[i][j], w, h);
				for (int k = 0; k < neighbours.size(); k++) {
					Tree* q = neighbours[k];
					Tree* adjTree = q->find1();
					Tree* adjNode = lowestNode[adjTree]->find2();
					if (curNode->getLabel() != adjNode->getLabel()) { // If the nodes are different
						if (curNode->getLevel() == adjNode->getLevel()) {
							curNode = adjNode->merge(curNode);
						}
						else {
							curNode->addAsLastSon(adjNode);
							curNode->setArea(curNode->getArea() + adjNode->getArea());
							curNode->setHighest(max(curNode->getHighest(), adjNode->getHighest()));
						}
						curTree = adjTree->link1(curTree);
						lowestNode[curTree] = curNode;
					}
				}
			}
		}
	}

	Tree* root = lowestNode[(nodes[0]->find2())->find1()];
	for (int i = 0; i < w*h; i++) {
		M[nodes[i]] = nodes[i]->find2();
	}
	return root;
}




// Keeping N Lobes algorithm and necessary functions
int removeLobe(int lobe, vector<Tree*>& nodes) {
	if (nodes[lobe]->getMark() == 1) {
		nodes[lobe] = nodes[removeLobe(nodes[lobe]->getParent()->getLabel(), nodes)];
	}
	return lobe;
}

bool compByArea(Tree*t1, Tree* t2)
{ return t1->getArea() < t2->getArea(); }


void keepNLobes(Tree* root, vector<Tree*>& nodes, map<Tree*, Tree*>& M, int N) {
	// We copy in a new vector the values of the tree (we do not want to copy the pointers, which
	// would not enable free modifications)

	vector<Tree *> T;

	for (int j = 0; j < nodes.size(); j++) {
		if (M[nodes[j]] == nodes[j]) { // We only copy the canonical element of the sets
			Tree* t = new Tree;
			t = nodes[j];
			T.push_back(t);
		}
	}

	// We sort the nodes by increasing area
	sort(T.begin(), T.end(),compByArea);


	for (int j = 0; j < nodes.size(); j++) {
		nodes[j] -> setMark(0);
	}

	// We keep in Q the labels of the nodes of the lobes to remove
	vector<int> Q = {};
	int L = root->nbLeaves();

	while (L > N) {

		int c = (T.front())->getLabel();// We take the node with smallest area left
		int p = nodes[c]->getParent()->getLabel();

		if (nodes[p]->nbSons() > 1) {
			L += -1;
			nodes[c]->setMark(1);
			Q.push_back(c);
		}
		T.erase(T.begin()); // We erase the node just processed
	}

	while (Q.size() > 0){
		removeLobe(Q.front(), nodes);
		Q.erase(Q.begin());
	}

	for (int j = 0; j < nodes.size(); j++) {
		nodes[j]->setLevel(M[nodes[j]]->getLevel());
	}

}


void maxima(Tree* root, vector<Tree*>& nodes, map<Tree*, Tree*>& M, int N) {
	vector<Tree *> T;

	for (int j = 0; j < nodes.size(); j++) {
		if (M[nodes[j]] == nodes[j]) { // We only copy the canonical element of the sets
			Tree* t = new Tree;
			t = nodes[j];
			T.push_back(t);
		}
	}

	// We sort the nodes by increasing area
	sort(T.begin(), T.end(), compByArea);

	for (vector<Tree*>::iterator it = T.end()-1; it != T.end() - N-1; it--) {
		// Lobes of maximal areas
		(*it)->setLevel(255);
	}

	for (int j = 0; j < nodes.size(); j++) {
		nodes[j]->setLevel(M[nodes[j]]->getLevel());
	}
}




int main() {
	/*
	// Cr�ation d'un set et test
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

	x->link1(y);
	z->link1(w);

	x->find2()->display("* "); // Affiche le parent de x, qui est y depuis l'op�ration link
	y->find()->display("* "); // Doit afficher la meme chose (est son propre parent)
	cout << endl;

	z->find2()->display("* "); // Affiche w
	cout << endl;

	Tree* v = y->merge(w);
	v->display("* ");
	cout << endl;
	*/

	// IMPORT d'IMAGE
	// r�cup�rer le fichier
	const char *image_file =
		(argc > 1) ? argv[1] : srcPath("lenna1.png");

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


	/*TEST DU TRI DES PIXELS (temps lin�aire)

	vector<int> sorted[256] = {};
	sortLevel(sorted,image, width, height);
	cout << sorted[100][0] << endl;
	*/


	//TEST du d�but de l'algo (it�ration et cr�ation de noeuds) -> ok
	clock_t time1 = clock();
	vector<Tree*> nodes;
	map<Tree*, Tree*> M;
	map<Tree*, Tree*> lowestNode;
	Tree* root = buildComponentTree(image, width, height, nodes, M, lowestNode);
	clock_t time2 = clock();
	cout << "Tree generated in: "
		<< ((float)(time2 - time1) / CLOCKS_PER_SEC) << " s" << endl;

	click();

	// Apply a filter to the tree
	keepNLobes(root, nodes, M, 10);


	// Reconstruct the picture with "nodes"
	byte* image2;
	loadGreyImage(image_file, image2, width, height);

	for (int i = 0; i < width*height; i++) {
		image2[i] = nodes[i]->getLevel();
	}
	// Display image2
	Window window2 = openWindow(width, height);
	setActiveWindow(window2);
	putGreyImage(IntPoint2(0, 0), image2, width, height);


	click();
	// 2nd filter
	vector<Tree*> nodes2;
	map<Tree*, Tree*> M2;
	map<Tree*, Tree*> lowestNode2;
	Tree* root2 = buildComponentTree(image2, width, height, nodes2, M2, lowestNode2);

	maxima(root2, nodes2, M2, 10);


	// Reconstruct the picture with "nodes"
	byte* image3;
	loadGreyImage(image_file, image3, width, height);

	for (int i = 0; i < width*height; i++) {
		image3[i] = nodes2[i]->getLevel();
	}
	// Display image3
	Window window3 = openWindow(width, height);
	setActiveWindow(window3);
	putGreyImage(IntPoint2(0, 0), image3, width, height);




	click();

	delete image;
	delete image2;
	return 0;
}
