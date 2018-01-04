#pragma once

#include "blieng/physics/blibody.h"

namespace blieng
{

/**
 * Static body for physics simulations
 */
class BliStaticBody : public BliBody
{
public:
    BliStaticBody(BliWorld *world, float x=0.0f, float y=0.0f);
    virtual ~BliStaticBody();
};

}
