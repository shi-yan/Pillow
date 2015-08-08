QT += core gui widgets

CONFIG += C++11

TARGET = "Pillow"
TEMPLATE = app

HEADERS += \
    AxisCursor.h \
    Camera/BackCamera.h \
    Camera/BottomCamera.h \
    UI/Button.h \
    Camera/Camera.h \
    Edge.h \
    ElementBase.h \
    Enum.h \
    Face.h \
    View/FourView.h \
    Camera/FrontCamera.h \
    Gird.h \
    Global.h \
    HistoryLog.h \
    HistoryManager.h \
    HistoryRecord.h \
    IndexArray.h \
    Camera/LeftCamera.h \
    Log_EdgeEndChange.h \
    Log_EdgeLeftChange.h \
    Log_EdgeRightChange.h \
    Log_EdgeStartChange.h \
    Log_FaceEdgeChange.h \
    Log_FaceEdgeInsert.h \
    Log_FaceEdgePop.h \
    Log_FaceEdgePush.h \
    Log_FaceEdgeRemove.h \
    Log_FaceEdgeSwap.h \
    Log_ObjectColorChange.h \
    Log_ObjectEdgeAdd.h \
    Log_ObjectEdgeRemove.h \
    Log_ObjectFaceAdd.h \
    Log_ObjectFaceRemove.h \
    Log_ObjectPositionChange.h \
    Log_ObjectRotationChange.h \
    Log_ObjectScaleChange.h \
    Log_ObjectVertexAdd.h \
    Log_ObjectVertexRemove.h \
    Log_SceneObjectAdd.h \
    Log_SceneObjectRemove.h \
    Log_VertexAdjacentChange.h \
    Log_VertexAdjacentInsert.h \
    Log_VertexAdjacentPop.h \
    Log_VertexAdjacentPush.h \
    Log_VertexAdjacentRemove.h \
    Log_VertexAdjacentSwap.h \
    Log_VertexNormalChange.h \
    Log_VertexPositionChange.h \
    Matrix.h \
    Object.h \
    Camera/PerspectiveCamera.h \
    Quaternion.h \
    Camera/RightCamera.h \
    Scene.h \
    Screen.h \
    View/SingleView.h \
    View/SplitedView.h \
    SubdivideEdge.h \
    SubdivideFace.h \
    SubdivideLevel.h \
    SubdivideVertex.h \
    TheMath.h \
    View/ThreeView.h \
    UI/ToolStrip.h \
    Camera/TopCamera.h \
    View/TwoView.h \
    UI/UILayer.h \
    Vector.h \
    Vertex.h \
    ViewportImage.h \
    MainWindow.h \
    GLCanvas.h

SOURCES += \
    AxisCursor.cpp \
    Camera/BackCamera.cpp \
    Camera/BottomCamera.cpp \
    UI/Button.cpp \
    Camera/Camera.cpp \
    Edge.cpp \
    ElementBase.cpp \
    Face.cpp \
    View/FourView.cpp \
    Camera/FrontCamera.cpp \
    Gird.cpp \
    HistoryLog.cpp \
    HistoryManager.cpp \
    HistoryRecord.cpp \
    IndexArray.cpp \
    Camera/LeftCamera.cpp \
    Log_EdgeEndChange.cpp \
    Log_EdgeLeftChange.cpp \
    Log_EdgeRightChange.cpp \
    Log_EdgeStartChange.cpp \
    Log_FaceEdgeChange.cpp \
    Log_FaceEdgeInsert.cpp \
    Log_FaceEdgePop.cpp \
    Log_FaceEdgePush.cpp \
    Log_FaceEdgeRemove.cpp \
    Log_FaceEdgeSwap.cpp \
    Log_ObjectColorChange.cpp \
    Log_ObjectEdgeAdd.cpp \
    Log_ObjectEdgeRemove.cpp \
    Log_ObjectFaceAdd.cpp \
    Log_ObjectFaceRemove.cpp \
    Log_ObjectPositionChange.cpp \
    Log_ObjectRotationChange.cpp \
    Log_ObjectScaleChange.cpp \
    Log_ObjectVertexAdd.cpp \
    Log_ObjectVertexRemove.cpp \
    Log_SceneObjectAdd.cpp \
    Log_SceneObjectRemove.cpp \
    Log_VertexAdjacentChange.cpp \
    Log_VertexAdjacentInsert.cpp \
    Log_VertexAdjacentPop.cpp \
    Log_VertexAdjacentPush.cpp \
    Log_VertexAdjacentRemove.cpp \
    Log_VertexAdjacentSwap.cpp \
    Log_VertexNormalChange.cpp \
    Log_VertexPositionChange.cpp \
    Matrix.cpp \
    Object.cpp \
    Camera/PerspectiveCamera.cpp \
    Quaternion.cpp \
    Camera/RightCamera.cpp \
    Scene.cpp \
    Screen.cpp \
    View/SingleView.cpp \
    View/SplitedView.cpp \
    SubdivideEdge.cpp \
    SubdivideFace.cpp \
    SubdivideLevel.cpp \
    SubdivideVertex.cpp \
    View/ThreeView.cpp \
    UI/ToolStrip.cpp \
    Camera/TopCamera.cpp \
    View/TwoView.cpp \
    UI/UILayer.cpp \
    Vector.cpp \
    Vertex.cpp \
    ViewportImage.cpp \
    main.cpp \
    MainWindow.cpp \
    GLCanvas.cpp



#LIBS += -lGLU

LIBS += -framework OpenGL

FORMS += \
    MainWindow.ui
