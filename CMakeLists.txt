cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_SUPPRESS_REGENERATION true)
project (SimpleMulticastChat)
add_executable(SimpleMulticastChat SimpleMulticastChat.cpp)

if (${UNIX})
    # *nix-specific includes or actions
    message("Linux stuff")
    target_link_libraries(SimpleMulticastChat pthread)
elseif (${WIN32})
  # Windows-specific includes or actions
  message("Windows stuff")
elseif (${APPLE})
  # ...
endif (${UNIX})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SimpleMulticastChat)
