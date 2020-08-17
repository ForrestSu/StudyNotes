#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
 
class Factory {
public:
    virtual ~Factory() = default;
    virtual const char* GetMainService(const char *serviceName) = 0;
};

typedef Factory* (*EntryGetServiceFactory)();

int main(int argc, char **argv)
{
    /** str_so_name is utf-8 encode */
    const char* str_so_name = "libtestzuv.so";
    uv_lib_t* m_hdl = (uv_lib_t*) malloc(sizeof(uv_lib_t));

    //1 open shared library
    int rc = uv_dlopen(str_so_name, m_hdl);
    if (rc == -1) {
        //uv_dlerror():  Returns the last uv_dlopen() or uv_dlsym() error message.
        fprintf(stderr, "uv_dlopen(%s) error: %s\n", str_so_name, uv_dlerror(m_hdl));
        return 1;
    }
    // 2 get entry address
    EntryGetServiceFactory entry = NULL;
    rc = uv_dlsym(m_hdl, "GetServiceFactory", (void **) &entry);
    if (rc == -1) {
        fprintf(stderr, "uv_dlsym(%s) can't find factory entry! error: <%s>.\n", str_so_name, uv_dlerror(m_hdl));
        return 1;
    }
    // 3 call c function: entry()
    Factory* factoryPtr = entry();
    if (factoryPtr == NULL) {
        fprintf(stderr, "error:  factoryInstancePtr is null!\n");
        return 1;
    }
    // 4 close shared library
    uv_dlclose(m_hdl);
    printf("exit!\n");
    return rc;
}
