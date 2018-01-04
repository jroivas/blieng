/*
 * Copyright 2014-2017 Jouni Roivas
 */

#ifndef __BLIENG_WALLCLOCK_H
#define __BLIENG_WALLCLOCK_H

#include <vector>

#include "blieng/item.h"
#include "blieng/3rdparty/auto_vector.h"

using std::unique_ptr;

namespace blieng
{

/**
 * Wallclock to control time progress in the universe.
 */
class Wallclock
{
public:
    /**
     * Create a wall clock from time producer item.
     *
     * \param time_producer Item which produces time
     */
    explicit Wallclock(Item *time_producer);
    /**
     * Go forward with time units.
     * Time units here are just general units on global scale,
     * real units produced may depend on the time producer item.
     *
     * \param amount Number of time units to go forward.
     */
    void forward(unsigned long int amount = 1);
    /**
     * Add a new producer, which may require time.
     * May require also any other resources.
     * Meant for timely based producing trees,
     * to get different resources or items generated timely based.
     *
     * \param item Item producer
     */
    bool addProducer(std::unique_ptr<Item> item);

private:
    /**
     * Produces timely based items.
     * Goes thorough the producers list and feed them with
     * certain amount of time.
     *
     * \param amount Amount of time to proceed
     */
    void produceTime(unsigned long int amount);

    /**
     * Tier 2 produce
     * Meant to produce items and producers in tier 2.
     * Tier 1 produces items with only time as dependency
     * or produces timely based resources inside the item.
     * Tier 2 produces items which depends on other items,
     * may be long process to get the chain produced.
     */
    void produceTier2();
    /**
     * Produce items in Tier 2.
     * Will produce on items in Tier 2, and push them back to
     * item list allowing them to produce more items.
     */
    void produceTier2Items();
    /**
     * Produce items int Tier 2 from producers.
     * Similar to \ref produceTier2Items but goes thorough
     * producers list generated with \ref addProducer.
     * Will push generated items to item list.
     */
    void produceTier2Producers();
    /**
     * Clean up exhauster items.
     * TODO: Does nothing for now
     */
    void cleanItems();
    /**
     * Combine items with same base.
     * This way have less item objects generating
     * new objects with more amount.
     * TODO: Does nothing for now
     */
    void combineItems();
    /**
     * Produce items.
     * Runs the produce loop, utilizes \ref produceTime and \ref produceTier2
     * and \consume to get new items generated.
     *
     * \param amount Amount of time to proceed
     */
    void produce(unsigned long int amount);
    /**
     * Run massive consume loop.
     * Tries to match items with available items to get stock
     * for them in order to produce new ones.
     *
     * \returns True if anything consumed, false if no matches found
     */
    bool consume();

    auto_vector<Item> items;  //!< List of all items
    auto_vector<Item> producers;  //!< List of producers
    Item *time_producer;  //!< The master time producer
    unsigned long int wall_time;  //!< Current time since generation
};

}  // namespace blieng

#endif  // __BLIENG_WALLCLOCK_H
