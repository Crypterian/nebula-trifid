#pragma once
//------------------------------------------------------------------------------
/**
	@class Base::ShaderReadWriteBufferBase
	
	A shader buffer represents a read/write buffer assignable inside a shader.

	Same as for ShaderReadWriteTextureBase, this class must be unlocked prior to updating, 
	and locked prior to being used by the GPU.
	
	(C) 2012-2014 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
namespace Base
{
class ShaderReadWriteBufferBase : public Core::RefCounted
{
	__DeclareClass(ShaderReadWriteBufferBase);
public:
	/// constructor
	ShaderReadWriteBufferBase();
	/// destructor
	virtual ~ShaderReadWriteBufferBase();

	/// setup buffer
	void Setup(const SizeT numBackingBuffers = DefaultNumBackingBuffers);
	/// discard buffer
	void Discard();

	/// set the size of the buffer
    void SetSize(const uint size);
	/// get the size of the buffer
    const uint GetSize() const;

	/// returns buffer handle
	void* GetHandle() const;

	/// update buffer
    void Update(void* data, uint offset, uint size);
    /// cycle to next buffer
    void CycleBuffers();

	/// locks buffer, blocking any GPU calls from working on it
	void Lock();
	/// unlocks buffer, allowing the buffer to be updated
	void Unlock();

	static const SizeT DefaultNumBackingBuffers = 3;

protected:
    bool isSetup;
	uint lockSemaphore;
    uint size;
    IndexT bufferIndex;
	SizeT numBuffers;
};

//------------------------------------------------------------------------------
/**
*/
inline void
ShaderReadWriteBufferBase::SetSize(const uint size)
{
	n_assert(!this->isSetup);
	this->size = size;
}

//------------------------------------------------------------------------------
/**
*/
inline const uint
ShaderReadWriteBufferBase::GetSize() const
{
	return this->size;
}

//------------------------------------------------------------------------------
/**
*/
inline void*
ShaderReadWriteBufferBase::GetHandle() const
{
	return 0;
}

} // namespace Base