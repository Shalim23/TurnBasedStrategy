#pragma once

//engine calls object's constructor multiple times
//so we use this to init object's fields once
#define INIT_ONCE(ARGS) \
if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))\
{\
ARGS\
}