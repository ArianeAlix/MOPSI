
/************************************************************************
 * Construction de l'arbre de composantes et application de filtres
 * Authors: Ariane Alix, Jean Cauvin, Louis Dumont
 * Last Modified: 22/01/2018
 */
#include <iostream>
#include <vector>

#include <Imagine/Graphics.h>
#include <Imagine/Images.h>

#include "Tree.h"
#include "Ctree.h"

using namespace std;
using namespace Imagine;


// Renvoie une image ou le nombre de couleurs a ete reduit a n
void reduitSpectre(byte* img, int w, int h, int n){
    int range= 256/n;
    assert (range != 0);
    for (int i=0; i<w*h; i++){
        img[i] = byte((int(img[i])/range)*range);
    }
    cout<<"Reduction du spectre terminee"<<endl;
}

// Renvoit le niveau moyen d'une image
int niveauMoyen(byte* img, int w, int h){
    int somme = 0;
    for (int i=0; i<w*h; i++){
        somme += int(img[i]);
    }
    return (somme/(w*h));
}

// Réhausse le niveau d'une image de "hausse"
void rehausse(byte* img, int hausse, int w, int h){
    for (int i=0; i<w*h; i++){
        img[i] = byte(int(img[i])+hausse);
    }
}

// Renvoie les tableau qui en i contient les indices des pixels de niveau 255-i
void sortLevel(vector<int> sorted[256], byte* image, int w, int h) {
    // pixels sorting in linear time
    for (int i = 0; i < w*h; i++) {
        sorted[255 - int(image[i])].push_back(i);// We stock the labels of the pixels in a tab of vectors, with decreasing order oflevels
    }
    return;
}

// Renvoie les indices des voisins déjà processés (en pq "émergés" de p)
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

//Construit l'arbre des composantes à partir de l'image et de tableaux créés à l'avance
//En commentaire: les lignes correspondantes du pseudo-code
Ctree* buildCTree(byte* img, int w, int h, Ctree* nodes, Tree* Qnodes, Tree* Qtree, int* lowestNode){

    // Initialization
    vector<int> sorted[256];   //1
    sortLevel(sorted,img,w,h); //1
    for (int i=0; i<w*h; i++){        //2
        nodes[i] = Ctree(int(img[i]), 1, i);  //2
        Qnodes[i]= Tree(i);            //2
        Qnodes[i].makeSet();
        Qtree[i] = Tree(i);            //2
        Qtree[i].makeSet();
        lowestNode[i]=i;              //2
    }

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
    Ctree* root = &nodes[lowestNode[Qtree[(Qnodes[0].find())->getLabel()].getLabel()]];
    //Correction sauvage pour faire marcher les filtres:
    for (int i=0; i<w*h; i++){
        if (nodes[i].getArea()==w*h){
            root = &nodes[i];
            cout<<"True root found"<<endl;
        }
    }
    return root;
}


//Décode un arbre de composantes et recrée l'image correspondante
byte* readCtree(Ctree* ComponentTree, Ctree* nodes, Tree* Qnodes, int w, int h){
    byte* res = new byte[w*h];
    for (int i=0; i<w*h; i++){
        res[i] = byte( nodes[(Qnodes[i].find())->getLabel()].getLevel());
    }
    return res;
}


void removeNode(Ctree* fatherNode, Ctree* nodeToRm, Tree* Qnodes, int w, int h){
    //All pixels that pointed to the removed nodes' canonical element now point to it's father
    for (int i=0; i<w*h; i++){
        if (Qnodes[i].getParent()->getLabel()==nodeToRm->getLabel()) {
            Qnodes[i].setParent(&Qnodes[fatherNode->getLabel()]);
        }
    }
    //We remove the node in itself from the Ctree
    int label=nodeToRm->getLabel();
    Ctree* cible;
    for (vector<Ctree*>::iterator i=fatherNode->m_sons.begin(); i!=fatherNode->m_sons.end(); i++){
        if ((*i)->getLabel() == label){
            cible = &(**i);
            break;
        }
    }
    fatherNode->adoptSons(cible);
}

//Fonction pour propager un niveau le long d'une branche (et ainsi faire disparaitre l'homogénéiser
void propageBranche(Ctree* origine, int lvl){
    origine->setLevel(lvl);
    for(vector<Ctree*>::iterator i=origine->m_sons.begin(); i!=origine->m_sons.end(); i++){
        propageBranche((*i), lvl);
    }
}

//Premier filtre: on "supprime" tous les noeuds dont l'aire est plus petite qu'un seuil
void removeSmallLeaves_area(Ctree* ComponentTree, int limitSize){
    for(vector<Ctree*>::iterator son= ComponentTree->m_sons.begin(); son!=ComponentTree->m_sons.end(); son++){
        if ((*son)->getArea() < limitSize){
            (*son)->setLevel(ComponentTree->getLevel());
        }
        removeSmallLeaves_area((*son), limitSize);
    }
}

//Premier filtre: on "supprime" tous les noeuds dont l'aire est plus petite qu'un seuil (supprime réellement les noeuds
void removeSmallLeaves_areaM(Ctree* ComponentTree, int limitSize, Tree* Qnodes, int w, int h, bool stop=false){
    int sons2see=ComponentTree->nbSons();
    while (sons2see>0) {
        Ctree* son = ComponentTree->getSon(sons2see-1);
        removeSmallLeaves_areaM(son, limitSize, Qnodes, w, h);
        if (son->getArea() < limitSize) {
            removeNode(ComponentTree, son, Qnodes, w, h);
        }
        sons2see -= 1;
    }
}

//On "supprime" tous les noeuds dont le volume est plus petit qu'un seuil
void removeSmallLeaves_volume(Ctree* ComponentTree, int limitSize){
    for(vector<Ctree*>::iterator son= ComponentTree->m_sons.begin(); son!=ComponentTree->m_sons.end(); son++){
        if ((*son)->getVolume() < limitSize){
            (*son)->setLevel(ComponentTree->getLevel());
        }
        removeSmallLeaves_volume((*son), limitSize);
    }
}

//Same idea as remove small leaves but deals with the darkest zones (dégradé etc)
//Used alone, the result isn't really pretty
void removeSmallZones(Ctree* ComponentTree, int limitSize){
    int area = ComponentTree->getArea();
    for(vector<Ctree *>::iterator son= ComponentTree->m_sons.begin(); son!=ComponentTree->m_sons.end(); son++){
        area -= (*son)->getArea();
        removeSmallZones((*son), limitSize);
    }
    if (area<limitSize && ComponentTree->nbSons()>0){
        ComponentTree->setLevel(ComponentTree->getSons()[0]->getLevel());
    }
}

//All the nodes above the level will be set as black
void blackLevel(Ctree* ComponentTree, int newLevel){
    int level = ComponentTree->getLevel();
    for(vector<Ctree *>::iterator son= ComponentTree->m_sons.begin(); son!=ComponentTree->m_sons.end(); son++){
        blackLevel((*son),newLevel);
    }
    if(level<newLevel){
        ComponentTree->setLevel(0);
    }
}

//RESULTAT PAS SUPER BEAU
//Réduit les composantes les plus déséquilibrées
//On veut éliminer les dégradés etc:
//Si un noeud n'a qu'un seul fils, et que celui-ci n'a qu'un seul fils (branche trop longue) alors on supprime le dernier fils
void collapseFilterM(Ctree* ComponentTree, Tree* Qnodes, int w, int h){
    int n = ComponentTree->nbSons(); //Les cas diffèrent selon le nombre de fils
    //Si le noeud n'a pas de fils on ne fait rien
    if (n==1){  //Si le noeud n'a qu'un fils, on vérifie que la branche n'est pas trop longue
        Ctree* son = ComponentTree->getSon(0);
        if (son->nbSons()==1){ //Si l'on détecte une branche trop longue
            removeNode(son, son->getSon(0), Qnodes, w, h);
            collapseFilterM(ComponentTree, Qnodes, w, h);
        }
        collapseFilterM(son, Qnodes, w, h);
    }
    if (n>1){ //Si le noeud a plusieurs fils, on ne modifie rien et on traite les fils
        for(vector<Ctree *>::iterator son= ComponentTree->m_sons.begin(); son!=ComponentTree->m_sons.end(); son++){
            collapseFilterM((*son), Qnodes, w,h);
        }
    }
}
void collapseFilter(Ctree* ComponentTree){
    int n = ComponentTree->nbSons(); //Les cas diffèrentes selon le nombre de fils
    //Si le noeud n'a pas de fils on ne fait rien
    if (n==1){  //Si le noeud n'a qu'un fils, on vérifie que la branche n'est pas trop longue
        Ctree* son = ComponentTree->getSon(0);
        if (son->nbSons()==1){ //Si l'on détecte une branche trop longue
            cout<<"a"<<endl;
            son->getSon(0)->setLevel(son->getLevel());
        }
        collapseFilter(son);
    }
    if (n>1){ //Si le noeud a plusieurs fils, on ne modifie rien et on traite les fils
        for(vector<Ctree *>::iterator son= ComponentTree->m_sons.begin(); son!=ComponentTree->m_sons.end(); son++){
            collapseFilter((*son));
        }
    }
}

//Filtre faisant désaparaître les partie déséquilibrées (en terme de volume) d'un arbre
//Si un noeud a des fils peu volumineux (en comparaison des autres) alors il les supprime
void equiFilter(Ctree* ComponentTree, float seuil) {
    int totVoS = 0; //Volume total des fils
    for (vector<Ctree *>::iterator son = ComponentTree->m_sons.begin(); son != ComponentTree->m_sons.end(); son++) {
        equiFilter((*son), seuil);
        totVoS += (*son)->getVolume();
    }
    for (vector<Ctree *>::iterator son = ComponentTree->m_sons.begin(); son != ComponentTree->m_sons.end(); son++) {
        if ( (float((*son)->getVolume()) / float(totVoS))   <   seuil){
            propageBranche((*son), ComponentTree->getLevel());
        }
    }
}

int main(int argc, char*argv[]) {

    //Checking of the command
    if(argc!=4){
        cout << "Syntax error" << endl;
        cout << "write : MOPSI_filtre name_img name_filter param_filter" << endl;
        return 0;
    }

    // Settings of the filter
    string name_img = argv[1];
    string name_filter = argv[2];
    int limitSize = stoi(argv[3]);



    // Get the client image
    byte* testImg;
    int width,height;


    loadGreyImage(name_img,testImg,width,height);


    cout << "Image size: " << width << "x" << height << endl;
    cout << "Number of pixels: " << width*height << endl;

    //Creating the Component Tree of an image
    Tree* Qtree  = new Tree[width*height];
    Tree* Qnodes = new Tree[width*height];
    Ctree* nodes = new Ctree[width*height];
    int* lowestNode = new int[width*height];
    Ctree* ComponentTree = buildCTree(testImg, width, height, nodes, Qnodes, Qtree, lowestNode);



    // Filter processing
    if(name_filter=="area"){
        removeSmallLeaves_area(ComponentTree, limitSize);
        cout << "area processing" << endl;
    }
    if(name_filter=="volume"){
        removeSmallLeaves_volume(ComponentTree, limitSize);
        cout << "volume processing" << endl;
    }
    if(name_filter=="zone"){
        removeSmallZones(ComponentTree,limitSize);
        cout << "zone processing" << endl;
    }
    if(name_filter=="equi"){
        equiFilter(ComponentTree,limitSize);
        cout << "equifilter processing" << endl;
    }
    

    byte* resImg = readCtree(ComponentTree, nodes, Qnodes, width, height);
    putGreyImage(IntPoint2(0,0), resImg, width,height);


    //Save the new image
    Image<byte> I(resImg,width,height);

    save(I,"image_output.png");




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
    reduitSpectre(testImg,width,height, 50);
    putGreyImage(IntPoint2(0,0), testImg, width,height);

    //Creating the Component Tree of an image
    Tree* Qtree = new Tree[width*height];
    Tree* Qnodes = new Tree[width*height];
    Ctree* nodes = new Ctree[width*height];
    int* lowestNode = new int[width*height];
    Ctree* ComponentTree = buildCTree(testImg, width, height, nodes, Qnodes, Qtree, lowestNode);
    //click();

    //Decoding the Ctree and testing the filters
    setActiveWindow(W2);
    removeSmallLeaves_area(ComponentTree, 10000);
    //blackLevel(ComponentTree, 100);
    byte* resImg = readCtree(ComponentTree, nodes, Qnodes, width, height);
    putGreyImage(IntPoint2(0,0), resImg, width,height);
    //click();


    cout<<"Tests termines"<<endl;
    click();
    delete [] resImg;


    //Deleting all the strctures created
    delete [] Qtree;
    delete [] Qnodes;
    delete [] nodes;
    delete [] lowestNode;

    //click();
    delete(testImg);

    click();
    delete(testImg);
    */



    return 0;
}
