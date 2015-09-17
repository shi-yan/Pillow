#ifndef OPENGLBACKEND_H
#define OPENGLBACKEND_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <vector>
#include "Core/Matrix.h"

class OpenGLBackend
{
private:
    GLuint m_UITextureID;
    GLuint m_UIShaderProgram;
    GLuint m_UIVertexShader;
    GLuint m_UIFragmentShader;
    unsigned int m_screenWidth;
    unsigned int m_screenHeight;
    GLuint m_UIScreenSizeUniform;
    GLuint m_UIOffsetUniform;

    Matrix m_modelView;
    Matrix m_projection;

    GLuint m_gridShaderProgram;
    GLuint m_gridVertexShader;
    GLuint m_gridFragmentShader;
    GLuint m_gridModelViewUniform;
    GLuint m_gridProjectionUniform;

    GLuint m_axisCursorShaderProgram;
    GLuint m_axisCursorVertexShader;
    GLuint m_axisCursorFragmentShader;
    GLuint m_axisCursorModelViewUniform;
    GLuint m_axisCursorProjectionUniform;
    GLuint m_axisCursorTransformUniform;
    GLuint m_axisCursorColorUniform;

    GLuint m_objectPhongShaderProgram;
    GLuint m_objectPhongVertexShader;
    GLuint m_objectPhongFragmentShader;

    GLuint m_objectPhongModelViewUniform;
    GLuint m_objectPhongProjectionUniform;
    GLuint m_objectPhongNormalUniform;
    GLuint m_objectPhongTransformUniform;

    GLuint m_objectPolygonVbo;
    GLuint m_objectPolygonVao;

    GLuint loadTexture(const char *fileName) const;

public:
    class RenderableGeometryData
    {
        GLuint m_vbo;
        GLuint m_vao;
        GLuint m_colorVbo;

    public:
        RenderableGeometryData(GLuint vbo, GLuint vao);
        ~RenderableGeometryData();

        GLuint getVbo() const;
        GLuint getVao() const;

        void setColorVbo(GLuint vbo);

        GLuint getColorVbo() const;
    };

    OpenGLBackend();
    void initialize();
    void deinitialize();
    void drawUITile(int x, int y, unsigned width, unsigned int height);
    void beginUI(unsigned int width, unsigned int height);
    void endUI();

    void updateGeometryWithVertices(void **id, const float * const vertices, unsigned int count, unsigned int size = 4) const;
    void deleteRenderableGeometryData(const void *id) const;

    void drawToolStripGeometry(const void * const id) const;
    void drawButtonStripGeometry(const void * const id, int offsetX = 0, int offsetY = 0) const;

    void setModelViewMatrix(const Matrix &in);
    void setProjectionMatrix(const Matrix &in);

    void updateGeometryWithVerticesAndColors(void **id, const std::vector<float> &vertices, const std::vector<unsigned char> &colors) const;
    void drawGridGeometry(const void * const id) const;

    void drawAxisCursorMove(const void * const id, const Matrix &transform, const Vector &color) const;
    void drawAxisCursorRotate(const void * const id, const Matrix &transform, const Vector &color) const;
    void drawAxisCursorScale(const void * const id, const Matrix &transform, const Vector &color) const;

    void setObjectMaterialAndTransform(const Matrix &transform);
    void drawObjectPolygon(const std::vector<float> &vertices) const;

    ~OpenGLBackend();

private:
    void compileShader(GLuint &programId, GLuint &vertexShaderId, GLuint &fragmentShaderId, const char *vertexSource, const char *fragmentSource);
};

#endif // OPENGLBACKEND_H
