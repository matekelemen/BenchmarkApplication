/// @author Máté Kelemen

// --- External Includes ---
#include "benchmark/benchmark.h"

// --- Benchmark Includes ---
#include "BenchmarkApplication/common.hpp"
#include "BenchmarkApplication/cmake_variables.hpp"

// --- Med Includes ---
#include "custom_io/med_model_part_io.h"

// --- Core Includes ---
#include "containers/model.h"
#include "includes/model_part.h"
#include "includes/kratos_application.h"


void InsertMasterSlaveConstraints(benchmark::State& rState)
{
    // Initialize Kratos
    Kratos::KratosApplication kratos("core");
    kratos.Register();

    for ([[maybe_unused]] auto dummy : rState) {
        rState.PauseTiming();

        { // <== open a scope to control the model life time
            Kratos::Model model;
            auto& r_model_part = model.CreateModelPart("root");
            r_model_part.AddNodalSolutionStepVariable(Kratos::DISPLACEMENT_X);

            // Load a model
            Kratos::MedModelPartIO((Kratos::GetBenchmarkDataDir() / "cube_quadratic_tets.med").string()).ReadModelPart(r_model_part);
            for (Kratos::Node& r_node : r_model_part.Nodes()) {
                r_node.AddDof(Kratos::DISPLACEMENT_X);
            }

            // Insert 100 linear master slave constraints
            // (or less if the model has fewer nodes)
            const std::size_t number_of_constraints = std::min(static_cast<std::size_t>(rState.range(0)),
                                                               r_model_part.NumberOfNodes() / 2);
            std::size_t i_constraint = 0ul;

            rState.ResumeTiming();

            for (std::size_t i_master_node=0ul; i_master_node<2*number_of_constraints; i_master_node+=2) {
                Kratos::Node& r_master_node = *(r_model_part.Nodes().begin() + i_master_node);
                Kratos::Node& r_slave_node  = *(r_model_part.Nodes().begin() + i_master_node + 1);
                r_model_part.CreateNewMasterSlaveConstraint(
                    "LinearMasterSlaveConstraint",
                    ++i_constraint,
                    r_master_node,
                    Kratos::DISPLACEMENT_X,
                    r_slave_node,
                    Kratos::DISPLACEMENT_X,
                    1.23,
                    4.56
                );
            } // for i_master_node

            // Exclude destructors from the benchmark
            rState.PauseTiming();
        } // <== destroy the model

        rState.ResumeTiming();
    }
}


BENCHMARK(InsertMasterSlaveConstraints)->Arg(1 << 6 )->Iterations(50);
BENCHMARK(InsertMasterSlaveConstraints)->Arg(1 << 8 )->Iterations(25);
BENCHMARK(InsertMasterSlaveConstraints)->Arg(1 << 10)->Iterations(12);
BENCHMARK(InsertMasterSlaveConstraints)->Arg(1 << 12)->Iterations(6 );
BENCHMARK(InsertMasterSlaveConstraints)->Arg(1 << 14)->Iterations(3 );
