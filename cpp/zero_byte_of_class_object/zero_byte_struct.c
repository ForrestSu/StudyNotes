#include <stdio.h>
#include <stdlib.h>
#include <memory>

struct ONE {};
struct ZERO { char x[0];};

int main()
{
  printf("%ld, %ld\n", sizeof(ONE), sizeof(ZERO));
  typedef ZERO UseType;
  UseType a;
  UseType b;
  
  UseType* pa = &a;
  UseType* pb = &b;

  // if( (&a) == (&b) ) is false, but  addressof is true
  if( std::addressof(a) == std::addressof(b) ){
    printf("&a == &b \n"); // OK
  } else {
    printf("&a != &b \n");
  }
  printf("&a = %p, &b = %p\n",  pa, pb);

  return 0;
}
