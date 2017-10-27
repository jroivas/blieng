#pragma once

#include "blieng/blibody.h"

namespace blieng
{

class BliStaticBody : public BliBody
{
public:
    BliStaticBody(BliWorld *world, float x=0.0f, float y=0.0f);
    virtual ~BliStaticBody();
};

}
