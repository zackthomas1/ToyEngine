#pragma once
#include "ToyEngine/core.h"

namespace ToyEngine 
{
    const int CUBE_SIDES = 6;

    class TextureCube 
    {
    public:
        virtual ~TextureCube() {} 
        
        virtual void Bind() const = 0;

        static Ref<TextureCube> Create(Array<std::string, CUBE_SIDES>& files);
    protected:
        TextureCube() {};
    };
}