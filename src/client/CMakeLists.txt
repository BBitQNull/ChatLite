add_executable(client)

target_sources(client PRIVATE
    client.cc
    ${PROJECT_SOURCE_DIR}/common/mem_reset.cc
)

target_include_directories(client PRIVATE ${PROJECT_SOURCE_DIR}/include/client)