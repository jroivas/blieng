#pragma once

#include "blieng/physics/blibody.h"

namespace blieng
{

class BliDynamicBody : public BliBody
{
public:
    BliDynamicBody(BliWorld *world, float x=0.0f, float y=0.0f);
    virtual ~BliDynamicBody();
};

}
