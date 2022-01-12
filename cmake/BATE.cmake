set(ZENO_WITH_LEGACY OFF)
set(ZENO_WITH_BACKWARD ON)
set(ZENO_BUILD_WORKER ON)
set(ZENO_BUILD_EDITOR ON)
set(ZENO_BUILD_DESIGNER ON)
set(ZENO_BUILD_TESTS OFF)
set(ZENO_BUILD_BENCHMARK OFF)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake/hipSYCL)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#add_custom_target(run COMMAND $<TARGET_FILE:zeno_worker> && (echo -n . && read -n1) || (echo -n ! && read -n1))
#add_dependencies(run zeno_worker)
add_custom_target(run COMMAND $<TARGET_FILE:zeno_editor> && (echo -n . && read -n1) || (echo -n ! && read -n1))
add_dependencies(run zeno_editor)
#add_compile_options(-fdiagnostics-color=always)
