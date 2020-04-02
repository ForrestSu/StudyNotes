#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <deque>
using namespace std;
#define MAXN 127

struct Trie {
    char ch;
    int is_word; // 标记是否为一个单词 0-No 1-Yes (如果使用累加，则支持重复计数)
    int is_marked; // 是否已经计数 0-未计数 1-已计数
    Trie *fail_ptr;
    Trie *next[MAXN];
    int data;
};

struct Trie* create_node(char ch, int level) {
    Trie *node = (Trie*) malloc(sizeof(Trie));
    memset(node, 0, sizeof(Trie));
    node->ch = ch;
    node->data = level;
    node->is_word = 0;
    node->is_marked = 0;
    node->fail_ptr = nullptr;
    return node;
}

bool insert_tree(Trie *root, const std::string& words) {
    Trie *cur = root;
    for (size_t i = 0; i < words.length(); ++i) {
        int8_t ch = words[i];
        Trie *child = cur->next[ch];
        if (child == nullptr) {
            child = create_node(ch, i + 1);
            cur->next[ch] = child;
        }
        cur = child;
    }
    cur->is_word = 1;
    return true;
}

// 链式查找失败指针并返回, 未找到返回缺省值(一般为root)
Trie* do_find_fail_ptr(Trie *cur, int ch, Trie *default_ptr) {
    Trie *result = default_ptr;
    Trie *ptrfail = cur->fail_ptr;
    // 链式查找:一直追溯到root
    while (ptrfail != nullptr) {
        // same name child occur, got it!
        if (ptrfail->next[ch] != nullptr) {
            result = ptrfail->next[ch];
            printf("为[%c]找到失败指针<%c>\n", ch, result->ch);
            break;
        }
        ptrfail = ptrfail->fail_ptr;
    }
    return result;
}
/***
 * (1) 按层(BFS搜索)维护失败指针，(失败指针是存在于Tire树上的一个元素节点而已)
 * (2) 每层节点负责维护下一层的失败指针
 * (3) 根节点的失败指针指向nil
 * (4) 根的第一层儿子的失败指针都指向root
 * (5) 每一层怎么为下一层找到失败指针呢?
 *     a. 先拿到一个儿子S，拿出自身的失败指针P；
 *     b. 判断该失败指针P是否也有同名的儿子S'呢？
 *     c. 如果有，则把S的失败指针指向 S',停止；
 *     d. 如果没有，P=P->fail_ptr 上，重复步骤(b)
 */
bool build_fail_ptr(Trie *root) {
    //这个nullptr在后面也作为一个搜索终止条件
    root->fail_ptr = nullptr;
    std::deque<Trie*> Q;
    Q.push_back(root);
    while (!Q.empty()) {
        Trie *cur = Q.front();
        Q.pop_front();
        for (int i = 0; i < MAXN; i++) {
            if (cur->next[i] != nullptr) {
                Trie *child = cur->next[i];
                // 为child节点维护失败指针
                child->fail_ptr = do_find_fail_ptr(cur, child->ch, root);
                // 把child 加入队列
                Q.push_back(child);
            }
        }
    }
    return true;
}

// 检查是否为一个单词, 并返回数量
int do_check_and_return_cnt(Trie *cur) {
    if (cur->is_word && cur->is_marked == 0) {
        // marked it
        cur->is_marked = 1;
        printf("match ch <%c>\n", cur->ch);
        return cur->is_word;
    }
    return 0;
}
/**
 *  返回有几种模式串出现过
 */
int search(Trie *root, const std::string& matchs) {
    int iCount = 0;
    Trie *cur = root;
    size_t idx = 0;
    while (idx < matchs.length()) {
        int ch = matchs[idx++];
        // match it
        if (cur->next[ch] != nullptr) {
            cur = cur->next[ch];
            iCount += do_check_and_return_cnt(cur);
        } else {
            //失配:当前已匹配串,检查其所有后缀子串,因为中间可能有模式串
            while (cur != root) {
                cur = cur->fail_ptr;
                // 针对后缀串计数
                iCount += do_check_and_return_cnt(cur);
            }
        }
    }
    printf("search(<%s>) occur %d times!\n", matchs.c_str(), iCount);
    return iCount;
}

// 按层打印Trie树
void show(Trie *root) {
    std::deque<Trie*> Q;
    Q.push_back(root);
    int max_level = -1;
    while (!Q.empty()) {
        Trie *cur = Q.front();
        if (max_level < cur->data) {
            max_level = cur->data;
            printf("\nlevel %d\n", max_level);
        }
        printf("%c <word:%d>", cur->ch, cur->is_word);
        Q.pop_front();
        for (int i = 0; i < MAXN; ++i) {
            if (cur->next[i] != nullptr) {
                Q.push_back(cur->next[i]);
            }
        }
    }
    printf("\n--------\n");
}

int main() {
    printf("%lld\n", sizeof(Trie));
    Trie *root = create_node('r', 0);
    {
        string words[] = { "she", "he" };
        insert_tree(root, words[0]);
        insert_tree(root, words[1]);
    }
    show(root);
    build_fail_ptr(root);

    string matchs = "sher";
    search(root, matchs);
    return 0;
}
