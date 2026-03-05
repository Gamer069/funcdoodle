function(funcdoodle_set_portaudio_defaults)
	set(PORTAUDIO_LIBDIR "" CACHE STRING "required")
	set(PORTAUDIO_INCLDIR "" CACHE STRING "required")

	if(NOT PORTAUDIO_LIBDIR)
		if(APPLE)
			set(PORTAUDIO_LIBDIR "/usr/local/opt/portaudio/lib/")
		elseif(UNIX AND NOT APPLE)
			set(PORTAUDIO_LIBDIR "/usr/lib/")
		elseif(WIN32)
			set(PORTAUDIO_LIBDIR "C:\\portaudio\\lib")
		endif()
	endif()

	if(NOT PORTAUDIO_INCLDIR)
		if(APPLE)
			set(PORTAUDIO_INCLDIR "/usr/local/opt/portaudio/include")
		elseif(UNIX AND NOT APPLE)
			set(PORTAUDIO_INCLDIR "/usr/local/include")
		elseif(WIN32)
			set(PORTAUDIO_INCLDIR "C:\\portaudio\\incl")
		endif()
	endif()

	set(PORTAUDIO_LIBDIR "${PORTAUDIO_LIBDIR}" PARENT_SCOPE)
	set(PORTAUDIO_INCLDIR "${PORTAUDIO_INCLDIR}" PARENT_SCOPE)
endfunction()

function(funcdoodle_apply_platform_links target)
	if(UNIX)
		target_link_libraries(${target}
			glfw
			${CMAKE_DL_LIBS}
			m
			pthread
			dl
		)
	elseif(WIN32)
		target_link_libraries(${target}
			glfw
			${CMAKE_DL_LIBS}
		)
	endif()

	if(UNIX AND NOT APPLE)
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
		target_link_libraries(${target} ${GTK3_LIBRARIES})
		target_include_directories(${target} PRIVATE ${GTK3_INCLUDE_DIRS})
	endif()

	if(APPLE)
		target_link_libraries(${target}
			${PORTAUDIO_LIBDIR}libportaudio.dylib
			"-framework OpenGL"
			"-framework Cocoa"
			"-framework IOKit"
			"-framework CoreFoundation"
			"-framework AppKit"
		)
	elseif(UNIX AND NOT APPLE)
		target_link_libraries(${target}
			${PORTAUDIO_LIBDIR}libportaudio.so
		)
	elseif(WIN32)
		target_link_libraries(${target}
			${CMAKE_DL_LIBS}
			${PORTAUDIO_LIBDIR}portaudio_x64.lib
		)
	endif()
endfunction()
