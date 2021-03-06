/*Raul P. Pelaez 2016. Global defines, the whole proyect is aware of this, both GPU and CPU code*/
#include<cuda_runtime.h>
#include<stdint.h>

#define UAMMD_VERSION "0.2"

/*Comment to compile for double precision*/
//#define SINGLE_PRECISION //Defined in Makefile

typedef unsigned int uint;
typedef unsigned long long int ullint;


#define fori(x,y) for(int i=x; i<int(y); i++)
#define forj(x,y) for(int j=x; j<int(y); j++)


#if defined SINGLE_PRECISION
typedef float real;
typedef float2 real2;
typedef float3 real3;
typedef float4 real4;
#else
typedef double real;
typedef double2 real2;
typedef double3 real3;
typedef double4 real4;
#endif


/*Some minimal information about the current system*/
#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H
struct SystemInfo{
  int dev = -1; //ID of the device 
  int cuda_arch = -1; //Cuda compute capability of the device 100*major+10*minor
  
};
#endif

extern SystemInfo sysInfo;
