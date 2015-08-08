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
		camera[index]->setCamera();
        return camera[index]->m_type._value;
	};

	bool onPanPress(size_t x,size_t y)
	{
		//这里要判断一下鼠标在哪个区域中
		if(selected==0 && x<splitX)
		{
			camera[selected]->onPanPress(x,y);
		}
		else if(selected==1 && x>splitX)
		{
			camera[selected]->onPanPress(x,y);
		}
		return true;
	};

	bool onPanDrag(size_t x,size_t y)
	{
		//这里要判断一下当前选择的pan区域
		camera[selected]->pan(x,y);
		return true;
	};

	void onWheel(float step,size_t x,size_t y)
	{
		//这里要判断一下鼠标在哪个区域中
		if(selected==0 && x<splitX)
		{
			camera[selected]->zoom(step);
		}
		else if(selected==1 && x>splitX)
		{
			camera[selected]->zoom(step);
		}
		y;
	};

	bool onRotateDrag(size_t x,size_t y)
	{
		camera[selected]->rotate(x,y);
		return true;
	};

	bool onRotatePress(size_t x,size_t y)
	{
		//这里要判断鼠标位置
		if(selected==0 && x<splitX)
		{
			camera[selected]->onRotatePress(x,y);
		}
		else if(selected==1 && x>splitX)
		{
			camera[selected]->onRotatePress(x,y);
		}
		return true;
	};

	bool onLeftDown(size_t x,size_t y)
	{
		y;
		if(x<splitX)
		{
			if(selected==0)
			{
				return false;
			}
			else
			{
				selected=0;
				return true;
			}
		}
		else if(x>splitX)
		{
			if(selected==1)
			{
				return false;
			}
			else
			{
				selected=1;
				return true;
			}
		}
		else
		{
			return false;
		}
	};

		bool onCtrlPress(size_t x,size_t y)
	{
		isCtrlMode=true;
		ctrlSX=x;
		ctrlSY=y;
		return true;
	};

			bool onCtrlRelease()
	{
		if(isCtrlMode)
		{
			isCtrlMode=false;
			ctrlSX=0;
			ctrlSY=0;
			return true;
		}else 
			return false;
	};

				bool onCtrlDrag(size_t x,size_t y)
	{
		if(isCtrlMode)
		{
			camera[selected]->setCamera();
			Vector horizontalDir(camera[selected]->getHorizontalDir());
            theScene->ctrlDrag(horizontalDir,camera[selected]->m_up,x-ctrlSX,y-ctrlSY,isExtrude);
			isExtrude=false;
			ctrlSX=x;
			ctrlSY=y;
			return true;
		}
		else 
			return false;
	};

	bool onSelectionPress(size_t x,size_t y)
	{
		isSelectionMode=true;
		selectionSX=x;
		selectionSY=y;
		selectionEX=x;
		selectionEY=y;
		return true;
	};

		bool onSelectionDrag(size_t x,size_t y)
	{
		if(isSelectionMode)
		{
			selectionEX=x;
			selectionEY=y;
			return true;
		}else
			return false;
	};

			bool onAxisDrag(size_t x,size_t y)
	{
		//if((selected==0 && x<splitX)||(selected==1 && x>splitX)){
		//这里要计算向量的点乘
		if(isAxisMode)
		{
			camera[selected]->setCamera();
			//这里必须强制转换
			Vector m((float)((int)x-(int)axisDragSX),(float)((int)y-(int)axisDragSY),0.0f);
			float work=psVECDOT(cursorDir,m);
			theScene->axisDrag(work,cursorMode,cursorDir.x,cursorDir.y);
			axisDragSX=x;
			axisDragSY=y;
			return true;
		}
		else 
			return false;//}else return false;
	};

				bool onAxisRelease()
	{
		if(isAxisMode)
		{
			isAxisMode=false;
			theScene->axisRelease();
			axisDragSX=0;
			axisDragSY=0;
			return true;
		}
		else
			return false;
	}

					bool onAxisPress(size_t x,size_t y)
	{
		//if((selected==0 && x<splitX)||(selected==1 && x>splitX)){
		camera[selected]->setCameraForSelectionS();
		cursorDir.z=0;
        bool result=theScene->isAxisSelected(camera[selected]->m_type._value,camera[selected]->getEye(),height,x,y,cursorDir.x,cursorDir.y,cursorMode);
		if(result)
		{
			axisDragSX=x;
			axisDragSY=y;
			cursorDir.normalize();
			isAxisMode=true;
		}
		return result;		
		/*}
		else
		{
			return false;
		}*/
	};

			bool onSingleSideSelectionRelease(bool isAppend)
	{
		if(isSelectionMode)
		{
			size_t x1=(selectionSX>selectionEX)?selectionEX:selectionSX;
			size_t y1=(selectionSY>selectionEY)?selectionEY:selectionSY;
			size_t x2=(selectionSX>selectionEX)?selectionSX:selectionEX;
			size_t y2=(selectionSY>selectionEY)?selectionSY:selectionEY;
			camera[selected]->setCameraForSelectionS();
			theScene->selectSingleSide(x1,y1,x2,y2,height,isAppend);
			isSelectionMode=false;
			selectionSX=0;
			selectionSY=0;
			selectionEX=0;
			selectionEY=0;
			return true;
		}
		else 
			return false;
	};

	bool onDualSideSelectionRelease(bool isAppend)
	{
		if(isSelectionMode)
		{
			size_t x1=(selectionSX>selectionEX)?selectionEX:selectionSX;
			size_t y1=(selectionSY>selectionEY)?selectionEY:selectionSY;
			size_t x2=(selectionSX>selectionEX)?selectionSX:selectionEX;
			size_t y2=(selectionSY>selectionEY)?selectionSY:selectionEY;
			camera[selected]->setCameraForSelectionD(x1,y1,x2,y2,height);
			theScene->selectDualSide(isAppend);
			isSelectionMode=false;
			selectionSX=0;
			selectionSY=0;
			selectionEX=0;
			selectionEY=0;
			return true;
		}
		else 
			return false;
	};

	bool onRotateRelease()
	{
		camera[selected]->onRotateRelease();
		return true;
	};
	
	bool onPanRelease()
	{
		camera[selected]->onPanRelease(0,0);
		panIndex=-1;
		return true;
	};

	void update(size_t theSplitX,size_t theSplitY,size_t theWidth,size_t theHeight)
	{
		splitX=theSplitX;
		splitY=theSplitY;
		width=theWidth;
		height=theHeight;
		camera[0]->updateSize(0.0f,0.0f,(float)splitX,(float)height);
		camera[1]->updateSize((float)splitX,0.0f,(float)(width-splitX),(float)height);
	};

public:
	~TwoView(void);
};
