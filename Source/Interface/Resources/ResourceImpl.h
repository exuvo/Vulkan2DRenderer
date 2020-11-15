#pragma once

#include "../../Core/SourceCommon.h"



namespace vk2d {

class Resource;

namespace _internal {



class ResourceManagerImpl;
class ResourceThreadLoadTask;
class ResourceThreadUnloadTask;
class ThreadPrivateResource;



class ResourceImpl
{
	friend class vk2d::_internal::ResourceManagerImpl;
	friend class vk2d::_internal::ResourceThreadLoadTask;
	friend class vk2d::_internal::ResourceThreadUnloadTask;

public:
															ResourceImpl()					= delete;

															ResourceImpl(
		vk2d::Resource									*	my_interface,
		uint32_t											loader_thread,
		vk2d::_internal::ResourceManagerImpl			*	resource_manager,
		vk2d::Resource									*	parent_resource );

															ResourceImpl(
		vk2d::Resource									*	my_interface,
		uint32_t											loader_thread,
		vk2d::_internal::ResourceManagerImpl			*	resource_manager,
		vk2d::Resource									*	parent_resource,
		const std::vector<std::filesystem::path>		&	paths );

	virtual													~ResourceImpl()					= default;

	// Checks if the resource is ready to be used.
	// Returns true if resource is loaded, false otherwise.
	virtual bool											IsLoaded()						= 0;

	// Blocks until the resource is ready to be used or an error happened.
	// Returns true if loading was successful, false otherwise.
	virtual bool											WaitUntilLoaded()				= 0;

protected:
	// Internal use only.
	// Return true if loading was successful.
	virtual bool											MTLoad(
		vk2d::_internal::ThreadPrivateResource			*	thread_resource )				= 0;

	// Internal use only.
	virtual void											MTUnload(
		vk2d::_internal::ThreadPrivateResource			*	thread_resource )				= 0;

private:
	// Internal use only.
	// If the resource creates any subresources, they must NOT be manually destroyed.
	// If subresources are deleted anywhere else it can lead to race conditions within resource manager.
	// In case any resource uses sub-sub-resources, the resource manager will handle all cleanup.
	void													DestroySubresources();

	// Internal use only.
	// Subresources can be created either in the resource constructor or MTLoad(). To create a subresource,
	// we can create them just like regular resources, just add parent information.
	void													AddSubresource(
		vk2d::Resource									*	subresource );

public:
	vk2d::Resource										*	GetParentResource();

	// Checks if the resource loading failed.
	// Returns true if failed to load, false otherwise.
	bool													FailedToLoad() const;

	// Gets the thread index that was responsible for loading this resource.
	uint32_t												GetLoaderThread();

	// Checks if the resource was loaded from a file.
	// Returns true if the resource origin is in a file, for example an image, false otherwise.
	bool													IsFromFile() const;

	// Returns the file path where the resource was loaded from,
	// if the was not loaded from a file, returns "".
	const std::vector<std::filesystem::path>			&	GetFilePaths() const;

	virtual bool											IsGood() const						= 0;

protected:
	// Internal use only, this tells if this resource should be managed and
	// deleted by the resource manager or another resource. If this returns
	// true then resource manager should not delete this resource directly.
	bool													IsSubResource() const;

	std::atomic_bool										load_function_ran					= {};
	std::atomic_bool										failed_to_load						= {};
	vk2d::Resource										*	my_interface						= {};

private:
	std::mutex												resource_mutex;
	vk2d::_internal::ResourceManagerImpl				*	resource_manager_parent				= {};
	uint32_t												loader_thread						= {};
	std::vector<std::filesystem::path>						file_paths							= {};
	std::vector<vk2d::Resource*>							subresources						= {};
	vk2d::Resource										*	parent_resource						= {};
	bool													is_from_file						= {};
};


} // _internal

} // vk2d