#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include <stdlib.h>
#include <stdbool.h>


typedef struct AVLTreeNode *AVLTree;
struct          AVLTreeNode {
        AVLTree father;
        AVLTree left;
        AVLTree right;
        int     height;
        char    data[1];
};

/*--------------------------------------------------------------------*/
AVLTree AVLtree_new();

//----------------------------------------
void    *AVLtree_getData(const AVLTree tree);
bool    AVLtree_setData(const AVLTree tree, const void *data, size_t size);
AVLTree AVLtree_create(const void *data, size_t size);

//----------------------------------------
size_t  AVLtree_getHeight(const AVLTree tree);
bool    AVLtree_setHeight(const AVLTree tree, int height);

//----------------------------------------
size_t  AVLtree_height_basedToNode(const AVLTree tree);
size_t  AVLtree_size_basedToNode(const AVLTree tree);

//----------------------------------------
AVLTree AVLtree_getMIN(const AVLTree node);
AVLTree AVLtree_getMAX(const AVLTree node);

//----------------------------------------
AVLTree AVLtree_rotateLeft(const AVLTree tree);
AVLTree AVLtree_rotateRight(const AVLTree tree);
AVLTree AVLtree_doubleRotateLeft(const AVLTree tree);
AVLTree AVLtree_doubleRotateRight(const AVLTree tree);

//----------------------------------------
AVLTree AVLtree_search(AVLTree tree, bool (*cmp)(const void *, const void *), void *data);

//----------------------------------------
AVLTree AVLtree_insertData(AVLTree tree, bool (*cmp)(const void *, const void *), const void *data, size_t size);
AVLTree AVLtree_insertNode(AVLTree tree, bool (*cmp)(const void *, const void *), const AVLTree newNode);

//----------------------------------------
AVLTree AVLtree_deleteNode(AVLTree tree, bool (*cmp) (const void *, const void *), const AVLTree node);
AVLTree AVLtree_deleteData(AVLTree tree, bool (*cmp) (const void *, const void *), void *data);

//----------------------------------------
void    AVLtree_pre_order(const AVLTree tree, void (*func)(void *, void *), void *extra_data);
void    AVLtree_in_order(const AVLTree tree, void (*func)(void *, void *), void *extra_data);
void    AVLtree_post_order(const AVLTree tree, void (*func)(void *, void *), void *extra_data);
/*--------------------------------------------------------------------*/

#endif