#pragma once
//------------------------------------------------------------------------------
/**
	Vulkan implementation of the render device.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "coregraphics/base/renderdevicebase.h"
#include "vkcmdbufferthread.h"
#include "vkdeferredcommand.h"
namespace Vulkan
{
class VkRenderDevice : public Base::RenderDeviceBase
{
	__DeclareClass(VkRenderDevice);
	__DeclareSingleton(VkRenderDevice);
public:

	/// constructor
	VkRenderDevice();
	/// destructor
	virtual ~VkRenderDevice();

	/// open the device
	bool Open();
	/// close the device
	void Close();

	/// begin complete frame
	bool BeginFrame(IndexT frameIndex);
	/// set the current vertex stream source
	void SetStreamVertexBuffer(IndexT streamIndex, const Ptr<CoreGraphics::VertexBuffer>& vb, IndexT offsetVertexIndex);
	/// set current vertex layout
	void SetVertexLayout(const Ptr<CoreGraphics::VertexLayout>& vl);
	/// set current index buffer
	void SetIndexBuffer(const Ptr<CoreGraphics::IndexBuffer>& ib);
	/// set the primitive group
	void SetPrimitiveGroup(const CoreGraphics::PrimitiveGroup& pg);
	/// perform computation
	void Compute(int dimX, int dimY, int dimZ, uint flag = NoBarrier); // use MemoryBarrierFlag
	/// begins pass with single rendertarget
	void BeginPass(const Ptr<CoreGraphics::RenderTarget>& rt, const Ptr<CoreGraphics::Shader>& passShader);
	/// begins pass with multiple rendertarget
	void BeginPass(const Ptr<CoreGraphics::MultipleRenderTarget>& mrt, const Ptr<CoreGraphics::Shader>& passShader);
	/// begins pass with rendertarget cube
	void BeginPass(const Ptr<CoreGraphics::RenderTargetCube>& rtc, const Ptr<CoreGraphics::Shader>& passShader);
	/// begin rendering a transform feedback with a vertex buffer as target
	void BeginFeedback(const Ptr<CoreGraphics::FeedbackBuffer>& fb, CoreGraphics::PrimitiveTopology::Code primType, const Ptr<CoreGraphics::Shader>& passShader);
	/// begin batch
	void BeginBatch(CoreGraphics::FrameBatchType::Code batchType);
	/// bake the current state of the render device (only used on DX12 and Vulkan renderers where pipeline creation is required)
	void BuildRenderPipeline();
	/// draw current primitives
	void Draw();
	/// draw indexed, instanced primitives (see method header for details)
	void DrawIndexedInstanced(SizeT numInstances, IndexT baseInstance);
	/// draw from stream output/transform feedback buffer
	void DrawFeedback(const Ptr<CoreGraphics::FeedbackBuffer>& fb);
	/// draw from stream output/transform feedback buffer, instanced
	void DrawFeedbackInstanced(const Ptr<CoreGraphics::FeedbackBuffer>& fb, SizeT numInstances);
	/// end batch
	void EndBatch();
	/// end current pass
	void EndPass();
	/// end current feedback
	void EndFeedback();
	/// end complete frame
	void EndFrame(IndexT frameIndex);
	/// present the rendered scene
	void Present();
	/// adds a scissor rect
	void SetScissorRect(const Math::rectangle<int>& rect, int index);
	/// sets viewport
	void SetViewport(const Math::rectangle<int>& rect, int index);

	/// save a screenshot to the provided stream
	CoreGraphics::ImageFileFormat::Code SaveScreenshot(CoreGraphics::ImageFileFormat::Code fmt, const Ptr<IO::Stream>& outStream);
	/// save a region of the screen to the provided stream
	CoreGraphics::ImageFileFormat::Code SaveScreenshot(CoreGraphics::ImageFileFormat::Code fmt, const Ptr<IO::Stream>& outStream, const Math::rectangle<int>& rect, int x, int y);

	/// call when window gets resized
	void DisplayResized(SizeT width, SizeT height);

	static const short MaxNumRenderTargets = 8;
	static const short MaxNumViewports = 16;

private:
	friend class VkTexture;
	friend class VkShapeRenderer;
	friend class VkTextRenderer;
	friend class VkTransformDevice;
	friend class VkDisplayDevice;
	friend class VkVertexBuffer;
	friend class VkIndexBuffer;
	friend class VkCpuSyncFence;
	friend class VkShader;
	friend class VkShaderProgram;
	friend class VkMemoryVertexBufferLoader;
	friend class VkMemoryIndexBufferLoader;
	friend class VkMemoryTextureLoader;
	friend class VkShaderServer;
	friend class VkRenderTarget;
	friend class VkRenderTargetCube;
	friend class VkMultipleRenderTarget;
	friend class VkDepthStencilTarget;
	friend class VkVertexLayout;
	friend class VkUniformBuffer;
	friend class VkShaderStorageBuffer;
	friend class VkStreamTextureLoader;
	friend class VkStreamTextureSaver;
	friend struct VkDeferredCommand;

	enum PipelineInfoBits
	{
		ShaderInfoSet = 1,
		VertexLayoutInfoSet = 2,
		FramebufferLayoutInfoSet = 4,
		InputLayoutInfoSet = 8,

		AllInfoSet = 15,

		PipelineBuilt = 16
	};

	// open Vulkan device context
	bool OpenVulkanContext();
	/// close opengl4 device context
	void CloseVulkanDevice();
	/// setup the requested adapter for the Vulkan device
	void SetupAdapter();
	/// select the requested buffer formats for the Vulkan device
	void SetupBufferFormats();
	/// setup the remaining presentation parameters
	void SetupPresentParams();
	/// set the initial Vulkan device state
	void SetInitialDeviceState();
	/// sync with gpu
	void SyncGPU();

	/// sets the current shader pipeline information
	void SetShaderPipelineInfo(const VkGraphicsPipelineCreateInfo& shader, const Ptr<VkShaderProgram>& program);
	/// sets the current vertex layout information
	void SetVertexLayoutPipelineInfo(const VkPipelineVertexInputStateCreateInfo& vertexLayout);
	/// sets the current framebuffer layout information
	void SetFramebufferLayoutInfo(const VkGraphicsPipelineCreateInfo& framebufferLayout);
	/// sets the current primitive layout information
	void SetInputLayoutInfo(VkPipelineInputAssemblyStateCreateInfo* inputLayout);
	/// create a new pipeline (or fetch from cache) and bind to command queue
	void CreatePipeline();

	/// update descriptors
	void UpdateDescriptors(const Util::FixedArray<VkDescriptorSet>& descriptors, const VkPipelineLayout& layout, uint32_t baseSet, uint32_t setCount, uint32_t* offsets, uint32_t offsetCount);
	/// update push ranges
	void UpdatePushRanges(const VkShaderStageFlags& stages, const VkPipelineLayout& layout, uint32_t offset, uint32_t size, void* data);

	/// setup queue from display
	void SetupQueue(uint32_t queueFamily, uint32_t queueIndex);
	/// figure out which memory type fits given memory bits and required properties
	VkResult GetMemoryType(uint32_t bits, VkMemoryPropertyFlags flags, uint32_t& index);

	/// allocate a buffer memory storage
	void AllocateBufferMemory(const VkBuffer& buf, VkDeviceMemory& bufmem, VkMemoryPropertyFlagBits flags, uint32_t& bufsize);
	/// allocate an image memory storage
	void AllocateImageMemory(const VkImage& img, VkDeviceMemory& imgmem, VkMemoryPropertyFlagBits flags, uint32_t& imgsize);

	/// update buffer memory from CPU memory, if deleteWhenDone is true, then the render device will assume the data is safe to take ownership of. (MUST BE DONE BETWEEN BeginFrame/EndFrame)
	void BufferUpdate(const VkBuffer& buf, VkDeviceSize offset, VkDeviceSize size, uint32_t* data);
	/// update image memory from CPU memory. (MUST BE DONE BETWEEN BeginFrame/EndFrame)
	void ImageUpdate(const VkImage& img, VkBufferImageCopy copy, VkDeviceSize size, uint32_t* data);
	/// setup staging buffer update for later execution
	void PushBufferUpdate(const VkBuffer& buf, VkDeviceSize offset, VkDeviceSize size, uint32_t* data);
	/// setup staging image update for later execution
	void PushImageUpdate(const VkImage& img, VkBufferImageCopy copy, VkDeviceSize size, uint32_t* data);
	/// perform image read-back, and saves to buffer (SLOW!)
	void ReadImage(const VkImage& img, VkBufferImageCopy copy, uint32_t& outMemSize, VkDeviceMemory& outMem, VkBuffer& outBuffer);
	/// perform image write-back, transitions data from buffer to image (SLOW!)
	void WriteImage(const VkBuffer& buf, const VkImage& img, VkBufferImageCopy copy);
	/// helper to begin immediate transfer
	VkCommandBuffer BeginImmediateTransfer();
	/// helper to end immediate transfer
	void EndImmediateTransfer(VkCommandBuffer cmdBuf);
	/// helper to end subpass commands
	void EndSubpassCommands();
	/// push delegate to direct list
	void PushCommand(const VkDeferredCommand& del);
	/// push delegate before frame
	void PushCommandStaging(const VkDeferredCommand& del);

	/// push image layout change
	void PushImageLayoutTransition(VkDeferredCommand::CommandQueueType queue, VkImageMemoryBarrier barrier);
	/// perform image layout transition immediately
	void ImageLayoutTransition(VkDeferredCommand::CommandQueueType queue, VkImageMemoryBarrier barrier);
	/// create image barrier
	static VkImageMemoryBarrier ImageMemoryBarrier(const VkImage& img, VkImageSubresourceRange subres, VkImageLayout oldLayout, VkImageLayout newLayout);
	/// transition image between layouts
	void ChangeImageLayout(const VkImageMemoryBarrier& barrier, const VkDeferredCommand::CommandQueueType& type);

	/// push image color clear
	void PushImageColorClear(const VkImage& image, const VkDeferredCommand::CommandQueueType& queue, VkImageLayout layout, VkClearColorValue clearValue, VkImageSubresourceRange subres);
	/// perform image color clear
	void ImageColorClear(const VkImage& image, const VkDeferredCommand::CommandQueueType& queue, VkImageLayout layout, VkClearColorValue clearValue, VkImageSubresourceRange subres);
	/// push image depth stencil clear
	void PushImageDepthStencilClear(const VkImage& image, const VkDeferredCommand::CommandQueueType& queue, VkImageLayout layout, VkClearDepthStencilValue clearValue, VkImageSubresourceRange subres);
	/// perform image depth stencil clear
	void ImageDepthStencilClear(const VkImage& image, const VkDeferredCommand::CommandQueueType& queue, VkImageLayout layout, VkClearDepthStencilValue clearValue, VkImageSubresourceRange subres);

	/// helper function to submit a command buffer
	void SubmitToQueue(VkQueue queue, VkPipelineStageFlags flags, uint32_t numBuffers, VkCommandBuffer* buffers);
	/// helper function to submit a fence
	void SubmitToQueue(VkQueue queue, VkFence fence);
	/// wait for queue to finish execution using fence, also resets fence
	void WaitForFence(VkFence fence);

	/// wait for deferred delegates to complete
	void UpdateDelegates();
	/// begin using the worker threads to build command buffers
	void BeginCmdThreads();
	/// end using the worker threads
	void EndCmdThreads();

	uint32_t adapter;
	uint32_t frameId;
	VkPhysicalDeviceMemoryProperties memoryProps;

	const uint32_t VkPoolMaxSets = 1024;
	const uint32_t VkPoolSetSize = 65535;

	VkPhysicalDevice devices[64];

	VkExtensionProperties physicalExtensions[64];

	uint32_t usedPhysicalExtensions;
	const char* deviceExtensionStrings[64];

	uint32_t usedExtensions;
	const char* extensions[64];

	uint32_t numQueues;
	VkQueueFamilyProperties queuesProps[64];

	uint32_t renderQueueFamily;
	uint32_t computeQueueFamily;
	uint32_t transferQueueFamily;
	uint32_t renderQueueIdx;
	uint32_t computeQueueIdx;
	uint32_t transferQueueIdx;

	// stuff used for the swap chain
	VkFormat format;
	VkColorSpaceKHR colorSpace;
	VkSwapchainKHR swapchain;

	uint32_t currentBackbuffer;
	Util::FixedArray<VkImage> backbuffers;
	Util::FixedArray<VkDeviceMemory> backbufferMem;
	Util::FixedArray<VkImageView> backbufferViews;
	uint32_t numBackbuffers;
	VkSemaphore displaySemaphore;
	VkRect2D displayRect;

	static VkDevice dev;
	static VkDescriptorPool descPool;
	static VkQueue renderQueue;
	static VkQueue computeQueue;
	static VkQueue transferQueue;
	static VkInstance instance;
	static VkPhysicalDevice physicalDev;
	static VkPipelineCache cache;
	static VkCommandBuffer mainCmdGfxBuffer;
	static VkCommandBuffer mainCmdCmpBuffer;
	static VkCommandBuffer mainCmdTransBuffer;
	VkFence mainCmdGfxFence;
	VkFence mainCmdCmpFence;
	VkFence mainCmdTransFence;

	VkCommandBufferInheritanceInfo passInfo;
	VkPipelineInputAssemblyStateCreateInfo inputInfo;
	VkPipelineColorBlendStateCreateInfo blendInfo;

	static const SizeT NumDrawThreads = 4;

	IndexT currentDrawThread;
	VkCommandPool dispatchableCmdDrawBufferPool[NumDrawThreads];
	VkCommandBuffer dispatchableDrawCmdBuffers[NumDrawThreads];
	Ptr<VkCmdBufferThread> drawThreads[NumDrawThreads];
	Threading::Event drawCompletionEvent[NumDrawThreads];

	static const SizeT NumTransferThreads = 1;
	IndexT currentTransThread;
	VkCommandPool dispatchableCmdTransBufferPool[NumTransferThreads];
	VkCommandBuffer dispatchableTransCmdBuffers[NumTransferThreads];
	Ptr<VkCmdBufferThread> transThreads[NumTransferThreads];
	Threading::Event transCompletionEvent[NumTransferThreads];

	static const SizeT NumComputeThreads = 4;
	IndexT currentComputeThread;
	VkCommandPool dispatchableCmdCompBufferPool[NumComputeThreads];
	VkCommandBuffer dispatchableCompCmdBuffers[NumComputeThreads];
	Ptr<VkCmdBufferThread> compThreads[NumComputeThreads];
	Threading::Event compCompletionEvent[NumComputeThreads];

	static const SizeT NumDeferredDelegates = 128;
	uint32_t currentDeferredDelegate;
	VkFence deferredDelegateFences[NumDeferredDelegates];
	Util::Queue<IndexT> freeFences;
	Util::Queue<IndexT> usedFences;

	IndexT delegateBucketIndex;
	Util::FixedArray<Util::Array<VkDeferredCommand>> fenceDelegateBuckets;
	Util::Array<VkDeferredCommand> nextFrameFenceDelegates;
	Util::Array<VkDeferredCommand> nextFrameDelegates;

	VkPipelineVertexInputStateCreateInfo vertexInfo;
	VkViewport* passViewports;
	uint32_t numVsInputs;

	// first pool is for persistent buffers, second is for transient
	static VkCommandPool mainCmdGfxPool;
	static VkCommandPool mainCmdCmpPool;
	static VkCommandPool mainCmdTransPool;
	
	VkCommandPool immediateCmdTransPool;

	VkGraphicsPipelineCreateInfo currentPipelineInfo;
	VkPipeline currentPipeline;
	uint currentPipelineBits;

	Ptr<VkShaderProgram> currentProgram;

#if NEBULAT_VULKAN_DEBUG
	VkDebugReportCallbackEXT debugCallbackHandle;
	PFN_vkCreateDebugReportCallbackEXT debugCallbackPtr;
#endif
};

} // namespace Vulkan