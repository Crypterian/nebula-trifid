//------------------------------------------------------------------------------
//  glfwinputserver.cc
//  (C) 2013-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/glfw/glfwinputserver.h"
#include "coregraphics/displaydevice.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/gamepad.h"
#include "graphics/display.h"

namespace OpenGL4
{
__ImplementClass(OpenGL4::GLFWInputServer, 'GLIS', Base::InputServerBase);
__ImplementSingleton(OpenGL4::GLFWInputServer);

using namespace Input;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
GLFWInputServer::GLFWInputServer()     
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
GLFWInputServer::~GLFWInputServer()
{
    if (this->IsOpen())
    {
        this->Close();
    }
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**    
*/
void
GLFWInputServer::Open()
{
    n_assert(!this->IsOpen());
    InputServerBase::Open();

    // create a default keyboard and mouse handler
    this->defaultKeyboard = Keyboard::Create();
    this->AttachInputHandler(InputPriority::Game, this->defaultKeyboard.upcast<InputHandler>());
    this->defaultMouse = Mouse::Create();
    this->AttachInputHandler(InputPriority::Game, this->defaultMouse.upcast<InputHandler>());

    // create 4 default gamepads (none of them have to be connected)
    IndexT playerIndex;
    for (playerIndex = 0; playerIndex < this->maxNumLocalPlayers; playerIndex++)
    {
        this->defaultGamePad[playerIndex] = GamePad::Create();
        this->defaultGamePad[playerIndex]->SetPlayerIndex(playerIndex);
        this->AttachInputHandler(InputPriority::Game, this->defaultGamePad[playerIndex].upcast<InputHandler>());
    }
}

//------------------------------------------------------------------------------
/**    
*/
void
GLFWInputServer::Close()
{
    n_assert(this->IsOpen());

    // call parent class
    InputServerBase::Close();
}

//------------------------------------------------------------------------------
/**    
*/
void
GLFWInputServer::OnFrame()
{
    //this->eventHandler->HandlePendingEvents();    
    InputServerBase::OnFrame();
}

} // namespace Win32