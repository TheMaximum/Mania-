#include "ManiaPP.h"

int main(int argc, char *argv[])
{
    ManiaPP* controller = new ManiaPP();
    controller->ConnectToServer();
    controller->Terminate();
}
