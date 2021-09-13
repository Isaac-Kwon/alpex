

set(TEST_EXE_ROOT
  makePass1
)
# analysisRunList

# for now put the executables in the build root
# for compatibility with existing scripts
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

foreach(exe ${TEST_EXE_ROOT})
  add_executable(${exe} ${exe}.cpp ${sources} ${headers})
  # target_include_directories(${exe} PRIVATE ${ROOT_INCLUDE_DIRS})
  set_property(TARGET ${exe} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
  target_link_libraries(${exe} alpex ROOT::Core ROOT::Gpad ROOT::Hist ROOT::Tree)
endforeach()

#include_directories(include)
message(STATUS "Target Executable: ${TEST_EXE_ROOT}")

# install(TARGETS
#   ${TEST_EXE_ROOT}
#   RUNTIME DESTINATION bin
# )
