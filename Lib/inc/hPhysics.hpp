#ifndef HPHYSICS_HPP
#define HPHYSICS_HPP

#include "hcommon.hpp"
#include "mbed.h"





class hPhysics{
public:
    hPhysics();
    virtual ~hPhysics();
    static hPhysics &getInstance();
    
    static hPhysics *mp_Instance;


};



#endif //HPHYSICS_HPP