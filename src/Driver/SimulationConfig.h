/*
  Raul P. Pelaez 2016. SimulationConfig class definition.

  This class inherits from Driver, its mostly sintactic sugar. The idea is to have a clean file that can be interpreted as the "input" of UAMMD, in which the parameters are set, the initial conditions are set and the different modules are put together to form a simulation.

*/

#ifndef SIMULATIONCONFIG_H
#define SIMULATIONCONFIG_H

#include"globals/defines.h"
#include "Driver.h"

#include<stdlib.h>
#include<string.h>

class SimulationConfig: public Driver{
public:
  SimulationConfig(int argc, char* argv[]);
  ~SimulationConfig(){}

};



#endif
