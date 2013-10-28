cmake_minimum_required(VERSION 2.8)

project(Channels)

include (CheckIncludeFiles)

set(USE_MLANG ON)

set(SOURCE_DIR "src")
set(HEADERS_DIR "include")

set(BOOST_ROOT "../boost_1_54_0")
#set(BOOST_STAGE "${boost_DIR}/stage/gcc/lib")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
	message(FATAL_ERROR
	"Channels need Boost library headers. MLang based char-set converter also needs boost system")
endif()

if( ${USE_ICONV})
	find_package(iconv REQUIRED)
	set(CONV_ENGINE_HEADERS ${iconv_HEADERS})
endif()

if( ${USE_MLANG} )
	check_include_files("Mlang.h" HAVE_MLANG_H)
	set(CONV_ENGINE_HEADERS "")
	#set(CONV_ENGINE_LIBS boost_SYSTEM)
	if(NOT(${HAVE_MLANG}))
		message(FATAL_ERROR "No Mlang headers found")
	endif()
endif()

if( ${USE_ICU} )
	find_package(icu REQUIRED)
endif()

include_directories(${HEADERS_DIR} ${Boost_INCLUDEDIR})

add_library(Channels SHARED ${SOURCE_DIR})
