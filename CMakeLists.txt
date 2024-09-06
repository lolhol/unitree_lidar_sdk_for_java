cmake_minimum_required(VERSION 3.0)
project(unitree_lidar_sdk)
find_package(Boost COMPONENTS iostreams REQUIRED)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -DNDEBUG") 

include_directories(include)
include_directories(/usr/lib/jvm/java-1.17.0-openjdk-amd64/include)
include_directories(/usr/lib/jvm/java-1.17.0-openjdk-amd64/include/linux)

file(GLOB UNTREELIDAR "include/*.h" "java/brigero/*.cpp")

add_library(unitreelidar4java SHARED ${UNTREELIDAR}
        java/brigero/jni_utils.h)
target_link_libraries(unitreelidar4java  libunitree_lidar_sdk.a)

set_target_properties(unitreelidar4java PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
install(TARGETS unitreelidar4java DESTINATION /usr/lib/jni)

link_directories(lib/${CMAKE_SYSTEM_PROCESSOR})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${CMAKE_SYSTEM_PROCESSOR})

add_executable(example_lidar
  examples/example_lidar.cpp
)
target_link_libraries(example_lidar  libunitree_lidar_sdk.a )

add_executable(example_lidar_udp
  examples/example_lidar_udp.cpp
)
target_link_libraries(example_lidar_udp  libunitree_lidar_sdk.a )

add_executable(unilidar_publisher_udp
  examples/unilidar_publisher_udp.cpp
)
target_link_libraries(unilidar_publisher_udp  libunitree_lidar_sdk.a )

add_executable(unilidar_subscriber_udp
  examples/unilidar_subscriber_udp.cpp
)
target_link_libraries(unilidar_subscriber_udp  libunitree_lidar_sdk.a )

add_custom_target(JavaCompile
    COMMAND javac -d ../java ../java/brigero/*.java
)

add_custom_target(JavaJarify
    COMMAND cd ../java && find brigero -name "*.class" | xargs jar cvf ../UnitreeLidar4Java.jar && cd ./brigero && rm -rf "*.class"
)

add_custom_target(CleanUpJavaCompile
    COMMAND cd ../java/brigero && rm -rf "*.class"
)

add_dependencies(JavaJarify JavaCompile)
