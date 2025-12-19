add_library(stage_uppercase_transform SHARED
        uppercase_transform.cc
)

target_include_directories(stage_uppercase_transform PRIVATE
        /opt/flow-pipe/include
)

set_target_properties(stage_uppercase_transform PROPERTIES
        OUTPUT_NAME stage_uppercase_transform
)

install(TARGETS stage_uppercase_transform
        LIBRARY DESTINATION /opt/flow-pipe/plugins
)
