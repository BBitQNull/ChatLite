add_executable(client)

target_sources(client PRIVATE
    client.cc
)

target_include_directories(client PRIVATE ${PROJECT_SOURCE_DIR}/include/client)