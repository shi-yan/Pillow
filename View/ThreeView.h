#pragma once
//#include "Global.h"
#include "SplitedView.h"

class ThreeView : public SplitedView
{
public:
    ThreeView(OpenGLBackend *backend, unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height);

    void onPaint() override;

    bool onLeftDown(unsigned int x,unsigned int y) override;
		
    CameraMode::__Enum setView(unsigned int index)  override;
	
    bool onPanPress(unsigned int x,unsigned int y) override;
	
    bool onPanDrag(unsigned int x,unsigned int y) override;

    bool onAxisDrag(unsigned int x,unsigned int y) override;

    bool onAxisRelease() override;

    bool onCtrlPress(unsigned int x,unsigned int y) override;

    bool onCtrlRelease() override;

    bool onCtrlDrag(unsigned int x,unsigned int y) override;

    bool onAxisPress(unsigned int x,unsigned int y) override;

    bool onSelectionPress(unsigned int x,unsigned int y) override;

    bool onSelectionDrag(unsigned int x,unsigned int y) override;

    bool onSingleSideSelectionRelease(bool isAppend) override;

    bool onDualSideSelectionRelease(bool isAppend) override;

    void onWheel(float step,unsigned int x,unsigned int y) override;

    bool onRotateDrag(unsigned int x,unsigned int y) override;

    bool onRotatePress(unsigned int x,unsigned int y) override;

    bool onRotateRelease() override;

    bool onPanRelease() override;

    void update(unsigned int splitX, unsigned int splitY, unsigned int width, unsigned int height) override;

    ~ThreeView(void) override;
};
