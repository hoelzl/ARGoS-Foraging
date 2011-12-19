link_directories(${CMAKE_BUILD_DIR}/build/loop_functions/evolution_loop_functions)
add_executable(argos_galib main.cpp)
target_link_libraries(argos_galib ${GALIB_LIBRARIES} evolution_loop_functions argos2_simulator)
