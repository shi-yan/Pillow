#include "SplitedView.h"

SplitedView::SplitedView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned height):isAxisMode(false),isSelectionMode(false),selectionSX(0),selectionSY(0),selectionEX(0),selectionEY(0),cursorMode(0),axisDragSX(0),axisDragSY(0),cursorDir(0),isCtrlMode(false),ctrlSX(0),ctrlSY(0),isExtrude(false)
{
      splitX=splitX;
      splitY=splitY;
      width=width;
      height=height;
      selected=0;
	panIndex=-1;
	rotateIndex=-1;
	zoomIndex=-1;
}



