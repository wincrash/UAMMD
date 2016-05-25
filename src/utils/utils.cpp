#include"utils.h"

//Took from Fluam, adapted to use float4
void cubicLattice(float *pos, float L, uint N){
    float dx, dy, dz;
    int nx, ny, nz, n;
    int np = N;

    nx = ny = nz = 1;
    while((nx*ny*nz)<np){
      if((nx*ny*nz)<np) nx++;
      if((nx*ny*nz)<np) ny++;
      if((nx*ny*nz)<np) nz++;
    }

    dx = L/float(nx);
    dy = L/float(ny);
    dz = L/float(nz);

    n = 0;
    
    for (int i=0;i<nz;i++)
      for(int j=0;j<ny;j++)
	for(int k=0;k<nx;k++)
	  if(n<np){
	    n = n + 1;
	    pos[4*(n-1)+0] = (k + 0.5f) * dx - L/2.0f;
	    pos[4*(n-1)+1] = (j + 0.5f) * dy - L/2.0f;
	    pos[4*(n-1)+2] = (i + 0.5f) * dz - L/2.0f;
	  }
}