#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "avltree.h"

void display_avl(AVLTree node) {
    if (!node)
        return;
    printf("%d-[%p]",*(int*)node->data, node);

    if(node->left)
        printf("(L:%d)", *(int*)node->left->data);
    if(node->right)
        printf("(R:%d)", *(int*)node->right->data);
    printf("\n");

    display_avl(node->left);
    display_avl(node->right);
}

bool    compare(const void * a, const void * b) {
    return *(int*)a < *(int*)b;
}

void    monPrintF(void * a, void * b) {
    printf("Valeur du noeud : %d\n", *(int*)a);
}

void    randomTree() {
    AVLTree racine;
    int     sizeTab;
    int     index;
    size_t  sizeInt;
    int     value;

    sizeInt = sizeof(int);
    sizeTab = rand() % 20;
    racine = NULL;

    index = 0;
    while (index++ < sizeTab) {
        value = rand() % 1000;
        racine = AVLtree_insertData(racine, compare, &value, sizeInt);
    }
    display_avl(racine);
}

/**
 * Tests réalisés pour les arbres AVL
 * N'affiche rien mais est basé sur des anticipations de valeurs
 * pour réaliser des pseudos tests unitaires
 * Et affichage final des valeurs des tris
 */
void    testArbresAVL(void){
    size_t sizeInt = sizeof(int);

    //test AVLtree_create
    int i = 5, j = 10;
    AVLTree racine = AVLtree_create(&i, sizeInt);
    AVLTree node1 = AVLtree_create(&j, sizeInt);
    assert(5 == *(int*)racine->data);
    assert(10 == *(int*)node1->data);
    printf("PASS -> AVLtree_create\n");

    //test AVLtree_getData
    assert(5 == *(int*)AVLtree_getData(racine));
    assert(10 == *(int*)AVLtree_getData(node1));
    printf("PASS -> AVLtree_getData\n");

    //test AVLtree_settData
    //AVLtree_settData n'est pas a utiliser comme fonction d'arbre
    //car celle-ci n'effectue aucune rotation
    int j2 = 9;
    AVLtree_setData(node1, &j2, sizeInt);
    assert(9 == *(int*)AVLtree_getData(node1));
    printf("PASS -> AVLtree_settData\n");

    //test AVLtree_insertNode
    racine = AVLtree_insertNode(racine, compare, node1);
    /*
     * Verification que 'node1' soit bien insere.
     * Celle-ci devrait se retrouver a la droite de la racine.
     */
    assert(*(int*)node1->data == *(int*)racine->right->data);
    printf("PASS -> AVLtree_insertNode\n");

    //test AVLtree_insertData
    int k = 15;
    racine = AVLtree_insertData(racine, compare, &k, sizeInt);
    // /!\ une rotation vers la droite doit etre effectue
    printf("------------\n");
    display_avl(racine);
    assert(9 == *(int*)racine->data);
    assert(5 == *(int*)racine->left->data);
    assert(15 == *(int*)racine->right->data);

    int m = 4, n = 3;
    racine = AVLtree_insertData(racine, compare, &m, sizeInt);
    printf("------------\n");
    display_avl(racine);
    assert(9 == *(int*)racine->data);
    assert(5 == *(int*)racine->left->data);
    assert(4 == *(int*)racine->left->left->data);
    assert(15 == *(int*)racine->right->data);

    printf("\n------------\n");
    racine = AVLtree_insertData(racine, compare, &n, sizeInt);
    // /!\ une rotation vers la gauche doit etre effectue
    printf("------------\n");
    display_avl(racine);
    assert(9 == *(int*)racine->data);
    assert(4 == *(int*)racine->left->data);
    assert(3 == *(int*)racine->left->left->data);
    assert(5 == *(int*)racine->left->right->data);
    assert(15 == *(int*)racine->right->data);
    printf("PASS -> AVLtree_insertData\n");

    //test AVLtree_getHeight
    assert(3 == AVLtree_getHeight(racine));
    assert(2 == AVLtree_getHeight(racine->left));
    assert(1 == AVLtree_getHeight(racine->right));
    printf("PASS -> AVLtree_getHeight\n");

    //test AVLtree_setHeight
    AVLtree_setHeight(racine, 10);
    assert(10 == AVLtree_getHeight(racine));
    AVLtree_setHeight(racine, 3);
    assert(3 == AVLtree_getHeight(racine));
    printf("PASS -> AVLtree_setHeight\n");

    //test AVLtree_height_basedToNode
    assert(3 == AVLtree_height_basedToNode(racine));
    assert(2 == AVLtree_height_basedToNode(racine->left));
    assert(1 == AVLtree_height_basedToNode(racine->right));
    printf("PASS -> AVLtree_height_basedToNode\n");

    //test AVLtree_size_basedToNode
    assert(5 == AVLtree_size_basedToNode(racine));
    assert(3 == AVLtree_size_basedToNode(racine->left));
    assert(1 == AVLtree_size_basedToNode(racine->right));
    printf("PASS -> AVLtree_size_basedToNode\n");

    //test AVLtree_getMIN
    assert(3 == *(int*)AVLtree_getMIN(racine)->data);
    assert(3 == *(int*)AVLtree_getMIN(racine->left)->data);
    assert(15 == *(int*)AVLtree_getMIN(racine->right)->data);
    printf("PASS -> AVLtree_getMIN\n");

    //test AVLtree_getMAX
    assert(15 == *(int*)AVLtree_getMAX(racine)->data);
    assert(5 == *(int*)AVLtree_getMAX(racine->left)->data);
    assert(15 == *(int*)AVLtree_getMAX(racine->right)->data);
    printf("PASS -> AVLtree_getMAX\n");

    //test AVLtree_search
    int notFound = 100;
    assert(15 == *(int*)AVLtree_search(racine, compare, &k)->data);
    assert(NULL == AVLtree_search(racine, compare, &notFound));
    printf("PASS -> AVLtree_search\n");

    racine = AVLtree_deleteNode(racine, compare, racine);
    printf("------------\n");
    display_avl(racine);
    assert(4 == *(int*)racine->data);
    assert(3 == *(int*)racine->left->data);
    assert(15 == *(int*)racine->right->data);
    assert(5 == *(int*)racine->right->left->data);
    printf("PASS -> AVLtree_deleteNode\n");

    int delData = 15;
    racine = AVLtree_deleteData(racine, compare, &delData);
    printf("------------\n");
    display_avl(racine);
    assert(4 == *(int*)racine->data);
    assert(3 == *(int*)racine->left->data);
    assert(5 == *(int*)racine->right->data);
    printf("PASS -> AVLtree_deleteData\n");

    //test AVLtree_pre_order
    printf("\nTri pre-order : \n");
    AVLtree_pre_order(racine, monPrintF, NULL);
    printf("PASS -> AVLtree_pre_order\n");

    //test AVLtree_in_order
    printf("\nTri in-order : \n");
    AVLtree_in_order(racine, monPrintF, NULL);
    printf("PASS -> AVLtree_in_order\n");

    //test AVLtree_post_order
    printf("\nTri post-order : \n");
    AVLtree_post_order(racine, monPrintF, NULL);
    printf("PASS -> AVLtree_post_order\n");

    //test AVLtree_deleteTree
    AVLtree_deleteTree(&racine);
    printf("------------\n");
    display_avl(racine);
    printf("PASS -> AVLtree_deleteTree\n");
}

int main(){
    testArbresAVL();

    printf("\n\n-----RANDOM TREE-------\n");

    srand(12345);
    randomTree();
    return EXIT_SUCCESS;
}
