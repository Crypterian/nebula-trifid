#pragma once
//------------------------------------------------------------------------------
/**
	Vulkan implementation of the render device.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "coregraphics/base/renderdevicebase.h"
#include "vkcmdbufferthread.h"
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
	void SetStreamSource(IndexT streamIndex, const Ptr<CoreGraphics::VertexBuffer>& vb, IndexT offsetVertexIndex);
	/// set current vertex layout
	void SetVertexLayout(const Ptr<CoreGraphics::VertexLayout>& vl);
	/// set current index buffer
	void SetIndexBuffer(const Ptr<CoreGraphics::IndexBuffer>& ib);
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
	/// adds a render target
	void AddRenderTarget(const GLuint& renderTarget) const;
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
	friend class VkVertexLayout;

	enum PipelineInfoBits
	{
		ShaderInfoSet = 1,
		VertexLayoutInfoSet = 2,
		FramebufferLayoutInfoSet = 4,

		AllInfoSet = 7
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
	void SetShaderPipelineInfo(const VkGraphicsPipelineCreateInfo& shader);
	/// sets the current vertex layout information
	void SetVertexLayoutPipelineInfo(const VkGraphicsPipelineCreateInfo& vertexLayout);
	/// sets the current framebuffer layout information
	void SetFramebufferLayoutInfo(const VkGraphicsPipelineCreateInfo& framebufferLayout);
	/// create a new pipeline (or fetch from cache) and bind to command queue
	void CreatePipeline();

	/// update descriptors
	void UpdateDescriptors(const Util::FixedArray<VkDescriptorSet>& descriptors, const VkPipelineLayout& layout, uint32_t baseSet, uint32_t setCount, uint32_t* offsets, uint32_t offsetCount);
	/// update push ranges
	void UpdatePushRanges(const VkShaderStageFlags& stages, uint32_t offset, uint32_t size, void* data);

	/// setup queue from display
	void SetupQueue(uint32_t queueFamily, uint32_t queueIndex);
	/// figure out which memory type fits given memory bits and required properties
	VkResult GetMemoryType(uint32_t bits, VkMemoryPropertyFlags flags, uint32_t& index);

	/// allocate a buffer memory storage
	void AllocateBufferMemory(const VkBuffer& buf, VkDeviceMemory& bufmem, VkMemoryPropertyFlagBits flags, uint32_t& bufsize);
	/// allocate an image memory storage
	void AllocateImageMemory(const VkImage& img, VkDeviceMemory& imgmem, VkMemoryPropertyFlagBits flags, uint32_t& imgsize);

	uint32_t adapter;
	uint32_t frameId;

	const uint32_t VkPoolMaxSets = 1024;
	const uint32_t VkPoolSetSize = 64;

	static VkDevice dev;
	static VkQueue displayQueue;
	static VkInstance instance;
	static VkPhysicalDevice physicalDev;
	static VkPipelineCache cache;
	static VkDescriptorPool descPool;
	static VkCommandBuffer mainCmdGfxBuffer;
	static VkCommandBuffer mainCmdCmpBuffer;
	static VkCommandBuffer mainCmdTransBuffer;

	static const SizeT NumThreads = 4;

	IndexT currentThread;
	VkCommandBuffer dispatchableCmdBuffers[NumThreads];
	Ptr<VkCmdBufferThread> threads[NumThreads];

	enum CmdCreationUsage
	{
		Persistent,
		Transient,

		NumCmdCreationUsages
	};

	// first pool is for persistent buffers, second is for transient
	static VkCommandPool cmdGfxPool[NumCmdCreationUsages];
	static VkCommandPool cmdCmpPool[NumCmdCreationUsages];		
	static VkCommandPool cmdTransPool[NumCmdCreationUsages];

	VkGraphicsPipelineCreateInfo currentPipelineInfo;
	VkPipeline currentPipeline;
	uint currentPipelineBits;

	uint32_t usedExtensions;
	const char* extensions[64];

	uint32_t usedPhysicalExtensions;
	const char* physicalExtensions[64];

#if NEBULAT_VULKAN_DEBUG
	PFN_vkCreateDebugReportCallbackEXT debugCallback;
	VkDebugReportCallbackEXT debugCallbackHandle;
#endif

	PFN_vkCreateSwapchainKHR swapChainCtor;
	PFN_vkDestroySwapchainKHR swapChainDtor;
	PFN_vkAcquireNextImageKHR swapChainNextImage;
	PFN_vkQueuePresentKHR swapChainPresent;

	uint32_t numQueues;
	VkQueueFamilyProperties* queuesProps;
	VkPhysicalDeviceMemoryProperties memoryProps;
	uint32_t renderQueueIdx;
	uint32_t computeQueueIdx;
	uint32_t transferQueueIdx;

	// stuff used for the swap chain
	VkFormat format;
	VkColorSpaceKHR colorSpace;

	PFN_vkGetPhysicalDeviceSurfaceSupportKHR surfaceSupport;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR surfaceFormats;
};
} // namespace Vulkan