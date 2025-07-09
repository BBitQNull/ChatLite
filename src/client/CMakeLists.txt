add_executable(client)

target_sources(client PRIVATE
    client.cc
    log_in.cc
    log_out.cc
    message_handler.cc
    username_input.cc
)

target_include_directories(client PRIVATE ${PROJECT_SOURCE_DIR}/include/client)
target_include_directories(client PRIVATE ${PROJECT_SOURCE_DIR}/common)