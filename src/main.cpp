#include "ManiaPP.h"
#include "../lib/pugixml/src/pugixml.hpp"

int main(int argc, char *argv[])
{
    ManiaPP* controller = new ManiaPP();
    if(controller->ConnectToServer())
    {
        controller->MainLoop();
        controller->Terminate();
    }
}
