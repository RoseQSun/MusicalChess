# Example GUI App CMakeLists.txt

# To get started on a new GUI app, copy this entire folder (containing this file and C++ sources) to
# a convenient location, and then start making modifications. For other examples of CMakeLists for
# GUI apps, check `extras/Projucer` and `examples/DemoRunner` in the JUCE repo.

# The first line of any CMake project should be a call to `cmake_minimum_required`, which checks
# that the installed CMake will be able to understand the following CMakeLists, and ensures that
# CMake's behaviour is compatible with the named version. This is a standard CMake command, so more
# information can be found in the CMake docs.

cmake_minimum_required(VERSION 3.15)

# The top-level CMakeLists.txt file for a project must contain a literal, direct call to the
# `project()` command. `project()` sets up some helpful variables that describe source/binary
# directories, and the current project version. This is a standard CMake command.

project(MUSICAL_CHESS VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

include_directories(thirdparty thirdparty/catch2)

# If you've installed JUCE somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of JUCE. If you've
# included JUCE directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.

# find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system
# or
add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE

# If your app depends the VST2 SDK, perhaps to host VST2 plugins, CMake needs to be told where
# to find the SDK on your system. This setup should be done before calling `juce_add_gui_app`.

# juce_set_vst2_sdk_path(...)

# `juce_add_gui_app` adds an executable target with the name passed as the first argument
# (GuiAppExample here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. This function accepts many optional arguments. Check the readme at
# `docs/CMake API.md` in the JUCE repo for the full list.

file(GLOB IMAGES assets/images/*.png)
juce_add_binary_data(ChessImageData NAMESPACE ChessImageData HEADER_NAME ChessImageData.h SOURCES ${IMAGES})

file(GLOB SOUNDS assets/sounds/*.ogg)
juce_add_binary_data(ChessSoundData NAMESPACE ChessSoundData HEADER_NAME ChessSoundData.h SOURCES ${SOUNDS})

juce_add_gui_app(MusicalChess
    # VERSION ...                       # Set this if the app version is different to the project version
    # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
    # ICON_SMALL ...
    # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
    # COMPANY_NAME ...                  # Specify the name of the app's author
    PRODUCT_NAME "MusicalChess"
    NEEDS_CURL TRUE)

juce_add_console_app(Tests
    PRODUCT_NAME "Tests")

juce_add_console_app(EngineTest
    PRODUCT_NAME "EngineTest")

juce_add_console_app(StreamTest
    PRODUCT_NAME "StreamTest"
    NEEDS_CURL TRUE)
juce_add_console_app(PGNTest
		PRODUCT_NAME "PGNTest")


juce_add_console_app(TerminalChess
    PRODUCT_NAME "TerminalChess")

juce_add_console_app(SonifierTest
		PRODUCT_NAME "SonifierTest")

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

# juce_generate_juce_header(GuiAppExample)

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

add_library(Common OBJECT src/GameState.cpp src/Util.cpp)

target_sources(MusicalChess
    PRIVATE
        src/BoardComponent.cpp
        src/CommentarySonifier.cpp
        src/EngineBridge.cpp
        src/GameStream.cpp
        src/Main.cpp
        src/MainComponent.cpp
        src/MainProcessor.cpp
        src/PGNParser.cpp
        src/Scheduler.cpp
        src/SoundProcessor.cpp
        src/StorySonifier.cpp
        src/ThreatsSonifier.cpp
        src/Waveform.cpp
        src/ZenSonifier.cpp)

target_sources(Tests
    PRIVATE
        src/MainProcessor.cpp
        src/SoundProcessor.cpp
        src/Scheduler.cpp
        src/Waveform.cpp
        tests/TestGameState.cpp
        tests/TestSynthesis.cpp
        tests/main.cpp)

target_sources(TerminalChess
    PRIVATE
        tests/TerminalChess.cpp)

target_sources(EngineTest
    PRIVATE
        src/EngineBridge.cpp
        tests/TestEngineBridge.cpp)

target_sources(StreamTest
    PRIVATE
        src/GameStream.cpp
        tests/Stream.cpp)

target_sources(SonifierTest
	PRIVATE
		src/SoundProcessor.cpp
        src/MainProcessor.cpp
        src/Scheduler.cpp
        src/SoundProcessor.cpp
        src/Waveform.cpp
		src/ZenSonifier.cpp
		tests/SonifierTest.cpp)


target_sources(PGNTest
		PRIVATE
			src/PGNParser.cpp
			src/PGNParser.h
#			src/GameState.cpp
#			src/GameState.h
			tests/TestPGNParser.cpp)

# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.

target_compile_definitions(MusicalChess
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=1     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_USE_OGG=1
	JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:MusicalChess,JUCE_PRODUCT_NAME>"
	JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:MusicalChess,JUCE_VERSION>")

target_compile_definitions(Tests
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0)    # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call

target_compile_definitions(StreamTest
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=1)    # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call    

target_compile_definitions(SonifierTest
		PRIVATE
		# JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
		JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
		JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
		JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:MusicalChess,JUCE_PRODUCT_NAME>"
		JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:MusicalChess,JUCE_VERSION>")



target_compile_definitions(PGNTest
		PRIVATE
		JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
		JUCE_USE_CURL=0)


# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# juce_add_binary_data(GuiAppData SOURCES ...)

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_gui_extra` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.

target_link_libraries(MusicalChess
    PRIVATE
        ChessImageData
        ChessSoundData
        Common
        juce::juce_gui_extra
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

target_link_libraries(Tests
    PRIVATE
        Common
        juce::juce_gui_extra
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

target_link_libraries(TerminalChess
    PRIVATE
        Common
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

target_link_libraries(EngineTest
    PRIVATE
        Common
        pthread)

target_link_libraries(StreamTest
    PRIVATE
        Common
        juce::juce_gui_extra
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

target_link_libraries(SonifierTest
    PRIVATE
        Common
        juce::juce_gui_extra
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

target_link_libraries(PGNTest
	PRIVATE
		Common
		juce::juce_gui_extra
		juce::juce_audio_utils
	PUBLIC
		juce::juce_recommended_config_flags
		juce::juce_recommended_lto_flags
		juce::juce_recommended_warning_flags)

        
