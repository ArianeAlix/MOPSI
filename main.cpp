
/************************************************************************
 * Construction de l'arbre de composantes et application de filtres
 * Authors: Ariane Alix, Jean Cauvin, Louis Dumont
 * Last Modified: 24/12/2017
 */

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>

#include <Imagine/Graphics.h>
#include <Imagine/Images.h>

#include "Tree.h"
#include "Ctree.h"

using namespace std;
using namespace Imagine;


// Renvoit une image ou le nombre de couleurs a ete reduit a n
void reduitSpectre(byte* img, int w, int h, int n){
    int range= 256/n;
    assert (range != 0);
    for (int i=0; i<w*h; i++){
        img[i] = byte((int(img[i])/range)*range);
    }
}

// Renvoit les tableau qui en i contient les indices des pixels de niveau 255-i
void sortLevel(vector<int> sorted[256], byte* image, int w, int h) {
    // pixels sorting in linear time
    for (int i = 0; i < w*h; i++) {
        sorted[255 - int(image[i])].push_back(i);// We stock the labels of the pixels in a tab of vectors, with decreasing order oflevels
    }
    return;
}

// Renvoit les indices des voisins processés (en pq "émergés" de p)
void neighbourhood(vector<int>& neighbours, Tree* nodes, Tree* p, int num, int w, int h) {
    // Borders are specials
    if (num%w != 0 && nodes[num - 1].wasSeen()) {
        // Checking if not on the left border
        neighbours.push_back(nodes[num - 1].getLabel());
    }
    if (num%w != w - 1 && nodes[num + 1].wasSeen()) {
        // Checking if not on the right border
        neighbours.push_back(nodes[num + 1].getLabel());
    }
    if (num > w && nodes[num - w].wasSeen()) {
        // Checking if not on the upper border
        neighbours.push_back(nodes[num - w].getLabel());
    }
    if (num < w*(h - 1) && nodes[num + w].wasSeen()) {
        // Checking if not on the lower border
        neighbours.push_back(nodes[num + w].getLabel());
    }
}


//En commentaire: les lignes correspondantes du pseudo-code
Ctree* buildCTree(byte* img, int w, int h, Ctree* nodes, Tree* Qnodes, Tree* Qtree, int* lowestNode){

    // Initialization
    vector<int> sorted[256];   //1
    sortLevel(sorted,img,w,h); //1
    for (int i=0; i<w*h; i++){        //2
        nodes[i] = Ctree(int(img[i]), 1);  //2
        Qnodes[i]= Tree(i);            //2
        Qnodes[i].makeSet();
        //cout<<(Qnodes[i]).find()->getLabel()<<" ";
        //cout<<(Qnodes[i]).find()->getLabel()<<" ";
        Qtree[i] = Tree(i);            //2
        Qtree[i].makeSet();
        lowestNode[i]=i;              //2
    }
    //cout<<endl;
    //for(int i=0; i<w*h; i++){
    //    cout<<(Qnodes[i]).find()->getLabel()<<" ";
    //}
    //cout<<endl;
    int testInt=0;
    //Treatment
    for (int l=0; l<256; l++){
        for (vector<int>::iterator p=sorted[l].begin(); p!=sorted[l].end(); p++){
            //testInt += 1;
            Qtree[*p].isSeen();
            Tree* curTree = Qtree[*p].find();                                       //4 pk ne pas prendre simplement Qtree[*p]?
            Tree* curNode = Qnodes[lowestNode[(*curTree).getLabel()]].find();       //5 pk ne pas prendre simplement Qnodes[*p]?
            vector<int> neighbours;
            neighbourhood(neighbours, Qtree, curTree, *p, w, h); //calculating the processed neighbours
            //cout<<neighbours.size();
            for (vector<int>::iterator q=neighbours.begin(); q!=neighbours.end(); q++){ //6
                testInt += 1;
                //cout<<"PASSAGE"<<endl;
                if (img[*q]>=img[*p]){                                                  //6  utile? (a priori non)
                    testInt += 1;
                    Tree* adjTree = Qtree[*q].find();                                   //7  ajdTree est alors le Tree de l'arbre partiel voisin
                    Tree* adjNode = Qnodes[lowestNode[(*adjTree).getLabel()]].find();   //8  ajdNode pareil mais vu en tant que node
                    if (curNode->getLabel() != adjNode->getLabel()){
                        if (nodes[curNode->getLabel()].getLevel() == nodes[adjNode->getLabel()].getLevel() ){
                            //merging the trees
                            //curNode = MergeNodes(adjNode, curNode);
                            Tree* tmpNode = adjNode->link(curNode);
                            Tree* tmpNode2;
                            if (tmpNode->getLabel() == curNode->getLabel()){
                                nodes[curNode->getLabel()].adoptSons(&nodes[adjNode->getLabel()]);
                                tmpNode2 = adjNode;
                            }
                            else{
                                nodes[adjNode->getLabel()].adoptSons(&nodes[curNode->getLabel()]);
                                tmpNode2 = curNode;
                            }
                            nodes[tmpNode->getLabel()].integrateData(&nodes[tmpNode2->getLabel()]);
                            curNode = tmpNode;
                        }
                        else{
                            //integrating the previously emerged island into the new one
                            //cout<<"Son added"<<endl;
                            nodes[curNode->getLabel()].addAsLastSon(&nodes[adjNode->getLabel()]);
                            nodes[curNode->getLabel()].integrateData(&nodes[adjNode->getLabel()]);
                        }
                        curTree = Qtree[adjTree->getLabel()].link(&Qtree[curTree->getLabel()]);
                        lowestNode[curTree->getLabel()] = curNode->getLabel();
                    }
                }
            }
        }
    }
    //cout<<testInt<<endl;
    //for(int i=0; i<w*h; i++){
    //    cout<<Qnodes[i].find()->getLabel()<<" ";
    //}
    Ctree* root = &nodes[lowestNode[Qtree[(Qnodes[510].find())->getLabel()].getLabel()]];
    return root;
}

byte* readCtree(Ctree* ComponentTree, Ctree* nodes, Tree* Qnodes, int w, int h){
    byte* res = new byte[w*h];
    for (int i=0; i<w*h; i++){
        res[i] = byte( nodes[(Qnodes[i].find())->getLabel()].getLevel());
    }
    return res;
}

void removeSmallLeaves(Ctree* ComponentTree, int limitSize){
    for(vector<Ctree*>::iterator son= ComponentTree->m_sons.begin(); son!=ComponentTree->m_sons.end(); son++){
        if ((*son)->getArea() < limitSize){
            (*son)->setLevel(ComponentTree->getLevel());
        }
        removeSmallLeaves((*son), limitSize);
    }
}

int main(int argc, char*argv[]) {

    //Checking of the command
    if(argc!=4){
        cout << "Syntax error" << endl;
        cout << "write : MOPSI_filtre name_filter param_filter format" << endl;
        return 0;
    }

    // Settings of the filter
    string name_filter = argv[1];
    int limitSize = boost::lexical_cast<int>(argv[2]);
    string format = argv[3];


    // Get the client image
    byte* testImg;
    int width,height;

    if(format=="png"){
        const char* testPath = srcPath("lenna.png");
        loadGreyImage(testPath,testImg,width,height);
    }
    else if(format=="jpg"){
        const char* testPath = srcPath("lenna.jpg");
        loadGreyImage(testPath,testImg,width,height);
    }
    else{
        cout << "Wrong format" << endl;
        return 0;
    }




    cout << "Image size: " << width << "x" << height << endl;
    cout << "Number of pixels: " << width*height << endl;

    //Creating the Component Tree of an image
    Tree* Qtree  = new Tree[width*height];
    Tree* Qnodes = new Tree[width*height];
    Ctree* nodes = new Ctree[width*height];
    int* lowestNode = new int[width*height];
    Ctree* ComponentTree = buildCTree(testImg, width, height, nodes, Qnodes, Qtree, lowestNode);



    //Decoding the Ctree
    ComponentTree->display();

    // Filter processing
    if(name_filter=="cartoon"){
        removeSmallLeaves(ComponentTree, limitSize);
    }
    else{
        cout << "Filter unknown" << endl;
        return 0;
    }

    byte* resImg = readCtree(ComponentTree, nodes, Qnodes, width, height);
    putGreyImage(IntPoint2(0,0), resImg, width,height);
    ComponentTree->display();


    //Save the new image
    Image<byte> I(resImg,width,height);
    if(format=="png"){
        save(I,"image_output.png");
    }
    else save(I,"image_ouput.jpg");


    //Deleting all the strctures created
    delete [] resImg;
    delete [] Qtree;
    delete [] Qnodes;
    delete [] lowestNode;




    /*
    // Test de l'affichage d'image

    //Loading Image test
    byte* testImg;
    const char* testPath = srcPath("lenna.png");
    int width,height;
    loadGreyImage(testPath,testImg,width,height);
    cout << "Image size: " << width << "x" << height << endl;
    cout << "Number of pixels: " << width*height << endl;




    //Modifying and displaying it
    Window W2=openWindow(width,height);
    Window W1=openWindow(width,height);
    setActiveWindow(W1);
    //reduitSpectre(testImg,width,height, 5);
    putGreyImage(IntPoint2(0,0), testImg, width,height);

    //Random tests
    Tree test(10);
    cout<<test.find()->getLabel()<<endl;

    //Creating the Component Tree of an image
    Tree* Qtree  = new Tree[width*height];
    Tree* Qnodes = new Tree[width*height];
    Ctree* nodes = new Ctree[width*height];
    int* lowestNode = new int[width*height];
    Ctree* ComponentTree = buildCTree(testImg, width, height, nodes, Qnodes, Qtree, lowestNode);
    click();
    //Decoding the Ctree (or trying to)
    setActiveWindow(W2);
    ComponentTree->display();
    removeSmallLeaves(ComponentTree, 100);
    byte* resImg = readCtree(ComponentTree, nodes, Qnodes, width, height);
    putGreyImage(IntPoint2(0,0), resImg, width,height);
    //ComponentTree->display();
    delete [] resImg;

    //Other Random Tests
    //cout << "Testing the Ctree construction" <<endl;
    //for (int i=0; i<width; i=i+5){
    //    cout << Qtree[i].getRank();
    //}
    //cout<<endl;

    //Deleting all the strctures created
    delete [] Qtree;
    delete [] Qnodes;
    delete [] lowestNode;


    //Testing the Tree structure
    //Tree* a = new Tree;
    //(*a).makeSet();
    //Tree* b = new Tree;
    //(*b).makeSet();
    //Tree* c = new Tree;
    //(*c).makeSet();
    //string* str = new string("*");
    //*b->link(a);
    //*c->link(b);
    //Tree* d = (*c).find();
    //(*c).pdisplay(str);
    //delete a;
    //delete b;
    //delete c;
    //delete str;

    //Testing the Ctree structure
    Ctree* C1 = new Ctree(1,1);
    Ctree* C2 = new Ctree(2,2);
    Ctree* C3 = new Ctree(3,3);
    Ctree* C4 = new Ctree(4,4);
    Ctree* C6 = new Ctree(6,6);
    (*C1).addAsLastSon(C2);
    (*C1).addAsLastSon(C4);
    (*C2).addAsLastSon(C3);
    (*C3).setLevel(5);
    (*C3).setArea(5);
    (*C3).addAsLastSon(C6);
    (*C1).adoptSons(C2);
    removeSmallLeaves(C1, 3);
    //(*C1).display();
    //cout << (*C3).getLevel() << "  " << (*C3).getArea() << endl;
    //cout << (*(*C2).getSon(0)).getLevel() << endl;
    delete C1;


    click();
    delete(testImg);
    */



    return 0;
}
