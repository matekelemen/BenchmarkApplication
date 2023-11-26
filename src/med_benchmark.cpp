/// @author Máté Kelemen

// --- External Includes ---
#include "benchmark/benchmark.h"

// --- Benchmark Includes ---
#include "BenchmarkApplication/cmake_variables.hpp"

// --- Med Includes ---
#include "custom_io/med_model_part_io.h"

// --- Core Includes ---
#include "containers/model.h"
#include "includes/model_part.h"
#include "includes/kratos_application.h"

// --- STL Includes ---
#include <filesystem>


void LoadMedFresh(benchmark::State& rState)
{
    // Initialize kratos
    Kratos::KratosApplication kratos("core");
    kratos.Register();

    for ([[maybe_unused]] auto dummy : rState) {
        rState.PauseTiming();
        Kratos::Model model;
        auto& r_model_part = model.CreateModelPart("root");
        rState.ResumeTiming();

        Kratos::MedModelPartIO((Kratos::GetBenchmarkDataDir() / "cube_quadratic_tets.med").string()).ReadModelPart(r_model_part);
        benchmark::DoNotOptimize(r_model_part);
    }
}

BENCHMARK(LoadMedFresh);
