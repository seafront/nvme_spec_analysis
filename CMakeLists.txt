cmake_minimum_required(VERSION 3.16)
project(NVMeLib VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler settings - stored for later use on project targets only
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Using Clang compiler: ${CMAKE_CXX_COMPILER}")
    if(WIN32)
        set(PROJECT_COMPILE_OPTIONS
            -Wall
            -Wextra
            -Wno-unused-parameter
            -Wno-missing-field-initializers
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-c++98-c++11-compat-binary-literal
            -Wno-padded
            -Wno-unsafe-buffer-usage
        )
    else()
        set(PROJECT_COMPILE_OPTIONS
            -Wall
            -Wextra
            -Wpedantic
            -Werror=return-type
            -Wno-unused-parameter
        )
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message(STATUS "Using GCC compiler: ${CMAKE_CXX_COMPILER}")
    set(PROJECT_COMPILE_OPTIONS
        -Wall
        -Wextra
        -Wpedantic
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(STATUS "Using MSVC compiler")
    set(PROJECT_COMPILE_OPTIONS /W4)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/header)

# Source files
set(NVME_SOURCES
    src/NVMeDevice.cpp
)

# Header files (for IDE integration)
set(NVME_HEADERS
    header/nvme.h
    header/nvme_abort.h
    header/nvme_async_event_request.h
    header/nvme_capacity_management.h
    header/nvme_controller_data_queue.h
    header/nvme_device_self_test.h
    header/nvme_directive_receive.h
    header/nvme_directive_send.h
    header/nvme_firmware_commit.h
    header/nvme_firmware_image_download.h
    header/nvme_format_nvm.h
    header/nvme_get_features.h
    header/nvme_get_log_page.h
    header/nvme_identify.h
    header/nvme_keep_alive.h
    header/nvme_migration_receive.h
    header/nvme_migration_send.h
    header/nvme_namespace_attachment.h
    header/nvme_namespace_management.h
    header/nvme_pcie_admin_commands.h
    header/nvme_sanitize.h
    header/nvme_security.h
    header/nvme_set_features.h
)

# Static Library
add_library(nvme_static STATIC ${NVME_SOURCES} ${NVME_HEADERS})
target_include_directories(nvme_static PUBLIC ${CMAKE_SOURCE_DIR}/header)
target_compile_options(nvme_static PRIVATE ${PROJECT_COMPILE_OPTIONS} ${COVERAGE_COMPILE_FLAGS})
target_link_options(nvme_static PRIVATE ${COVERAGE_LINK_FLAGS})
set_target_properties(nvme_static PROPERTIES OUTPUT_NAME nvme)

# Shared Library (optional)
option(BUILD_SHARED_LIBS "Build shared library" OFF)
if(BUILD_SHARED_LIBS)
    add_library(nvme_shared SHARED ${NVME_SOURCES} ${NVME_HEADERS})
    target_include_directories(nvme_shared PUBLIC ${CMAKE_SOURCE_DIR}/header)
    target_compile_options(nvme_shared PRIVATE ${PROJECT_COMPILE_OPTIONS})
    set_target_properties(nvme_shared PROPERTIES OUTPUT_NAME nvme)
endif()

# Code coverage option
option(ENABLE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(STATUS "Code coverage enabled (LLVM)")
        set(COVERAGE_COMPILE_FLAGS -fprofile-instr-generate -fcoverage-mapping)
        set(COVERAGE_LINK_FLAGS -fprofile-instr-generate)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        message(STATUS "Code coverage enabled (gcov)")
        set(COVERAGE_COMPILE_FLAGS --coverage -fprofile-arcs -ftest-coverage)
        set(COVERAGE_LINK_FLAGS --coverage)
    endif()
endif()

# Test executable (optional)
option(BUILD_TESTS "Build test executable" ON)
if(BUILD_TESTS)
    # Fetch Google Test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    # Test source files
    set(TEST_SOURCES
        test/main.cpp
        test/NVMeDeviceTest.cpp
        test/IdentifyTest.cpp
        test/GetSetFeaturesTest.cpp
        test/GetLogPageTest.cpp
        test/FirmwareTest.cpp
        test/NamespaceManagementTest.cpp
        test/FormatNVMTest.cpp
        test/SanitizeTest.cpp
        test/LockdownTest.cpp
        test/QueueManagementTest.cpp
        test/MiscCommandsTest.cpp
        test/StructureSizeTest.cpp
    )

    add_executable(nvme_test ${TEST_SOURCES})
    target_link_libraries(nvme_test PRIVATE nvme_static GTest::gtest)
    target_compile_options(nvme_test PRIVATE ${PROJECT_COMPILE_OPTIONS} ${COVERAGE_COMPILE_FLAGS} -Wno-global-constructors -Wno-exit-time-destructors)
    target_link_options(nvme_test PRIVATE ${COVERAGE_LINK_FLAGS})

    include(GoogleTest)
    gtest_discover_tests(nvme_test)
endif()

# Install targets
install(TARGETS nvme_static
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(FILES ${NVME_HEADERS} DESTINATION include/nvme)

# Print configuration summary
message(STATUS "")
message(STATUS "=== NVMeLib Build Configuration ===")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message(STATUS "BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
message(STATUS "BUILD_TESTS: ${BUILD_TESTS}")
message(STATUS "ENABLE_COVERAGE: ${ENABLE_COVERAGE}")
message(STATUS "===================================")
message(STATUS "")
