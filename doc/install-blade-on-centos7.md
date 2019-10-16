
# 在 centos-7 上安装 blade 

## install

先安装deps:
yum install -y  scons ccache  distcc  cmake curl 

```bash
git clone https://github.com/chen3feng/blade-build.git
cd blade-build
./install
```

## Usage

1 工作目录在 builder 文件夹下

> mkdir test_blade && cd test_blade  
> touch main.cpp  
> touch BUILD  

```cpp
// main.cpp
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "hello, world!" << std::endl;
    return 0;
}
```

```
# BUILD 文件内容
cc_binary (
  name = 'app',
  srcs = 'main.cpp',
)
```

2 跳出 test_blade 文件夹，编译工程

> cd ..  
> blade build test_blade -pdebug 

3 运行

> blade run test_blade:app