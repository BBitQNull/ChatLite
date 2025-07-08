add_executable(server)

target_sources(server PRIVATE
    server.cc
    ${PROJECT_SOURCE_DIR}/common/mem_reset.cc
    logic_unit.cc
    io_handler.cc
    broadcast_message.cc
)

target_include_directories(server PRIVATE ${PROJECT_SOURCE_DIR}/include/server)