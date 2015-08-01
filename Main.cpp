#include "wx/wxprec.h"

#ifndef WX_PRECOMP
       #include "wx/wx.h"
#endif
#include "wx/splash.h"
#include "wx/utils.h"

#include "Main.h"
#include "OpenGLCanvas.h"
#include "NewCubeDialog.h"
#include "AboutDialog.h"
#include "NewCylinderDialog.h"
#include "NewSphereDialog.h"
#include "NewPlaneDialog.h"
#include "SceneInfoDialog.h"
#include "MaterialDialog.h"
#include "ExtrudeDialog.h"
#include "InstantMirrorDialog.h"
#include "ReferenceImageDialog.h"

Scene *theScene;
Screen *theScreen;

IMPLEMENT_APP(PolygonStudio)

bool PolygonStudio::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);
	wxImage::AddHandler(new wxJPEGHandler);
	wxBitmap splashMap;
	if (splashMap.LoadFile("splash.dat", wxBITMAP_TYPE_PNG))
	{
      wxSplashScreen* splash = new wxSplashScreen(splashMap,
          wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
          500, NULL, -1, wxDefaultPosition, wxDefaultSize,
          wxSIMPLE_BORDER|wxSTAY_ON_TOP);
	}
	wxYield();
	theScene=new Scene();
	theScene->initialize();
//	test();
    MainFrame *theMainFrame = new MainFrame(_T("Pillow 0.1a"));
	theMainFrame->CreateStatusBar();
	theMainFrame->SetStatusText(_T("Pillow 0.1a"));
	theMainFrame->Show(true);

	OpenGLCanvas *theCanvas=new OpenGLCanvas(theMainFrame, wxID_ANY,wxDefaultPosition, wxDefaultSize);
	SetTopWindow(theMainFrame);
	theCanvas->initScreen();
	return true;
}

MainFrame::MainFrame(const wxString& title):wxFrame(NULL, wxID_ANY, title,wxDefaultPosition, wxDefaultSize,wxDEFAULT_FRAME_STYLE|wxMAXIMIZE, "MainFrame")
{
	SetIcon(wxICON(pillow));
	this->Maximize(true); 
	wxMenuBar *menuBar = new wxMenuBar();
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(MenuFileNewScene, _T("New Scene"), _T("New scene."));
	fileMenu->Append(MenuFileOpenScene, _T("Open Scene"), _T("Open a file."));
	fileMenu->Append(MenuFileSaveScene, _T("Save Scene"), _T("Save file."));
	fileMenu->Append(MenuFileSaveSceneAs, _T("Save Scene As"), _T("Save scene as."));
	fileMenu->AppendSeparator();
	fileMenu->Append(MenuFileLoadIntoScene, _T("Load Into Scene"), _T("Load into scene."));
	fileMenu->AppendSeparator();
//	fileMenu->Append(MenuFileConsole, _T("Console"), _T("Open the console."));
//	fileMenu->AppendSeparator();
	fileMenu->Append(MenuFileExit, _T("Exit"), _T("Exit Pillow."));
	wxMenu *editMenu = new wxMenu;
	editMenu->Append(MenuEditUndo, _T("* Undo"), _T("Undo the current scene."));
	editMenu->Append(MenuEditRedo, _T("* Redo"), _T("Redo the current scene."));
	editMenu->AppendSeparator();
	editMenu->Append(MenuEditScreenshot, _T("Screenshot"), _T("Save the screenshot."));
	editMenu->AppendSeparator();
	editMenu->Append(MenuEditHistoryView, _T("* History View"), _T("History View."));
	wxMenu *createMenu = new wxMenu;
	createMenu->Append(MenuCreateCube,_T("Cube"), _T("Create cube."));
	createMenu->Append(MenuCreateCylinder,_T("Cylinder"), _T("Create cylinder."));
	createMenu->Append(MenuCreateSphere,_T("Sphere"), _T("Create sphere."));
//	createMenu->Append(MenuCreateCone,_T("Cone"), _T("Create cone."));
//	createMenu->Append(MenuCreateTorus,_T("Torus"), _T("Create torus."));
	createMenu->Append(MenuCreatePlane,_T("Plane"), _T("Create plane."));
	createMenu->AppendSeparator();
	createMenu->Append(MenuCreateClone,_T("Clone"), _T("Create Clone."));
	createMenu->Append(MenuCreateMirror,_T("Mirror"), _T("Create Mirror."));
	createMenu->AppendSeparator();
	createMenu->Append(MenuCreateMirrorInstance,_T("Mirror Instance"), _T("Create Mirror instance."));
	createMenu->Append(MenuCreateRemoveMirrorInstance,_T("Remove Mirror Instance"), _T("Remove Mirror instance."));
	wxMenu *modifyMenu = new wxMenu;
	modifyMenu->Append(MenuModifyMerge,_T("Merge"), _T("Merge Selection."));
	modifyMenu->Append(MenuModifyDetach,_T("Detach"),_T("Detach Selection."));
	modifyMenu->Append(MenuModifyDeleteVertex,_T("Delete Vertex"),_T("Delete Selected Vertices."));
//	modifyMenu->Append(MenuModifyMirror,_T("Mirror"), _T("Mirror."));
//	modifyMenu->Append(MenuModifyBevel,_T("Bevel"), _T("Bevel."));
	modifyMenu->Append(MenuModifyExtrude,_T("Extrude"), _T("Extrude."));
	modifyMenu->Append(MenuModifySplit,_T("Split"), _T("Split tool."));
	modifyMenu->Append(MenuModifyCollapse,_T("Collapse"), _T("Collapse."));
	modifyMenu->Append(MenuModifyTargetWeld,_T("Target Weld"), _T("Target weld."));
	modifyMenu->Append(MenuModifyUpdateNormal,_T("update Normal"), _T("update normal."));
	wxMenu *subdivisionMenu = new wxMenu;
	subdivisionMenu->Append(MenuSubdivisionSubdivide,_T("Subdivide"), _T("Subdivide."));
	subdivisionMenu->Append(MenuSubdivisionUnsubdivide,_T("Unsubdivide"), _T("Unsubdivide."));
	subdivisionMenu->Append(MenuSubdivisionRefineControlMesh,_T("Refine Control Mesh"), _T("Refine control mesh."));
	wxMenu *selectionMenu = new wxMenu;
	wxMenu* selectionMenuSelectionMode = new wxMenu;
	selectionMenuSelectionMode->AppendRadioItem(MenuSelectionVertexSelection,_T("Vertex Selection"), _T("Vertex selection."));
	selectionMenuSelectionMode->AppendRadioItem(MenuSelectionEdgeSelection,_T("Edge Selection"), _T("Edge selection."));
	selectionMenuSelectionMode->AppendRadioItem(MenuSelectionFaceSelection,_T("Face Selection"), _T("Face selection."));
	selectionMenuSelectionMode->AppendRadioItem(MenuSelectionObjectSelection,_T("Object Selection"), _T("Object selection."));
	selectionMenu->Append(MenuSelectionSelectionMode,_T("Selection Mode"),selectionMenuSelectionMode);
	selectionMenu->Append(MenuSelectionSelectAll,_T("Select All"), _T("Select all."));
	selectionMenu->Append(MenuSelectionInvertSelect,_T("Invert Selection"), _T("Invert Selection."));
//	selectionMenu->Append(MenuSelectionSelectLoop,_T("Select Loop"), _T("Select loop."));
	selectionMenu->Append(MenuSelectionFrameSelection,_T("* Frame Selection"), _T("Frame selection."));

	wxMenu *optionMenu = new wxMenu;
	optionMenu->Append(MenuOptionMaterial,_T("Material"), _T("Set material."));
	wxMenu *optionMenuObjectDisplayMode = new wxMenu;

	optionMenuObjectDisplayMode->Append(MenuOptionObjectDisplayModeWireframe,_T("Wireframe"), _T("Wireframe display mode."));
	optionMenuObjectDisplayMode->Append(MenuOptionObjectDisplayModeFlat,_T("Flat"), _T("Flat display mode."));
	optionMenuObjectDisplayMode->Append(MenuOptionObjectDisplayModeFlatWire,_T("Flat Wire"), _T("Flat wire display mode."));
	optionMenuObjectDisplayMode->Append(MenuOptionObjectDisplayModeSmooth,_T("Smooth"), _T("Smooth display mode."));
	optionMenuObjectDisplayMode->Append(MenuOptionObjectDisplayModeSmoothWire,_T("Smooth Wire"), _T("Smooth wire display mode."));
//	optionMenuObjectDisplayMode->Append(MenuOptionObjectDisplayModeTransparentFlat,_T("Transparent Flat"), _T("Transparent flat display mode."));
//	optionMenuObjectDisplayMode->Append(MenuOptionObjectDisplayModeTransparentSmooth,_T("Transparent Smooth"), _T("Transparent smooth display mode."));
	optionMenu->Append(MenuOptionObjectDisplayMode,_T("Object Display Mode"), optionMenuObjectDisplayMode);


	optionMenu->Append(MenuOptionHideSelection,_T("Hide Selection"), _T("Hide selection."));
	optionMenu->Append(MenuOptionHideUnselection,_T("Hide Unselection"), _T("Hide unselection."));
	optionMenu->Append(MenuOptionShowAll,_T("Show All"), _T("Show all."));
	wxMenu *optionMenuViewportLayout = new wxMenu;
	optionMenuViewportLayout->Append(MenuOptionViewportLayoutSingle,_T("Single"), _T("Single."));
	optionMenuViewportLayout->Append(MenuOptionViewportLayoutFourView,_T("Four View"), _T("Four view."));
	optionMenuViewportLayout->Append(MenuOptionViewportLayoutVerticalSplited,_T("Vertical Splited"), _T("Vertical Splited."));
	optionMenuViewportLayout->Append(MenuOptionViewportLayoutThreeView,_T("Three view"), _T("Three view."));
	optionMenu->Append(MenuOptionViewportLayout,_T("Viewport Layout"),optionMenuViewportLayout);
	wxMenu *optionMenuViewportCamera = new wxMenu;
	optionMenuViewportCamera->Append(MenuOptionViewportCameraPerspective,_T("Perspective"), _T("Perspective."));
	optionMenuViewportCamera->Append(MenuOptionViewportCameraTop,_T("Top"), _T("Top."));
	optionMenuViewportCamera->Append(MenuOptionViewportCameraBottom,_T("Bottom"), _T("Bottom."));
	optionMenuViewportCamera->Append(MenuOptionViewportCameraLeft,_T("Left"), _T("Left."));
	optionMenuViewportCamera->Append(MenuOptionViewportCameraRight,_T("Right"), _T("Right."));
	optionMenuViewportCamera->Append(MenuOptionViewportCameraFront,_T("Front"), _T("Front."));
	optionMenuViewportCamera->Append(MenuOptionViewportCameraBack,_T("Back"), _T("Back."));
	
	optionMenu->Append(MenuOptionViewportCamera,_T("Viewport Camera"),optionMenuViewportCamera);
	optionMenu->AppendCheckItem(MenuOptionShowGird,_T("Show Gird"), _T("Show gird."));
	optionMenu->Append(MenuOptionSetViewportImage,_T("Set Viewport Image"), _T("Set viewport Image."));
	optionMenu->Append(MenuOptionDeleteViewportImage,_T("Delete Viewport Image"), _T("Delete Viewport Image."));
	optionMenu->Append(MenuOptionSceneInfo,_T("Scene Info"), _T("Scene Info."));

	wxMenu *helpMenu = new wxMenu;
//	helpMenu->Append(MenuHelpHelp, _T("&Help"), _T("Show help."));
	helpMenu->Append(MenuHelpAbout, _T("&About"), _T("Show about."));
	
	menuBar->Append(fileMenu, _T("&File"));
	menuBar->Append(editMenu, _T("&Edit"));
	menuBar->Append(createMenu, _T("&Create"));
	menuBar->Append(modifyMenu, _T("&Modify"));
	menuBar->Append(subdivisionMenu, _T("&Subdivision"));
	menuBar->Append(selectionMenu, _T("&Selection"));
	menuBar->Append(optionMenu, _T("&Option"));
	menuBar->Append(helpMenu, _T("&Help"));
	SetMenuBar(menuBar);
	//this->theConsoleDialog= new ConsoleDialog(this);
//	theConsole->dialogInstance=this->theConsoleDialog;
}

void MainFrame::onMenuFileNewScene(wxCommandEvent& event)
{
	theScene->clearScene();
	Refresh(false);
	event;
}

void MainFrame::onMenuFileOpenScene(wxCommandEvent& event)
{
	event;
	wxString wildcards=wxString::Format(_T("PWB files (*.pwb)|*.pwb"),wxFileSelectorDefaultWildcardStr,wxFileSelectorDefaultWildcardStr);
	wxFileDialog dialog(this, _T("Open file:"),wxEmptyString, wxEmptyString, wildcards,wxOPEN);
	if(dialog.ShowModal()==wxID_OK)
	{
		wxArrayString paths, filenames;
		dialog.GetPaths(paths);
		dialog.GetFilenames(filenames);
//		size_t count=paths.GetCount();
		if(dialog.GetFilterIndex()==0)
		{
				//s.Printf(_T("File %d: %s (%s)\n"),(int)n, paths[n].c_str(), filenames[n].c_str());
				//msg+=s;
				theScene->clearScene();
				theScene->loadFromPWB(paths[0].c_str());
		}
		Refresh(false);
	}
}

void MainFrame::onMenuFileSaveScene(wxCommandEvent& event)
{
	if(theScene->fileName=="")
	{
		//|Document files (*.doc)|*.doc
		wxFileDialog dialog(this,_T("Save file:"),wxEmptyString,_T("untitle"),_T("Pillow Binary Files (*.pwb)|*.pwb"),wxSAVE|wxOVERWRITE_PROMPT);
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
	event;
}

void MainFrame::onMenuFileSaveSceneAs(wxCommandEvent& event)
{
	wxFileDialog dialog(this,_T("Save file:"),wxEmptyString,_T("untitle"),_T("Pillow Binary Files (*.pwb)|*.pwb|OBJ Files (*.obj)|*.obj"),wxSAVE|wxOVERWRITE_PROMPT);
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
	event;
}

void MainFrame::onMenuFileLoadIntoScene(wxCommandEvent& event)
{
	wxString wildcards=wxString::Format(_T("PWB files (*.pwb)|*.pwb|OBJ files (*.obj)|*.obj"),wxFileSelectorDefaultWildcardStr,wxFileSelectorDefaultWildcardStr);
	wxFileDialog dialog(this, _T("Load into the scene:"),wxEmptyString, wxEmptyString, wildcards,wxMULTIPLE);
	if(dialog.ShowModal()==wxID_OK)
	{
		wxArrayString paths, filenames;
		dialog.GetPaths(paths);
		dialog.GetFilenames(filenames);
		size_t count=paths.GetCount();
		if(dialog.GetFilterIndex()==0)
		{
			for(size_t n=0;n<count;n++)
			{
				//s.Printf(_T("File %d: %s (%s)\n"),(int)n, paths[n].c_str(), filenames[n].c_str());
				//msg+=s;
				theScene->loadFromPWB(paths[n].c_str());
			}
		}
		else if(dialog.GetFilterIndex()==1)
		{
			for(size_t n=0;n<count;n++)
			{
				//s.Printf(_T("File %d: %s (%s)\n"),(int)n, paths[n].c_str(), filenames[n].c_str());
				//msg+=s;
//				theScene->loadFromOBJ(paths[n].c_str());
			}
		}
		Refresh(false);
/*		s.Printf(_T("Filter index: %d"), dialog.GetFilterIndex());
		msg+=s;
		wxMessageDialog dialog2(this, msg, _T("Selected files"));
		dialog2.ShowModal();*/
	}
	event;
}

/*void MainFrame::onMenuFileConsole(wxCommandEvent& event)
{
	this->theConsoleDialog->Show(true);
	this->theConsoleDialog->refresh();
	event;
}*/

void MainFrame::onMenuFileExit(wxCommandEvent& event)
{
	Close(true);
	event;
}

void MainFrame::onMenuEditUndo(wxCommandEvent& event)
{event;
}

void MainFrame::onMenuEditRedo(wxCommandEvent& event)
{event;
}

void MainFrame::onMenuEditScreenshot(wxCommandEvent& event)
{
	wxFileDialog dialog(this,_T("Save screenshot:"),wxEmptyString,_T("untitle"),_T("JPEG Files (*.jpg)|*.jpg"),wxSAVE|wxOVERWRITE_PROMPT);
    if (dialog.ShowModal() == wxID_OK)
	{
			theScreen->screenShot(dialog.GetPath().c_str());
	}
	event;
}

void MainFrame::onMenuEditHistoryView(wxCommandEvent& event)
{
event;
}

void MainFrame::onMenuCreateCube(wxCommandEvent& event)
{
	//theConsole->cprintf("cube\n");
	//this->theConsoleDialog->refresh();
	NewCubeDialog *newCubeDialog=new NewCubeDialog(this);
	newCubeDialog->ShowModal();
	//newCubeDialog->refresh();
	newCubeDialog->Destroy();
	Refresh(false);
	event;
}

void MainFrame::onMenuCreateCylinder(wxCommandEvent& event)
{
	NewCylinderDialog *newCylinderDialog=new NewCylinderDialog(this);
	newCylinderDialog->ShowModal();
	newCylinderDialog->Destroy();
	Refresh(false);
	event;
}

void MainFrame::onMenuCreateSphere(wxCommandEvent& event)
{
	NewSphereDialog *newSphereDialog=new NewSphereDialog(this);
	newSphereDialog->ShowModal();
	newSphereDialog->Destroy();
	Refresh(false);
	event;
}

/*void MainFrame::onMenuCreateCone(wxCommandEvent& event)
{event;
}*/

/*void MainFrame::onMenuCreateTorus(wxCommandEvent& event)
{event;
}*/

void MainFrame::onMenuCreatePlane(wxCommandEvent& event)
{
	NewPlaneDialog *newPlaneDialog=new NewPlaneDialog(this);
	newPlaneDialog->ShowModal();
	newPlaneDialog->Destroy();
	Refresh(false);
	event;
}

void MainFrame::onMenuCreateClone(wxCommandEvent& event)
{
	theScene->clone();
	Refresh(false);
	event;
}

void MainFrame::onMenuCreateMirror(wxCommandEvent& event)
{
		theScene->mirrorClone();
	Refresh(false);
event;
}

void MainFrame::onMenuCreateMirrorInstance(wxCommandEvent& event)
{
		InstantMirrorDialog *instantMirrorDialog=new InstantMirrorDialog(this);
	instantMirrorDialog->ShowModal();
	instantMirrorDialog->Destroy();
	Refresh(false);
	
	event;
}

void MainFrame::onMenuCreateRemoveMirrorInstance(wxCommandEvent& event)
{
	theScene->unMirror();
	Refresh(false);
	event;
}

void MainFrame::onMenuModifyMerge(wxCommandEvent& event)
{
	theScene->onMerge();
	Refresh(false);
	event;
}

void MainFrame::onMenuModifyDetach(wxCommandEvent& event)
{
	theScene->onDetach();
	Refresh(false);
	event;
}

void MainFrame::onMenuModifyDeleteVertex(wxCommandEvent& event)
{
	theScene->deleteVertex();
	Refresh(false);
	event;
}

/*void MainFrame::onMenuModifyMirror(wxCommandEvent& event)
{
	event;
}*/

/*void MainFrame::onMenuModifyBevel(wxCommandEvent& event)
{
	event;
}*/

void MainFrame::onMenuModifyExtrude(wxCommandEvent& event)
{
	ExtrudeDialog *extrudeDialog=new ExtrudeDialog(this);
	extrudeDialog->ShowModal();
	extrudeDialog->Destroy();
	Refresh(false);
	event;
}

void MainFrame::onMenuModifySplit(wxCommandEvent& event)
{
	if(!theScene->isSplitMode)
	{
		theScene->beginSplit();
		Refresh(false);
	}
	else
	{
		theScene->endSplit();
		Refresh(false);
	}
	event;
}

void MainFrame::onMenuModifyCollapse(wxCommandEvent& event)
{
	theScene->weldVertex();
	Refresh(false);
	event;
}

void MainFrame::onMenuModifyTargetWeld(wxCommandEvent& event)
{event;
}

void MainFrame::onMenuModifyUpdateNormal(wxCommandEvent& event)
{
	theScene->updateNormal();
	Refresh(false);
	event;
}

void MainFrame::onMenuSubdivisionSubdivide(wxCommandEvent& event)
{
	theScene->subdivide();
	Refresh(false);
	event;
}

void MainFrame::onMenuSubdivisionUnsubdivide(wxCommandEvent& event)
{
	theScene->unSubdivide();
	Refresh();
	event;
}

void MainFrame::onMenuSubdivisionRefineControlMesh(wxCommandEvent& event)
{
	 wxMessageDialog dialog( NULL, _T("You cannot revert this operation!"),
		 _T("Warning:"), wxNO_DEFAULT|wxYES_NO|wxICON_INFORMATION);

    switch ( dialog.ShowModal() )
    {
        case wxID_YES:
			theScene->redefineControlPoint();
			Refresh(false);
            break;
        case wxID_NO:
            break;
        default:
            wxLogError(wxT("Unexpected wxMessageDialog return code!"));
			break;
    }
	event;
}

void MainFrame::onMenuSelectionSelectionMode(wxCommandEvent& event)
{event;
}

void MainFrame::onMenuSelectionVertexSelection(wxCommandEvent& event)
{
	theScene->changeSelectionMode(SelectionMode::Vertex);
	Refresh(false);
	event;
}

void MainFrame::onMenuSelectionEdgeSelection(wxCommandEvent& event)
{
	theScene->changeSelectionMode(SelectionMode::Edge);
	Refresh(false);
	event;
}

void MainFrame::onMenuSelectionFaceSelection(wxCommandEvent& event)
{
	theScene->changeSelectionMode(SelectionMode::Face);
	Refresh(false);
	event;
}

void MainFrame::onMenuSelectionObjectSelection(wxCommandEvent& event)
{
	theScene->changeSelectionMode(SelectionMode::Object);
	Refresh(false);
	event;
}

void MainFrame::onMenuSelectionSelectAll(wxCommandEvent& event)
{
	theScene->selectAll();
	Refresh(false);
	event;
}

void MainFrame::onMenuSelectionInvertSelect(wxCommandEvent& event)
{
	theScene->invertSelection();
	Refresh(false);
	event;
}

/*void MainFrame::onMenuSelectionSelectLoop(wxCommandEvent& event)
{
	event;
}*/

void MainFrame::onMenuOptionMaterial(wxCommandEvent& event)
{
	MaterialDialog *materialDialog=new MaterialDialog(this);
	materialDialog->ShowModal();
	materialDialog->Destroy();
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionObjectDisplayMode(wxCommandEvent& event)
{
	event;
}

void MainFrame::onMenuOptionObjectDisplayModeWireframe(wxCommandEvent& event)
{
	theScene->changeShadeMode(RenderType::Wire);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionObjectDisplayModeFlat(wxCommandEvent& event)
{
	theScene->changeShadeMode(RenderType::Faced);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionObjectDisplayModeFlatWire(wxCommandEvent& event)
{
	theScene->changeShadeMode(RenderType::WireframeFaced);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionObjectDisplayModeSmooth(wxCommandEvent& event)
{
	theScene->changeShadeMode(RenderType::Smooth);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionObjectDisplayModeSmoothWire(wxCommandEvent& event)
{
	theScene->changeShadeMode(RenderType::Wireframe);
	Refresh(false);
	event;
}

void MainFrame::onMenuSelectionFrameSelection(wxCommandEvent& event)
{
	event;
}

void MainFrame::onMenuOptionHideSelection(wxCommandEvent& event)
{
	theScene->hideSelection();
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionHideUnselection(wxCommandEvent& event)
{
	theScene->hideUnselection();
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionShowAll(wxCommandEvent& event)
{
	theScene->unhideAll();
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionViewportLayout(wxCommandEvent& event)
{
	event;
}

void MainFrame::onMenuOptionViewportLayoutSingle(wxCommandEvent& event)
{
	theScreen->changeView(35);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionViewportLayoutFourView(wxCommandEvent& event)
{
	theScreen->changeView(32);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionViewportLayoutVerticalSplited(wxCommandEvent& event)
{
	theScreen->changeView(34);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionViewportLayoutThreeView(wxCommandEvent& event)
{
	theScreen->changeView(33);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionViewportCamera(wxCommandEvent& event)
{
	event;
}

void MainFrame::onMenuOptionViewportCameraPerspective(wxCommandEvent& event)
{
	theScreen->changeCamera(CameraMode::Perspective);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionViewportCameraTop(wxCommandEvent& event)
{
	theScreen->changeCamera(CameraMode::Top);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionViewportCameraBottom(wxCommandEvent& event)
{
	theScreen->changeCamera(CameraMode::Bottom);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionViewportCameraLeft(wxCommandEvent& event)
{
	theScreen->changeCamera(CameraMode::Left);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionViewportCameraRight(wxCommandEvent& event)
{
	theScreen->changeCamera(CameraMode::Right);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionViewportCameraFront(wxCommandEvent& event)
{
	theScreen->changeCamera(CameraMode::Front);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionViewportCameraBack(wxCommandEvent& event)
{
	theScreen->changeCamera(CameraMode::Back);
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionShowGird(wxCommandEvent& event)
{
	theScreen->switchGird();
	Refresh();
	event;
}

void MainFrame::onMenuOptionSetViewportImage(wxCommandEvent& event)
{
	ReferenceImageDialog *referenceImageDialog=new ReferenceImageDialog(this);
	referenceImageDialog->ShowModal();
	referenceImageDialog->Destroy();
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionDeleteViewportImage(wxCommandEvent& event)
{
	theScreen->disableReference();
	Refresh(false);
	event;
}

void MainFrame::onMenuOptionSceneInfo(wxCommandEvent& event)
{
	SceneInfoDialog *sceneInfoDialog=new SceneInfoDialog(this);
	sceneInfoDialog->ShowModal();
	sceneInfoDialog->Destroy();
	Refresh(false);
	event;
}

/*void MainFrame::onMenuHelpHelp(wxCommandEvent& event)
{
	wxLaunchDefaultBrowser(_T("http://billconan.blogspot.com"));
	event;
}*/

void MainFrame::onMenuHelpAbout(wxCommandEvent& event)
{
	AboutDialog *aboutDialog=new AboutDialog(this);
	aboutDialog->ShowModal();
	aboutDialog->Destroy();
	event;
}

MainFrame::~MainFrame()
{
//	delete this->theConsoleDialog;
	//delete theScene;
	//delete theScreen;
}