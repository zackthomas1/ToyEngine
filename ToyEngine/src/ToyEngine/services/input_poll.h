#pragma once
#include <utility>
#include "ToyEngine/events/event.h"

namespace ToyEngine {
    class InputPoll {
    public:
        virtual ~InputPoll() {}
        
        virtual KeyState Key(KeyCode key) = 0;
        virtual float MouseVerticalScroll() = 0;
        virtual std::pair<float, float> MousePos() = 0;
    };
}