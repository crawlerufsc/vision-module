#ifndef __OCUPPANCY_GRID_H
#define __OCUPPANCY_GRID_H

template <typename T>
class OccupancyGrid
{
public:
   virtual int GetWidth() = 0;
   virtual int GetHeight() = 0;
   virtual T* GetResult() = 0;
   virtual void ComputeOcuppancyGrid(void *frame, int width, int height) = 0;
};

#endif