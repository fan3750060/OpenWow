#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freetype28.lib")
#pragma comment(lib, "libmpq.lib")
#pragma comment(lib, "glfw3.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

// CORE

#include <owIntfs.h>

#include <owCore.h>
#pragma comment(lib, "owCore.lib")

#include <owRender.h>
#pragma comment(lib, "owRender.lib")

#include <owGUI.h>
#pragma comment(lib, "owGUI.lib")

#include <owEngine.h>
#pragma comment(lib, "owEngine.lib")

// GAME

#include <owGameDBC.h>
#pragma comment(lib, "owGameDBC.lib")

#include <owGame.h>
#pragma comment(lib, "owGame.lib")

#include <owGameM2.h>
#pragma comment(lib, "owGameM2.lib")

#include <owGameWMO.h>
#pragma comment(lib, "owGameWMO.lib")



#include <owSocket.h>
#pragma comment(lib, "owSocket.lib")

#include <owClient.h>
#pragma comment(lib, "owClient.lib")