add_executable(client)

target_sources(client PRIVATE
    client.cc
)

target_include_directories(client PUBLIC ${PROJECT_SOURCE_DIR}/include/client)