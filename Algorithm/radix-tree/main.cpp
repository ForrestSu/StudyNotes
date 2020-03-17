#include <cstdio>
#include <cstring>
#include <cassert>
#include "radix_tree.hpp"

void insert_node(radix_tree* tree, const char *key)
{
    size_t len = strlen(key);
    bool isOk = tree->insert((const unsigned char*)key, len);
    assert(isOk);
}

void do_test()
{
    printf("sizeof(std::size_t) = %ld\n", sizeof(std::size_t)); //8 bytes
    radix_tree tree;
    printf("sizeof(tree) = %ld\n", sizeof(tree)); // 16 bytes

    insert_node(&tree, "zhangsan");
    insert_node(&tree, "zhanghaha");
    insert_node(&tree, "zhanghaha2");
    tree.print();
}

int main()
{
    do_test();
    return 0;
}
