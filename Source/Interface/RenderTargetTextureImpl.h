#pragma once

#include "Core/SourceCommon.h"

#include "Interface/RenderTargetTexture.h"

#include "Types/BlurType.h"

#include "System/CommonTools.h"
#include "System/ShaderInterface.h"
#include "System/MeshBuffer.h"
#include "System/RenderTargetTextureDependecyGraphInfo.hpp"
#include "System/DescriptorSet.h"
#include "System/VulkanMemoryManagement.h"

#include "Interface/InstanceImpl.h"

#include "Interface/SamplerImpl.h"

#include "Interface/Texture.h"
#include "Interface/TextureImpl.h"




namespace vk2d {

class Mesh;

namespace _internal {

class InstanceImpl;
class MeshBuffer;
class RenderTargetTextureImpl;



/// @brief		Used to select the implementation. Each works a bit differently from the others.
///				We use 2 to 4 Vulkan images depending on the implementation used. "Attachment" is the first
///				render, it alone can have multisample enabled, "Sampled" is the final product of this
///				pipeline, it alone can have multiple mip maps, Buffer1 and Buffer2 are used as needed
///				as intermediate render target images, those must always be 1 sample and 1 mipmap. <br>
///				Depending on the type of render target texture used, different rendering paths are used
///				with different amount of images.
///				<table>
///				<caption>Images</caption>
///				<tr><th> Name					<th> Description
///				<tr><td> Attachment				<td> Render attachment image where everything is rendered by the main pass.
///				<tr><td> Sampled				<td> Image that's presentable in a shader. The part that can be used as a texture.
///				<tr><td> Buffer 1				<td> Intermediate image that's used to temporarily store image data for further processing.
///				<tr><td> Buffer 2				<td> Same as Buffer 1.
///				</table>
///				<br>
///				<table>
///				<caption>Finalization pipeline</caption>
///				<tr><th> No multisample, No blur	<th> With multisample, No blur	<th> No multisample, With blur	<th> With multisample, With blur
///				<tr><td> (Attachment)				<td> (Attachment)				<td> (Attachment)				<td> (Attachment)
///				<tr><td><b> Generate mip maps		<td> Resolve multisamples		<td> Blur pass 1				<td> Resolve multisamples	</b>
///				<tr><td> (Sampled)					<td> (Buffer 1)					<td> (Buffer 1)					<td> (Buffer 1)
///				<tr><td><b>							<td> Generate mip maps			<td> Blur pass 2				<td> Blur pass 1			</b>
///				<tr><td>							<td> (Sampled)					<td> (Attachment)				<td> (Buffer 2)
///				<tr><td><b>							<td>							<td> Generate mip maps			<td> Blur pass 2			</b>
///				<tr><td>							<td>							<td> (Sampled)					<td> (Buffer 1)
///				<tr><td><b>							<td>							<td>							<td> Generate mip maps		</b>
///				<tr><td>							<td>							<td>							<td> (Sampled)
///				</table>
enum class RenderTargetTextureType
{
	NONE						= 0,	///< Not a type, used for error detection.
	DIRECT,								///< No multisample, No blur
	WITH_MULTISAMPLE,					///< With multisample, No blur
	WITH_BLUR,							///< No multisample, With blur
	WITH_MULTISAMPLE_AND_BLUR,			///< With multisample, with blur
};



// Render target implementation
class RenderTargetTextureImpl :
	public vk2d::_internal::TextureImpl
{
private:
	struct SwapBuffer
	{
		vk2d::_internal::CompleteImageResource							attachment_image							= {};	// Render attachment, Multisampled, 1 mip level
		vk2d::_internal::CompleteImageResource							buffer_1_image								= {};	// Buffer image, used as multisample resolve and blur buffer
		vk2d::_internal::CompleteImageResource							buffer_2_image								= {};	// Buffer image, used as second blur buffer
		vk2d::_internal::CompleteImageResource							sampled_image								= {};	// Output, sampled image with mip mapping
		VkFramebuffer													vk_render_framebuffer						= {};	// Framebuffer for the main render
		VkFramebuffer													vk_blur_framebuffer_1						= {};	// Framebuffer for blur pass 1
		VkFramebuffer													vk_blur_framebuffer_2						= {};	// Framebuffer for blur pass 2

		VkCommandBuffer													vk_transfer_command_buffer					= {};	// Data transfer command buffer, this transfers vertex, index, etc... data in the primary render queue.
		VkCommandBuffer													vk_render_command_buffer					= {};	// Primary render, if no blur is used then also embeds mipmap generation.

		VkSubmitInfo													vk_transfer_submit_info						= {};
		VkSubmitInfo													vk_render_submit_info						= {};

		VkTimelineSemaphoreSubmitInfo									vk_render_timeline_semaphore_submit_info	= {};

		std::vector<VkSemaphore>										render_wait_for_semaphores;
		std::vector<uint64_t>											render_wait_for_semaphore_timeline_values;			// Used with render_wait_for_semaphores.
		std::vector<VkPipelineStageFlags>								render_wait_for_pipeline_stages;

		VkSemaphore														vk_transfer_complete_semaphore				= {};	// Binary
		VkSemaphore														vk_render_complete_semaphore				= {};	// Binary if blur enabled, Timeline if blur enabled.

		uint64_t														render_counter								= {};	// Used with the vk_render_complete_semaphore to determine value to wait for.

		std::vector<vk2d::_internal::RenderTargetTextureDependencyInfo>	render_target_texture_dependencies			= {};

		uint32_t														render_commitment_request_count				= {};
		//std::mutex													render_commitment_request_mutex				= {};

		bool															has_been_submitted							= {};
		bool															contains_non_pending_sampled_image			= {};	// Sampled image ready to be used anywhere without checks or barriers.
	};

public:
	RenderTargetTextureImpl(
		vk2d::RenderTargetTexture									*	my_interface,
		vk2d::_internal::InstanceImpl								*	instance,
		const vk2d::RenderTargetTextureCreateInfo					&	create_info );

	~RenderTargetTextureImpl();

	void																SetSize(
		vk2d::Vector2u													new_size );
	vk2d::Vector2u														GetSize() const;
	uint32_t															GetLayerCount() const;

	uint32_t															GetCurrentSwapBuffer() const;
	VkImage																GetVulkanImage() const;
	VkImageView															GetVulkanImageView() const;
	VkImageLayout														GetVulkanImageLayout() const;
	VkFramebuffer														GetFramebuffer(
		vk2d::_internal::RenderTargetTextureDependencyInfo			&	dependency_info ) const;
	VkSemaphore															GetAllCompleteSemaphore(
		vk2d::_internal::RenderTargetTextureDependencyInfo			&	dependency_info ) const;

	uint64_t															GetRenderCounter(
		vk2d::_internal::RenderTargetTextureDependencyInfo			&	dependency_info ) const;

	bool																IsTextureDataReady();

	// Begins the render operations. You must call this before using any drawing commands.
	// For best performance you should calculate game logic first, when you're ready to draw
	// call this function just before your first draw command.
	bool																BeginRender();

	// Ends the rendering operations. You must call this after you're done drawing.
	// This will display the results on screen.
	bool																EndRender(
		vk2d::BlurType													blur_type,
		vk2d::Vector2f													blur_amount );

	bool																SynchronizeFrame();
	bool																WaitIdle();

	// Should be called once render is definitely going to happen. When this is called,
	// SynchronizeFrame() will start blocking until the the contents of the
	// RenderTargerTexture have been fully rendered. BeginRender() can be called however,
	// it will swap the buffers so 2 renders can be queued, however third call to
	// BeginRender() will be blocked until the first BeginRender() call has been rendered.
	// TODO: Figure out how to best track render target texture commitments, a render target can be re-used in multiple places but should only be rendered once while at the same time submissions should be grouped together.
	bool																CommitRenderTargetTextureRender(
		vk2d::_internal::RenderTargetTextureDependencyInfo			&	dependency_info,
		vk2d::_internal::RenderTargetTextureRenderCollector			&	collector );

	// This notifies that the render target texture has been submitted to rendering.
	void																ConfirmRenderTargetTextureRenderSubmission(
		vk2d::_internal::RenderTargetTextureDependencyInfo			&	dependency_info );

	// This notifies that the render target texture has been successfully rendered.
	void																ConfirmRenderTargetTextureRenderFinished(
		vk2d::_internal::RenderTargetTextureDependencyInfo			&	dependency_info );

	// In case something goes wrong, allows cancelling render commission.
	void																AbortRenderTargetTextureRender(
		vk2d::_internal::RenderTargetTextureDependencyInfo			&	dependency_info );

	void																ResetRenderTargetTextureRenderDependencies(
		uint32_t														swap_buffer_index );

	// Add child dependency, child render targets must render before this.
	void																CheckAndAddRenderTargetTextureDependency(
		uint32_t														swap_buffer_index,
		vk2d::Texture												*	texture );

	vk2d::_internal::RenderTargetTextureDependencyInfo					GetDependencyInfo();

	void																DrawTriangleList(
		const std::vector<vk2d::VertexIndex_3>						&	indices,
		const std::vector<vk2d::Vertex>								&	vertices,
		const std::vector<float>									&	texture_channel_weights,
		const std::vector<vk2d::Matrix4f>							&	transformations,
		bool															filled,
		vk2d::Texture												*	texture,
		vk2d::Sampler												*	sampler );

	void																DrawTriangleList(
		const std::vector<uint32_t>									&	raw_indices,
		const std::vector<vk2d::Vertex>								&	vertices,
		const std::vector<float>									&	texture_channel_weights,
		const std::vector<vk2d::Matrix4f>							&	transformations,
		bool															filled,
		vk2d::Texture												*	texture,
		vk2d::Sampler												*	sampler );

	void																DrawLineList(
		const std::vector<vk2d::VertexIndex_2>						&	indices,
		const std::vector<vk2d::Vertex>								&	vertices,
		const std::vector<float>									&	texture_channel_weights,
		const std::vector<vk2d::Matrix4f>							&	transformations,
		vk2d::Texture												*	texture,
		vk2d::Sampler												*	sampler,
		float															line_width );

	void																DrawLineList(
		const std::vector<uint32_t>									&	raw_indices,
		const std::vector<vk2d::Vertex>								&	vertices,
		const std::vector<float>									&	texture_channel_weights,
		const std::vector<vk2d::Matrix4f>							&	transformations,
		vk2d::Texture												*	texture,
		vk2d::Sampler												*	sampler,
		float															line_width );

	void																DrawPointList(
		const std::vector<vk2d::Vertex>								&	vertices,
		const std::vector<float>									&	texture_channel_weights,
		const std::vector<vk2d::Matrix4f>							&	transformations,
		vk2d::Texture												*	texture,
		vk2d::Sampler												*	sampler );

	void																DrawMesh(
		const vk2d::Mesh											&	mesh,
		const std::vector<vk2d::Matrix4f>							&	transformations );

	bool																IsGood() const;

private:
	bool																DetermineType();

	bool																CreateCommandBuffers();
	void																DestroyCommandBuffers();

	bool																CreateFrameDataBuffers();
	void																DestroyFrameDataBuffers();

	bool																CreateImages(
		vk2d::Vector2u													new_size );
	void																DestroyImages();

	bool																CreateRenderPasses();
	void																DestroyRenderPasses();

	bool																CreateFramebuffers();
	void																DestroyFramebuffers();

	bool																CreateSynchronizationPrimitives();
	void																DestroySynchronizationPrimitives();

	bool																RecordTransferCommandBuffer(
		vk2d::_internal::RenderTargetTextureImpl::SwapBuffer		&	swap );

	bool																UpdateSubmitInfos(
		vk2d::_internal::RenderTargetTextureImpl::SwapBuffer		&	swap,
		const std::vector<VkSemaphore>								&	wait_for_semaphores,
		const std::vector<uint64_t>									&	wait_for_semaphore_timeline_values,
		const std::vector<VkPipelineStageFlags>						&	wait_for_semaphore_pipeline_stages );

	vk2d::_internal::TimedDescriptorPoolData						&	GetOrCreateDescriptorSetForSampler(
		vk2d::Sampler												*	sampler );

	vk2d::_internal::TimedDescriptorPoolData						&	GetOrCreateDescriptorSetForTexture(
		vk2d::Texture												*	texture );

	void																CmdPushBlurTextureDescriptorWritesDirectly(
		VkCommandBuffer													command_buffer,
		VkPipelineLayout												use_pipeline_layout,
		uint32_t														set_index,
		VkImageView														source_image,
		VkImageLayout													source_image_layout );

	/// @brief		Record commands to finalize render into the sampled image.
	///				This includes resolving multisamples, blur, mipmap generation and storing the result
	///				into sampled image to be used later as a texture.
	///				<br>
	///				Finalization goes through multiple stages and uses multiple buffers.
	/// @note		Multithreading: Main thread only.
	/// @see		vk2d::_internal::RenderTargetTextureType
	/// @param[in]	swap
	///				Reference to internal structure which contains all the information about the current frame.
	void																CmdFinalizeRender(
		vk2d::_internal::RenderTargetTextureImpl::SwapBuffer		&	swap,
		vk2d::BlurType													blur_type,
		vk2d::Vector2f													blur_amount );

	/// @brief		Record commands to copy an image to the final sampled image, then generate mipmaps for it.
	///				Called by vk2d::_internal::RenderTargetTextureImpl::CmdFinalizeNonBlurredRender().
	/// @note		Multithreading: Main thread only.
	/// @param[in]	command_buffer
	///				Command buffer where to record mipmap blit commands to.
	/// @param[in]	source_image
	///				Reference to image object from where data is copied and blitted from.
	///				Only mip level 0 is accessed.
	///				Must have been created with VK_IMAGE_USAGE_TRANSFER_SRC_BIT flag.
	///				After this function returns source image layout will be VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL.
	/// @param[in]	source_image_layout
	///				Source image current layout. See Vulkan documentation about VkImageLayout.
	/// @param[in]	source_image_pipeline_barrier_src_stage
	///				Vulkan pipeline stage flags that must complete before source image data is accessed.
	/// @param[in]	destination_image
	///				Sampled image to be used as shader read only optimal, has to have correct amount of mip levels.
	void																CmdBlitMipmapsToSampledImage(
		VkCommandBuffer													command_buffer,
		vk2d::_internal::CompleteImageResource						&	source_image,
		VkImageLayout													source_image_layout,
		VkPipelineStageFlagBits											source_image_pipeline_barrier_src_stage,
		vk2d::_internal::CompleteImageResource						&	destination_image );

	bool																CmdRecordBlurCommands(
		vk2d::_internal::RenderTargetTextureImpl::SwapBuffer		&	swap,
		VkCommandBuffer													command_buffer,
		vk2d::BlurType													blur_type,
		vk2d::Vector2f													blur_amount,
		vk2d::_internal::CompleteImageResource						&	source_image,
		VkImageLayout													source_image_layout,
		VkPipelineStageFlagBits											source_image_pipeline_barrier_src_stage,
		vk2d::_internal::CompleteImageResource						&	intermediate_image,
		vk2d::_internal::CompleteImageResource						&	destination_image );

	void																CmdBindGraphicsPipelineIfDifferent(
		VkCommandBuffer													command_buffer,
		const vk2d::_internal::GraphicsPipelineSettings				&	pipeline_settings );

	void																CmdBindSamplerIfDifferent(
		VkCommandBuffer													command_buffer,
		vk2d::Sampler												*	sampler,
		VkPipelineLayout												use_pipeline_layout );

	void																CmdBindTextureIfDifferent(
		VkCommandBuffer													command_buffer,
		vk2d::Texture												*	texture,
		VkPipelineLayout												use_pipeline_layout );

	void																CmdSetLineWidthIfDifferent(
		VkCommandBuffer													command_buffer,
		float															line_width );

	bool																CmdUpdateFrameData(
		VkCommandBuffer													command_buffer );

	vk2d::RenderTargetTexture										*	my_interface								= {};
	vk2d::_internal::InstanceImpl									*	instance									= {};
	vk2d::RenderTargetTextureCreateInfo									create_info_copy							= {};

	vk2d::_internal::RenderTargetTextureType							type										= {};

	VkFormat															surface_format								= {};
	vk2d::Vector2u														size										= {};
	vk2d::Multisamples													samples										= {};
	std::vector<VkExtent2D>												mipmap_levels								= {};
	bool																granularity_aligned							= {};

	vk2d::_internal::CompleteBufferResource								frame_data_staging_buffer					= {};
	vk2d::_internal::CompleteBufferResource								frame_data_device_buffer					= {};
	vk2d::_internal::PoolDescriptorSet									frame_data_descriptor_set					= {};

	VkCommandPool														vk_graphics_command_pool					= {};
	//VkCommandPool														vk_compute_command_pool						= {};

	VkRenderPass														vk_attachment_render_pass					= {};
	VkRenderPass														vk_blur_render_pass_1						= {};
	VkRenderPass														vk_blur_render_pass_2						= {};

	std::unique_ptr<vk2d::_internal::MeshBuffer>						mesh_buffer;

	uint32_t															current_swap_buffer							= {};
	std::array<vk2d::_internal::RenderTargetTextureImpl::SwapBuffer, 2>	swap_buffers								= {};

	VkImageLayout														vk_attachment_image_final_layout			= {};
	VkImageLayout														vk_sampled_image_final_layout				= {};
	VkAccessFlags														vk_sampled_image_final_access_mask			= {};

	vk2d::_internal::GraphicsPipelineSettings							previous_graphics_pipeline_settings			= {};
	vk2d::Texture													*	previous_texture							= {};
	vk2d::Sampler													*	previous_sampler							= {};
	float																previous_line_width							= {};

	std::map<vk2d::Sampler*, vk2d::_internal::TimedDescriptorPoolData>	sampler_descriptor_sets						= {};
	std::map<vk2d::Texture*, vk2d::_internal::TimedDescriptorPoolData>	texture_descriptor_sets						= {};
	std::map<VkImageView, std::map<VkImageView, vk2d::_internal::TimedDescriptorPoolData>>
																		image_descriptor_sets						= {};

	bool																is_good										= {};
};



} // _internal

} // vk2d
