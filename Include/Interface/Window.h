#pragma once

#include "Core/Common.h"

#include "Types/Vector2.hpp"
#include "Types/Rect2.hpp"
#include "Types/Matrix4.hpp"
#include "Types/Transform.h"
#include "Types/Color.hpp"
#include "Types/MeshPrimitives.hpp"
#include "Types/Multisamples.h"
#include "Types/RenderCoordinateSpace.hpp"

#include <memory>
#include <string>
#include <vector>
#include <filesystem>



namespace vk2d {

namespace _internal {
class InstanceImpl;
class WindowImpl;
class CursorImpl;
class MonitorImpl;

void UpdateMonitorLists( bool globals_locked );
} // _internal



class Instance;
class Texture;
class Mesh;
class WindowEventHandler;
class Window;
class Cursor;
class Monitor;
class Sampler;



enum class ButtonAction : int32_t {
	RELEASE				= 0,	///< Button was lift up.
	PRESS				= 1,	///< Button was pressed down.
	REPEAT				= 2,	///< Button was held down long and is being repeated by the OS, this is used in text input when user wants to insert same character multiple times.
};

enum class MouseButton : int32_t {
	BUTTON_1			= 0,		///< Left mouse button
	BUTTON_2			= 1,		///< Right mouse button
	BUTTON_3			= 2,		///< Middle mouse button
	BUTTON_4			= 3,		///< Forward side button
	BUTTON_5			= 4,		///< Backward side button
	BUTTON_6			= 5,		///< (Extra mouse button)
	BUTTON_7			= 6,		///< (Extra mouse button)
	BUTTON_8			= 7,		///< (Extra mouse button)
	BUTTON_LAST			= BUTTON_8,	///< (Extra mouse button)
	BUTTON_LEFT			= BUTTON_1,	///< Left mouse button
	BUTTON_RIGHT		= BUTTON_2,	///< Right mouse button
	BUTTON_MIDDLE		= BUTTON_3,	///< Middle mouse button
};

enum class ModifierKeyFlags : int32_t {
	SHIFT				= 0x0001,	///< Shift key, either left or right
	CONTROL				= 0x0002,	///< Ctrl key, either left or right
	ALT					= 0x0004,	///< Alt key, either left or right
	SUPER				= 0x0008	///< Windows key, either left or right
};
inline ModifierKeyFlags operator|( ModifierKeyFlags f1, ModifierKeyFlags f2 )
{
	return ModifierKeyFlags( int32_t( f1 ) | int32_t( f2 ) );
}
inline ModifierKeyFlags operator&( ModifierKeyFlags f1, ModifierKeyFlags f2 )
{
	return ModifierKeyFlags( int32_t( f1 ) & int32_t( f2 ) );
}

/// @brief		Cursor state dictates the behavior with window.
enum class CursorState : int32_t {
	NORMAL,		///< Normal cursor, allowed to leave the window area and is visible at all times.
	HIDDEN,		///< Hidden cursor on window area, cursor is allowed to leave the window area and becomes visible when it does.
	LOCKED		///< Cursor is locked to the window, it's not visible and it's typically not allowed to leave the window area.
};

/// @brief		These are the key codes for each and every individual keyboard button.
enum class KeyboardButton : int32_t {
	KEY_UNKNOWN			= -1,	///< Unrecognized keyboard button

	KEY_SPACE			= 32,	///< Space bar
	KEY_APOSTROPHE		= 39,	///< <tt>'</tt>
	KEY_COMMA			= 44,	///< <tt>,</tt>
	KEY_MINUS			= 45,	///< <tt>-</tt>
	KEY_PERIOD			= 46,	///< <tt>.</tt>
	KEY_SLASH			= 47,	///< <tt>/</tt> forward slash
	KEY_0				= 48,	///< Top row <tt>0</tt>
	KEY_1				= 49,	///< Top row <tt>1</tt>
	KEY_2				= 50,	///< Top row <tt>2</tt>
	KEY_3				= 51,	///< Top row <tt>3</tt>
	KEY_4				= 52,	///< Top row <tt>4</tt>
	KEY_5				= 53,	///< Top row <tt>5</tt>
	KEY_6				= 54,	///< Top row <tt>6</tt>
	KEY_7				= 55,	///< Top row <tt>7</tt>
	KEY_8				= 56,	///< Top row <tt>8</tt>
	KEY_9				= 57,	///< Top row <tt>9</tt>
	KEY_SEMICOLON		= 59,	///< <tt>;</tt>
	KEY_EQUAL			= 61,	///< <tt>=</tt>
	KEY_A				= 65,
	KEY_B				= 66,
	KEY_C				= 67,
	KEY_D				= 68,
	KEY_E				= 69,
	KEY_F				= 70,
	KEY_G				= 71,
	KEY_H				= 72,
	KEY_I				= 73,
	KEY_J				= 74,
	KEY_K				= 75,
	KEY_L				= 76,
	KEY_M				= 77,
	KEY_N				= 78,
	KEY_O				= 79,
	KEY_P				= 80,
	KEY_Q				= 81,
	KEY_R				= 82,
	KEY_S				= 83,
	KEY_T				= 84,
	KEY_U				= 85,
	KEY_V				= 86,
	KEY_W				= 87,
	KEY_X				= 88,
	KEY_Y				= 89,
	KEY_Z				= 90,
	KEY_LEFT_BRACKET	= 91,	///< <tt>[</tt>
	KEY_BACKSLASH		= 92,	///< <tt>\\</tt> back slash
	KEY_RIGHT_BRACKET	= 93,	///< <tt>]</tt>
	KEY_GRAVE_ACCENT	= 96,	///< <tt>`</tt>
	KEY_WORLD_1			= 161,	///< non-US #1
	KEY_WORLD_2			= 162,	///< non-US #2

	KEY_ESCAPE			= 256,	///< <tt>Esc</tt>
	KEY_ENTER			= 257,	///< <tt>Enter</tt>, Underneath backspace
	KEY_TAB				= 258,	///< <tt>Tab</tt>
	KEY_BACKSPACE		= 259,	///< <tt>Backspace</tt>
	KEY_INSERT			= 260,	///< <tt>Insert</tt>
	KEY_DELETE			= 261,	///< <tt>Delete</tt>
	KEY_RIGHT			= 262,	///< Right arrow key
	KEY_LEFT			= 263,	///< Left arrow key
	KEY_DOWN			= 264,	///< Down arrow key
	KEY_UP				= 265,	///< Up arrow key
	KEY_PAGE_UP			= 266,	///< <tt>Page up</tt>
	KEY_PAGE_DOWN		= 267,	///< <tt>Page down</tt>
	KEY_HOME			= 268,	///< <tt>Home</tt>
	KEY_END				= 269,	///< <tt>End</tt>
	KEY_CAPS_LOCK		= 280,	///< <tt>Caps lock</tt>
	KEY_SCROLL_LOCK		= 281,	///< <tt>Scroll lock</tt>
	KEY_NUM_LOCK		= 282,	///< <tt>Num lock</tt>
	KEY_PRINT_SCREEN	= 283,	///< <tt>Print screen</tt>
	KEY_PAUSE			= 284,	///< <tt>Pause</tt>
	KEY_F1				= 290,
	KEY_F2				= 291,
	KEY_F3				= 292,
	KEY_F4				= 293,
	KEY_F5				= 294,
	KEY_F6				= 295,
	KEY_F7				= 296,
	KEY_F8				= 297,
	KEY_F9				= 298,
	KEY_F10				= 299,
	KEY_F11				= 300,
	KEY_F12				= 301,
	KEY_F13				= 302,
	KEY_F14				= 303,
	KEY_F15				= 304,
	KEY_F16				= 305,
	KEY_F17				= 306,
	KEY_F18				= 307,
	KEY_F19				= 308,
	KEY_F20				= 309,
	KEY_F21				= 310,
	KEY_F22				= 311,
	KEY_F23				= 312,
	KEY_F24				= 313,
	KEY_F25				= 314,
	KEY_NUMPAD_0		= 320,
	KEY_NUMPAD_1		= 321,
	KEY_NUMPAD_2		= 322,
	KEY_NUMPAD_3		= 323,
	KEY_NUMPAD_4		= 324,
	KEY_NUMPAD_5		= 325,
	KEY_NUMPAD_6		= 326,
	KEY_NUMPAD_7		= 327,
	KEY_NUMPAD_8		= 328,
	KEY_NUMPAD_9		= 329,
	KEY_NUMPAD_DECIMAL	= 330,	///< Numpad <tt>.</tt> or <tt>,</tt> depending on region
	KEY_NUMPAD_DIVIDE	= 331,	///< Numpad <tt>/</tt>
	KEY_NUMPAD_MULTIPLY	= 332,	///< Numpad <tt>*</tt>
	KEY_NUMPAD_SUBTRACT	= 333,	///< Numpad <tt>-</tt>
	KEY_NUMPAD_ADD		= 334,	///< Numpad <tt>+</tt>
	KEY_NUMPAD_ENTER	= 335,	///< Numpad <tt>Enter</tt>
	KEY_NUMPAD_EQUAL	= 336,	///< Numpad <tt>=</tt> (often missing)
	KEY_LEFT_SHIFT		= 340,	///< Left <tt>Shift</tt>
	KEY_LEFT_CONTROL	= 341,	///< Left <tt>Ctrl</tt>
	KEY_LEFT_ALT		= 342,	///< Left <tt>Alt</tt>
	KEY_LEFT_SUPER		= 343,	///< Left Super/Windows key 
	KEY_RIGHT_SHIFT		= 344,	///< Right <tt>Shift</tt>
	KEY_RIGHT_CONTROL	= 345,	///< Right <tt>Ctrl</tt>
	KEY_RIGHT_ALT		= 346,	///< Right <tt>Alt</tt>
	KEY_RIGHT_SUPER		= 347,	///< Right Super/Windows key
	KEY_MENU			= 348,	///< Menu

	KEY_LAST			= KEY_MENU,	///< Used to get the number of total key entries.
};

/// @brief		Parameters to construct a vk2d::Window.
struct WindowCreateInfo {
	bool								resizeable					= true;											///< Can we use the cursor to resize the window.
	bool								visible						= true;											///< Is the window visible after created.
	bool								decorated					= true;											///< Does the window have default OS borders and buttons.
	bool								focused						= true;											///< Is the window focused and brought forth when created.
	bool								maximized					= false;										///< Is the window maximized to fill the screen when created.
	bool								transparent_framebuffer		= false;										///< Is the alpha value of the render interpreted as a transparent window background.
	vk2d::RenderCoordinateSpace			coordinate_space			= vk2d::RenderCoordinateSpace::TEXEL_SPACE;		///< Coordinate system to be used, see vk2d::RenderCoordinateSpace.
	vk2d::Vector2u						size						= { 800, 600 };									///< Window content initial pixel size
	vk2d::Vector2u						min_size					= { 32, 32 };									///< Minimum size of the window. (also works when drag resizing, this value may be adjusted to suit the hardware)
	vk2d::Vector2u						max_size					= { UINT32_MAX, UINT32_MAX };					///< Maximum size of the window. (also works when drag resizing, this value may be adjusted to suit the hardware)
	vk2d::Monitor					*	fullscreen_monitor			= {};											///< Fullscreen monitor pointer, nullptr is windowed, use Instance::GetPrimaryMonitor() to use primary monitor for fullscreen.
	uint32_t							fullscreen_refresh_rate		= UINT32_MAX;									///< Refresh rate in fullscreen mode, UINT32_MAX uses maximum refresh rate available.
	bool								vsync						= true;											///< Vertical synchronization, works in both windowed and fullscreen modes, usually best left on for 2d graphics.
	vk2d::Multisamples					samples						= vk2d::Multisamples::SAMPLE_COUNT_1;			///< Multisampling, must be a single value from vk2d::Multisamples. Uses more GPU resources if higher than 1.
	std::string							title						= "";											///< Window title text.
	vk2d::WindowEventHandler		*	event_handler				= nullptr;										///< Pointer to a custom event handler that will be used with this window (input events, keyboard, mouse...) See vk2d::WindowEventHandler.
};





/// @brief		Video mode the monitor can natively work in.
struct MonitorVideoMode {
	vk2d::Vector2u		resolution;
	uint32_t			red_bit_count;
	uint32_t			green_bit_count;
	uint32_t			blue_bit_count;
	uint32_t			refresh_rate;
};

/// @brief		Gamma ramp for manual gamma adjustment on the monitor at different
///				intensity levels per color.
///				Ramp is made out of nodes that are evenly spaced from lowest to
///				highest value. Input must have at least 2 nodes, values are linearly
///				interpolated inbetween nodes to fill the entire range.
///				This gamma ramp is applied in addition to the hardware or OS
///				gamma correction (usually approximation of sRGB gamma) so setting
///				a linear gamma ramp will result in already gamma corrected image.
struct GammaRampNode {
	float	red;
	float	green;
	float	blue;
};







// Monitor object holds information about the physical monitor
class Monitor {
	friend class vk2d::Window;
	friend class vk2d::_internal::WindowImpl;
	friend void vk2d::_internal::UpdateMonitorLists( bool globals_locked );

	/// @brief		This object should not be directly constructed, it is created and
	///				destroyed automatically by VK2D.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	preconstructed_impl
	///				This is the actual implementation details given to this interface by
	///				VK2D.
	VK2D_API																				Monitor(
		std::unique_ptr<vk2d::_internal::MonitorImpl>	&&	preconstructed_impl );

public:
	/// @brief		Monitor constructor for a null monitor, needed for default
	///				initialization.
	///				This object should not be directly constructed, it is created and
	///				destroyed automatically by VK2D whenever a new instance is created
	///				or if monitor is connected or disconnected while the application is
	///				running.
	/// @note		Multithreading: Main thread only.
	VK2D_API																				Monitor();

	/// @note		Multithreading: Main thread only.
	VK2D_API																				Monitor(
		const vk2d::Monitor								&	other );

	/// @note		Multithreading: Main thread only.
	VK2D_API																				Monitor(
		vk2d::Monitor									&&	other )							noexcept;

	/// @note		Multithreading: Main thread only.
	VK2D_API																				~Monitor();

	/// @brief		Get current video mode, resolution, bits per color and refresh rate.
	/// @note		Multithreading: Main thread only.
	/// @return		Current monitor video mode.
	VK2D_API vk2d::MonitorVideoMode							VK2D_APIENTRY					GetCurrentVideoMode() const;

	/// @brief		Get all video modes supported by the monitor.
	/// @note		Multithreading: Main thread only.
	/// @return		A vector of video modes supported by the monitor.
	VK2D_API std::vector<vk2d::MonitorVideoMode>			VK2D_APIENTRY					GetVideoModes() const;

	/// @brief		Set monitor gamma. Automatically generates a gamma ramp from this value
	///				and uses it to set the gamma. This value is in addition to the hardware
	///				or OS gamma correction value so 1.0 (linear) is considered already gamma
	///				corrected.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	gamma
	///				Value greater than 0.0. Default/original gamma value is 1.0 which
	///				produces linear gamma.
	VK2D_API void											VK2D_APIENTRY					SetGamma(
		float												gamma );

	/// @brief		Get monitor gamma ramp.
	/// @note		Multithreading: Main thread only.
	/// @see		vk2d::GammaRampNode.
	/// @return		A vector of gamma ramp nodes at equal spacing where first node is minimum
	///				brightness and last node is maximum brightness.
	VK2D_API std::vector<vk2d::GammaRampNode>				VK2D_APIENTRY					GetGammaRamp();

	/// @brief		Set monitor gamma manually with gamma ramp.
	/// @note		Multithreading: Main thread only.
	/// @see		vk2d::GammaRampNode.
	/// @param[in]	ramp
	///				A vector of gamma nodes where all nodes are considered
	///				evenly spaced. First node is minimum brightness, last node
	///				is maximum brightness. Number of gamma ramp nodes must be
	///				2 or more. Values inbetween nodes are automatically
	///				linearly interpolated so number of nodes only effects
	///				quality of the gamma ramp.
	VK2D_API void											VK2D_APIENTRY					SetGammaRamp(
		const std::vector<vk2d::GammaRampNode>			&	ramp );

	VK2D_API vk2d::Monitor								&	VK2D_APIENTRY					operator=(
		const vk2d::Monitor								&	other );

	VK2D_API vk2d::Monitor								&	VK2D_APIENTRY					operator=(
		vk2d::Monitor									&&	other )							noexcept;

	/// @brief		VK2D class object checker function.
	/// @note		Multithreading: Main thread only.
	/// @return		true if class object was created successfully,
	///				false if something went wrong
	VK2D_API bool											VK2D_APIENTRY					IsGood() const;

private:
	std::unique_ptr<vk2d::_internal::MonitorImpl>			impl;
};







/// @brief		Mouse cursor is nothing more than an image that represents the
///				location of the mouse on window, just like on the desktop environment.
/// 
///				This cursor object is used to swap out the OS cursor image to another
///				while the cursor is hovering over the window.
///				This is sometimes called "hardware" cursor in many applications.
///				On "software" cursor implementations your application would hide this
///				type of cursor and implement it's own using a texture that's drawn at
///				cursor location inside the game loop.
///				As "hardware" cursor is updated separately from your application it's
///				usually a good idea to use it in case your application has framerate
///				issues or it locks up completely.
class Cursor {
	friend class vk2d::_internal::InstanceImpl;
	friend class vk2d::Window;
	friend class vk2d::_internal::WindowImpl;

	/// @brief		This object should not be directly constructed, it is created by
	///				vk2d::Instance::CreateCursor().
	/// @note		Multithreading: Main thread only.
	/// @param[in]	instance
	///				A pointer to instance that owns this cursor.
	/// @param[in]	image_path
	///				Path to an image file that will be used as a cursor.
	/// @param[in]	hot_spot
	///				Hot spot is the texel offset from the top left corner of the image
	///				to where the actual pointing tip of the cursor is located.
	///				For example if the cursor is a 64*64 texel image of a circle where
	///				the circle is exactly centered and the center of the circle is the
	///				"tip" of the cursor, then the cursor's hot spot is 32*32.
	VK2D_API																		Cursor(
		vk2d::_internal::InstanceImpl		*	instance,
		const std::filesystem::path			&	image_path,
		vk2d::Vector2i							hot_spot );

	/// @brief		This object should not be directly constructed, it is created by
	///				vk2d::Instance::CreateCursor().
	/// @note		Multithreading: Main thread only.
	/// @param[in]	instance
	///				A pointer to instance that owns this cursor.
	/// @param[in]	image_size
	///				Size of the image in texels.
	/// @param[in]	image_data
	///				Image texel data, from left to right, top to bottom order. Input
	///				vector size must be large enough to contain all texels at given
	///				texel size.
	/// @param[in]	hot_spot
	///				Hot spot is the texel offset from the top left corner of the image
	///				to where the actual pointing tip of the cursor is located.
	///				For example if the cursor is a 64*64 texel image of a circle where
	///				the circle is exactly centered and the center of the circle is the
	///				"tip" of the cursor, then the cursor's hot spot is 32*32.
	VK2D_API																		Cursor(
		vk2d::_internal::InstanceImpl		*	instance,
		vk2d::Vector2u							image_size,
		const std::vector<vk2d::Color8>		&	image_data,
		vk2d::Vector2i							hot_spot );

public:
	/// @note		Multithreading: Main thread only.
	VK2D_API																		Cursor(
		vk2d::Cursor						&	other );

	/// @note		Multithreading: Main thread only.
	VK2D_API																		Cursor(
		vk2d::Cursor						&&	other )								noexcept;

	/// @note		Multithreading: Main thread only.
	VK2D_API									VK2D_APIENTRY						~Cursor();

	/// @note		Multithreading: Main thread only.
	VK2D_API vk2d::Cursor					&	VK2D_APIENTRY						operator=(
		vk2d::Cursor						&	other );

	/// @note		Multithreading: Main thread only.
	VK2D_API vk2d::Cursor					&	VK2D_APIENTRY						operator=(
		vk2d::Cursor						&&	other )								noexcept;

	/// @brief		Get cursor image texel size.
	/// @note		Multithreading: Main thread only.
	/// @return		Size of the cursor image in texels.
	VK2D_API vk2d::Vector2u						VK2D_APIENTRY						GetSize();

	/// @brief		Get hot spot location in texels.
	/// @note		Multithreading: Main thread only.
	/// @return		The hot spot of the cursor image.
	///				Hot spot is the offset of the image to the "tip" of the cursor
	///				starting from top left of the image.
	VK2D_API vk2d::Vector2i						VK2D_APIENTRY						GetHotSpot();

	/// @brief		Get texel data of the cursor image.
	/// @note		Multithreading: Main thread only.
	/// @return		A vector of texels in left to right, top to bottom order.
	///				You will also need to use vk2d::Cursor::GetSize() in order to correctly
	///				interpret the texels.
	VK2D_API std::vector<vk2d::Color8>			VK2D_APIENTRY						GetTexelData();

	/// @brief		VK2D class object checker function.
	/// @note		Multithreading: Main thread only.
	/// @return		true if class object was created successfully,
	///				false if something went wrong
	VK2D_API bool								VK2D_APIENTRY						IsGood() const;

private:
	std::unique_ptr<vk2d::_internal::CursorImpl>	impl;
};







class Window {
	friend class vk2d::_internal::InstanceImpl;

private:
	/// @brief		This object should not be directly constructed, it is created by
	///				vk2d::Instance::CreateOutputWindow().
	/// @note		Multithreading: Main thread only.
	/// @param[in]	instance
	///				A pointer to instance that owns this window.
	/// @param[in]	window_create_info
	///				Window creation parameters.
	VK2D_API																		Window(
		vk2d::_internal::InstanceImpl				*	instance,
		const vk2d::WindowCreateInfo				&	window_create_info );

public:
	/// @note		Multithreading: Main thread only.
	VK2D_API																		~Window();

	/// @brief		Signal that the window should now close. This function does not actually
	///				close the window but rather just sets a flag that it should close, the
	///				main program will have to manually remove the window from Instance.
	/// @note		Multithreading: Main thread only.
	VK2D_API void										VK2D_APIENTRY				CloseWindow();

	/// @brief		Checks if the window wants to close.
	/// @note		Multithreading: Main thread only.
	/// @return		true if window close is requested, false if window close has not been requestd.
	VK2D_API bool										VK2D_APIENTRY				ShouldClose();

	/// @brief		Takes a screenshot of the next image that will be rendered
	///				and saves it into a file.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	save_path
	///				Path where the file will be saved. Extension will determine the file
	///				format used. Supported file formats are: <br>
	///				<table>
	///				<tr><th> Format												<th> Extension
	///				<tr><td> Portable network graphics (RGBA) or (RGB)			<td> <tt>.png</tt>
	///				<tr><td> Microsoft Windows Bitmap 24-bit (BGR) non-RLE		<td> <tt>.bmp</tt>
	///				<tr><td> Truevision Targa (RGBA) or (RGB) RLE compressed	<td> <tt>.tga</tt>
	///				<tr><td> JPEG (RGB)											<td> <tt>.jpg</tt> or <tt>.jpeg</tt>
	///				</table>
	///				Unsupported or unknown extensions will be saved as PNG.
	/// @param[in]	include_alpha
	///				Include transparency of the scene in the saved image.
	///				true if you want transparency channel included, false otherwise.
	///				Note that not all formats can save transparency, in which case this
	///				parameter is ignored.
	VK2D_API void										VK2D_APIENTRY				TakeScreenshotToFile(
		const std::filesystem::path					&	save_path,
		bool											include_alpha				= false );

	/// @brief		Takes a screenshot of the next image that will be rendered
	///				and calls an event callback to give the data to the application.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	include_alpha
	///				Include transparency of the scene in the saved image.
	///				true if you want transparency channel included, false if you want
	///				transparency channel set to opaque.
	VK2D_API void										VK2D_APIENTRY				TakeScreenshotToData(
		bool											include_alpha);

	/// @brief		Sets focus to this window, should be called before entering
	///				fullscreen mode from windowed mode.
	/// @note		Multithreading: Main thread only.
	VK2D_API void										VK2D_APIENTRY				Focus();

	/// @brief		Set window opacity.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	opacity
	///				Value between 0.0 and 1.0 where 0.0 is completely transparent and
	///				1.0 is completely opaque.
	VK2D_API void										VK2D_APIENTRY				SetOpacity(
		float											opacity );

	/// @brief		Gets the current opacity of this window.
	/// @note		Multithreading: Main thread only.
	/// @return		Current opacity value between 0.0 and 1.0 where 0.0 is completely
	///				transparent and 1.0 is completely opaque.
	VK2D_API float										VK2D_APIENTRY				GetOpacity();

	/// @brief		Hides or un-hides the window. Hidden windows do not receive user input.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	hidden
	///				true to hide the window, false to un-hide.
	VK2D_API void										VK2D_APIENTRY				Hide(
		bool											hidden );

	/// @brief		Gets the hidden status of the window.
	/// @note		Multithreading: Main thread only.
	/// @return		true if window is hidden, false if it's visible.
	VK2D_API bool										VK2D_APIENTRY				IsHidden();

	/// @brief		Disable or enable all events for a specific window.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	disable_events
	///				true to disable all events, false to enable all events.
	VK2D_API void										VK2D_APIENTRY				DisableEvents(
		bool											disable_events );

	/// @brief		Checks if events are enabled or disabled.
	/// @see		vk2d::Window::DisableEvents().
	/// @note		Multithreading: Main thread only.
	/// @return		true if events are disabled, false if events are enabled.
	VK2D_API bool										VK2D_APIENTRY				AreEventsDisabled();

	/// @brief		Enter fullscreen or windowed mode.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	monitor
	///				A pointer to a monitor where you wish to display the contents of
	///				this window in fullscreen mode.
	///				<tt>nullptr</tt> if you wish to enter windowed mode.
	/// @param[in]	frequency
	///				Target refresh rate of the monitor when entering fullscreen mode.
	///				Ignored when entering windowed mode.
	VK2D_API void										VK2D_APIENTRY				SetFullscreen(
		vk2d::Monitor								*	monitor,
		uint32_t										frequency );

	/// @brief		Checks if we're in fullscreen or windowed mode.
	/// @note		Multithreading: Main thread only.
	/// @return		true if this window has entered fullscreen mode on any monitor,
	///				false if windowed.
	VK2D_API bool										VK2D_APIENTRY				IsFullscreen();

	/// @brief		Get cursor position on window surface without using events.
	/// @note		Multithreading: Main thread only.
	/// @return		The cursor position on window surface in texels where 0*0 coordinate
	///				is top left corner of the window.
	VK2D_API vk2d::Vector2d								VK2D_APIENTRY				GetCursorPosition();

	/// @brief		Set cursor position in relation to this window.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	new_position
	///				New mouse cursor position. This changes the position of the OS or
	///				the "hardware" cursor.
	VK2D_API void										VK2D_APIENTRY				SetCursorPosition(
		vk2d::Vector2d									new_position );

	/// @brief		Sets the OS or "hardware" cursor image to something else.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	cursor
	///				A pointer to cursor object to use from now on while the OS cursor
	///				is located inside this window.
	VK2D_API void										VK2D_APIENTRY				SetCursor(
		vk2d::Cursor								*	cursor );

	/// @brief		Get last contents of the OS clipboard if it's a string. This way
	///				you can copy from another application and paste it right inside yours.
	/// @note		Multithreading: Main thread only.
	/// @return		Last clipboard contents if it was text.
	VK2D_API std::string								VK2D_APIENTRY				GetClipboardString();

	/// @brief		Set OS clipboard last entry to some string. This is useful if you
	///				wish to enable copying text from your application and allowing
	///				user to paste it some other application.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	str
	///				Text to send to the OS clipboard.
	VK2D_API void										VK2D_APIENTRY				SetClipboardString(
		const std::string							&	str );

	/// @brief		Set window title that shows up on the title bar of the window.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	title
	///				New title text of the window.
	VK2D_API void										VK2D_APIENTRY				SetTitle(
		const std::string							&	title );

	/// @brief		Gets title of the window.
	/// @note		Multithreading: Main thread only.
	/// @return		Current title of the window.
	VK2D_API std::string								VK2D_APIENTRY				GetTitle();

	/// @brief		Set window icon that shows up in OS taskbar/toolbar when the
	///				application is running.
	/// @note		Multithreading: Main thread only.
	/// @param		image_paths
	///				Path to image to use. Supported formats are: <br>
	///				<table>
	///					<tr>
	///						<th>Format</th>	<th>Support level</th>
	///					</tr>
	///					<tr>
	///						<td>JPEG</td>	<td>baseline & progressive supported, 12 bits-per-channel/arithmetic not supported</td>
	///					</tr>
	///					<tr>
	///						<td>PNG</td>	<td>1/2/4/8/16 bit-per-channel supported</td>
	///					</tr>
	///					<tr>
	///						<td>TGA</td>	<td>not sure what subset, if a subset</td>
	///					</tr>
	///					<tr>
	///						<td>BMP</td>	<td>no support for 1bpp or RLE</td>
	///					</tr>
	///					<tr>
	///						<td>PSD</td>	<td>composited view only, no extra channels, 8/16 bits-per-channel</td>
	///					</tr>
	///					<tr>
	///						<td>PIC</td>	<td>Softimage PIC</td>
	///					</tr>
	///					<tr>
	///						<td>PNM</td>	<td>PPM and PGM binary only</td>
	///					</tr>
	///				</table>
	VK2D_API void										VK2D_APIENTRY				SetIcon(
		const std::vector<std::filesystem::path>	&	image_paths );

	/// @brief		Sets window position on the virtual screen space.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	new_position
	///				New position in the virtual monitor space. Virtual in this case
	///				means that if the user has 2 or more monitor setup and the desktop
	///				is set to be continuous from one monitor to the next, the window
	///				coordinates determine in which monitor the window will appear,
	///				this depends on the user's monitor setup however.
	VK2D_API void										VK2D_APIENTRY				SetPosition(
		vk2d::Vector2i									new_position );

	/// @brief		Get window current position on the virtual screen space.
	/// @note		Multithreading: Main thread only.
	/// @return		Position of the window.
	VK2D_API vk2d::Vector2i								VK2D_APIENTRY				GetPosition();

	/// @brief		Set size of the window. More specifically this sets the framebuffer
	///				size or the content size of the window to a new size, window decorators
	///				are extended to fit the new content size.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	new_size
	///				New size of the window.
	VK2D_API void										VK2D_APIENTRY				SetSize(
		vk2d::Vector2u									new_size );

	/// @brief		Get content/framebuffer size of the window.
	/// @note		Multithreading: Main thread only.
	/// @return		Size of the window.
	VK2D_API vk2d::Vector2u								VK2D_APIENTRY				GetSize();

	/// @brief		Iconifies the window to the taskbar or restores it back into a window.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	minimized
	///				true if window should be iconified, false if restored from iconified state.
	VK2D_API void										VK2D_APIENTRY				Iconify(
		bool											minimized );

	/// @brief		Checks if the window is currently iconified.
	/// @see		vk2d::Window::Iconify().
	/// @note		Multithreading: Main thread only.
	/// @return		true if window is iconified to the task/tool-bar, false if window is fully
	///				visible.
	VK2D_API bool										VK2D_APIENTRY				IsIconified();

	/// @brief		Sets the window to be maximized or normal. When maximized the window will
	///				be sized to fill the entire workable space of the desktop monitor so that
	///				the window edges, top bar and the OS task/tool-bar is not covered.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	maximized
	///				true if you wish to maximize the window, false if you want floating window.
	VK2D_API void										VK2D_APIENTRY				SetMaximized(
		bool											maximized );

	/// @brief		Gets the maximized status.
	/// @see		vk2d::Window::SetMaximized()
	/// @note		Multithreading: Main thread only.
	/// @return		true if the window is maximized, false if it's floating.
	VK2D_API bool										VK2D_APIENTRY				GetMaximized();

	/// @brief		Set the cursor to be visible, invisible or constrained inside the window.
	/// @see		vk2d::CursorState
	/// @note		Multithreading: Main thread only.
	/// @param[in]	new_state
	///				New state of the cursor to be used from now on.
	VK2D_API void										VK2D_APIENTRY				SetCursorState(
		vk2d::CursorState								new_state );

	/// @brief		Returns the current state of the cursor, either normal, hidden, or constrained.
	/// @see		vk2d::CursorState
	/// @note		Multithreading: Main thread only.
	/// @return		Current state of the cursor.
	VK2D_API vk2d::CursorState							VK2D_APIENTRY				GetCursorState();

	/// @brief		Begins the render operations. This signals the beginning of the render block and
	///				you must call this before using any drawing commands.
	///				For best performance you should calculate game logic first, when you're ready to draw
	///				call this function just before your first draw command. Every draw call must be
	///				between this and vk2d::Window::EndRender().
	/// @see		vk2d::Window::EndRender()
	/// @note		Multithreading: Main thread only.
	/// @return		true if operation was successful, false on error and if you should quit.
	VK2D_API bool										VK2D_APIENTRY				BeginRender();

	/// @brief		Ends the rendering operations. This signals the end of the render block and you must
	///				call this after you're done drawing everything in order to display the results on the
	///				window surface.
	/// @see		vk2d::Window::BeginRender()
	/// @note		Multithreading: Main thread only.
	/// @return		true if operation was successful, false on error and if you should quit.
	VK2D_API bool										VK2D_APIENTRY				EndRender();

	/// @brief		Draw triangles directly.
	///				Best used if you want to manipulate and draw vertices directly.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	indices
	///				List of indices telling how to form triangles between vertices.
	/// @param[in]	vertices
	///				List of vertices that define the shape.
	/// @param[in]	texture_channel_weights
	///				Only has effect if provided texture has more than 1 layer.
	///				This tell how much weight each texture layer has on each vertex.
	///				TODO: Need to check formatting... Yank Niko, he forgot...
	/// @param[in]	solid
	///				If true, renders solid polygons, if false renders as wireframe.
	/// @param[in]	texture
	///				Pointer to texture, see vk2d::Vertex for UV mapping details.
	///				Can be nullptr in which case a white texture is used (vertex colors only).
	/// @param[in]	sampler
	///				Pointer to sampler which determines how the texture is drawn.
	///				Can be nullptr in which case the default sampler is used.
	VK2D_API void										VK2D_APIENTRY				DrawTriangleList(
		const std::vector<vk2d::VertexIndex_3>		&	indices,
		const std::vector<vk2d::Vertex>				&	vertices,
		const std::vector<float>					&	texture_channel_weights,
		const std::vector<vk2d::Matrix4f>			&	transformations				= {},
		bool											solid						= true,
		vk2d::Texture								*	texture						= nullptr,
		vk2d::Sampler								*	sampler						= nullptr );

	/// @brief		Draws lines directly.
	///				Best used if you want to manipulate and draw vertices directly.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	indices
	///				List of indices telling how to form lines between vertices.
	/// @param[in]	vertices 
	///				List of vertices that define the shape.
	/// @param[in]	texture_channel_weights 
	///				Only has effect if provided texture has more than 1 layer.
	///				This tell how much weight each texture layer has on each vertex.
	///				TODO: Need to check formatting... Yank Niko, he forgot...
	/// @param[in]	texture 
	///				Pointer to texture, see vk2d::Vertex for UV mapping details.
	///				Can be nullptr in which case a white texture is used (vertex colors only).
	/// @param[in]	sampler 
	///				Pointer to sampler which determines how the texture is drawn.
	///				Can be nullptr in which case the default sampler is used.
	VK2D_API void										VK2D_APIENTRY				DrawLineList(
		const std::vector<vk2d::VertexIndex_2>		&	indices,
		const std::vector<vk2d::Vertex>				&	vertices,
		const std::vector<float>					&	texture_channel_weights,
		const std::vector<vk2d::Matrix4f>			&	transformations				= {},
		vk2d::Texture								*	texture						= nullptr,
		vk2d::Sampler								*	sampler						= nullptr,
		float											line_width					= 1.0f );

	/// @brief		Draws points directly.
	///				Best used if you want to manipulate and draw vertices directly.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	vertices 
	///				List of vertices that define where and how points are drawn.
	/// @param[in]	texture_channel_weights 
	///				Only has effect if provided texture has more than 1 layer.
	///				This tell how much weight each texture layer has on each vertex.
	///				TODO: Need to check formatting... Yank Niko, he forgot...
	/// @param[in]	texture 
	///				Pointer to texture, see vk2d::Vertex for UV mapping details.
	///				Can be nullptr in which case a white texture is used (vertex colors only).
	/// @param[in]	sampler 
	///				Pointer to sampler which determines how the texture is drawn.
	///				Can be nullptr in which case the default sampler is used.
	VK2D_API void										VK2D_APIENTRY				DrawPointList(
		const std::vector<vk2d::Vertex>				&	vertices,
		const std::vector<float>					&	texture_channel_weights,
		const std::vector<vk2d::Matrix4f>			&	transformations				= {},
		vk2d::Texture								*	texture						= nullptr,
		vk2d::Sampler								*	sampler						= nullptr );

	/// @brief		Draws an individual point.
	///				Inefficient, for when you really just need a single point drawn without extra information.
	///				As soon as you need to draw 2 or more points, use vk2d::Window::DrawPointList() instead.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	location
	///				Where to draw the point to, depends on the coordinate system. See vk2d::RenderCoordinateSpace for more info.
	/// @param[in]	color
	///				Color of the point to be drawn.
	/// @param[in]	size
	///				Size of the point to be drawn, sizes larger than 1.0f will appear as a rectangle.
	VK2D_API void										VK2D_APIENTRY				DrawPoint(
		vk2d::Vector2f									location,
		vk2d::Colorf									color						= { 1.0f, 1.0f, 1.0f, 1.0f },
		float											size						= 1.0f );

	/// @brief		Draws an individual line.
	///				Inefficient, for when you really just need a single line drawn without extra information.
	///				As soon as you need to draw 2 or more lines, use vk2d::Window::DrawLineList() instead.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	point_1
	///				Coordinates of the starting point of the line, depends on the coordinate system. See vk2d::RenderCoordinateSpace for more info.
	/// @param[in]	point_2
	///				Coordinates of the ending point of the line, depends on the coordinate system. See vk2d::RenderCoordinateSpace for more info.
	/// @param[in]	color 
	///				Color of the line to be drawn.
	VK2D_API void										VK2D_APIENTRY				DrawLine(
		vk2d::Vector2f									point_1,
		vk2d::Vector2f									point_2,
		vk2d::Colorf									color						= { 1.0f, 1.0f, 1.0f, 1.0f },
		float											line_width					= 1.0f );

	/// @brief		Draws a rectangle.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	area
	///				Area of the rectangle that will be covered, depends on the coordinate system. See
	///				vk2d::RenderCoordinateSpace for more info about what values should be used.
	/// @param[in]	solid
	///				true if the inside of the rectangle is drawn, false for the outline only.
	/// @param[in]	color
	///				Color of the rectangle to be drawn.
	VK2D_API void										VK2D_APIENTRY				DrawRectangle(
		vk2d::Rect2f									area,
		bool											solid						= true,
		vk2d::Colorf									color						= { 1.0f, 1.0f, 1.0f, 1.0f } );

	/// @brief		Draws an ellipse or a circle.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	area
	///				Rectangle area in which the ellipse must fit. See vk2d::RenderCoordinateSpace for
	///				more info about what values should be used.
	/// @param[in]	solid
	///				true to draw the inside of the ellipse/circle, false to draw the outline only.
	/// @param[in]	edge_count
	///				How many corners this ellipse should have, or quality if you prefer. This is a float value for
	///				"smoother" transitions between amount of corners, in case this value is animated.
	/// @param[in]	color
	///				Color of the ellipse/circle to be drawn.
	VK2D_API void										VK2D_APIENTRY				DrawEllipse(
		vk2d::Rect2f									area,
		bool											solid						= true,
		float											edge_count					= 64.0f,
		vk2d::Colorf									color						= { 1.0f, 1.0f, 1.0f, 1.0f } );

	/// @brief		Draws an ellipse or a circle that has a "slice" cut out, similar to usual pie graphs.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	area
	///				Rectangle area in which the ellipse must fit. See vk2d::RenderCoordinateSpace for
	///				more info about what values should be used.
	/// @param[in]	begin_angle_radians
	///				Angle (in radians) where the slice cut should start.
	/// @param[in]	coverage
	///				Size of the slice, value is between 0 to 1 where 0 is not visible and 1 draws the full ellipse.
	/// @param[in]	solid
	///				true to draw the inside of the pie, false to draw the outline only.
	/// @param[in]	edge_count 
	///				How many corners the complete ellipse should have, or quality if you prefer. This is a float value for
	///				"smoother" transitions between amount of corners, in case this value is animated.
	/// @param[in]	color 
	///				Color of the pie to be drawn.
	VK2D_API void										VK2D_APIENTRY				DrawEllipsePie(
		vk2d::Rect2f									area,
		float											begin_angle_radians,
		float											coverage,
		bool											solid						= true,
		float											edge_count					= 64.0f,
		vk2d::Colorf									color						= { 1.0f, 1.0f, 1.0f, 1.0f } );

	/// @brief		Draw a rectangular pie, similar to drawing a rectangle but which has a pie slice cut out.
	/// @note		Multithreading: Main thread only.
	/// @param[in]	area
	///				Area of the rectangle to be drawn. See vk2d::RenderCoordinateSpace for
	///				more info about what values should be used.
	/// @param[in]	begin_angle_radians
	///				Angle (in radians) where the slice cut should start.
	/// @param[in]	coverage 
	///				Size of the slice, value is between 0 to 1 where 0 is not visible and 1 draws the full rectangle.
	/// @param[in]	solid
	///				true to draw the inside of the pie rectangle, false to draw the outline only.
	/// @param[in]	color 
	///				Color of the pie rectangle to be drawn.
	VK2D_API void										VK2D_APIENTRY				DrawRectanglePie(
		vk2d::Rect2f									area,
		float											begin_angle_radians,
		float											coverage,
		bool											solid						= true,
		vk2d::Colorf									color						= { 1.0f, 1.0f, 1.0f, 1.0f } );

	/// @brief		Draws a rectangle with texture and use the size of the texture to determine size of the rectangle.
	/// @warning	!! If window surface coordinate space is normalized, this will not work properly as bottom
	///				right coordinates are offsetted by the texture size !! See vk2d::RenderCoordinateSpace for more info.
	/// @param[in]	location
	///				Draw location of the texture, this is the top left corner of the texture,
	///				depends on the coordinate system. See vk2d::RenderCoordinateSpace for more info.
	/// @param[in]	texture
	///				Texture to draw.
	/// @param[in]	color 
	///				Color multiplier of the texture texel color, eg. If Red color is 0.0 then texture will
	///				lack all Red color, or if alpha channel of the color is 0.5 then texture appears half transparent.
	VK2D_API void										VK2D_APIENTRY				DrawTexture(
		vk2d::Vector2f									location,
		vk2d::Texture								*	texture,
		vk2d::Colorf									color						= { 1.0f, 1.0f, 1.0f, 1.0f } );

	/// @brief		The most useful drawing method. Draws vk2d::Mesh which contains all information needed for the render.
	/// @note		Multithreading: Main thread only.
	/// @see		vk2d::Mesh
	/// @param[in]	mesh
	///				Mesh object to draw.
	/// @param[in]	transformation
	///				Draw using transformation.
	VK2D_API void										VK2D_APIENTRY				DrawMesh(
		const vk2d::Mesh							&	mesh,
		const vk2d::Transform						&	transformations				= {} );

	/// @brief		The most useful drawing method. Draws vk2d::Mesh which contains all information needed for the render.
	/// @note		Multithreading: Main thread only.
	/// @see		vk2d::Mesh
	/// @param[in]	mesh
	///				Mesh object to draw.
	/// @param[in]	transformations
	///				Draw using transformation. This is a std::vector where each element equals one draw.
	///				Using multiple transformations results the mesh being drawn multiple times using
	///				different transformations. This is also called instanced drawing.
	VK2D_API void										VK2D_APIENTRY				DrawMesh(
		const vk2d::Mesh							&	mesh,
		const std::vector<vk2d::Transform>			&	transformation );

	/// @brief		The most useful drawing method. Draws vk2d::Mesh which contains all information needed for the render.
	/// @note		Multithreading: Main thread only.
	/// @see		vk2d::Mesh
	/// @param[in]	mesh
	///				Mesh object to draw.
	/// @param[in]	transformations
	///				Draw using transformation. This is a std::vector where each element equals one draw.
	///				Using multiple transformations results the mesh being drawn multiple times using
	///				different transformations. This is also called instanced drawing.
	VK2D_API void										VK2D_APIENTRY				DrawMesh(
		const vk2d::Mesh							&	mesh,
		const std::vector<vk2d::Matrix4f>			&	transformations );

	/// @brief		VK2D class object checker function.
	/// @note		Multithreading: Any thread.
	/// @return		true if class object was created successfully,
	///				false if something went wrong
	VK2D_API bool										VK2D_APIENTRY				IsGood() const;


public:
	std::unique_ptr<vk2d::_internal::WindowImpl>		impl;
};







/// @brief		Window event handler base class. You can override member methods to receive keyboard, mouse, gamepad, other... events. <br>
///				Example event handler:
/// @code		
///				class MyEventHandler : public vk2d::WindowEventHandler
///				{
///				public:
///					// Keyboard button was pressed, released or kept down ( repeating ).
///					void										VK2D_APIENTRY		EventKeyboard(
///						vk2d::Window						*	window,
///						vk2d::KeyboardButton					button,
///						int32_t									scancode,
///						vk2d::ButtonAction						action,
///						vk2d::ModifierKeyFlags					modifierKeys
///					) override
///					{};
///				};
///	@endcode
///	@note		You must use VK2D_APIENTRY when overriding events.
class WindowEventHandler {
public:
	/// @brief		Window position changed.
	/// @param[in]	window
	///				Which window's position changed.
	/// @param[in]	position
	///				Where the window moved to.
	virtual void								VK2D_APIENTRY		EventWindowPosition(
		vk2d::Window						*	window,
		vk2d::Vector2i							position )
	{};

	/// @brief		Window size changed.
	/// @param[in]	window
	///				Which window's position changed.
	/// @param[in]	size
	///				what's the new size of the window.
	virtual void								VK2D_APIENTRY		EventWindowSize(
		vk2d::Window						*	window,
		vk2d::Vector2u							size )
	{};

	/// @brief		Window wants to close when this event runs, either the user pressed the "X", the OS
	///				wants to close the window, user pressed Alt+F4...
	///				This event is not called when the window is actually closed, only when the window should be. <br>
	///				Default behavior is: @code window->CloseWindow(); @endcode
	///				If you override this function, you'll have to handle closing the window yourself.
	/// @param[in]	window
	///				Window that should be closed.
	virtual void								VK2D_APIENTRY		EventWindowClose(
		vk2d::Window						*	window )
	{
		window->CloseWindow();
	};

	/// @brief		Window refreshed itself. <br>
	///				Not that useful nowadays as modern OSes don't use CPU to draw the windows anymore,
	///				this doesn't trigger often if at all. Might remove this event later.
	/// @param[in]	window
	///				Window that refreshed itself.
	virtual void								VK2D_APIENTRY		EventWindowRefresh(
		vk2d::Window						*	window )
	{};

	/// @brief		Window gained or lost focus. Ie. Became topmost window, or lost the topmost position.
	/// @param[in]	window
	///				Window that became topmost or lost it's position.
	/// @param[in]	focused
	///				true if the window became topmost, false if it lost the topmost position.
	virtual void								VK2D_APIENTRY		EventWindowFocus(
		vk2d::Window						*	window,
		bool									focused )
	{};

	/// @brief		Window was iconified to the taskbar or recovered from there.
	/// @param[in]	window
	///				Window that was iconified or recovered.
	/// @param[in]	iconified
	///				true if the window was iconified, false if recovered from taskbar.
	virtual void								VK2D_APIENTRY		EventWindowIconify(
		vk2d::Window						*	window,
		bool									iconified )
	{};

	/// @brief		Window was maximized or recovered from maximized state.
	/// @param[in]	window
	///				Window that was maximized or recovered from maximized state.
	/// @param[in]	maximized
	///				true if maximized or false if recevered from maximized state.
	virtual void								VK2D_APIENTRY		EventWindowMaximize(
		vk2d::Window						*	window,
		bool									maximized )
	{};


	/// @brief		Mouse button was pressed or released.
	/// @param[in]	window
	///				Window that the mouse click happened in.
	/// @param[in]	button
	///				Which mouse button was clicked or released.
	/// @param[in]	action
	///				Tells if the button was pressed or released.
	/// @param[in]	modifier_keys
	///				What modifier keys were also pressed down when the mouse button was clicked or released.
	virtual void								VK2D_APIENTRY		EventMouseButton(
		vk2d::Window						*	window,
		vk2d::MouseButton						button,
		vk2d::ButtonAction						action,
		vk2d::ModifierKeyFlags					modifier_keys )
	{};

	/// @brief		Mouse moved to a new position on the window.
	/// @param[in]	window
	///				Which window the mouse movement happened in.
	/// @param[in]	position
	///				Tells the new mouse position.
	virtual void								VK2D_APIENTRY		EventCursorPosition(
		vk2d::Window						*	window,
		vk2d::Vector2d							position )
	{};

	/// @brief		Mouse cursor moved on top of the window area, or left it.
	/// @param[in]	window
	///				Which window the mouse cursor entered.
	/// @param[in]	entered
	///				true if entered, false if cursor left the window area.
	virtual void								VK2D_APIENTRY		EventCursorEnter(
		vk2d::Window						*	window,
		bool									entered )
	{};

	/// @brief		Mouse wheel was scrolled.
	/// @param[in]	window
	///				Which window the scrolling happened in.
	/// @param[in]	scroll
	///				Scroll direction vector telling what changed since last event handling.
	///				This is a vector2 because some mice have sideways scrolling. Normal vertical
	///				scrolling is reported in the Y axis, sideways movement in the X axis.
	virtual void								VK2D_APIENTRY		EventScroll(
		vk2d::Window						*	window,
		vk2d::Vector2d							scroll )
	{};

	/// @brief		Keyboard button was pressed, released or kepth down (repeating).
	/// @param[in]	window
	///				Which window the keyboard event happened in.
	/// @param[in]	button
	///				Keyboard button that was pressed, released or is repeating.
	/// @param[in]	scancode
	///				Raw scancode from the keyboard, may change from platform to platform.
	/// @param[in]	action
	///				Tells if the button was pressed or released or repeating.
	/// @param[in]	modifier_keys
	///				What modifier keys were also kept down.
	virtual void								VK2D_APIENTRY		EventKeyboard(
		vk2d::Window						*	window,
		vk2d::KeyboardButton					button,
		int32_t									scancode,
		vk2d::ButtonAction						action,
		vk2d::ModifierKeyFlags					modifier_keys )
	{};

	/// @brief		Text input event, use this if you want to know the character that was received from
	///				combination of keyboard presses. Character is in UTF-32 format.
	/// @param[in]	window
	///				Window where we're directing text input to.
	/// @param[in]	character
	///				Resulting combined character from the OS. Eg. A = 'a', Shift+A = 'A'.
	///				This also takes into consideration the region and locale setting of the
	///				OS and keyboard.
	/// @param[in]	modifier_keys
	///				What modifier keys were pressed down when character event was generated.
	virtual void								VK2D_APIENTRY		EventCharacter(
		vk2d::Window						*	window,
		uint32_t								character,
		vk2d::ModifierKeyFlags					modifier_keys )
	{};


	/// @brief		File was drag-dropped onto the window.
	/// @param[in]	window
	///				Window where files were dragged and dropped onto.
	/// @param[in]	files
	///				List of file paths.
	virtual void								VK2D_APIENTRY		EventFileDrop(
		vk2d::Window						*	window,
		std::vector<std::filesystem::path>		files )
	{};

	/// @brief		Screenshot event, called when screenshot was successfully saved to disk
	///				or it's ready for manual manipulation or if there was an error somewhere.
	///				Screenshots can either be saved to disk directly or the image data can be returned here.
	///				Taking screenshots is multithreaded operation and will take a while to process,
	///				so we need to use this event to notify when it's ready.
	/// @param[in]	window
	///				Window where the screenshot was taken from.
	/// @param[in]	screenshot_path
	///				Screenshot save path if screenshot was saved to disk.
	/// @param[in]	screenshot_data
	///				Screenshot data if screenshot was not automatically saved to disk.
	///				WARNING! Returned data will be destroyed as soon as this event finishes,
	///				so if you want to further modify the data you'll have to copy it somewhere else first.
	/// @param[in]	success
	///				true if taking the screenshot was successful, either saved to file or returned here as data.
	///				false if there was an error.
	/// @param[in]	error_message
	///				if success was false then this tells what kind of error we encountered.
	virtual void								VK2D_APIENTRY		EventScreenshot(
		vk2d::Window						*	window,
		const std::filesystem::path			&	screenshot_path,
		const vk2d::ImageData				&	screenshot_data,
		bool									success,
		const std::string					&	error_message )
	{};
};





}
