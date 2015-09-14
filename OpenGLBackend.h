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

    void updateToolStripGeometry(void **id, const float * const vertices) const;
    void deleteRenderableGeometryData(const void *id) const;
    void drawToolStripGeometry(const void * const id) const;

    void updateButtonGeometry(void **id, const float * const vertices) const;
    void drawButtonStripGeometry(const void * const id, int offsetX = 0, int offsetY = 0) const;

    void setModelViewMatrix(const Matrix &in);
    void setProjectionMatrix(const Matrix &in);

    void updateGridGeometry(void **id, const std::vector<float> &vertices, const std::vector<float> &colors) const;
    void drawGridGeometry(const void * const id) const;
    ~OpenGLBackend();
};

#endif // OPENGLBACKEND_H
