cmake_minimum_required(VERSION 3.10)
project(raspi-dashboard-server C CXX)

find_package(SDL2 REQUIRED CONFIG)

set (RASPI_DASHBOARD_SERVER_SRCS
    src/main.cpp
)

set (RASPI_DASHBOARD_SERVER_HEADERS

)

set (RASPI_DASHBOARD_SERVER_INCLUDE_DIRS
    ${SDL_INCLUDE_DIRS}
    vendor/clay
)

set (RASPI_DASHBOARD_SERVER_LIBRARIES
    SDL2::SDL2
)

add_executable(raspi-dashboard-server ${RASPI_DASHBOARD_SERVER_SRCS} ${RASPI_DASHBOARD_SERVER_HEADERS})
target_link_libraries(raspi-dashboard-server PRIVATE ${RASPI_DASHBOARD_SERVER_LIBRARIES})
target_include_directories(raspi-dashboard-server PUBLIC ${RASPI_DASHBOARD_SERVER_INCLUDE_DIRS} src)

