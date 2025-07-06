add_executable(server)

target_sources(server PRIVATE
    server.cc
)

target_include_directories(server PUBLIC ${PROJECT_SOURCE_DIR}/include/server)

