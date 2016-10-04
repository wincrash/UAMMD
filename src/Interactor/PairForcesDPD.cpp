#include"PairForcesDPD.h"

using namespace pair_forces_ns;
/*PairForcesDPD inherits from PairForces*/
PairForcesDPD::PairForcesDPD(): PairForces(LJ), rngCPU(gcnf.seed){

  cerr<<"Initializing DPD Submodule..."<<endl;

  gamma = gcnf.gamma;
  paramsDPD.gamma = gamma;
  paramsDPD.noiseAmp = sqrt(2.0f*gamma*gcnf.T/gcnf.dt);


  sortVel = Vector4(N); sortVel.fill_with(make_float4(0.0f)); sortVel.upload();

  /*Pass sortVel to bind it to a texture reference*/
  initPairForcesDPDGPU(paramsDPD, sortVel, N);

  /*Warmup rng*/
  fori(0, 1000) seed = rngCPU.next();


  cerr<<"DPD Submodule\tDONE!!"<<endl;
}


/*** CONSTRUCT NEIGHBOUR LIST ***/
void PairForcesDPD::makeNeighbourListDPD(){

  makeCellListDPD(pos, vel, sortPos, sortVel, particleIndex, particleHash, cellStart, cellEnd, N, ncells);
  
}

void PairForcesDPD::sumForce(){

  /*** CONSTRUCT NEIGHBOUR LIST ***/
  makeNeighbourListDPD();

  /*Move the seed to the next step*/
  seed = rngCPU.next();
  
  /*** COMPUTE FORCES USING THE NEIGHBOUR LIST***/
  computePairForceDPD(force, particleIndex, N, seed);
}


float PairForcesDPD::sumEnergy(){ return 0.0f;}
float PairForcesDPD::sumVirial(){ return 0.0f;}
