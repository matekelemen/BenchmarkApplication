// --- External Includes ---
#include "benchmark/benchmark.h"

// --- Benchmark Includes ---
#include "BenchmarkApplication/common.hpp"

// --- Core Includes ---
#include "expression/container_data_io.h"
#include "expression/container_expression.h"
#include "expression/variable_expression_io.h"
#include "includes/variables.h"


void LiteralFlatExpression(benchmark::State& rState)
{
    // Generate model to read from
    Kratos::Model& r_model = Kratos::ModelFactory::GetModel();
    Kratos::ModelPart& r_model_part = r_model.GetModelPart("main");

    // Create a literal expression
    using NodeExpression = Kratos::ContainerExpression<
        Kratos::ModelPart::NodesContainerType,
        Kratos::MeshType::Local
    >;
    NodeExpression expression(r_model_part);

    for (auto dummy : rState) {
        Kratos::VariableExpressionIO::Read<Kratos::MeshType::Local>(expression,
                                                                    &Kratos::DISPLACEMENT,
                                                                    true);
        benchmark::DoNotOptimize(r_model_part);
    }
}

BENCHMARK(LiteralFlatExpression);
