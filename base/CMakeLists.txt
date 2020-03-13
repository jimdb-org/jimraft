set(base_SOURCES
    fs_util.cpp
    status.cpp
    util.cpp
)

add_library(jim-base STATIC ${base_SOURCES})
add_dependencies(jim-base build-3rd)

install (TARGETS jim-base DESTINATION lib)
install (FILES status.h DESTINATION include/jim/base)
