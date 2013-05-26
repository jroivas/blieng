#include "blieng/configure.h"
#include "zomb/configure.h"

bool zomb::initializeConfiguration()
{
    blieng::Configure *conf = blieng::Configure::getInstance();

    // Define mandatory keys
    conf->addKey("clock", blieng::Configure::KeyUInt);
    conf->addKey("clock_background", blieng::Configure::KeyUInt);

    conf->addKey("walker_interval", blieng::Configure::KeyUInt);

    conf->addKey("map_width_min", blieng::Configure::KeyUInt);
    conf->addKey("map_height_min", blieng::Configure::KeyUInt);

    conf->addKey("act_button_width", blieng::Configure::KeyUInt);
    conf->addKey("act_button_height", blieng::Configure::KeyUInt);

    conf->addKey("chr_width_min", blieng::Configure::KeyUInt);
    conf->addKey("chr_height_min", blieng::Configure::KeyUInt);
    conf->addKey("chr_count", blieng::Configure::KeyUInt);
    conf->addKey("chr_size", blieng::Configure::KeyUInt);
    conf->addKey("chr_map_size", blieng::Configure::KeyUInt);

    conf->addKey("default_zoom_level", blieng::Configure::KeyUInt);
    conf->addKey("path_width", blieng::Configure::KeyUInt);
    conf->addKey("townname_max_width", blieng::Configure::KeyUInt);
    conf->addKey("town_sensitivity_distance", blieng::Configure::KeyUInt);
    conf->addKey("mouse_wheel_sensitivity", blieng::Configure::KeyUInt);

    conf->addKey("zombie_size", blieng::Configure::KeyUInt);
    conf->addKey("zombie_steps", blieng::Configure::KeyUInt);
    conf->addKey("fight_safe_area", blieng::Configure::KeyUInt);

    conf->addKey("random_freq", blieng::Configure::KeyUInt);
    conf->addKey("random_prob", blieng::Configure::KeyUInt);

    conf->addKey("bite_prob", blieng::Configure::KeyDouble);
    conf->addKey("bite_threshold", blieng::Configure::KeyDouble);
    conf->addKey("zombie_limiter", blieng::Configure::KeyDouble);

    conf->addKey("zombie_images", blieng::Configure::KeyString);
    conf->addKey("chr_images", blieng::Configure::KeyString);

    conf->addKey("debug", blieng::Configure::KeyBool);

    conf->addKey("zombie_damage_multiple_max", blieng::Configure::KeyDouble);
    conf->addKey("zombie_damage_row_reduce", blieng::Configure::KeyDouble);
    conf->addKey("zombie_damage_left_leg", blieng::Configure::KeyDouble);
    conf->addKey("zombie_damage_right_leg", blieng::Configure::KeyDouble);
    conf->addKey("zombie_damage_right_arm", blieng::Configure::KeyDouble);
    conf->addKey("zombie_damage_left_arm", blieng::Configure::KeyDouble);
    conf->addKey("zombie_damage_torso", blieng::Configure::KeyDouble);

    conf->addKey("player_damage_multiple_max", blieng::Configure::KeyDouble);
    conf->addKey("player_damage_row_reduce", blieng::Configure::KeyDouble);
    conf->addKey("player_damage_loot_range", blieng::Configure::KeyUInt);

    // Load it
    conf->load("zomb.json");

    //conf->addKey("", blieng::Configure::KeyUInt);
    return conf->validate();
}
