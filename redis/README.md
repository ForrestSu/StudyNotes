

## how use redis in Go?





## Does 'hiredis' support Redis Sentinel and Redis Cluster?

YES and NO.

由于您可以使用`hiredis`来向 Redis 发送任何命令，从而可以从Redis Sentinel获取 master/slave 信息，也可以从Redis Cluster获取插槽信息。 因此hiredis可以与Redis Sentinel和Redis Cluster一起使用。

但是，由于hiredis没有用于哨兵和集群的高级API，因此您必须自己完成许多工作。如果需要高级API，则需要尝试[其他库](https://redis.io/clients)，例如：

如果您使用进行编码C，则可以尝试 [hireddis-vip](https://github.com/vipshop/hiredis-vip)，它支持Redis Cluster。但是我不确定它是否支持Redis Sentinel。

如果您使用进行编码C++，则可以尝试[redis-plus-plus](https://github.com/sewenew/redis-plus-plus)，它支持Redis Cluster和Redis Sentinel，并且具有类似STL的接口。

声明：这是 redis-plus-plus作者的回答。

```cpp
// Example on redis-plus-plus

#include <sw/redis++/redis++.h>

try {
    auto cluster = RedisCluster("tcp://127.0.0.1:7000");
    cluster.set("key", "val");
    auto val = cluster.get("key");
    if (val) cout << *val << endl;
} catch (const Error &e) {
    cerr << e.what() << endl;
}
```


参考链接：

- [Does 'hiredis' support Redis Sentinel and Redis Cluster?](https://stackoverflow.com/questions/59147990/does-hiredis-support-redis-sentinel-and-redis-cluster)
