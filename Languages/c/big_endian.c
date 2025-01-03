#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    int a = 0x123456;
    char *ptr = (char *)&a;

    printf("a: %x (size int: %lu bytes) address: %p\n", a, sizeof(a), &a);
    /**Big-Endian Byte Order: The most significant byte (the "big end") of the data,
    * is placed at the byte with the lowest address.
    */
    if(ptr[0] == 0x12){
        printf("Big endian!\n");
    } else {
        printf("Little endian!\n");
    }
    /// show
    int k = 0;
    for (k = 0; k < 4; ++k){
        printf("[%d] %x\n", k, ptr[k]);
    }
    return 0;
}