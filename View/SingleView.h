#pragma once
#include "Global.h"
#include "SplitedView.h"
#include <time.h>

class SingleView:public SplitedView
{
public:
    SingleView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height);
	
    CameraMode::__Enum setView(size_t index) override;

    bool onPanPress(size_t x,size_t y) override;

    bool onPanDrag(size_t x,size_t y) override;

    void onWheel(float step,size_t x,size_t y) override;

    bool onRotateDrag(size_t x,size_t y) override;

    bool onRotatePress(size_t x,size_t y) override;

    bool onCtrlPress(size_t x,size_t y) override;

    bool onCtrlRelease() override;

    bool onCtrlDrag(size_t x,size_t y) override;

    bool onRotateRelease() override;
	
    bool onPanRelease() override;

    void update(size_t theSplitX,size_t theSplitY,size_t theWidth,size_t theHeight) override;

    bool onSelectionPress(size_t x,size_t y) override;

    bool onSelectionDrag(size_t x,size_t y) override;

    bool onSingleSideSelectionRelease(bool isAppend) override;

    bool onDualSideSelectionRelease(bool isAppend) override;

    bool onAxisDrag(size_t x,size_t y) override;

    bool onAxisRelease() override;

    bool onAxisPress(size_t x,size_t y) override;

    void onPaint() override;

public:
    ~SingleView(void) override;
};
