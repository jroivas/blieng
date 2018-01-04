/*
 * Copyright 2014-2018 Jouni Roivas
 */

#pragma once

#include "blieng/physics/blibody.h"

namespace blieng
{

/**
 * Dynamic body for physics simulations.
 */
class BliDynamicBody : public BliBody
{
public:
    BliDynamicBody(BliWorld *world, float x=0.0f, float y=0.0f);
    virtual ~BliDynamicBody();
};

}
