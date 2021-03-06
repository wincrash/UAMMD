# **Universally Adaptable Multiscale Molecular Dynamics (UAMMD) ver 0.2**

<img src="https://github.com/raulppelaez/uammd/blob/development/src/res/poster.png" width="300"><img src="https://github.com/raulppelaez/uammd/blob/development/src/res/shotlogo.png" width="500">  


**See the wiki for more info!**  
**You can find videos on the youtube channel**  http://bit.ly/2r5WoOn

## DESCRIPTION

-----------------

Raul P. Pelaez 2016


A fast generic multiscale CUDA Molecular Dynamics code made into modules for expandability and generality.

UAMMD can perform several types of simulations, to this moment multiple integrators are implemented allowing it to perform:

	-Molecular dynamics (MD)
	-Brownian Dynamics  (BD)
	-Brownian Hydrodynamics (BDHI)
	-Dissipative Particle Dynamics (DPD)

Multiple building blocks are provided in order for the user to construct a certain simulation, 
highly templated so the user can easily add in the input the specific interactions when they are not implemented by default. 

For example, there is not a harmonic trap module, but you can write a simple functor in the input file (directly in device code!) telling that each particle should experiment a force when it is trying to leave the box and you are set!. You can do the same with a bonded force, an interaction that needs to transverse a neighbour list, an nbody interaction... See the examples folder and the wiki for more info!

UAMMD is coded into separated modules, with a SimulationConfig driver in C++ that can hold many modules in order to construct a simulation. For example, the simulation could have a VerletNVT module and and PairForces interactor module to create a molecular dynamics simulation. Or a DPD integrator module with Nbody interactor module, etc.

There are three types of modules:

      1. Integrators
      2. Interactors
	  3. Measurables

**Interactors**

An Interactor is an abstract entity that has the ability of computing the forces acting of each particle due to some interaction.
For example, an Interactor could compute the pair Lennard Jonnes forces between each particle pair of the system or sum the forces due to the particles being joined by springs. 

**Integrators**

An Integrator is an entity that has the ability of moving the particle positions to the next time step. 
In order to do so it can hold any number of Interactors and use them to compute the forces at any time.

**Measurables**

A Measurable is any computation that has to be performed between steps of the simulation, it can be any magnitude that is calculated from the simulation state (positions, forces..).
A measurable can compute the energy, the radial function distribution or any arbitrary computation that does not change the simulation state.

----------------------

These objects are abstract classes that can be derived to create all kinds of functionality and add new physics. Just create a new class that inherits Interactor, Integrator or Measurable and override the virtual methods with the new functionality.


Finally there is a Driver that puts them all together and controls the flow of the simulation.

**The UAMMD input is set when compiling using: make INPUT_FILE=myinput.cpp**

The default input is Driver/SimulationConfig.cpp. Any file containing at least the definition of the constructor of the class "SimulationConfig" is considered an input file. See the wiki/Input-File for more info!

You can specify a different input file implementing the simulation configuration when compiling by:

``` $make INPUT_FILE=my_input.cpp ```


# Currently Implemented

See the wiki page for each interactor for more info and instructions!

-----------------------
**Interactors:**

	1.Pair Forces: Implements hash (Morton hash) sort neighbour cell list construction algorithm to evaluate pair forces given some short range potential function, LJ i.e. Ultra fast
	2.Bonded forces: Allows to join pairs of particles via bonds (i.e a harmonic spring) (Instructions in BondedForces.h)
	3.Three body angle bonded forces: Allows to join triples of particles via angle springs (Instructions in BondedForces.h)
    4.NBody forces: All particles interact with every other via some force.
	5.External forces: A custom force function that will be applied to each particle individually.
	6.Pair Forces DPD: A thermostat that uses the Pair Forces module to compute the interactions between particles as given by dissipative particle dynamics.
	
**Integrators:**

	1.Two step velocity verlet NVE
	2.Two step velocity verlet NVT with BBK thermostat
	3.Euler Maruyama Brownian dynamics (BD)
	4. Brownian Dynamics with Hydrodynamic interactions (BDHI)
	4.1 Euler Maruyama w/HI via RPY tensor 
	4.1.1 Using the Cholesky decomposition on the full Mobility matrix to compute the stochastic term. Open Boundaries.
	4.1.2 Using the Lanczos algorithm and a matrix free method to compute the stochastic term. Open Boundaries.
	4.1.3 Using the Positively Split Ewald method with rapid stochastic sampling. Periodic Boundary Conditions

**Measurables**
	
	1.Energy Measurable. Computes the total, potential and kinetic energy and the virial pressure of the system

----------------------

You can select between single and double precision via Makefile, changing the OPTIONS variable. Single precision is used by default, remember to recompile the entire code when changing the precision. This last step is very important, as failing to do so will result in unexpected behavior.


## USAGE

-------------------
If you dont have cub (thrust comes bundled with the CUDA installation) clone or download the v1.5.2 (see dependencies).
The whole cub repository uses 175mb, so I advice to download the v1.5.2 zip only.
The Makefile expects to find cub in /usr/local/cub, but you can change it. CUB doesnt need to be compiled.

Hardcode the configuration (Integrator, Interactor, initial conditions..) in Driver/SimulationConfig.cpp or any other input file, set number of particles, size of the box, dt, etc, there.
You can change the integrator at any time during the execution of the simulation, see examples/ for a guide.

Then compile with make and run. You can use the --device X flag to specify a certain GPU.

You may need to adequate the Makefile to you particular system

## DEPENDENCIES

---------------------
Depends on:

	1. CUB       (v1.5.2 used)                  :   https://github.com/NVlabs/cub
	2. thrust    (v1.8.2 bundled with CUDA used):   https://github.com/thrust/thrust
	3. CUDA 6.5+ (v7.5 used)                    :   https://developer.nvidia.com/cuda-downloads

This code makes use of the following CUDA packages:
	
	1. cuRAND
	2. cuBLAS
	3. cuSolver
	

## REQUERIMENTS

--------------------
Needs an NVIDIA GPU with compute capability sm_3.5+
Needs g++ with full C++11 support, 4.8+ recommended

## TESTED ON

------------
	 - GTX980 (sm_52)  on Ubuntu 14.04 with CUDA 7.5 and g++ 4.8
     - GTX980 (sm_52)  on Ubuntu 16.04 with CUDA 7.5 and g++ 5.3.1
     - GTX980 (sm_52), GTX780 (sm_35), GTX480(sm_20) and GTX580(sm_20) on CentOS 6.5 with CUDA 7.5 and g++ 4.8
	 - GTX1080 (sm_61), Tesla P1000 (sm_60) on CentOS 6.5 with CUDA 8.0 and g++ 4.8
     - K40 (sm_35), GTX780(sm_35) on CentOS 6.5 with CUDA 8.0 and g++ 4.8

## BENCHMARK

------------

Current benchmark:

	GTX980 CUDA-7.5
	N = 2^20
	L = 128
	dt = 0.001f
	1e4 steps
	PairForces with rcut = 2.5 and no energy measure
	VerletNVT, no writing to disk, T = 0.1
	Starting in a cubicLattice


#### ------HIGHSCORE---------

	Number of cells: 51 51 51; Total cells: 132651
	Initializing...
	DONE!!
	Initialization time: 0.15172s
	Computing step: 10000
	Mean step time: 127.33 FPS
	Total time: 78.535s
	real 1m19.039s
    user 0m53.772s
	sys  0m25.212s


## NOTES FOR DEVELOPERS

The procedure to implement a new module is the following:

	1. Create a new class that inherits from one of the parents (Interactor, Integrator, Measurable...) and overload the virtual methods. You want as long as the virtual methods are overloaded.
	2. Include the new header in global/Modules.h
	3. Add the new sources in the Makefile.
	4. Initialize them as needed in Driver/SimulationConfig.cpp as in the examples.
	
It is advised to keep "__device__" functions as inline and always in the same compilation unit as the kernel that uses them.

-------------------------------
In globals/globals.h are the definitions of some variables that will be available throughout the entire project. These are mainly parameters. It also contains the position, force and an optional velocity arrays.

In the creation of a new module (Interactor or Integrator) for interoperability with the already existing modules, the code expects you to use the variables from global, the Vectors storing the positions, forces and velocities of each particle (when needed). These Vectors start with zero size and are initialized in Driver.cpp. However, your code should check the size of the arrays at startup with Vector::size() and initialize them if the size doesnt match the number of particles (i.e is 0).

Currently the code initializes pos and force Vectors in Driver.cpp, after the parameters are set. Vel should be initialized in the constructor of any module that needs it, see VerletNVT for an example.

**Guidelines**

Each module should have its own namespace, or adhere to an existing one, in order to avoid naming conflicts. This allows to name the functions and parameters in a more human readable way.

If you want to make small changes to an existing module, without changing it. Then you should create a new module that inherits it, and overload the necesary functions.

------------------------------------------

## ACKNOWLEDGMENTS

UAMMD was developed at the Departamento de Física Teórica de la Materia Condensada of Universidad Autónoma de Madrid (UAM) under supervision of Rafael Delgado-Buscalioni. Acknowledgment is made to the Donors of the American Chemical Society Petroleum Research Fund (**PRF# 54312-ND9**) for support of this research and to Spanish MINECO projects **FIS2013- 47350-C05-1-R and FIS2013-50510-EXP**.

Acknowledgment is made to NVIDIA Corporation.
