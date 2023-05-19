#include <assert.h>
#include <stdlib.h>
#include <strings.h>

#include "helpers.h"
#include "world.h"

struct container *create_world()
{
    // rooms
    struct room *apartment = create_room("Apartment",
                                         "You find yourself in your cozy apartment, where sleek and minimalist design is\n"
                                         "seamlessly integrated with the latest advancements in smart home technology.\n"
                                         "The apartment boasts a built-in autonomous garden and an energy station,\n"
                                         "offering you a self-sufficient and sustainable lifestyle.\n"
                                         "As you look towards the north, you see your garage with a Hyperloop capsule parked.");

    struct room *garden = create_room("Garden",
                                      "The garden is a room isolated from the outside world, with artificial sunlight illuminating\n"
                                      "the shelves filled with a variety of plants. Some of the fruit plants have completed their\n"
                                      "growth cycle and are now bearing ripe fruits that are ready for harvesting.");

    struct room *estation = create_room("Energy station",
                                        "The energy station is the power center of your apartment, featuring high-tech kitchen for\n"
                                        "your organic energy needs as well as recharge pads for electric energy.\n"
                                        "With the ban of public food stations due to their negative impact on the environment,\n"
                                        "it's always wise to stock up on something from your energy station before venturing out.");

    struct room *garage = create_room("Garage",
                                      "Your garage is the designated spot for your Hyperloop capsule, always primed for intercity\n"
                                      "travel. Its modern design complements the sleek aesthetic of your apartment. It's linked to\n"
                                      "the Hyperloop network, which only allows periodic launches several times an hour, tho.");

    struct room *tempa = create_room("temp a", "desc");
    struct room *tempb = create_room("temp b", "desc");
    struct room *tempc = create_room("temp c", "desc");
    struct room *tempd = create_room("temp d", "desc");
    struct room *tempe = create_room("temp e", "desc");
    struct room *tempf = create_room("temp f", "desc");
    struct room *tempg = create_room("temp g", "desc");
    struct room *temph = create_room("temp h", "desc");
    struct room *tempi = create_room("temp i", "desc");
    struct room *tempj = create_room("temp j", "desc");
    struct room *tempk = create_room("temp k", "desc");
    struct room *templ = create_room("temp l", "desc");
    struct room *tempm = create_room("temp m", "desc");
    struct room *tempn = create_room("temp n", "desc");

    set_exits_from_room(apartment, garage, garden, NULL, estation);
    set_exits_from_room(estation, NULL, NULL, apartment, NULL);
    set_exits_from_room(garden, apartment, NULL, NULL, NULL);
    set_exits_from_room(garage, tempa, apartment, NULL, NULL);

    set_exits_from_room(tempa, tempb, garage, NULL, NULL);
    set_exits_from_room(tempb, tempc, tempa, NULL, NULL);
    set_exits_from_room(tempc, tempd, tempb, NULL, NULL);
    set_exits_from_room(tempd, tempe, tempc, NULL, NULL);
    set_exits_from_room(tempe, tempf, tempd, NULL, NULL);
    set_exits_from_room(tempf, tempg, tempe, NULL, NULL);
    set_exits_from_room(tempg, temph, tempf, NULL, NULL);
    set_exits_from_room(temph, tempi, tempg, NULL, NULL);
    set_exits_from_room(tempi, tempj, temph, NULL, NULL);
    set_exits_from_room(tempj, tempk, tempi, NULL, NULL);
    set_exits_from_room(tempk, templ, tempj, NULL, NULL);
    set_exits_from_room(templ, tempm, tempk, NULL, NULL);
    set_exits_from_room(tempm, tempn, templ, NULL, NULL);
    set_exits_from_room(tempn, NULL, tempm, NULL, NULL);

    // items
    struct item *arduino = create_item("Arduino Uno",
                                       "A microcontroller board that utilizes the 8-bit ATmega328P microchip. It features 14\n"
                                       "digital input/output pins and 6 analog inputs. This piece of silicon is very rare to\n"
                                       "find these days due to government restrictions. Each new hardware piece is connected\n"
                                       "to the global network and is not usable without it, allowing for easy tracking of\n"
                                       "illegal activities.",
                                       MOVABLE | USABLE | EXAMINABLE);

    struct item *sleeppod = create_item("Sleeping pod",
                                        "The sleeping pod is pressurized with oxygen, providing an optimal sleeping environment.\n"
                                        "Research has shown that sleeping vertically, like in old space movies such as Interstellar\n"
                                        "or The Martian, is more beneficial and space-efficient even on Earth.",
                                        USABLE | EXAMINABLE);

    struct item *watches = create_item("Old Rolex watches",
                                       "These used to cost a fortune back in the day, now, they are mostly useless since every\n"
                                       "assistive technology you can imagine is implanted in your brain.",
                                       MOVABLE | USABLE | EXAMINABLE);

    add_item_to_room(apartment, arduino);
    add_item_to_room(apartment, sleeppod);
    add_item_to_room(apartment, watches);

    add_item_to_room(garage, create_item("itemtemp a", "desc", MOVABLE | USABLE | EXAMINABLE));
    add_item_to_room(garage, create_item("itemtemp b", "desc", MOVABLE | USABLE | EXAMINABLE));
    add_item_to_room(garage, create_item("itemtemp c", "desc", MOVABLE | USABLE | EXAMINABLE));
    add_item_to_room(garage, create_item("itemtemp d", "desc", MOVABLE | USABLE | EXAMINABLE));
    add_item_to_room(garage, create_item("itemtemp e", "desc", MOVABLE | USABLE | EXAMINABLE));

    // world
    struct container *world = add_room_to_world(NULL, apartment);
    add_room_to_world(world, garden);
    add_room_to_world(world, estation);
    add_room_to_world(world, garage);

    add_room_to_world(world, tempa);
    add_room_to_world(world, tempb);
    add_room_to_world(world, tempc);
    add_room_to_world(world, tempd);
    add_room_to_world(world, tempe);
    add_room_to_world(world, tempf);
    add_room_to_world(world, tempg);
    add_room_to_world(world, temph);
    add_room_to_world(world, tempi);
    add_room_to_world(world, tempj);
    add_room_to_world(world, tempk);
    add_room_to_world(world, templ);
    add_room_to_world(world, tempm);
    add_room_to_world(world, tempn);

    return world;
}

struct container *add_room_to_world(struct container *world, struct room *room)
{
    CHECK_NULL(room);

    // check for duplicate room
    for (struct container *current = world; current != NULL; current = current->next)
    {
        ASSERT(current->type == ROOM);
        if (strcasecmp(current->room->name, room->name) == 0)
        {
            return NULL;
        }
    }

    // add room
    return create_container(world, ROOM, room);
}

struct container *destroy_world(struct container *world)
{
    ASSERT(world != NULL);
    return destroy_containers(world);
}

struct room *get_room(struct container *world, char *name)
{
    ASSERT(world != NULL && name != NULL);
    return get_from_container_by_name(world, name);
}