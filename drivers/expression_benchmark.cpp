// --- External Includes ---
#include "benchmark/benchmark.h"

// --- Benchmark Includes ---
#include "custom_utilities/common.hpp"

// --- Core Includes ---
#include "expression/specialized_container_expression.h"
#include "expression/container_data_io.h"
#include "expression/container_expression.h"


void LiteralFlatExpression(benchmark::State& rState)
{
    // Generate model to read from
    Kratos::Model& r_model = Kratos::ModelFactory::GetModel();
    Kratos::ModelPart& r_model_part = r_model.GetModelPart("main");

    // Create a literal expression
    using NodeExpression = Kratos::SpecializedContainerExpression<
        Kratos::ModelPart::NodesContainerType,
        Kratos::ContainerDataIO<Kratos::ContainerDataIOTags::Historical>
    >;
    NodeExpression expression(r_model_part);
    expression.Read(Kratos::DISPLACEMENT);

    for (auto dummy : rState) {
        expression.Evaluate(Kratos::DISPLACEMENT);
        benchmark::DoNotOptimize(r_model_part);
    }
}


BENCHMARK(LiteralFlatExpression);


BENCHMARK_MAIN();
