set(common_SOURCES
    logger.cpp
)

foreach(f IN LISTS common_SOURCES)
    set_source_files_properties(${f} PROPERTIES
        COMPILE_DEFINITIONS "__FNAME__=\"common/${f}\"")
endforeach()

add_library(jim-common STATIC ${common_SOURCES})
add_dependencies(jim-common build-3rd)
install (TARGETS jim-common DESTINATION lib)
