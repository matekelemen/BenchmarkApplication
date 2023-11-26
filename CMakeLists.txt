project(BenchmarkApplication CXX)

find_package(benchmark REQUIRED)

# Generate sources from templates
set(KRATOS_BENCHMARK_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
file(GLOB source_templates "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp.in")
foreach(source_template ${source_templates})
    # Configure and output to intermediate file (read-write permissions)
    get_filename_component(source_template_name "${source_template}" NAME_WE)
    configure_file("${source_template}" "${CMAKE_CURRENT_SOURCE_DIR}/src/${source_template_name}.cpp")

    # Withdraw write permission from the generated file
    file(CHMOD
         "${CMAKE_CURRENT_SOURCE_DIR}/src/${source_template_name}.cpp"
         FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
endforeach()

file(GLOB sources src/*.cpp)
add_executable(KratosBenchmark ${sources})

target_include_directories(KratosBenchmark PRIVATE
                           "${KRATOS_SOURCE_DIR}/kratos"
                           "${CMAKE_CURRENT_SOURCE_DIR}/include"
                           "${KRATOS_SOURCE_DIR}/applications/MedApplication")

target_link_libraries(KratosBenchmark
                      PRIVATE
                      KratosCore
                      KratosMedCore
                      benchmark::benchmark)
