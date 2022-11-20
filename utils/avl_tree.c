//
// Created by nathan on 11/19/22.
//
#include <stddef.h>

#include "avl_tree.h"

static int height(struct avl_node* n)
{
    int h = 0;
    if (n != NULL) {
        int l_height = height(n->lhs);
        int r_height = height(n->rhs);
        int max_height = l_height > r_height ? l_height : r_height;
        h = max_height + 1;
    }
    return h;
}

static int difference(struct avl_node* n)
{
    int l_height = height(n->lhs);
    int r_height = height(n->rhs);
    int b_factor = l_height - r_height;
    return b_factor;
}

static struct avl_node* rr_rotate(struct avl_node* parent)
{
    struct avl_node* n;
    n = parent->rhs;
    parent->rhs = n->lhs;
    n->lhs = parent;
    return n;
}

static struct avl_node* ll_rotate(struct avl_node* parent)
{
    struct avl_node *n;
    n = parent->lhs;
    parent->lhs = n->rhs;
    n->rhs = parent;
    return n;
}

static struct avl_node* lr_rotate(struct avl_node* parent)
{
    struct avl_node *n;
    n = parent->lhs;
    parent->lhs = rr_rotate(n);
    return ll_rotate(parent);
}

static struct avl_node* rl_rotate(struct avl_node* parent)
{
    struct avl_node* n;
    n = parent->rhs;
    parent->rhs = ll_rotate(n);
    return rr_rotate(parent);
}

static struct avl_node* balance(struct avl_node* n)
{
    int bal_factor = difference(n);
    if (bal_factor > 1) {
        if (difference(n->lhs) > 0)
            n = ll_rotate(n);
        else
            n = lr_rotate(n);
    } else if (bal_factor < -1) {
        if (difference(n->rhs) > 0)
            n = rl_rotate(n);
        else
            n = rr_rotate(n);
    }
    return n;
}


void avl_tree_init(struct avl_tree* tree, int (*comparator)(struct avl_node*, struct avl_node*))
{
    tree->root = NULL;
    tree->comparator = comparator;
}

static struct avl_node* insert(struct avl_tree* tree, struct avl_node* subtree, struct avl_node* node)
{
    if (NULL == subtree) {
        node->lhs = NULL;
        node->rhs = NULL;
        return node;
    } else if (tree->comparator(node, subtree) < 0) {
        subtree->lhs = insert(tree, subtree->lhs, node);
        subtree = balance(subtree);
    } else if (tree->comparator(node, subtree) > 0) {
        subtree->rhs = insert(tree, subtree->rhs, node);
        subtree = balance(subtree);
    }
    return subtree;
}

void avl_tree_insert(struct avl_tree* tree, struct avl_node* node)
{
    node->lhs = NULL;
    node->rhs = NULL;

    // special case: root is null
    if (NULL == tree->root) {
        tree->root = node;
        return;
    }

    tree->root = insert(tree, tree->root, node);
}

static void postorder(struct avl_node *node, void (*callback)(struct avl_node*))
{
    if (NULL == node) {
        return;
    }
    postorder(node->lhs, callback);
    postorder(node->rhs, callback);
    callback(node);
}

void avl_tree_postorder_traversal(struct avl_tree* tree, void (*callback)(struct avl_node*))
{
    postorder(tree->root, callback);
}

struct avl_node* avl_tree_search(struct avl_tree* tree, int (*query)(struct avl_node* node))
{
    struct avl_node* cur = tree->root;

    while (cur) {
        int d = query(cur);
        if (0 == d) {
            return cur;
        } else if (0 < d) {
            cur = cur->lhs;
        } else {
            cur = cur->rhs;
        }
    }

    return NULL;
}