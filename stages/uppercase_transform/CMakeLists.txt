cmake_minimum_required(VERSION 3.20)

project(stage_uppercase_transform LANGUAGES CXX)

# ------------------------------------------------------------
# Locate installed Flow-Pipe runtime
# ------------------------------------------------------------
# This assumes the runtime was installed to /opt/flow-pipe
# (as done by your Docker images)
list(APPEND CMAKE_PREFIX_PATH "/opt/flow-pipe")

find_package(flowpipe REQUIRED)

# ------------------------------------------------------------
# Stage plugin
# ------------------------------------------------------------
add_library(stage_uppercase_transform SHARED
        uppercase_transform.cc
)

# Flow-Pipe public headers
target_include_directories(stage_uppercase_transform
        PRIVATE
        /opt/flow-pipe/include
)

# ------------------------------------------------------------
# Link against runtime shared library
# ------------------------------------------------------------
target_link_libraries(stage_uppercase_transform
        PRIVATE
        flowpipe::flowpipe_runtime
)

# ------------------------------------------------------------
# C++ standard
# ------------------------------------------------------------
target_compile_features(stage_uppercase_transform
        PRIVATE
        cxx_std_20
)

# ------------------------------------------------------------
# Warnings (recommended)
# ------------------------------------------------------------
target_compile_options(stage_uppercase_transform
        PRIVATE
        -Wall -Wextra -Wpedantic
)

# ------------------------------------------------------------
# Hide all symbols by default (ABI safety)
# ------------------------------------------------------------
set_target_properties(stage_uppercase_transform PROPERTIES
        OUTPUT_NAME stage_uppercase_transform
        PREFIX "lib"
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES

        # Ensure runtime .so is found relative to plugins dir
        INSTALL_RPATH "$ORIGIN/../lib"
)

# ------------------------------------------------------------
# Install
# ------------------------------------------------------------
install(TARGETS stage_uppercase_transform
        LIBRARY DESTINATION /opt/flow-pipe/plugins
)
