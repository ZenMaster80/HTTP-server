# CMakeList.txt : CMake project for HTTP-server, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (HTTP-server "HTTP-server.cpp" "HTTP-server.h" "main.cpp")

set_property(TARGET HTTP-server PROPERTY CXX_STANDARD 20)
set_property(TARGET HTTP-server PROPERTY CXX_STANDARD_REQUIRED ON)

if (WIN32)
	target_link_libraries(HTTP-server PRIVATE ws2_32)

endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  


endif()

# TODO: Add tests and install targets if needed.
