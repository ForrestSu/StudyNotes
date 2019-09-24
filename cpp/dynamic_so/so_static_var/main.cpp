#include <stdio.h>
#include "def.h" 

int main()
{
   printf("m_static_int = %d\n", A::m_static_int);
   return 0;
}
