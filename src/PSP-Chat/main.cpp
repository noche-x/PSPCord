#include <Platform/Platform.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspmath.h>
#include <iostream>

PSP_MODULE_INFO("PSP-Chat", 0, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

#define MC_PSP
#include <Utilities/Timer.h>
#include <Utilities/Logger.h>
#include <Utilities/Input.h>
#include <Graphics/Dialogs.h>
#include <Graphics/RendererCore.h>
#include <Network/NetworkDriver.h>

using namespace Stardust;

#define pspprint pspDebugScreenPrintf

enum PacketIDS
{
    NONE = 0,
    SEND_MESSAGE_PACKET = 302,
    RECV_MESSAGE_PACKET = 303,
};

Network::PacketOut *sendMessagePacket(std::string username, std::string message)
{
    Network::PacketOut *p = new Network::PacketOut();
    p->ID = PacketIDS::SEND_MESSAGE_PACKET;
    Network::encodeString(username, *p);
    Network::encodeString(message, *p);
    return p;
}

int handler(Network::PacketIn *packet)
{
    std::string username = Network::decodeString(*packet);
    std::string message = Network::decodeString(*packet);
    Utilities::app_Logger->log(username);
    Utilities::app_Logger->log(message);

    return 0;
}

int main()
{
    // stardust init
    Platform::initPlatform("psp-chat");
    Graphics::g_RenderCore.SetFontStyle(16.f, 0xFFFFFFFF, 0x000000FF, 0, 0);

    //Enable low-level logger
    Utilities::app_Logger->currentLevel = Utilities::LOGGER_LEVEL_TRACE;
    Utilities::detail::core_Logger->currentLevel = Utilities::LOGGER_LEVEL_TRACE;

    FILE *pFile;
    char buffer[100];

    pFile = fopen("ip.txt", "r");
    if (pFile == NULL)
        Utilities::app_Logger->log("Error opening file");
    else
    {
        while (!feof(pFile))
        {
            if (fgets(buffer, 100, pFile) == NULL)
                break;
            fputs(buffer, stdout);
        }
        fclose(pFile);
    }

    // test log
    Utilities::app_Logger->log("Hello World!");

    // get the keys that are pressed
    Utilities::updateInputs();

    // network init
    if (!Network::g_NetworkDriver.Init())
        Graphics::ShowMessageError("failed to initalize the network driver", 1);

    std::string username = "";
    unsigned short outtext[128];
    //unsigned short *desc = (unsigned short *)strtol("username", NULL, 0);
    unsigned short desc[] = {
        'u',
        's',
        'e',
        'r',
        'n',
        'a',
        'm',
        'e',
    };
    while (true)
    {
        Graphics::g_RenderCore.BeginCommands(true);
        Graphics::g_RenderCore.SetClearColor(104, 79, 137, 255);
        Graphics::g_RenderCore.Clear();
        Graphics::g_RenderCore;
        if (Graphics::ShowOSK(desc, outtext, 128) != -1)
        {
            for (int j = 0; outtext[j]; j++)
            {
                unsigned c = outtext[j];

                if (32 <= c && c <= 127) // print ascii only
                    username += c;
            }
            break;
        }
        else
        {
            if (Graphics::ShowMessageYesNo("are you sure you want to exit") == 1)
                sceKernelExitGame();
        }

        Graphics::g_RenderCore.EndCommands(true);
    }

    std::string message = "do you want " + username + " as your username?";
    while (true)
    {
        Graphics::g_RenderCore.DebugPrint(10, 10, "bitch ass text");
        Graphics::g_RenderCore.BeginCommands(true);
        //bgra
        //rgba
        Graphics::g_RenderCore.SetClearColor(137, 79, 104, 255);
        Graphics::g_RenderCore.Clear();
        auto val = Graphics::ShowMessageYesNo(message.c_str());
        if (val == -1)
        {
            username = "";
            if (Graphics::ShowOSK(desc, outtext, 128) != -1)
            {
                for (int j = 0; outtext[j]; j++)
                {
                    unsigned c = outtext[j];

                    if (32 <= c && c <= 127) // print ascii only
                        username += c;
                }
                message = "do you want " + username + " as your username?";
            }
            else
            {
                if (Graphics::ShowMessageYesNo("are you sure you want to exit") == 1)
                    sceKernelExitGame();
            }
        }
        else if (val == 1)
            break;

        Graphics::g_RenderCore.EndCommands(true);
    }
    
    Network::g_NetworkDriver.AddPacketHandler(PacketIDS::RECV_MESSAGE_PACKET, handler);
    
    while (1)
    {
        // connect to lan server
        Network::g_NetworkDriver.Connect(35700, buffer);
        // login packet creation

        std::string field = "";
        unsigned short outtext[128];
        //unsigned short *desc = (unsigned short *)strtoul("message", NULL, 0);
            unsigned short desc[] = {
        'm',
        'e',
        's',
        's',
        'a',
        'g',
        'e',
    };
        while (true)
        {
            Graphics::g_RenderCore.BeginCommands(true);
            Graphics::g_RenderCore.SetClearColor(89, 67, 77, 255);
            Graphics::g_RenderCore.Clear();
            if (Graphics::ShowOSK(desc, outtext, 128) != -1)
            {
                for (int j = 0; outtext[j]; j++)
                {
                    unsigned c = outtext[j];

                    if (32 <= c && c <= 127) // print ascii only
                        field += c;
                }
                break;
            }
            else
            {
                if (Graphics::ShowMessageYesNo("are you sure you want to exit") == 1)
                    sceKernelExitGame();
            }
            Graphics::g_RenderCore.EndCommands(true);
        }

        auto packet = sendMessagePacket(username, std::string(field));

        // add packet to the list
        Network::g_NetworkDriver.AddPacket(packet);
        // send the packet queue
        Network::g_NetworkDriver.SendPackets();

        //Network::g_NetworkDriver.AddPacketHandler(PacketIDS::RECV_MESSAGE_PACKET, handler);

        //Network::g_NetworkDriver.ReceivePacket();
        Network::g_NetworkDriver.HandlePackets();
    }

    Network::g_NetworkDriver.ClearPacketHandlers();
    Network::g_NetworkDriver.ClearPacketQueue();
    Network::g_NetworkDriver.Cleanup();

    Platform::exitPlatform();
    return 0;
}