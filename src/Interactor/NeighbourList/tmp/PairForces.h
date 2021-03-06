/*
Raul P. Pelaez 2016. Short range pair forces Interactor implementation.

  Interactor is intended to be a module that computes and sums the forces acting on each particle
    due to some interaction, like and external potential or a pair potential.

  The positions and forces must be provided, they are not created by the module.

  This module implements a Neighbour cell list search and computes the force between all neighbouring
  particle pairs. 
  The Neighbour search is implemented using hash short with a 30 bit morton code (from the cell coordinates) as hash.

  The Neighbour list is constructed as follows:
  
  1-Compute a hash for each particle based on its cell. Store in particleHash, also fill particleIndex with the index of each particle (particleIndex[i] = i)
  2-Sort particleIndex based on particleHash (sort by key). This way the particles in a same cell are one after the other in particleIndex. The Morton hash also improves the memory acces patter in the GPU.
  3-Fill cellStart and cellEnd with the indices of particleIndex in which a cell starts and ends. This allows to identify where all the [indices of] particles in a cell are in particleIndex, again, one after the other.
  
  The transversal of this cell list is done by transversing, for each particle, the 27 neighbour cells of that particle's cell.
  
  Force is evaluated using table lookups (with texture memory)
  
TODO:
100- Fix particle types, differentiate between color and tag/type.
100- Make a better potential generalization.
 */

#ifndef PAIRFORCES_H
#define PAIRFORCES_H
#include"globals/defines.h"
#include"utils/utils.h"
#include"globals/globals.h"
#include"Interactor.h"
#include"misc/Potential.h"
#include"PairForcesGPU.cuh"
#include<cstdint>
#include<memory>
#include<functional>

//The currently implemented forces, custom allows for an arbitrary function tu be used as force function
enum pairForceType{LJ,NONE,CUSTOM};

real forceLJ(real r2);
real energyLJ(real r2);
real nullForce(real r2);


class PairForces: public Interactor{
public:
  //PairForces(pairForceType fs = LJ);
  PairForces(pairForceType fs = LJ,
	     std::function<real(real)> customForceFunction = nullForce,
  	     std::function<real(real)> customEnergyFunction = nullForce);
  ~PairForces();

  void sumForce() override;
  real sumEnergy() override;
  real sumVirial() override;

  void setPotParam(uint i, uint j, real2 param);
protected:
  uint ncells;
  Vector4 sortPos;//, oldPos;
  
  void init();
  void makeNeighbourList();
  
  Vector<uint> particleHash, particleIndex; 
  Vector<uint> cellStart, cellEnd;
 
  Vector<real> energyArray, virialArray;
  
  pair_forces_ns::Params params; //CPU version of parameters
  pair_forces_ns::Params *paramsGPU; //GPU global memory version of parameters
  
  //These handle the selected force functions
  
  Potential pot;
  std::function<real(real)> customForceFunction;
  std::function<real(real)> customEnergyFunction;
  
  real rcut;
  
  pairForceType forceSelector;
  
  static uint pairForcesInstances;
  static uint pairForcesConstantMemoryOwner;
  uint my_instance;
};

#endif
