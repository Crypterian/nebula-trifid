//------------------------------------------------------------------------------
// framefullscreeneffect.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "framesubpassfullscreeneffect.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"

using namespace CoreGraphics;
namespace Frame2
{

__ImplementClass(Frame2::FrameSubpassFullscreenEffect, 'FFLE', Frame2::FrameOp);
//------------------------------------------------------------------------------
/**
*/
FrameSubpassFullscreenEffect::FrameSubpassFullscreenEffect()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
FrameSubpassFullscreenEffect::~FrameSubpassFullscreenEffect()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
FrameSubpassFullscreenEffect::Setup()
{
	this->fsq.Setup(this->tex->GetWidth(), this->tex->GetHeight());
}

//------------------------------------------------------------------------------
/**
*/
void
FrameSubpassFullscreenEffect::Run(const IndexT frameIndex)
{
	RenderDevice* renderDevice = RenderDevice::Instance();
	ShaderServer* shaderServer = ShaderServer::Instance();

	// activate shader
	shaderServer->SetActiveShader(this->shaderState->GetShader());
	this->shaderState->Apply();

	// draw
	this->shaderState->Commit();
	this->fsq.Draw();
}

} // namespace Frame2