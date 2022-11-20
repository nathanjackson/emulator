//
// Created by nathan on 11/19/22.
//

#ifndef EMULATOR_AVL_TREE_H
#define EMULATOR_AVL_TREE_H

#ifdef __cplusplus
extern "C"
{
#endif

struct avl_node {
    struct avl_node *lhs;
    struct avl_node *rhs;
};

struct avl_tree {
    struct avl_node *root;

    int (*comparator)(struct avl_node *, struct avl_node *);
};

void avl_tree_init(struct avl_tree *tree, int (*comparator)(struct avl_node *, struct avl_node *));

void avl_tree_insert(struct avl_tree *tree, struct avl_node *node);

void avl_tree_postorder_traversal(struct avl_tree *tree, void (*callback)(struct avl_node *));

struct avl_node *avl_tree_search(struct avl_tree *tree, int (*query)(struct avl_node *node));

#ifdef __cplusplus
}
#endif

#endif //EMULATOR_AVL_TREE_H
