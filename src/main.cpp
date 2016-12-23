#include "ManiaPP.h"
#include "../lib/pugixml/src/pugixml.hpp"

/*void* custom_allocate(size_t size)
{
    return new (std::nothrow) char[size];
}

void custom_deallocate(void* ptr)
{
    delete[] static_cast<char*>(ptr);
}*/

int main(int argc, char *argv[])
{
    //pugi::set_memory_management_functions(custom_allocate, custom_deallocate);

    ManiaPP* controller = new ManiaPP();
    if(controller->ConnectToServer())
    {
        controller->MainLoop();
        controller->Terminate();
    }
}
