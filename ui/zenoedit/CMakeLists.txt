find_package(Qt5 COMPONENTS Widgets OpenGL Network REQUIRED)

message(STATUS "Found Qt5 version ${Qt5_VERSION}: ${Qt5_DIR}")

#add_custom_target(
#    clean_resource_file
#    COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_BINARY_DIR}/ui/zenoedit/qrc_font.o
#    COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_BINARY_DIR}/ui/zenoedit/qrc_icons.o
#)

file(GLOB_RECURSE source CONFIGURE_DEPENDS *.h *.cpp *.ui)
file(GLOB_RECURSE resource CONFIGURE_DEPENDS *.qrc)
file(GLOB_RECURSE bigresource CONFIGURE_DEPENDS *icons.qrc *font.qrc)
FOREACH(item ${resource})
    IF(item IN_LIST bigresource)
        LIST(REMOVE_ITEM resource ${item})
    ENDIF(item IN_LIST bigresource)
ENDFOREACH(item)

qt5_add_big_resources(source ${bigresource})  # cihou msvc: fatal error C1060: compiler is out of heap space
qt5_add_resources(source ${resource})

if (WIN32)
    enable_language("RC")
    set (WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/zenoedit.rc)
endif()

add_executable(zenoedit ${source} ${WIN32_RESOURCES})
# Add a dependency to ensure that the resource file is deleted before being regenerated

# add_dependencies(zenoedit clean_resource_file)

set_property(TARGET zenoedit PROPERTY AUTOUIC ON)
set_property(TARGET zenoedit PROPERTY AUTOMOC ON)
set_property(TARGET zenoedit PROPERTY AUTORCC ON)
set_property(TARGET zenoedit PROPERTY WIN32_EXECUTABLE OFF)

if (ZENO_DEBUG_MSVC)
    zeno_dbg_msvc(zenoedit)
endif()

if (NOT ZENO_MULTIPROCESS OR NOT ZENO_IPC_USE_TCP)
    find_package(Threads REQUIRED)
    target_link_libraries(zenoedit PRIVATE Threads::Threads)
endif()
target_link_libraries(zenoedit PRIVATE Qt5::Widgets Qt5::OpenGL Qt5::Network)

if (WIN32)
    target_link_libraries(zenoedit PRIVATE Dbghelp Version)
endif()
target_link_libraries(zenoedit PRIVATE zenoui zenoio zenomodel zenovis)
target_link_libraries(zenoedit PRIVATE zeno)
target_include_directories(zenoedit PRIVATE . .. ../zenoui ../include)
target_include_directories(zenoedit PRIVATE ${Qt5Widgets_PRIVATE_INCLUDE_DIRS})
if(ZENO_LIVESYNC)
    find_package(Crow CONFIG REQUIRED)
    target_link_libraries(zenoedit PRIVATE Crow::Crow)
    target_compile_definitions(zenoedit PRIVATE -DZENO_LIVESYNC)
endif()

if (ZENO_MULTIPROCESS)
    target_compile_definitions(zenoedit PRIVATE -DZENO_MULTIPROCESS)
    if (ZENO_IPC_USE_TCP)
        target_compile_definitions(zenoedit PRIVATE -DZENO_IPC_USE_TCP)
    endif()
endif()

if (ZENO_INSTALL_TARGET)
    install(TARGETS zenoedit EXPORT ZenoTargets)
endif()
