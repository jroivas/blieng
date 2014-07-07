#include "blieng/blieng.h"


blieng::BliengState::BliengState()
{
    m_data = nullptr;
    m_config = nullptr;
    m_styleconfig = nullptr;
}

blieng::BliengState::~BliengState()
{
    if (m_config != nullptr) {
        delete m_config;
    }
    if (m_styleconfig != nullptr) {
        delete m_styleconfig;
    }
    if (m_data != nullptr) {
        delete m_data;
    }
}

void blieng::BliengState::initialize()
{
    m_data = new blieng::Data();

    boost::shared_ptr<blieng::BliengState> _state(this);

    m_config = new blieng::Configure(_state);
    m_styleconfig = new blieng::Configure(_state);
}
