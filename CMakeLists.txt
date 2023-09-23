project(BenchmarkApplication CXX)

find_package(benchmark)

# Generate sources from templates
set(KRATOS_BENCHMARK_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
file(GLOB source_templates "${CMAKE_CURRENT_SOURCE_DIR}/custom_utilities/*.cpp.in")
foreach(source_template ${source_templates})
    # Configure and output to intermediate file (read-write permissions)
    get_filename_component(source_template_name "${source_template}" NAME_WE)
    configure_file("${source_template}" "${CMAKE_CURRENT_SOURCE_DIR}/custom_utilities/${source_template_name}.cpp")

    # Withdraw write permission from the generated file
    file(CHMOD
         "${CMAKE_CURRENT_SOURCE_DIR}/custom_utilities/${source_template_name}.cpp"
         FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
endforeach()

include_directories("${KRATOS_SOURCE_DIR}/kratos" "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB library_sources custom_utilities/*.cpp)
add_library(BenchmarkCore ${library_sources})

add_executable(distance_benchmark "${CMAKE_CURRENT_SOURCE_DIR}/drivers/distance_benchmark.cpp")
target_link_libraries(distance_benchmark PRIVATE
                      KratosCore
                      BenchmarkCore
                      benchmark::benchmark)

add_executable(profiler_benchmark "${CMAKE_CURRENT_SOURCE_DIR}/drivers/profiler_benchmark.cpp")
target_link_libraries(profiler_benchmark PRIVATE
                      KratosCore
                      BenchmarkCore
                      benchmark::benchmark)

add_executable(expression_benchmark "${CMAKE_CURRENT_SOURCE_DIR}/drivers/expression_benchmark.cpp")
target_link_libraries(expression_benchmark PRIVATE
                      KratosCore
                      BenchmarkCore
                      benchmark::benchmark)

add_executable(med_benchmark "${CMAKE_CURRENT_SOURCE_DIR}/drivers/med_benchmark.cpp")
target_link_libraries(med_benchmark PRIVATE
                      KratosCore
                      BenchmarkCore
                      KratosMedCore
                      benchmark::benchmark)
target_include_directories(med_benchmark PRIVATE
                           "${KRATOS_SOURCE_DIR}/applications/MedApplication")
