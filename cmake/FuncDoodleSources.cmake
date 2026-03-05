set(FUNCDOODLE_GLAD_SOURCES
	lib/glad/src/glad.c
)

set(FUNCDOODLE_IMGUI_SOURCES
	lib/imgui/imgui.cpp
	lib/imgui/imgui_draw.cpp
	lib/imgui/imgui_widgets.cpp
	lib/imgui/imgui_demo.cpp
	lib/imgui/imgui_tables.cpp
	lib/imgui/backends/imgui_impl_glfw.cpp
	lib/imgui/backends/imgui_impl_opengl3.cpp
)

function(funcdoodle_set_nfd_sources out_var)
	if(APPLE)
		set(_nfd lib/nfd/src/nfd_cocoa.m lib/nfd/src/nfd_common.c)
	elseif(WIN32)
		set(_nfd lib/nfd/src/nfd_win.cpp lib/nfd/src/nfd_common.c)
	elseif(UNIX AND NOT APPLE)
		set(_nfd lib/nfd/src/nfd_gtk.c lib/nfd/src/nfd_common.c)
	endif()

	set(${out_var} ${_nfd} PARENT_SCOPE)
endfunction()
