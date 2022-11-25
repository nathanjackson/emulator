//
// Created by nathan on 11/19/22.
//

#include <gtest/gtest.h>

#include "utils/avl_tree.h"

struct test_struct
{
    struct avl_node node;
    int value;
};

TEST(avl_tree_tests, insert)
{
    struct avl_tree tree;

    avl_tree_init(&tree, [](struct avl_node* lhs, struct avl_node* rhs) {
        struct test_struct *lhs_test = reinterpret_cast<test_struct *>(lhs);
        struct test_struct *rhs_test = reinterpret_cast<test_struct *>(rhs);
        return lhs_test->value - rhs_test->value;
    });

    struct test_struct *x = new struct test_struct;
    x->value = 7;
    avl_tree_insert(&tree, reinterpret_cast<avl_node*>(x));

    x = new struct test_struct;
    x->value = 6;
    avl_tree_insert(&tree, reinterpret_cast<avl_node*>(x));

    x = new struct test_struct;
    x->value = 5;
    avl_tree_insert(&tree, reinterpret_cast<avl_node*>(x));

    x = new struct test_struct;
    x->value = 4;
    avl_tree_insert(&tree, reinterpret_cast<avl_node*>(x));

    struct avl_node* search_result = avl_tree_search(&tree, NULL, [](struct avl_node* node, void* unused) {
        struct test_struct* test = reinterpret_cast<test_struct*>(node);
        return test->value - 4;
    });
    EXPECT_TRUE(NULL != search_result);

    avl_tree_postorder_traversal(&tree, [](struct avl_node* node) {
        delete node;
    });
}

struct region
{
    struct avl_node node;
    uint32_t addr;
    size_t sz;
};

TEST(avl_tree_tests, insert_range)
{
    struct avl_tree tree;

    avl_tree_init(&tree, [](struct avl_node* lhs, struct avl_node* rhs) {
        struct region *lhs_region = reinterpret_cast<region*>(lhs);
        struct region *rhs_region = reinterpret_cast<region*>(rhs);

        if ((lhs_region->addr + lhs_region->sz - 1) > (rhs_region->addr)) {
            return 1;
        } else {
            return -1;
        }
    });

    struct region *x = new struct region;
    x->addr = 0x0;
    x->sz = 0x400;
    avl_tree_insert(&tree, reinterpret_cast<avl_node*>(x));

    x = new struct region;
    x->addr = 0x500;
    x->sz = 0x7700;
    avl_tree_insert(&tree, reinterpret_cast<avl_node*>(x));

    x = new struct region;
    x->addr = 0x7C00;
    x->sz = 512;
    avl_tree_insert(&tree, reinterpret_cast<avl_node*>(x));

    struct avl_node* search_result = avl_tree_search(&tree, NULL, [](struct avl_node* node, void* unused) {
        struct region* test = reinterpret_cast<region*>(node);

        if ((test->addr < 0x30) && (0x30 <= (test->addr + test->sz - 1))) {
            return 0;
        } else if (0x30 < test->addr) {
            return 1;
        } else {
            return -1;
        }
    });
    ASSERT_TRUE(search_result);

    search_result = avl_tree_search(&tree, NULL, [](struct avl_node* node, void* unused) {
        struct region* test = reinterpret_cast<region*>(node);

        if ((test->addr < 0xFFFF) && (0xFFFF <= (test->addr + test->sz - 1))) {
            return 0;
        } else if (0xFFFF < test->addr) {
            return 1;
        } else {
            return -1;
        }
    });
    ASSERT_FALSE(search_result);


    avl_tree_postorder_traversal(&tree, [](struct avl_node* node) {
        delete node;
    });
}