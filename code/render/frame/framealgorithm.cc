//------------------------------------------------------------------------------
//  framealgorithm.cc
//  (C) 2013 gscept
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "framealgorithm.h"
#include "algorithm/attic/algorithmbase.h"

using namespace Core;
namespace Frame
{
__ImplementClass(Frame::FrameAlgorithm, 'FRAL', Core::RefCounted);

//------------------------------------------------------------------------------
/**
*/
FrameAlgorithm::FrameAlgorithm()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
FrameAlgorithm::~FrameAlgorithm()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
FrameAlgorithm::Discard()
{
	this->algorithm->Discard();
	this->algorithm = 0;
}

//------------------------------------------------------------------------------
/**
*/
void 
FrameAlgorithm::Begin()
{
	this->algorithm->Begin();
}

//------------------------------------------------------------------------------
/**
*/
void 
FrameAlgorithm::Render(IndexT frameIndex)
{
	this->algorithm->Render();
}

//------------------------------------------------------------------------------
/**
*/
void 
FrameAlgorithm::End()
{
	this->algorithm->End();
}

//------------------------------------------------------------------------------
/**
*/
void 
FrameAlgorithm::OnDisplayResize(SizeT width, SizeT height)
{
	this->algorithm->OnDisplayResized(width, height);
}

} // namespace Frame