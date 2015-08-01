#pragma once
//#include "ConsoleDialog.h"

class PolygonStudio : public wxApp
{
	public:
		virtual bool OnInit();
};

DECLARE_APP(PolygonStudio)

class MainFrame : public wxFrame
{
private:
//	ConsoleDialog *theConsoleDialog;
	public:
		MainFrame(const wxString& title);
	~MainFrame(void);
void onMenuFileNewScene(wxCommandEvent& event);
void onMenuFileOpenScene(wxCommandEvent& event);
void onMenuFileSaveScene(wxCommandEvent& event);
void onMenuFileSaveSceneAs(wxCommandEvent& event);
void onMenuFileLoadIntoScene(wxCommandEvent& event);
//void onMenuFileConsole(wxCommandEvent& event);
void onMenuFileExit(wxCommandEvent& event);
void onMenuEditUndo(wxCommandEvent& event);
void onMenuEditRedo(wxCommandEvent& event);
void onMenuEditScreenshot(wxCommandEvent& event);
void onMenuEditHistoryView(wxCommandEvent& event);

void onMenuCreateCube(wxCommandEvent& event);
void onMenuCreateCylinder(wxCommandEvent& event);
void onMenuCreateSphere(wxCommandEvent& event);
void onMenuCreatePlane(wxCommandEvent& event);

void onMenuCreateClone(wxCommandEvent& event);
void onMenuCreateMirror(wxCommandEvent& event);
void onMenuCreateMirrorInstance(wxCommandEvent& event);
void onMenuCreateRemoveMirrorInstance(wxCommandEvent& event);

void onMenuModifyMerge(wxCommandEvent& event);
void onMenuModifyDetach(wxCommandEvent& event);
void onMenuModifyDeleteVertex(wxCommandEvent& event);
void onMenuModifyExtrude(wxCommandEvent& event);
void onMenuModifySplit(wxCommandEvent& event);
void onMenuModifyCollapse(wxCommandEvent& event);
void onMenuModifyTargetWeld(wxCommandEvent& event);
void onMenuModifyUpdateNormal(wxCommandEvent& event);

void onMenuSubdivisionSubdivide(wxCommandEvent& event);
void onMenuSubdivisionUnsubdivide(wxCommandEvent& event);
void onMenuSubdivisionRefineControlMesh(wxCommandEvent& event);

void onMenuSelectionSelectionMode(wxCommandEvent& event);
void onMenuSelectionVertexSelection(wxCommandEvent& event);
void onMenuSelectionEdgeSelection(wxCommandEvent& event);
void onMenuSelectionFaceSelection(wxCommandEvent& event);
void onMenuSelectionObjectSelection(wxCommandEvent& event);
void onMenuSelectionSelectAll(wxCommandEvent& event);
void onMenuSelectionInvertSelect(wxCommandEvent& event);
void onMenuSelectionFrameSelection(wxCommandEvent& event);

void onMenuOptionMaterial(wxCommandEvent& event);
void onMenuOptionObjectDisplayMode(wxCommandEvent& event);
void onMenuOptionObjectDisplayModeWireframe(wxCommandEvent& event);
void onMenuOptionObjectDisplayModeFlat(wxCommandEvent& event);
void onMenuOptionObjectDisplayModeFlatWire(wxCommandEvent& event);
void onMenuOptionObjectDisplayModeSmooth(wxCommandEvent& event);
void onMenuOptionObjectDisplayModeSmoothWire(wxCommandEvent& event);
void onMenuOptionObjectDisplayModeTransparentFlat(wxCommandEvent& event);
void onMenuOptionObjectDisplayModeTransparentSmooth(wxCommandEvent& event);

void onMenuOptionHideSelection(wxCommandEvent& event);
void onMenuOptionHideUnselection(wxCommandEvent& event);
void onMenuOptionShowAll(wxCommandEvent& event);
void onMenuOptionViewportLayout(wxCommandEvent& event);
void onMenuOptionViewportLayoutSingle(wxCommandEvent& event);
void onMenuOptionViewportLayoutFourView(wxCommandEvent& event);
void onMenuOptionViewportLayoutVerticalSplited(wxCommandEvent& event);
void onMenuOptionViewportLayoutThreeView(wxCommandEvent& event);
void onMenuOptionViewportCamera(wxCommandEvent& event);
void onMenuOptionViewportCameraPerspective(wxCommandEvent& event);
void onMenuOptionViewportCameraTop(wxCommandEvent& event);
void onMenuOptionViewportCameraBottom(wxCommandEvent& event);
void onMenuOptionViewportCameraLeft(wxCommandEvent& event);
void onMenuOptionViewportCameraRight(wxCommandEvent& event);
void onMenuOptionViewportCameraFront(wxCommandEvent& event);
void onMenuOptionViewportCameraBack(wxCommandEvent& event);
void onMenuOptionShowGird(wxCommandEvent& event);
void onMenuOptionSetViewportImage(wxCommandEvent& event);
void onMenuOptionDeleteViewportImage(wxCommandEvent& event);
void onMenuOptionSceneInfo(wxCommandEvent& event);
//void onMenuHelpHelp(wxCommandEvent& event);
void onMenuHelpAbout(wxCommandEvent& event);



	



	private:
		DECLARE_EVENT_TABLE()
};

enum
{
	MenuFileNewScene=200,
	MenuFileOpenScene,
	MenuFileSaveScene,
	MenuFileSaveSceneAs,
	MenuFileLoadIntoScene,
	MenuFileExit,

	MenuEditUndo,
	MenuEditRedo,
	MenuEditScreenshot,
	MenuEditHistoryView,

	MenuCreateCube,
	MenuCreateCylinder,
	MenuCreateSphere,
	MenuCreatePlane,

	MenuCreateClone,
	MenuCreateMirror,
	MenuCreateMirrorInstance,
	MenuCreateRemoveMirrorInstance,

	MenuModifyMerge,
	MenuModifyDetach,
	MenuModifyDeleteVertex,
	MenuModifyExtrude,
	MenuModifySplit,
	MenuModifyCollapse,
	MenuModifyTargetWeld,
	MenuModifyUpdateNormal,

	MenuSubdivisionSubdivide,
	MenuSubdivisionUnsubdivide,
	MenuSubdivisionRefineControlMesh,
	
	MenuSelectionSelectionMode,
	MenuSelectionVertexSelection,
	MenuSelectionEdgeSelection,
	MenuSelectionFaceSelection,
	MenuSelectionObjectSelection,
	MenuSelectionSelectAll,
	MenuSelectionInvertSelect,
	MenuSelectionFrameSelection,

	MenuOptionMaterial,
	MenuOptionObjectDisplayMode,
	MenuOptionObjectDisplayModeWireframe,
	MenuOptionObjectDisplayModeFlat,
	MenuOptionObjectDisplayModeFlatWire,
	MenuOptionObjectDisplayModeSmooth,
	MenuOptionObjectDisplayModeSmoothWire,
	
	MenuOptionHideSelection,
	MenuOptionHideUnselection,
	MenuOptionShowAll,
	MenuOptionViewportLayout,
	MenuOptionViewportLayoutSingle,
	MenuOptionViewportLayoutFourView,
	MenuOptionViewportLayoutVerticalSplited,
	MenuOptionViewportLayoutThreeView,
	MenuOptionViewportCamera,
	MenuOptionViewportCameraPerspective,
	MenuOptionViewportCameraTop,
	MenuOptionViewportCameraBottom,
	MenuOptionViewportCameraLeft,
	MenuOptionViewportCameraRight,
	MenuOptionViewportCameraFront,
	MenuOptionViewportCameraBack,
	MenuOptionShowGird,
	MenuOptionSetViewportImage,
	MenuOptionDeleteViewportImage,
	MenuOptionSceneInfo,

	MenuHelpAbout
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(MenuFileNewScene,MainFrame::onMenuFileNewScene)
	EVT_MENU(MenuFileOpenScene,MainFrame::onMenuFileOpenScene)
	EVT_MENU(MenuFileSaveScene,MainFrame::onMenuFileSaveScene)
	EVT_MENU(MenuFileSaveSceneAs,MainFrame::onMenuFileSaveSceneAs)
	EVT_MENU(MenuFileLoadIntoScene,MainFrame::onMenuFileLoadIntoScene)
	//EVT_MENU(MenuFileConsole,MainFrame::onMenuFileConsole)
	EVT_MENU(MenuFileExit,MainFrame::onMenuFileExit)

	EVT_MENU(MenuEditUndo,MainFrame::onMenuEditUndo)
	EVT_MENU(MenuEditRedo,MainFrame::onMenuEditRedo)
	EVT_MENU(MenuEditScreenshot,MainFrame::onMenuEditScreenshot)
	EVT_MENU(MenuEditHistoryView,MainFrame::onMenuEditHistoryView)

	EVT_MENU(MenuCreateCube,MainFrame::onMenuCreateCube)
	EVT_MENU(MenuCreateCylinder,MainFrame::onMenuCreateCylinder)
	EVT_MENU(MenuCreateSphere,MainFrame::onMenuCreateSphere)
	EVT_MENU(MenuCreatePlane,MainFrame::onMenuCreatePlane)

	EVT_MENU(MenuCreateClone,MainFrame::onMenuCreateClone)
	EVT_MENU(MenuCreateMirror,MainFrame::onMenuCreateMirror)
	EVT_MENU(MenuCreateMirrorInstance,MainFrame::onMenuCreateMirrorInstance)
	EVT_MENU(MenuCreateRemoveMirrorInstance,MainFrame::onMenuCreateRemoveMirrorInstance)

	EVT_MENU(MenuModifyMerge,MainFrame::onMenuModifyMerge)
	EVT_MENU(MenuModifyDetach,MainFrame::onMenuModifyDetach)
	EVT_MENU(MenuModifyDeleteVertex,MainFrame::onMenuModifyDeleteVertex)
	EVT_MENU(MenuModifyExtrude,MainFrame::onMenuModifyExtrude)
	EVT_MENU(MenuModifySplit,MainFrame::onMenuModifySplit)
	EVT_MENU(MenuModifyCollapse,MainFrame::onMenuModifyCollapse)
	EVT_MENU(MenuModifyTargetWeld,MainFrame::onMenuModifyTargetWeld)
	EVT_MENU(MenuModifyUpdateNormal,MainFrame::onMenuModifyUpdateNormal)

	EVT_MENU(MenuSubdivisionSubdivide,MainFrame::onMenuSubdivisionSubdivide)
	EVT_MENU(MenuSubdivisionUnsubdivide,MainFrame::onMenuSubdivisionUnsubdivide)
	EVT_MENU(MenuSubdivisionRefineControlMesh,MainFrame::onMenuSubdivisionRefineControlMesh)
	
	EVT_MENU(MenuSelectionSelectionMode,MainFrame::onMenuSelectionSelectionMode)
	EVT_MENU(MenuSelectionVertexSelection,MainFrame::onMenuSelectionVertexSelection)
	EVT_MENU(MenuSelectionEdgeSelection,MainFrame::onMenuSelectionEdgeSelection)
	EVT_MENU(MenuSelectionFaceSelection,MainFrame::onMenuSelectionFaceSelection)
	EVT_MENU(MenuSelectionObjectSelection,MainFrame::onMenuSelectionObjectSelection)
	EVT_MENU(MenuSelectionSelectAll,MainFrame::onMenuSelectionSelectAll)
	EVT_MENU(MenuSelectionInvertSelect,MainFrame::onMenuSelectionInvertSelect)
	EVT_MENU(MenuSelectionFrameSelection,MainFrame::onMenuSelectionFrameSelection)

	EVT_MENU(MenuOptionMaterial,MainFrame::onMenuOptionMaterial)
	EVT_MENU(MenuOptionObjectDisplayMode,MainFrame::onMenuOptionObjectDisplayMode)
	EVT_MENU(MenuOptionObjectDisplayModeWireframe,MainFrame::onMenuOptionObjectDisplayModeWireframe)
	EVT_MENU(MenuOptionObjectDisplayModeFlat,MainFrame::onMenuOptionObjectDisplayModeFlat)
	EVT_MENU(MenuOptionObjectDisplayModeFlatWire,MainFrame::onMenuOptionObjectDisplayModeFlatWire)
	EVT_MENU(MenuOptionObjectDisplayModeSmooth,MainFrame::onMenuOptionObjectDisplayModeSmooth)
	EVT_MENU(MenuOptionObjectDisplayModeSmoothWire,MainFrame::onMenuOptionObjectDisplayModeSmoothWire)
	EVT_MENU(MenuOptionHideSelection,MainFrame::onMenuOptionHideSelection)
	EVT_MENU(MenuOptionHideUnselection,MainFrame::onMenuOptionHideUnselection)
	EVT_MENU(MenuOptionShowAll,MainFrame::onMenuOptionShowAll)
	EVT_MENU(MenuOptionViewportLayout,MainFrame::onMenuOptionViewportLayout)
	EVT_MENU(MenuOptionViewportLayoutSingle,MainFrame::onMenuOptionViewportLayoutSingle)
	EVT_MENU(MenuOptionViewportLayoutFourView,MainFrame::onMenuOptionViewportLayoutFourView)
	EVT_MENU(MenuOptionViewportLayoutVerticalSplited,MainFrame::onMenuOptionViewportLayoutVerticalSplited)
	EVT_MENU(MenuOptionViewportLayoutThreeView,MainFrame::onMenuOptionViewportLayoutThreeView)
	EVT_MENU(MenuOptionViewportCamera,MainFrame::onMenuOptionViewportCamera)
	EVT_MENU(MenuOptionViewportCameraPerspective,MainFrame::onMenuOptionViewportCameraPerspective)
	EVT_MENU(MenuOptionViewportCameraTop,MainFrame::onMenuOptionViewportCameraTop)
	EVT_MENU(MenuOptionViewportCameraBottom,MainFrame::onMenuOptionViewportCameraBottom)
	EVT_MENU(MenuOptionViewportCameraLeft,MainFrame::onMenuOptionViewportCameraLeft)
	EVT_MENU(MenuOptionViewportCameraRight,MainFrame::onMenuOptionViewportCameraRight)
	EVT_MENU(MenuOptionViewportCameraFront,MainFrame::onMenuOptionViewportCameraFront)
	EVT_MENU(MenuOptionViewportCameraBack,MainFrame::onMenuOptionViewportCameraBack)
	EVT_MENU(MenuOptionShowGird,MainFrame::onMenuOptionShowGird)
	EVT_MENU(MenuOptionSetViewportImage,MainFrame::onMenuOptionSetViewportImage)
	EVT_MENU(MenuOptionDeleteViewportImage,MainFrame::onMenuOptionDeleteViewportImage)
	EVT_MENU(MenuOptionSceneInfo,MainFrame::onMenuOptionSceneInfo)

	EVT_MENU(MenuHelpAbout,MainFrame::onMenuHelpAbout)
END_EVENT_TABLE()