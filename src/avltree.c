#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <unistd.h>

#include "avltree.h"
#include "min-max.h"


/*
 * Retourne un une valeur nulle, permettant d'initialiser une variable.
 * Reproduit l'idee d'une instantiation sans le principe d'allocation de
 * memoire.
 * Pour creer une instance de 'AVLTree', voir la methode #AVLtree_create().
 */
AVLTree AVLtree_new() {
    return NULL;
}

/*--------------------------------------------------------------------*/
/*
 * Getter sur la valeur 'data' de 'AVLTree'
 */
void    *AVLtree_getData(const AVLTree tree) {
    if (tree) {
        return &tree->data;
    }
    return NULL;
}

/*
 * Setter sur la valeur 'data' de 'AVLTree'
 * Cette fonction n'est pas a utiliser comme fonction d'arbre car celle-ci n'effectue aucune rotation.
 */
bool AVLtree_setData(const AVLTree tree, const void *data, size_t size) {
    if (tree) {
        memcpy(tree->data, data, size);
        return true;
    }
    return false;
}

/*
 * Cree une nouvelle instantiation de la structure 'AVLTree'.
 * Prends en parametre une donnee 'data' pouvant etre n'importe de quel type ainsi que sa taille.
 * L'idee ici est d'utiliser le champ 'data' de la structure 'AVLTree' afin d'augmenter sa taille
 * lors de l'allocation de memoire pour pouvoir y faire rentrer n'importe quelle valeur.
 */
AVLTree AVLtree_create(const void *data, size_t size) {
    AVLTree tree;

    tree = AVLtree_new();
    if ((tree = (AVLTree) malloc((sizeof(AVLTree) * 2) + sizeof(int) + size))) {
        tree->left = NULL;
        tree->right = NULL;
        memcpy(tree->data, data, size);
    }
    return tree;
}

//----------------------------------------
/*
 * Getter sur la valeur 'height' de 'AVLTree'
 */
size_t  AVLtree_getHeight(const AVLTree tree) {
    if (tree)
        return tree->height;
    else
        return 0;
}

/*
 * Setter sur la valeur 'height' de 'AVLTree'
 */
bool    AVLtree_setHeight(const AVLTree tree, int height) {
    if (tree) {
        tree->height = height;
        return true;
    }
    return false;
}

//----------------------------------------
/*
 * Renvoie la profondeur (hauteur de l'arbre) base sur un noeud donne en parametre.
 */
size_t  AVLtree_height_basedToNode(const AVLTree tree) {
    if (tree)
        return 1 + MAX (AVLtree_height_basedToNode(tree->left),AVLtree_height_basedToNode(tree->right));
    else
        return 0;
}

/*
 * Renvoie le nombre de noeud base sur un noeud donne en parametre.
 */
size_t  AVLtree_size_basedToNode(const AVLTree tree) {
    if (tree)
        return 1 + AVLtree_size_basedToNode(tree->left) + AVLtree_size_basedToNode(tree->right);
    else
        return 0;
}

//----------------------------------------
/*
 * Retourne la valeur minimale de l'arbre.
 * On ne fait pas de comparaison entre les noeuds, car nous estimons que l'arbre est
 * correctement complete et que celui-ci est donc la valeur la plus a gauche de l'arbre.
 */
AVLTree AVLtree_getMIN(const AVLTree node) {
    if (node && node->left)
        return AVLtree_getMIN(node->left);
    return node;
}

/*
 * Retourne la valeur maximale de l'arbre.
 * On ne fait pas de comparaison entre les noeuds, car nous estimons que l'arbre est
 * correctement complete et que celui-ci est donc la valeur la plus a droite de l'arbre.
 */
AVLTree AVLtree_getMAX(const AVLTree node) {
    if (node && node->right)
        return AVLtree_getMAX(node->right);
    return node;
}

//----------------------------------------
/*
 * Fonction de rotation de l'arbre, base sur un noeud.
 * Nous interchangeons les pointeurs pour artificiellement tourner les noeuds vers la gauche.
 */
AVLTree AVLtree_rotateLeft(const AVLTree tree) {
    AVLTree oNode;

    oNode = tree->left;
    tree->left = oNode->right;
    oNode->right = tree;

    AVLtree_setHeight(tree, MAX(AVLtree_getHeight(tree->left), AVLtree_getHeight(tree->right)) + 1);
    AVLtree_setHeight(oNode, MAX(AVLtree_getHeight(oNode->left), tree->height) + 1);
    return oNode;
}

/*
 * Fonction de rotation de l'arbre, base sur un noeud.
 * Nous interchangeons les pointeurs pour artificiellement tourner les noeuds vers la droite.
 */
AVLTree AVLtree_rotateRight(const AVLTree tree) {
    AVLTree oNode;

    oNode = tree->right;
    tree->right = oNode->left;
    oNode->left = tree;

    AVLtree_setHeight(tree, MAX(AVLtree_getHeight(tree->left), AVLtree_getHeight(tree->right)) + 1);
    AVLtree_setHeight(oNode, MAX(AVLtree_getHeight(oNode->right), tree->height) + 1);
    return oNode;
}

/*
 * Fonction de rotation de l'arbre, base sur un noeud.
 * Nous interchangeons les pointeurs pour artificiellement tourner les noeuds vers la droite
 * puis vers la gauche.
 */
AVLTree AVLtree_doubleRotateLeft(const AVLTree tree) {
    tree->left = AVLtree_rotateRight(tree->left);
    return AVLtree_rotateLeft(tree);
}

/*
 * Fonction de rotation de l'arbre, base sur un noeud.
 * Nous interchangeons les pointeurs pour artificiellement tourner les noeuds vers la gauche
 * puis vers la droite.
 */
AVLTree AVLtree_doubleRotateRight(const AVLTree tree) {
    tree->right = AVLtree_rotateLeft(tree->right);
    return AVLtree_rotateRight(tree);
}

//----------------------------------------
/*
 * Recherche et renvoie un noeud base sur la fonction de comparaison donne en parametre ainsi qu'une
 * donne libre. Il est possible de chercher un noeud precis en le donnant comme 'data' et en utilisant
 * une fonction de comparaison adequate.
 */
AVLTree AVLtree_search(AVLTree tree, bool (*cmp)(const void *, const void *), void *data) {
    if (tree) {
        if (cmp(data, tree->data))
            return AVLtree_search(tree->left, cmp, data);
        else if (cmp(tree->data, data))
            return AVLtree_search(tree->right, cmp, data);
        else
            return tree;
    }
    return NULL;
}

//----------------------------------------
/*
 * Fonction d'insertion de nouvelle donnee recursive.
 * initialement, nous avons la racine en tant que parametre 'tree'.
 * Au fil des recursions, ce parametre va permettre de se deplacer vers un endroit
 * correspondant a l'emplacement voulu en utilisant la fonction de comparaison pointe.
 * L'arrive se caracterise par une fin de parcours (NULL trouve dans une branche) et se suit
 * la creation d'un nouveau noeud avec les parametres 'data' et 'size'.
 * Ce nouveau noeud est remonte lors de la completion de la pile de recursion et positionne au
 * plus bas de l'arbre.
 * S'en suit des tests permettant de savoir si il y a besoin d'effectuer des rotations pour
 * reequilibrer l'arbre. Les bonnes rotations sont donc effectues au fil de la resolution de la
 * recursion.
 * Si la racine devait etre differente car elle ne correspondrait plus aux donnees,
 * celle-ci serait retourne par cette fonction.
 */
AVLTree AVLtree_insertData(AVLTree tree, bool (*cmp)(const void *, const void *), const void *data, size_t size) {
    if (data) {
        if (!tree) {
            if (!(tree = AVLtree_create(data, size)))
                return NULL;
        } else if (cmp(data, tree->data)) {
            tree->left = AVLtree_insertData(tree->left, cmp, data, size);

            if (AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right) == 2) {
                if (cmp(data, tree->left->data))
                    tree = AVLtree_rotateLeft(tree);
                else
                    tree = AVLtree_doubleRotateLeft(tree);
            }

        } else if (cmp(tree->data, data)) {
            tree->right = AVLtree_insertData(tree->right, cmp, data, size);

            if (AVLtree_getHeight(tree->right) - AVLtree_getHeight(tree->left) == 2) {
                if (cmp(tree->right->data, data))
                    tree = AVLtree_rotateRight(tree);
                else
                    tree = AVLtree_doubleRotateRight(tree);
            }
        }

        AVLtree_setHeight(tree, MAX(AVLtree_getHeight(tree->left), AVLtree_getHeight(tree->right))+ 1);
        return tree;
    }
    return NULL;
}

/*
 * Fonction d'insertion de nouvelle donnee recursive.
 * initialement, nous avons la racine en tant que parametre 'tree'.
 * Au fil des recursions, ce parametre va permettre de se deplacer vers un endroit
 * correspondant a l'emplacement voulu en utilisant la fonction de comparaison pointe.
 * L'arrive se caracterise par une fin de parcours (NULL trouve dans une branche) et se suit
 * le positionnement du noeud entre initialement en parametre.
 * Ce nouveau noeud est remonte lors de la completion de la pile de recursion et positionne au
 * plus bas de l'arbre.
 * S'en suit des tests permettant de savoir si il y a besoin d'effectuer des rotations pour
 * reequilibrer l'arbre. Les bonnes rotations sont donc effectues au fil de la resolution de la
 * recursion.
 * Si la racine devait etre differente car elle ne correspondrait plus aux donnees,
 * celle-ci serait retourne par cette fonction.
 */
AVLTree AVLtree_insertNode(AVLTree tree, bool (*cmp) (const void *, const void *), const AVLTree newNode) {
    if (newNode && newNode->data) {
        if (!tree) {
            tree = newNode;
        } else if (cmp(newNode->data, tree->data)) {
            tree->left = AVLtree_insertNode(tree->left, cmp, newNode);

            if (AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right) == 2) {
                if (cmp(newNode->data, tree->left->data))
                    tree = AVLtree_rotateLeft(tree);
                else
                    tree = AVLtree_doubleRotateLeft(tree);
            }

        } else if (cmp(tree->data, newNode->data)) {
            tree->right = AVLtree_insertNode(tree->right, cmp, newNode);

            if (AVLtree_getHeight(tree->right) - AVLtree_getHeight(tree->left) == 2) {
                if (cmp(tree->right->data, newNode->data))
                    tree = AVLtree_rotateRight(tree);
                else
                    tree = AVLtree_doubleRotateRight(tree);
            }
        }

        AVLtree_setHeight(tree, MAX(AVLtree_getHeight(tree->left), AVLtree_getHeight(tree->right))+ 1);
        return tree;
    }
    return NULL;
}

//----------------------------------------
/*
* Fonction de suppression de noeud.
* Celle-ci va d'abord se positionner par recursion sur le noeud correspondant au noeud voulant être supprime.
* Apres, nous separons les etats entre les noeuds possedant 0 ou 1 fils et le reste pour effecter un traitement
* permettant de liberer la memoire.
* A la suite, nous changeons la taille du noeud et balancons les noeuds si nous avons besoin avant de le
* retourner pour depiler la recursion et refaire ces operations.
*/
AVLTree AVLtree_deleteNode(AVLTree tree, bool (*cmp) (const void *, const void *), const AVLTree delNode) {
    if (tree == NULL)
        return tree;

    if (cmp(delNode->data, tree->data)) {
        tree->left = AVLtree_deleteNode(tree->left, cmp, delNode);

    } else if (cmp(tree->data, delNode->data)) {
        tree->right = AVLtree_deleteNode(tree->right, cmp, delNode);

    } else {
        if (!tree->left || !tree->right) {
            AVLTree oNode;

            oNode = (tree->left) ? tree->left : tree->right;
            if (!oNode) {
                oNode = tree;
                tree = NULL;
            } else {
                *tree = *oNode;
            }
            free(oNode);
        } else {
            AVLTree oNode;

            oNode = tree->right;
            while (oNode->left)
                oNode = oNode->left;

            *tree->data = *oNode->data;
            tree->right = AVLtree_deleteNode(tree->right, cmp, delNode);
        }
    }

    if (tree == NULL)
        return tree;

    AVLtree_setHeight(tree, MAX(AVLtree_getHeight(tree->left), AVLtree_getHeight(tree->right))+ 1);

    int balance = AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right);
    if (balance > 1 && (AVLtree_getHeight(tree->left->left) - AVLtree_getHeight(tree->left->right)) >= 0)
        return AVLtree_rotateLeft(tree);
    if (balance > 1 && (AVLtree_getHeight(tree->left->left) - AVLtree_getHeight(tree->left->right)) < 0)
        return AVLtree_doubleRotateLeft(tree);

    if (balance < -1 && (AVLtree_getHeight(tree->right->left) - AVLtree_getHeight(tree->right->right)) <= 0)
        return AVLtree_rotateRight(tree);
    if (balance < -1 && (AVLtree_getHeight(tree->right->left) - AVLtree_getHeight(tree->right->right)) > 0)
        return AVLtree_doubleRotateRight(tree);

    return tree;
}


/*
 * Fonction de suppression de noeud.
 * Celle-ci va d'abord se positionner par recursion sur le noeud correspondant au noeud voulant être supprime.
 * Apres, nous separons les etats entre les noeuds possedant 0 ou 1 fils et le reste pour effecter un traitement
 * permettant de liberer la memoire.
 * A la suite, nous changeons la taille du noeud et balancons les noeuds si nous avons besoin avant de le
 * retourner pour depiler la recursion et refaire ces operations.
 */
AVLTree AVLtree_deleteData(AVLTree tree, bool (*cmp) (const void *, const void *), void *data) {
    if (tree == NULL)
        return tree;

    if (cmp(data, tree->data)) {
        tree->left = AVLtree_deleteData(tree->left, cmp, data);

    } else if (cmp(tree->data, data)) {
        tree->right = AVLtree_deleteData(tree->right, cmp, data);

    } else {
        if (!tree->left || !tree->right) {
            AVLTree oNode;

            oNode = (tree->left) ? tree->left : tree->right;
            if (!oNode) {
                oNode = tree;
                tree = NULL;
            } else {
                *tree = *oNode;
            }
            free(oNode);
        } else {
            AVLTree oNode;

            oNode = tree->right;
            while (oNode->left)
                oNode = oNode->left;

            *tree->data = *oNode->data;
            tree->right = AVLtree_deleteData(tree->right, cmp, data);
        }
    }

    if (tree == NULL)
        return tree;

    AVLtree_setHeight(tree, MAX(AVLtree_getHeight(tree->left), AVLtree_getHeight(tree->right))+ 1);

    int balance = AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right);
    if (balance > 1 && (AVLtree_getHeight(tree->left->left) - AVLtree_getHeight(tree->left->right)) >= 0)
        return AVLtree_rotateRight(tree);
    if (balance > 1 && (AVLtree_getHeight(tree->left->left) - AVLtree_getHeight(tree->left->right)) < 0)
        return AVLtree_doubleRotateRight(tree);

    if (balance < -1 && (AVLtree_getHeight(tree->right->left) - AVLtree_getHeight(tree->right->right)) <= 0)
        return AVLtree_rotateLeft(tree);
    if (balance < -1 && (AVLtree_getHeight(tree->right->left) - AVLtree_getHeight(tree->right->right)) > 0)
        return AVLtree_doubleRotateLeft(tree);

    return tree;
}

/*
 * Fonction permettant de libérer la mémoire de tout l'arbre
 */
void    AVLtree_deleteTree(AVLTree *tree) {
    if (*tree) {
        AVLtree_deleteTree(&((*tree)->left));
        AVLtree_deleteTree(&((*tree)->right));
        free(*tree);
        *tree = NULL;
    }
}

//----------------------------------------
/*
 * Fonction de pretraitement recursive.
 * Applique recursivement une fonction donnee en parametre avant de parcourir ses noeuds associes.
 */
void    AVLtree_pre_order(const AVLTree tree, void (*func) (void *, void *), void *extra_data) {
    if (tree) {
        func(tree->data, extra_data);
        AVLtree_pre_order(tree->left, func, extra_data);
        AVLtree_pre_order(tree->right, func, extra_data);
    }
}

/*
 * Fonction de traitement recursive.
 * Applique recursivement une fonction donnee en parametre entre les parcours des noeuds de gauche
 * et de droite.
 */
void    AVLtree_in_order(const AVLTree tree, void (*func) (void *, void *), void *extra_data) {
    if (tree) {
        AVLtree_in_order(tree->left, func, extra_data);
        func (tree->data, extra_data);
        AVLtree_in_order(tree->right, func, extra_data);
    }
}

/*
 * Fonction d' apres traitement recursive.
 * Applique recursivement une fonction donnee en parametre apres avoir parcouru tout les noeuds
 * associes.
 */
void    AVLtree_post_order(const AVLTree tree, void (*func) (void *, void *), void *extra_data) {
    if (tree) {
        AVLtree_post_order(tree->left, func, extra_data);
        AVLtree_post_order(tree->right, func, extra_data);
        func (tree->data, extra_data);
    }
}

//----------------------------------------