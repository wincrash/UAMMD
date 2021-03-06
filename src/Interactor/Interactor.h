/*Raul P. Pelaez 2016. Interactor abstract class base implementation

  Interactor is intended to be a module that computes and sums the forces acting on each particle
    due to some interaction, like and external potential or a pair potential.

  The positions and forces must be provided, they are not created by the module.

  This is an abstract class that should be derived to implement new interactors, Interactor itself cannot be instanciated.

 Currently Implemented interactors:
    1.Pair Forces: Implements hash (cell index) sort neighbour list construction algorithm to evaluate pair forces given some potential function, LJ i.e. Ultra fast
    2.Bonded forces: Allows to join pairs of particles via springs (Instructions in BondedForces.h)
    3.NBody forces: All particles interact with every other via some potential.
    4.External forces: A custom force function that will be applied to each particle individually.

TODO:
100- Implement methods to change parameters during simulation, virtual SetNewConfig...
 */

#ifndef INTERACTOR_H
#define INTERACTOR_H
#include"globals/defines.h"
#include"utils/utils.h"
#include"globals/globals.h"
#include<cstdint>
#include<memory>

class Interactor{
public:
  Interactor():
    Interactor(128, gcnf.L, gcnf.N){}
  Interactor(int blocksize, real3 L, int N, int Nstart = 0):name("UnnamedInteractor"),BLOCKSIZE(blocksize), L(L), N(N),Nstart(Nstart) {}
  virtual ~Interactor(){}

  virtual void sumForce() = 0;
  virtual real sumEnergy() = 0;
  virtual real sumVirial() = 0;


  virtual void print_info(){};
  void print(){
    std::cerr<<"Using "<<name<<" module."<<std::endl;
    print_info();
  }

protected:
  uint N;
  int Nstart;
  real3 L;
  string name;
  int BLOCKSIZE;
  
};

typedef vector<shared_ptr<Interactor>> InteractorArray;
#endif
