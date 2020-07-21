#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <iostream>
using namespace std;

struct ONE {};
struct ZERO { char x[0]; };

int main() {
  cout << sizeof(ONE) << ", " << sizeof(ZERO) << endl;

  ONE* po1 = new ONE;
  ONE* po2 = new ONE;
  cout << po1 << ", " << po2 << endl;

  ZERO* pz1 = new ZERO;
  ZERO* pz2 = new ZERO;
  cout << pz1 << ", " << pz2 << endl;
}

