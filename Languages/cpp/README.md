## C++ awesome




## 常用库列表

### 1 JSON

[picojson](https://github.com/kazuho/picojson) 仅包含头文件轻量Json库（性能上还过的去)

### 命令行参数解析

(1) [gflags](https://github.com/gflags/gflags)

Gflags 是Google内部使用的命令行参数库， 它与其他库不同(例如 getopt())， 
flag的定义可以分散在不同的源代码中，而不仅是列出一个地方的(例如main)。
实际上，这意味着单个源代码文件需要定义和使用对该文件有意义的标志，
应用程序链接到该文件将拥有这些标志，并且gflags库将自动适当地处理这些flag。

由于采用了这种技术，因此在灵活性和简化代码重用方面均获得了重大收益。
但是，如果存在两个文件定义相同flag，然后将它们链接在一起时产生错误的危险。
