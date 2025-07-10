#pragma once
#include "ToyEngine/enum.h"

namespace ToyEngine {
    class InputPoll {
    public:
        virtual ~InputPoll() {}
        
        virtual void Init() = 0;
        virtual eKeyState Key(eKeyCode key) = 0;
        virtual float MouseVerticalScroll() = 0;
        virtual std::pair<float, float> MousePos() = 0;
    protected: 
        InputPoll() {}
    };

    class NullInputPoll : public InputPoll {
    public:
         NullInputPoll() {}

        virtual void Init() override
        {
            TY_CORE_WARN( "Platform Invalid. Null service provider - Input poll service disabled." );
        }
        virtual eKeyState Key(eKeyCode key) override { return eKeyState::kRelease; }
        virtual float MouseVerticalScroll() override { return 0.0f; }
        virtual std::pair<float, float> MousePos() override { return std::pair<float, float>(0.0f, 0.0f); }
    };

    class InputPollGLFW : public InputPoll {
    public:
        InputPollGLFW() {}
        ~InputPollGLFW() {}

        virtual void Init();
        virtual eKeyState Key(eKeyCode key) override;
        virtual float MouseVerticalScroll() override;
        virtual std::pair<float, float> MousePos() override;
    };
}