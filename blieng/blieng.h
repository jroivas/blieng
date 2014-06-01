/*
 * Copyright 2014 Blistud:io
 */

#ifndef __BLIENG_BLIENG_H
#define __BLIENG_BLIENG_H

#include <boost/smart_ptr/shared_ptr.hpp>

#include "blieng/data.h"
#include "blieng/configure.h"

namespace blieng
{

class Configure;

/**
 * Class to combine all state information and objects
 * in easy passable format.
 */
class BliengState
{
public:
    BliengState();
    virtual ~BliengState();

    void initialize();

    inline void setData(blieng::Data* _data) {
        m_data = _data;
    }
    inline void setConfig(blieng::Configure* _config) {
        m_config = _config;
    }
    inline void setStyleConfig(blieng::Configure*  _config) {
        m_styleconfig = _config;
    }

    blieng::Data* m_data;
    blieng::Configure* m_config;
    blieng::Configure* m_styleconfig;
};

}

#endif  // __BLIENG_BLIENG_H
