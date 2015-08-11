#include "SubdivideFace.h"

SubdivideFace::SubdivideFace(int v1, int v2, int v3, int v4)
    :ElementBase(),
      center(0),
      normal()
{
	edge[0]=v1;
	edge[1]=v2;
	edge[2]=v3;
	edge[3]=v4;
	subFace[0]=0;
	subFace[1]=0;
	subFace[2]=0;
	subFace[3]=0;
}

SubdivideFace::~SubdivideFace(void)
{
}
