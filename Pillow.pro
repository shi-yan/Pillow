QT += core gui widgets

CONFIG += C++11

TARGET = "Pillow"
TEMPLATE = app

HEADERS += \
    UI/AxisCursor.h \
    Camera/BackCamera.h \
    Camera/BottomCamera.h \
    UI/Button.h \
    Camera/Camera.h \
    Core/Edge.h \
    Core/ElementBase.h \
    Enum.h \
    Core/Face.h \
    View/FourView.h \
    Camera/FrontCamera.h \
    UI/Grid.h \
    History/HistoryLog.h \
    History/HistoryManager.h \
    History/HistoryRecord.h \
    Core/IndexArray.h \
    Camera/LeftCamera.h \
    History/Log_EdgeEndChange.h \
    History/Log_EdgeLeftChange.h \
    History/Log_EdgeRightChange.h \
    History/Log_EdgeStartChange.h \
    History/Log_FaceEdgeChange.h \
    History/Log_FaceEdgeInsert.h \
    History/Log_FaceEdgePop.h \
    History/Log_FaceEdgePush.h \
    History/Log_FaceEdgeRemove.h \
    History/Log_FaceEdgeSwap.h \
    History/Log_ObjectColorChange.h \
    History/Log_ObjectEdgeAdd.h \
    History/Log_ObjectEdgeRemove.h \
    History/Log_ObjectFaceAdd.h \
    History/Log_ObjectFaceRemove.h \
    History/Log_ObjectPositionChange.h \
    History/Log_ObjectRotationChange.h \
    History/Log_ObjectScaleChange.h \
    History/Log_ObjectVertexAdd.h \
    History/Log_ObjectVertexRemove.h \
    History/Log_SceneObjectAdd.h \
    History/Log_SceneObjectRemove.h \
    History/Log_VertexAdjacentChange.h \
    History/Log_VertexAdjacentInsert.h \
    History/Log_VertexAdjacentPop.h \
    History/Log_VertexAdjacentPush.h \
    History/Log_VertexAdjacentRemove.h \
    History/Log_VertexAdjacentSwap.h \
    History/Log_VertexNormalChange.h \
    History/Log_VertexPositionChange.h \
    Core/Matrix.h \
    Core/Object.h \
    Camera/PerspectiveCamera.h \
    Core/Quaternion.h \
    Camera/RightCamera.h \
    Core/Scene.h \
    Screen.h \
    View/SingleView.h \
    View/SplitedView.h \
    Core/SubdivideEdge.h \
    Core/SubdivideFace.h \
    Core/SubdivideLevel.h \
    Core/SubdivideVertex.h \
    Core/Math.h \
    View/ThreeView.h \
    UI/ToolStrip.h \
    Camera/TopCamera.h \
    View/TwoView.h \
    UI/UILayer.h \
    Core/Vector.h \
    Core/Vertex.h \
    View/ViewportImage.h \
    MainWindow.h \
    GLCanvas.h \
    Dialog/NewCubeDialog.h \
    Dialog/NewCylinderDialog.h \
    GraphicsBackend/OpenGLBackend.h

SOURCES += \
    UI/AxisCursor.cpp \
    Camera/BackCamera.cpp \
    Camera/BottomCamera.cpp \
    UI/Button.cpp \
    Camera/Camera.cpp \
    Core/Edge.cpp \
    Core/ElementBase.cpp \
    Core/Face.cpp \
    View/FourView.cpp \
    Camera/FrontCamera.cpp \
    UI/Grid.cpp \
    History/HistoryLog.cpp \
    History/HistoryManager.cpp \
    History/HistoryRecord.cpp \
    Core/IndexArray.cpp \
    Camera/LeftCamera.cpp \
    History/Log_EdgeEndChange.cpp \
    History/Log_EdgeLeftChange.cpp \
    History/Log_EdgeRightChange.cpp \
    History/Log_EdgeStartChange.cpp \
    History/Log_FaceEdgeChange.cpp \
    History/Log_FaceEdgeInsert.cpp \
    History/Log_FaceEdgePop.cpp \
    History/Log_FaceEdgePush.cpp \
    History/Log_FaceEdgeRemove.cpp \
    History/Log_FaceEdgeSwap.cpp \
    History/Log_ObjectColorChange.cpp \
    History/Log_ObjectEdgeAdd.cpp \
    History/Log_ObjectEdgeRemove.cpp \
    History/Log_ObjectFaceAdd.cpp \
    History/Log_ObjectFaceRemove.cpp \
    History/Log_ObjectPositionChange.cpp \
    History/Log_ObjectRotationChange.cpp \
    History/Log_ObjectScaleChange.cpp \
    History/Log_ObjectVertexAdd.cpp \
    History/Log_ObjectVertexRemove.cpp \
    History/Log_SceneObjectAdd.cpp \
    History/Log_SceneObjectRemove.cpp \
    History/Log_VertexAdjacentChange.cpp \
    History/Log_VertexAdjacentInsert.cpp \
    History/Log_VertexAdjacentPop.cpp \
    History/Log_VertexAdjacentPush.cpp \
    History/Log_VertexAdjacentRemove.cpp \
    History/Log_VertexAdjacentSwap.cpp \
    History/Log_VertexNormalChange.cpp \
    History/Log_VertexPositionChange.cpp \
    Core/Matrix.cpp \
    Core/Object.cpp \
    Camera/PerspectiveCamera.cpp \
    Core/Quaternion.cpp \
    Camera/RightCamera.cpp \
    Core/Scene.cpp \
    Screen.cpp \
    View/SingleView.cpp \
    View/SplitedView.cpp \
    Core/SubdivideEdge.cpp \
    Core/SubdivideFace.cpp \
    Core/SubdivideLevel.cpp \
    Core/SubdivideVertex.cpp \
    View/ThreeView.cpp \
    UI/ToolStrip.cpp \
    Camera/TopCamera.cpp \
    View/TwoView.cpp \
    UI/UILayer.cpp \
    Core/Vector.cpp \
    Core/Vertex.cpp \
    View/ViewportImage.cpp \
    main.cpp \
    MainWindow.cpp \
    GLCanvas.cpp \
    Dialog/NewCubeDialog.cpp \
    Dialog/NewCylinderDialog.cpp \
    GraphicsBackend/OpenGLBackend.cpp \
    Core/Scene_Draw.cpp \
    Core/Scene_File.cpp \
    Core/Scene_Create.cpp

unix:!mac {
LIBS += -lGL
}

macx {
LIBS += -framework OpenGL
}
FORMS += \
    MainWindow.ui \
    Dialog/NewCubeDialog.ui \
    Dialog/NewCylinderDialog.ui

RESOURCES += \
    resources.qrc
