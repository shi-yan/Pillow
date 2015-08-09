#pragma once
#include "Global.h"
#include "SplitedView.h"

class TwoView:public SplitedView
{
public:
    TwoView(unsigned int splitX,unsigned int splitY,unsigned int width,unsigned int height);
	
	void onPaint();

	CameraMode::__Enum setView(size_t index)
	{
        m_camera[index]->setCamera();
        return m_camera[index]->m_type._value;
    }

	bool onPanPress(size_t x,size_t y)
	{
        if(m_selected==0 && x<m_splitX)
		{
            m_camera[m_selected]->onPanPress(x,y);
		}
        else if(m_selected==1 && x>m_splitX)
		{
            m_camera[m_selected]->onPanPress(x,y);
		}
		return true;
    }

	bool onPanDrag(size_t x,size_t y)
	{
        m_camera[m_selected]->pan(x,y);
		return true;
    }

	void onWheel(float step,size_t x,size_t y)
	{
        if(m_selected==0 && x<m_splitX)
		{
            m_camera[m_selected]->zoom(step);
		}
        else if(m_selected==1 && x>m_splitX)
		{
            m_camera[m_selected]->zoom(step);
		}
		y;
    }

	bool onRotateDrag(size_t x,size_t y)
	{
        m_camera[m_selected]->rotate(x,y);
		return true;
    }

	bool onRotatePress(size_t x,size_t y)
	{
        if(m_selected==0 && x<m_splitX)
		{
            m_camera[m_selected]->onRotatePress(x,y);
		}
        else if(m_selected==1 && x>m_splitX)
		{
            m_camera[m_selected]->onRotatePress(x,y);
		}
		return true;
    }

	bool onLeftDown(size_t x,size_t y)
	{
		y;
        if(x<m_splitX)
		{
            if(m_selected==0)
			{
				return false;
			}
			else
			{
                m_selected=0;
				return true;
			}
		}
        else if(x>m_splitX)
		{
            if(m_selected==1)
			{
				return false;
			}
			else
			{
                m_selected=1;
				return true;
			}
		}
		else
		{
			return false;
		}
    }

    bool onCtrlPress(size_t x,size_t y)
	{
        m_isCtrlMode=true;
        m_ctrlSX=x;
        m_ctrlSY=y;
		return true;
    }

    bool onCtrlRelease()
	{
        if(m_isCtrlMode)
		{
            m_isCtrlMode=false;
            m_ctrlSX=0;
            m_ctrlSY=0;
			return true;
        }
        else
        {
            return false;
        }
    }

    bool onCtrlDrag(size_t x,size_t y)
	{
        if(m_isCtrlMode)
		{
            m_camera[m_selected]->setCamera();
            Vector horizontalDir(m_camera[m_selected]->getHorizontalDir());
            theScene->ctrlDrag(horizontalDir,m_camera[m_selected]->m_up,x-m_ctrlSX,y-m_ctrlSY,m_isExtrude);
            m_isExtrude=false;
            m_ctrlSX=x;
            m_ctrlSY=y;
			return true;
		}
        else
        {
            return false;
        }
    }

	bool onSelectionPress(size_t x,size_t y)
	{
        m_isSelectionMode=true;
        m_selectionSX=x;
        m_selectionSY=y;
        m_selectionEX=x;
        m_selectionEY=y;
		return true;
    }

    bool onSelectionDrag(size_t x,size_t y)
	{
        if(m_isSelectionMode)
		{
            m_selectionEX=x;
            m_selectionEY=y;
			return true;
        }
        else
        {
            return false;
        }
    }

    bool onAxisDrag(size_t x,size_t y)
	{
        if(m_isAxisMode)
		{
            m_camera[m_selected]->setCamera();
            Vector m((float)((int)x-(int)m_axisDragSX),(float)((int)y-(int)m_axisDragSY),0.0f);
            float work=psVECDOT(m_cursorDir,m);
            theScene->axisDrag(work,m_cursorMode,m_cursorDir.x,m_cursorDir.y);
            m_axisDragSX=x;
            m_axisDragSY=y;
			return true;
		}
        else
        {
            return false;
        }
    }

    bool onAxisRelease()
	{
        if(m_isAxisMode)
		{
            m_isAxisMode=false;
            theScene->axisRelease();
            m_axisDragSX=0;
            m_axisDragSY=0;
			return true;
		}
        else
        {
            return false;
        }
	}

    bool onAxisPress(size_t x,size_t y)
	{
        m_camera[m_selected]->setCameraForSelectionS();
        m_cursorDir.z=0;
        bool result=theScene->isAxisSelected(m_camera[m_selected]->m_type._value,m_camera[m_selected]->getEye(),m_height,x,y,m_cursorDir.x,m_cursorDir.y,m_cursorMode);
		if(result)
		{
            m_axisDragSX=x;
            m_axisDragSY=y;
            m_cursorDir.normalize();
            m_isAxisMode=true;
		}
		return result;		
    }

    bool onSingleSideSelectionRelease(bool isAppend)
	{
        if(m_isSelectionMode)
		{
            size_t x1=(m_selectionSX>m_selectionEX)?m_selectionEX:m_selectionSX;
            size_t y1=(m_selectionSY>m_selectionEY)?m_selectionEY:m_selectionSY;
            size_t x2=(m_selectionSX>m_selectionEX)?m_selectionSX:m_selectionEX;
            size_t y2=(m_selectionSY>m_selectionEY)?m_selectionSY:m_selectionEY;
            m_camera[m_selected]->setCameraForSelectionS();
            theScene->selectSingleSide(x1,y1,x2,y2,m_height,isAppend);
            m_isSelectionMode=false;
            m_selectionSX=0;
            m_selectionSY=0;
            m_selectionEX=0;
            m_selectionEY=0;
			return true;
		}
        else
        {
            return false;
        }
    }

	bool onDualSideSelectionRelease(bool isAppend)
	{
        if(m_isSelectionMode)
		{
            size_t x1=(m_selectionSX>m_selectionEX)?m_selectionEX:m_selectionSX;
            size_t y1=(m_selectionSY>m_selectionEY)?m_selectionEY:m_selectionSY;
            size_t x2=(m_selectionSX>m_selectionEX)?m_selectionSX:m_selectionEX;
            size_t y2=(m_selectionSY>m_selectionEY)?m_selectionSY:m_selectionEY;
            m_camera[m_selected]->setCameraForSelectionD(x1,y1,x2,y2,m_height);
			theScene->selectDualSide(isAppend);
            m_isSelectionMode=false;
            m_selectionSX=0;
            m_selectionSY=0;
            m_selectionEX=0;
            m_selectionEY=0;
			return true;
		}
        else
        {
            return false;
        }
    }

	bool onRotateRelease()
	{
        m_camera[m_selected]->onRotateRelease();
		return true;
    }
	
	bool onPanRelease()
	{
        m_camera[m_selected]->onPanRelease(0,0);
        m_panIndex=-1;
		return true;
    }

	void update(size_t theSplitX,size_t theSplitY,size_t theWidth,size_t theHeight)
	{
        m_splitX=theSplitX;
        m_splitY=theSplitY;
        m_width=theWidth;
        m_height=theHeight;
        m_camera[0]->updateSize(0.0f,0.0f,(float)m_splitX,(float)m_height);
        m_camera[1]->updateSize((float)m_splitX,0.0f,(float)(m_width-m_splitX),(float)m_height);
    }

public:
	~TwoView(void);
};
