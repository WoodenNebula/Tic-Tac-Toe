#pragma once

#include "Networking/Sockets/Sockets.h"
#include "Networking/NetworkSubsystem.h"

namespace Engine
{
struct SAppCmdLineArgs
{
    Networking::ENetworkRole NetworkRoomState;
    Sockets::SAddress SockAddress;

    static void PrintUsageAndExit(int rc = 1);
    static SAppCmdLineArgs ParseArgs(int argc, char** argv);
};
}
