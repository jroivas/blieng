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
    /**
     * Contruct empty state info
     */
    BliengState();
    ~BliengState();

    /**
     * Initializes state info with fresh
     * instances of different state objects
     */
    void initialize();

    /**
     * Override data object
     *
     * \param _data New data object
     */
    inline void setData(blieng::Data* _data)
    {
        m_data = _data;
    }
    /**
     * Override config object
     *
     * \param _config New config object
     */
    inline void setConfig(blieng::Configure* _config)
    {
        m_config = _config;
    }
    /**
     * Override style config object
     *
     * \param _config New style config object
     */
    inline void setStyleConfig(blieng::Configure*  _config)
    {
        m_styleconfig = _config;
    }

    blieng::Data* m_data;
    blieng::Configure* m_config;
    blieng::Configure* m_styleconfig;
};

}

#endif  // __BLIENG_BLIENG_H
