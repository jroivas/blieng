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

    inline void setData(boost::shared_ptr<blieng::Data> _data) {
        m_data = _data;
    }
    inline void setConfig(boost::shared_ptr<blieng::Configure> _config) {
        m_config = _config;
    }

    boost::shared_ptr<blieng::Data> m_data;
    boost::shared_ptr<blieng::Configure> m_config;
};

}

#endif  // __BLIENG_BLIENG_H
