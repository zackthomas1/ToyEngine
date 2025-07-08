#pragma once
#include <utility>
#include "ToyEngine/enum.h"

namespace ToyEngine {
    class InputPoll {
    public:
        virtual ~InputPoll() {}
        
        virtual eKeyState Key(eKeyCode key) = 0;
        virtual float MouseVerticalScroll() = 0;
        virtual std::pair<float, float> MousePos() = 0;
    protected: 
        InputPoll() {}
    };
}