#pragma once
#include "Global.h"
#include "SplitedView.h"
#include <time.h>

class SingleView : public SplitedView
{
public:
    SingleView(OpenGLBackend *backend, unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height);
	
    CameraMode::__Enum setView(unsigned int index) override;

    bool onPanPress(unsigned int x,unsigned int y) override;

    bool onPanDrag(unsigned int x,unsigned int y) override;

    void onWheel(float step,unsigned int x,unsigned int y) override;

    bool onRotateDrag(unsigned int x,unsigned int y) override;

    bool onRotatePress(unsigned int x,unsigned int y) override;

    bool onCtrlPress(unsigned int x,unsigned int y) override;

    bool onCtrlRelease() override;

    bool onCtrlDrag(unsigned int x,unsigned int y) override;

    bool onRotateRelease() override;
	
    bool onPanRelease() override;

    void update(unsigned int theSplitX,unsigned int theSplitY,unsigned int theWidth,unsigned int theHeight) override;

    bool onSelectionPress(unsigned int x,unsigned int y) override;

    bool onSelectionDrag(unsigned int x,unsigned int y) override;

    bool onSingleSideSelectionRelease(bool isAppend) override;

    bool onDualSideSelectionRelease(bool isAppend) override;

    bool onAxisDrag(unsigned int x,unsigned int y) override;

    bool onAxisRelease() override;

    bool onAxisPress(unsigned int x,unsigned int y) override;

    void onPaint() override;

public:
    ~SingleView(void) override;
};
