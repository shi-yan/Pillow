#ifndef OPENGLBACKEND_H
#define OPENGLBACKEND_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

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

    GLuint loadTexture(const char *fileName) const;

public:
    class RenderableGeometryData
    {
        GLuint m_vbo;
        GLuint m_vao;

    public:
        RenderableGeometryData(GLuint vbo, GLuint vao);
        ~RenderableGeometryData();

        GLuint getVbo() const;
        GLuint getVao() const;
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

    ~OpenGLBackend();

};

#endif // OPENGLBACKEND_H
