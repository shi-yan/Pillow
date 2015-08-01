#pragma once
#include "FourView.h"
#include "ThreeView.h"
#include "TwoView.h"
#include "SingleView.h"
#include "UILayer.h"
#include "wx/utils.h"
#include <wx/datetime.h>
#include "AboutDialog.h"
enum
{
	Four=32,
	Three,
	Two,
	Single
};

class Screen
{
private:
	FourView *fourView;
	ThreeView *threeView;
	SingleView *singleView;
	TwoView *twoView;
	SplitedView *currentView;
	UILayer *uiLayer;
	bool isSelectionMode;
	bool isAxisMode;
	bool isAltMode;
	bool isCtrlMode;
	bool isExtrude;
	wxWindow *mainFrame;
public:

	int width;
	int height;
Screen(wxWindow *theMain);
void updateScreen(unsigned int width,unsigned int height);
	void refresh(){};
	void onPaint();
	void screenShot(const char *fileName);
	void initialize();

	void changeCamera(CameraMode::__Enum type)
	{
		currentView->changeCamera(type);
	};

	void disableReference()
	{
		currentView->disableReference();
	}

	void getViewportImage(GLuint &textID,std::string &path,Vector &position,size_t &width,size_t &height)
	{
		currentView->getViewportImage(textID,path,position,width,height);
	}

	void setViewportImage(const char *path,GLuint imageID,Vector &position,size_t width,size_t height)
	{
		currentView->setViewportImage(path,imageID,position,width,height);
	}

	void onCtrlDown()
	{
		isCtrlMode=true;
	};

	void onCtrlUp()
	{
		isCtrlMode=false;
	};

	void changeView(int type)
	{
		if(type==Four)
		{
			currentView=fourView;
		}
		else
		if(type==Three)
		{
			currentView=threeView;
		}
		else
		if(type==Two)
		{
			currentView=twoView;
		}
		else 
		if(type==Single)
		{
			currentView=singleView;
		}
	};

	void onAltDown()
	{
		if(!isSelectionMode)
		{
			isAltMode=true;
		}
	}

	void onAltUp()
	{
		isAltMode=false;
	}

	bool onLeftDrag(int x,int y);
	bool onMiddleDrag(int x,int y);

	bool onMiddleRelease(bool isAppend)
	{
		if(isAltMode)
		{
			currentView->onPanRelease();
			isAltMode=false;
			return true;
		}
		else
		{
			currentView->onDualSideSelectionRelease(isAppend);
			isSelectionMode=false;
			return true;
		}
	};

	

	bool onLeftPress(int x,int y)
	{
		if(isAltMode)
		{
			currentView->onRotatePress(x,y);
			return true;
		}
		else
		{
			if(uiLayer->onLeftPress(x,y))
			{
				return true;
			}
			else
			{
				if(currentView->onLeftDown(x,y))
				{
					isAltMode=false;
					isAxisMode=false;
					isSelectionMode=false;
					isCtrlMode=false;
					return true;
				}
				else
				{
					if(currentView->onAxisPress(x,y))
					{
						isAxisMode=true;
						return true;
					}
					else
					{
						if(isCtrlMode)
						{
							currentView->onCtrlPress(x,y);
							return true;
						}
						else
						{
							currentView->onSelectionPress(x,y);
							isSelectionMode=true;
							isAxisMode=false;
							return true;
						}
					}
				}
			}
		}
	};

	bool onMoving(int x,int y)
	{
		return uiLayer->onMoving(x,y);
	}

	void onExtrudePress()
	{
		if(!isExtrude)
		{
			currentView->onExtrudeDown();
			isExtrude=true;
		}
	};
	
	void onExtrudeRelease()
	{
		currentView->onExtrudeRelease();
		isExtrude=false;
	};

	bool onLeftRelease(bool isAppend)
	{
		if(isAltMode)
		{
			currentView->onRotateRelease();
			isAltMode=false;
			return true;
		}
		else
		{
			ButtonEventType eventType=uiLayer->onLeftRelease();
			if(eventType!=ButtonEventType::Nothing)
			{
				if(eventType==ButtonEventType::Smooth)
				{
					toolButtonSmooth();
				}
				else if(eventType==ButtonEventType::Wire)
				{
					toolButtonWire();
				}
				else if(eventType==ButtonEventType::Faced)
				{
					toolButtonFaced();
				}
				else if(eventType==ButtonEventType::Wireframe)
				{
					toolButtonWireframe();
				}
				else if(eventType==ButtonEventType::WireframeFaced)
				{
					toolButtonWireframeFaced();
					//theScene->theObjectList[1]->testSelection();
				}
				else if(eventType==ButtonEventType::SingleView)
				{
					toolButtonSingleView();
				}
				else if(eventType==ButtonEventType::TwoView)
				{
					toolButtonTwoView();
				}
				else if(eventType==ButtonEventType::ThreeView)
				{
					toolButtonThreeView();
				}
				else if(eventType==ButtonEventType::FourView)
				{
					toolButtonFourView();
				}
				else if(eventType==ButtonEventType::VertexSelectionMode)
				{
					theScene->changeSelectionMode(SelectionMode::Vertex);
				}
				else if(eventType==ButtonEventType::EdgeSelectionMode)
				{
					theScene->changeSelectionMode(SelectionMode::Edge);
				}
				else if(eventType==ButtonEventType::FaceSelectionMode)
				{
					theScene->changeSelectionMode(SelectionMode::Face);
				}
				else if(eventType==ButtonEventType::ObjectSelectionMode)
				{
					theScene->changeSelectionMode(SelectionMode::Object);
				}
				else if(eventType==ButtonEventType::Help)
				{
					wxLaunchDefaultBrowser(_T("http://billconan.blogspot.com"));
				}
				else if(eventType==ButtonEventType::CreateCube)
				{
					theScene->newCube(0,0,0,100,100,100,1,1,1);
				}
				else if(eventType==ButtonEventType::CreateCylinder)
				{
					theScene->newCylinder(0,0,0,20,80,AxisMode::axisZ,8,1,1);
				}
				else if(eventType==ButtonEventType::CreateSphere)
				{
					theScene->newSphere(0,0,0,50,AxisMode::axisZ,8,4);
				}
				else if(eventType==ButtonEventType::CreatePlane)
				{
					theScene->newPlane(0,0,0,100,100,AxisMode::axisZ,1,1);
				}
				else if(eventType==ButtonEventType::CameraBack)
				{
					changeCamera(CameraMode::Back);
				}
				else if(eventType==ButtonEventType::CameraFront)
				{
					changeCamera(CameraMode::Front);
				}
				else if(eventType==ButtonEventType::CameraBottom)
				{
					changeCamera(CameraMode::Bottom);
				}
				else if(eventType==ButtonEventType::CameraTop)
				{
					changeCamera(CameraMode::Top);
				}
				else if(eventType==ButtonEventType::CameraLeft)
				{
					changeCamera(CameraMode::Left);
				}
				else if(eventType==ButtonEventType::CameraRight)
				{
					changeCamera(CameraMode::Right);
				}
				else if(eventType==ButtonEventType::CameraPerspective)
				{
					changeCamera(CameraMode::Perspective);
				}
				else if(eventType==ButtonEventType::Screenshot)
				{
					wxString filename;
					filename<<wxDateTime::GetTimeNow()<<_T(".jpg");
					screenShot(filename.c_str());
				}
				else if(eventType==ButtonEventType::About)
				{
					AboutDialog *aboutDialog=new AboutDialog(mainFrame);
					aboutDialog->ShowModal();
					aboutDialog->Destroy();
				}
				else if(eventType==ButtonEventType::New)
				{
					theScene->clearScene();
				}
				else if(eventType==ButtonEventType::Open)
				{
					wxString wildcards=wxString::Format(_T("PWB files (*.pwb)|*.pwb"),wxFileSelectorDefaultWildcardStr,wxFileSelectorDefaultWildcardStr);
					wxFileDialog dialog(mainFrame, _T("Open file:"),wxEmptyString, wxEmptyString, wildcards,wxOPEN);
					if(dialog.ShowModal()==wxID_OK)
					{
						wxArrayString paths, filenames;
						dialog.GetPaths(paths);
						dialog.GetFilenames(filenames);
						if(dialog.GetFilterIndex()==0)
						{
							theScene->clearScene();
							theScene->loadFromPWB(paths[0].c_str());
						}
					}
				}
				else if(eventType==ButtonEventType::Save)
				{
					if(theScene->fileName=="")
					{
						wxFileDialog dialog(mainFrame,_T("Save file:"),wxEmptyString,_T("untitle"),_T("Pillow Binary Files (*.pwb)|*.pwb"),wxSAVE|wxOVERWRITE_PROMPT);
						dialog.SetFilterIndex(1);
						if (dialog.ShowModal() == wxID_OK)
						{
							//wxLogMessage(_T("%s, filter %d"),dialog.GetPath().c_str(), dialog.GetFilterIndex());
							if(dialog.GetFilterIndex()==0)
							{
								theScene->fileName=dialog.GetPath().c_str();
								theScene->saveToFilePWB(dialog.GetPath().c_str());
							}
						}
					}
					else
					{
						theScene->saveToFilePWB(theScene->fileName.c_str());
					}
				}
				else if(eventType==ButtonEventType::SaveAs)
				{
					wxFileDialog dialog(mainFrame,_T("Save file:"),wxEmptyString,_T("untitle"),_T("Pillow Binary Files (*.pwb)|*.pwb|OBJ Files (*.obj)|*.obj"),wxSAVE|wxOVERWRITE_PROMPT);
					dialog.SetFilterIndex(0);
					if (dialog.ShowModal() == wxID_OK)
					{
						//wxLogMessage(_T("%s, filter %d"),dialog.GetPath().c_str(), dialog.GetFilterIndex());
						if(dialog.GetFilterIndex()==0)
						{
							theScene->fileName=dialog.GetPath().c_str();
							theScene->saveToFilePWB(dialog.GetPath().c_str());
						}
								else if(dialog.GetFilterIndex()==1)
		{
			theScene->fileName=dialog.GetPath().c_str();
			theScene->saveToFileOBJ(dialog.GetPath().c_str());
		}
					}
				}
				else if(eventType==ButtonEventType::DeleteVertex)
				{
					theScene->deleteVertex();
				}
				else if(eventType==ButtonEventType::Split)
				{
					if(!theScene->isSplitMode)
					{
						theScene->beginSplit();
					}
					else
					{
						theScene->endSplit();
					}
				}
				else if(eventType==ButtonEventType::Extrude)
				{
					theScene->extrudeFaceGroup(1,1,1);
				}
				else if(eventType==ButtonEventType::Collapse)
				{
					theScene->weldVertex();
				}
				return true;
			}
			else
			{
				if(currentView->onAxisRelease())
				{
					isAxisMode=false;
					return true;
				}
				else
				{
					if(currentView->onCtrlRelease())
					{
						isCtrlMode=false;
						return true;
					}
					else
					{
						currentView->onSingleSideSelectionRelease(isAppend);
						isSelectionMode=false;
						return true;
					}
				}
			}
		}
	}

	void toolButtonWireframeFaced()
	{
		theScene->changeShadeMode(RenderType::WireframeFaced);
	}

	void switchGird()
	{
		currentView->switchGird();
	}

	void toolButtonSmooth()
	{
		theScene->changeShadeMode(RenderType::Smooth);
	}

	void toolButtonSingleView()
	{
		changeView(Single);
	};

	void toolButtonTwoView()
	{
		changeView(Two);
	};

	void toolButtonThreeView()
	{
		changeView(Three);
	};

	void toolButtonFourView()
	{
		changeView(Four);	
	};

	void toolButtonFaced()
	{
		theScene->changeShadeMode(RenderType::Faced);
	}

	void toolButtonWireframe()
	{
		theScene->changeShadeMode(RenderType::Wireframe);
	}

	void toolButtonWire()
	{
		theScene->changeShadeMode(RenderType::Wire);
	}

	bool onMiddlePress(int x,int y)
	{
		if(isAltMode)
		{
			currentView->onPanPress(x,y);
			return true;
		}
		else
		{
			currentView->onSelectionPress(x,y);
			isSelectionMode=true;
			return true;
		}
	};

	bool onWheel(float step,int x,int y)
	{
		if(isAltMode)
		{
			currentView->onWheel(step,x,y);
			return true;
		}else return false;
	};

	private:
		void Begin2D()
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
		
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			
			gluOrtho2D(0, width, height, 0);
			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D); 
			glShadeModel(GL_SMOOTH); 
			glDisable(GL_DEPTH_TEST);
			glEnable (GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		};
	
		void End2D()
		{
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_TEXTURE_2D); 
			glEnable(GL_LIGHTING);
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		};
public:
	~Screen(void)
	{
		delete fourView;
		delete twoView;
		delete threeView;
		delete singleView;
		delete uiLayer;
	};
};

extern Screen *theScreen;
