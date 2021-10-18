#include "hPhysics.hpp"


hPhysics* hPhysics::mp_Instance=NULL;

hPhysics::hPhysics(){
}

hPhysics::~hPhysics(){


}


hPhysics& hPhysics::getInstance(void) {
    if (mp_Instance == NULL) {
        mp_Instance = new hPhysics();
    }
    return *mp_Instance;
}