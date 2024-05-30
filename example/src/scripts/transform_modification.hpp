#pragma once

#include "sparrow_engine/behavior.hpp"

namespace SparrowEngine::Example::Scripts {
    class TransformModification : public SparrowEngine::Behavior {
    public:
        using SparrowEngine::Behavior::Behavior;

        void update() override;
    };
}