#include <iostream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

#define DEST_V 8500
#define MAX_V  9900

/**
 * 1 比如报销金额为5000, 凑满5000金额的最小的发票选取方案
 */
struct DP {
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
     *  from 2020.12.25 to 2019.04.05
     */
    vector<int> items = {2132, 1483, 1290, 2093, 2218, 2064, 2329, 2727, 1542, 1790, 1760, 1574 };
    printf("items.size() = %lu\n", items.size());
    for (int id = 0; id < (int) items.size(); ++id) {
        zero_one_pack(items[id]);
    }

    bool isFind = false;
    for (int i = DEST_V; i <= MAX_V; ++i) {
        if (dp[i].value > 0) {
            printf("money: %.2f, Minimum solution = %.2f.\n", DEST_V/100.0, dp[i].value/100.0);
            printf("the plan is:\n");
            recursive_print_plan(dp[i].value);
            isFind = true;
            break;
        }
    }
    puts(isFind ? "OK!": "can't find the plan!");
    return 0;
}
