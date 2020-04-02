#include <iostream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

#define DEST_V 5000
#define MAX_V  6000

/**
 * 1 比如报销金额为5000, 凑满5000金额的最小的发票选取方案
 */
struct DP{
    int value;//当前背包的总价值
    int select_value;//最后选取的一个物品的价值
} dp[MAX_V + 1];

void init() {
    for (int i = 0; i <= MAX_V; ++i) {
        dp[i].value = -1;
        dp[i].select_value = -1;
    }
    dp[0].value = 0;
}
// 01背包: 每个物品选或者不选
void zero_one_pack(int item) {
    int cost = item;
    int value = item;
    for (int V = MAX_V; V >= cost; --V) {
        if (dp[V - cost].value >= 0) {
            int value_if_select_it = dp[V - cost].value + value;
            if (value_if_select_it > dp[V].value) {
                dp[V].value = value_if_select_it;
                dp[V].select_value = value;
            }
        }
    }
}

// 递归回溯时打印方案
void recursive_print_plan(int V) {
    if (V > 0) {
        recursive_print_plan(V - dp[V].select_value);
        printf("items: %d\n", dp[V].select_value);
    }
}

int main() {

    init();
    /**
     *  from 2020.03.30 to 2019.04.05
     */
    vector<int> items = { 1530, 3344, 1953, 2360, 2093, 2218, 2064, 2329, 2727, 1542, 1300, 1790, 1760, 1574, 1530 };
    printf("items.size() = %lld\n", items.size());
    for (int id = 0; id < (int) items.size(); ++id) {
        zero_one_pack(items[id]);
    }

    for (int i = DEST_V; i <= MAX_V; ++i) {
        if (dp[i].value > 0) {
            printf("minimum money = %d.\n", dp[i].value);
            printf("the plan is:\n");
            recursive_print_plan(dp[i].value);
            puts("OK!");
            break;
        }
    }
}
