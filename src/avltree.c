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
    if ((tree = (AVLTree) malloc((sizeof(AVLTree) * 3) + sizeof(int) + size))) {
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
    AVLTree ofNode;

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
 *
 */
AVLTree AVLtree_delete(AVLTree tree, bool (*cmp) (const void *, const void *), const AVLTree delNode) {
    //TODO
    if (tree)
        printf("in[%d]\n", *(int*)tree->data);
    else
        printf("in[NULL]\n");

    AVLTree p;

    if(tree == NULL)
    {
        return NULL;
    }
    else if(cmp(tree->data, delNode->data))		// insert in right subtree
    {
        tree->right = AVLtree_delete(tree->right, cmp, delNode);
        //TODO
        printf("right -> %d\n", AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right) >= 0);
        if(AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right) == 2)
            if(AVLtree_getHeight(tree->left->left) - AVLtree_getHeight(tree->left->right) >= 0)
                tree = AVLtree_rotateRight(tree);
            else
                tree = AVLtree_doubleRotateRight(tree);
    }
    else if(cmp(delNode->data, tree->data))
    {
        tree->left = AVLtree_delete(tree->left, cmp, delNode);
        //TODO
        printf("left -> %d\n", AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right) <= 0);
        if(AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right) == -2)	//Rebalance during windup
            if(AVLtree_getHeight(tree->right->left) - AVLtree_getHeight(tree->right->right) <= 0)
                tree = AVLtree_rotateLeft(tree);
            else
                tree = AVLtree_doubleRotateLeft(tree);
    }
    else {

        AVLTree oNode = tree;

        if (tree->left && tree->right) {
            AVLTree parent = tree->right;
            tree = parent->left;
            if (tree) {
                while (tree->left) {
                    parent = tree;
                    tree = tree->left;
                }
                parent->left = tree->right;
                tree->right = oNode->right;
            } else
                tree = parent;
            tree->left = oNode->left;

        } else if (tree->left)
            tree = tree->left;
        else
            tree = tree->right;
        free(oNode);





        if(tree->right!=NULL) {
            p = tree->right;

            while(p->left!= NULL)
                p = p->left;

            tree->data = p->data;
            tree->right = AVLtree_delete(tree->right,cmp, delNode);

            if(AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right) == 2)
                if(AVLtree_getHeight(tree->left->left) - AVLtree_getHeight(tree->left->right) >= 0)
                    tree = AVLtree_rotateRight(tree);
                else
                    tree = AVLtree_doubleRotateRight(tree);
				}
        else
            return(tree->left);
    }
    AVLtree_setHeight(tree, MAX(AVLtree_getHeight(tree->left), AVLtree_getHeight(tree->right))+ 1);
    return(tree);




    if (delNode && delNode->data && tree && tree->data) {
        if (cmp(delNode->data, tree->data)) {
            tree->left = AVLtree_delete(tree->left, cmp, delNode);

            if (AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right) == 2) {
                if (cmp(delNode->data, tree->left->data))
                    tree = AVLtree_rotateLeft(tree);
                else
                    tree = AVLtree_doubleRotateLeft(tree);
            }

        } else if (cmp(tree->data, delNode->data)) {
            tree->right = AVLtree_delete(tree->right, cmp, delNode);

            if (AVLtree_getHeight(tree->right) - AVLtree_getHeight(tree->left) == 2) {
                if (cmp(tree->right->data, delNode->data))
                    tree = AVLtree_rotateRight(tree);
                else
                    tree = AVLtree_doubleRotateRight(tree);
            }

        } else {
            AVLTree oNode;

            oNode = tree;
            if (tree->left) {
                tree = tree->left;
            } else if (tree->right) {
                tree = tree->right;
            } else {
                tree = NULL;
            }
            free(oNode);
        }

        if (tree)
            AVLtree_setHeight(tree, MAX(AVLtree_getHeight(tree->left), AVLtree_getHeight(tree->right))+ 1);

        //TODO
        if (tree)
            printf("out[%d]\n", *(int*)tree->data);
        else
            printf("out[NULL]\n");
        return tree;
    }
    //TODO
    printf("out[NULL]\n");
    return NULL;



    /*
    if (node) {
        if (cmp(node->data, tree->data)) {
            tree->left = AVLtree_delete(tree->left, cmp, node);

            if (AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right) == 2) {
                //TODO
                printf("left -> %d\n", AVLtree_getHeight(tree->right->left) - AVLtree_getHeight(tree->right->right) <= 0);
                if (AVLtree_getHeight(tree->right->left) - AVLtree_getHeight(tree->right->right) <= 0)
                    tree = AVLtree_rotateLeft(tree);
                else
                    tree = AVLtree_doubleRotateLeft(tree);
            }

        } else if (cmp(tree->data, node->data)) {
            tree->right = AVLtree_delete(tree->right, cmp, node);
            if (AVLtree_getHeight(tree->right) - AVLtree_getHeight(tree->left) == 2) {
                //TODO
                printf("right -> %d\n", AVLtree_getHeight(tree->right->left) - AVLtree_getHeight(tree->right->right) >= 0);
                if (AVLtree_getHeight(tree->left->left) - AVLtree_getHeight(tree->left->right) >= 0)
                    tree = AVLtree_rotateRight(tree);
                else
                    tree = AVLtree_doubleRotateRight(tree);
            }
        } else {
            AVLTree oNode = tree;

            if (tree->left && tree->right) {
                AVLTree parent = tree->right;
                tree = parent->left;
                if (tree) {
                    while (tree->left) {
                        parent = tree;
                        tree = tree->left;
                    }
                    parent->left = tree->right;
                    tree->right = oNode->right;
                } else
                    tree = parent;
                tree->left = oNode->left;

            } else if (tree->left)
                tree = tree->left;
            else
                tree = tree->right;
            free(oNode);
        }

        AVLtree_setHeight(tree, MAX(AVLtree_getHeight(tree->left), AVLtree_getHeight(tree->right))+ 1);
        return tree;
    }
    return NULL;*/

/*
    if (node) {
        if (tree && tree == node) {
            free(node);
        } else if (cmp(node->data, tree->data)) {
            AVLtree_delete(tree->left, cmp, node);

            if (AVLtree_getHeight(tree->left) - AVLtree_getHeight(tree->right) == 2) {
                if (cmp(node->data, tree->left->data))
                    tree = AVLtree_rotateLeft(tree);
                else
                    tree = AVLtree_doubleRotateLeft(tree);
            }

        } else if (cmp(tree->data, node->data)) {
            AVLtree_delete(tree->right, cmp, node);

            if (AVLtree_getHeight(tree->right) - AVLtree_getHeight(tree->left) == 2) {
                if (cmp(tree->right->data, node->data))
                    tree = AVLtree_rotateRight(tree);
                else
                    tree = AVLtree_doubleRotateRight(tree);
            }
        }

        return tree;
    }
    return NULL;*/
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