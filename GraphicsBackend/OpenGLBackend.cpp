#include "OpenGLBackend.h"
#include <QDebug>
#include <QImage>

OpenGLBackend::OpenGLBackend()
    :m_UITextureID(0),
     m_UIShaderProgram(0),
     m_UIVertexShader(0),
     m_UIFragmentShader(0),
     m_screenWidth(0),
     m_screenHeight(0),
     m_UIScreenSizeUniform(0),
     m_UIOffsetUniform(0),
     m_modelView(),
     m_projection(),
     m_gridShaderProgram(0),
     m_gridVertexShader(0),
     m_gridFragmentShader(0),
     m_gridModelViewUniform(0),
     m_gridProjectionUniform(0),
     m_axisCursorShaderProgram(0),
     m_axisCursorVertexShader(0),
     m_axisCursorFragmentShader(0),
     m_axisCursorModelViewUniform(0),
     m_axisCursorProjectionUniform(0),
     m_axisCursorTransformUniform(0),
     m_axisCursorColorUniform(0),
     m_objectPhongShaderProgram(0),
     m_objectPhongVertexShader(0),
     m_objectPhongFragmentShader(0),
     m_objectPhongModelViewUniform(0),
     m_objectPhongProjectionUniform(0),
     m_objectPhongNormalUniform(0),
     m_objectPhongTransformUniform(0),
     m_objectPolygonVbo(0),
     m_objectPolygonVao(0)
{
}

void OpenGLBackend::initialize()
{
    m_UITextureID = loadTexture("://ui.dat");

    const char* vertex_shader_ui =
    "#version 150\n"
    "in vec4 vp;"
    "uniform vec2 screenSize;"
    "uniform vec2 offset;"
    "out vec2 texCoord;"
    "void main () {"
    "  gl_Position = vec4 ((vp.x+offset.x)*2.0/screenSize.x-1.0, -((vp.y+offset.y)*2.0/screenSize.y - 1.0), 0.0, 1.0);"
    //"gl_Position = vec4 (vp.xy,0.0, 1.0);"
    "  texCoord = vp.zw;"
    "}";

    const char* fragment_shader_ui =
    "#version 150\n"
    "uniform sampler2D asset;"
    "in vec2 texCoord;"
    "out vec4 frag_colour;"
    "void main () {"
    "  frag_colour = texture (asset, texCoord);"
    //"frag_colour = vec4(1.0,0.0,0.0,1.0);"
    "}";

   compileShader(m_UIShaderProgram, m_UIVertexShader, m_UIFragmentShader, vertex_shader_ui, fragment_shader_ui);

   glUseProgram (m_UIShaderProgram);
   GLuint texLoc = glGetUniformLocation(m_UIShaderProgram, "asset");
   glUniform1i(texLoc, 0);
   m_UIScreenSizeUniform = glGetUniformLocation(m_UIShaderProgram, "screenSize");
   m_UIOffsetUniform = glGetUniformLocation(m_UIShaderProgram, "offset");

   const char* vertex_shader_grid =
   "#version 150\n"
   "in vec3 vp;"
   "in vec3 col;"
   "uniform mat4 modelView;"
   "uniform mat4 projection;"
   "out vec3 color;"
   "void main () {"
   "  color = col;"
   "  gl_Position = projection * modelView * vec4 (vp.xyz, 1.0);"
   "}";

   const char* fragment_shader_grid =
   "#version 150\n"
   "out vec4 frag_colour;"
   "in vec3 color;"
   "void main () {"
   "  frag_colour = vec4(color.xyz, 1.0);"
   "}";

   compileShader(m_gridShaderProgram, m_gridVertexShader, m_gridFragmentShader, vertex_shader_grid, fragment_shader_grid);

   glUseProgram (m_gridShaderProgram);

   m_gridModelViewUniform = glGetUniformLocation(m_gridShaderProgram, "modelView");
   m_gridProjectionUniform = glGetUniformLocation(m_gridShaderProgram, "projection");

   const char* vertex_shader_axisCursor =
   "#version 150\n"
   "in vec3 vp;"
   "uniform vec3 col;"
   "uniform mat4 modelView;"
   "uniform mat4 projection;"
   "uniform mat4 transform;"
   "out vec3 color;"
   "void main () {"
   "  color = col;"
   "  gl_Position = projection * modelView * transform * vec4 (vp.xyz, 1.0);"
   "}";

   const char* fragment_shader_axisCursor =
   "#version 150\n"
   "out vec4 frag_colour;"
   "in vec3 color;"
   "void main () {"
   "  frag_colour = vec4(color.xyz, 1.0);"
   "}";

   compileShader(m_axisCursorShaderProgram,m_axisCursorVertexShader, m_axisCursorFragmentShader,vertex_shader_axisCursor,fragment_shader_axisCursor);

   glUseProgram (m_axisCursorShaderProgram);
   glBindAttribLocation (m_axisCursorShaderProgram, 0, "vp");
   glBindAttribLocation (m_axisCursorShaderProgram, 1, "col");
   m_axisCursorModelViewUniform = glGetUniformLocation(m_axisCursorShaderProgram, "modelView");
   m_axisCursorProjectionUniform = glGetUniformLocation(m_axisCursorShaderProgram, "projection");
   m_axisCursorTransformUniform = glGetUniformLocation(m_axisCursorShaderProgram, "transform");
   m_axisCursorColorUniform = glGetUniformLocation(m_axisCursorShaderProgram, "col");

   const char* vertex_shader_phong =
   "#version 150\n"
   "in vec3 vp;"
   "in vec3 np;"
   "uniform mat4 modelView;"
   "uniform mat4 projection;"
  // "uniform mat4 transform;"
   "uniform mat3 normal;"
   "out vec3 v;"
   "out vec3 N;"
   "void main () {"
   "    v = vec3(modelView  * vec4(vp.xyz, 1.0));"
   "    N = normalize(normal * np);"
   "    gl_Position = projection * modelView  * vec4(vp.xyz, 1.0);"
   "}";

   const char* fragment_shader_phong =
   "#version 150\n"
   "out vec4 frag_colour;"
   "in vec3 v;"
   "in vec3 N;"
   "void main () {"
   "  frag_colour = vec4(1.0, 0.0, 0.0, 1.0);"
   "}";

    compileShader(m_objectPhongShaderProgram, m_objectPhongVertexShader, m_objectPhongFragmentShader, vertex_shader_phong, fragment_shader_phong);

    glUseProgram(m_objectPhongShaderProgram);
    glBindAttribLocation (m_objectPhongShaderProgram, 0, "vp");
    glBindAttribLocation (m_objectPhongShaderProgram, 1, "np");
    m_objectPhongModelViewUniform = glGetUniformLocation(m_objectPhongShaderProgram, "modelView");
    m_objectPhongProjectionUniform = glGetUniformLocation(m_objectPhongShaderProgram, "projection");
    m_objectPhongTransformUniform = glGetUniformLocation(m_objectPhongShaderProgram, "transform");
    m_objectPhongTransformUniform = glGetUniformLocation(m_objectPhongShaderProgram, "normal");

    glGenBuffers (1, &m_objectPolygonVbo);
    glGenVertexArrays (1, &m_objectPolygonVao);
}

void OpenGLBackend::deinitialize()
{
    glDeleteProgram(m_UIShaderProgram);
    glDeleteShader(m_UIVertexShader);
    glDeleteShader(m_UIFragmentShader);
    glDeleteProgram(m_gridShaderProgram);
    glDeleteShader(m_gridVertexShader);
    glDeleteShader(m_gridFragmentShader);
    glDeleteProgram(m_axisCursorShaderProgram);
    glDeleteShader(m_axisCursorVertexShader);
    glDeleteShader(m_axisCursorFragmentShader);
    glDeleteProgram(m_objectPhongShaderProgram);
    glDeleteShader(m_objectPhongVertexShader);
    glDeleteShader(m_objectPhongFragmentShader);
    glDeleteTextures(1, &m_UITextureID);
    if (m_objectPolygonVao)
    {
        glDeleteVertexArrays(1, &m_objectPolygonVao);
    }
    if(m_objectPolygonVbo)
    {
        glDeleteVertexArrays(1, &m_objectPolygonVbo);
    }
}

OpenGLBackend::~OpenGLBackend()
{

}

void OpenGLBackend::drawUITile(int x, int y, unsigned width, unsigned int height)
{

}

void OpenGLBackend::beginUI(unsigned int width, unsigned int height)
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram (m_UIShaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_UITextureID);

    if (width != m_screenWidth || height != m_screenHeight)
    {
        m_screenWidth = width;
        m_screenHeight = height;
        glUniform2f(m_UIScreenSizeUniform, m_screenWidth, m_screenHeight);
    }
}

void OpenGLBackend::endUI()
{

}

GLuint OpenGLBackend::loadTexture(const char *fileName) const
{
    GLuint textureId = 0;
    QImage textureImage(fileName);
    textureImage = textureImage.convertToFormat(QImage::Format_RGBA8888);

    int height = textureImage.height();
    int width = textureImage.width();

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return textureId;
}

OpenGLBackend::RenderableGeometryData::RenderableGeometryData(GLuint vbo, GLuint vao)
    :m_vbo(vbo),
      m_vao(vao),
      m_colorVbo(0)
{
}

OpenGLBackend::RenderableGeometryData::~RenderableGeometryData()
{
    if (m_vao)
    {
        glDeleteVertexArrays(1, &m_vao);
    }
    if(m_vbo)
    {
        glDeleteVertexArrays(1, &m_vbo);
    }
    if (m_colorVbo)
    {
        glDeleteVertexArrays(1, &m_colorVbo);
    }
}

GLuint OpenGLBackend::RenderableGeometryData::getVbo() const
{
    return m_vbo;
}

GLuint OpenGLBackend::RenderableGeometryData::getVao() const
{
    return m_vao;
}

void OpenGLBackend::RenderableGeometryData::setColorVbo(GLuint vbo)
{
    m_colorVbo = vbo;
}

GLuint OpenGLBackend::RenderableGeometryData::getColorVbo() const
{
    return m_colorVbo;
}

void OpenGLBackend::updateGeometryWithVertices(void **id, const float * const vertices, unsigned int count, unsigned int size) const
{
    if (*id == nullptr)
    {
        GLuint vbo = 0;
        GLuint vao = 0;
        glGenBuffers (1, &vbo);
        glGenVertexArrays (1, &vao);
        *id = new RenderableGeometryData(vbo, vao);
    }

    RenderableGeometryData *renderableData = (RenderableGeometryData*)(*id);

    glBindBuffer (GL_ARRAY_BUFFER, renderableData->getVbo());
    glBufferData (GL_ARRAY_BUFFER, count * sizeof (float), vertices, GL_STATIC_DRAW);

    glBindVertexArray (renderableData->getVao());
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, renderableData->getVbo());
    glVertexAttribPointer (0, size, GL_FLOAT, GL_FALSE, 0, NULL);
}

void OpenGLBackend::deleteRenderableGeometryData(const void *id) const
{
    RenderableGeometryData *renderableData = (RenderableGeometryData*)(id);
    delete renderableData;
}

void OpenGLBackend::drawToolStripGeometry(const void * const id) const
{
    glUniform2f(m_UIOffsetUniform, 0, 0);
    RenderableGeometryData *renderableData = (RenderableGeometryData*)(id);
    glBindVertexArray (renderableData->getVao());
    glDrawArrays (GL_TRIANGLE_STRIP, 0, 8);
}

void OpenGLBackend::drawButtonStripGeometry(const void * const id, int offsetX, int offsetY) const
{
    glUniform2f(m_UIOffsetUniform, offsetX, offsetY);
    const RenderableGeometryData *renderableData = (RenderableGeometryData*)(id);
    glBindVertexArray (renderableData->getVao());
    glDrawArrays (GL_TRIANGLE_FAN, 0, 4);
}

void OpenGLBackend::setModelViewMatrix(const Matrix &in)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    m_modelView = in;
    glUseProgram(m_axisCursorShaderProgram);
    glUniformMatrix4fv(m_axisCursorModelViewUniform, 1, false, &m_modelView.m[0][0]);
    glUseProgram(m_gridShaderProgram);
    glUniformMatrix4fv(m_gridModelViewUniform, 1, false, &m_modelView.m[0][0]);
    glUseProgram(m_objectPhongShaderProgram);
    glUniformMatrix4fv(m_objectPhongModelViewUniform, 1, false, &m_modelView.m[0][0]);
}

void OpenGLBackend::setProjectionMatrix(const Matrix &in)
{
    m_projection = in;
    glUseProgram(m_axisCursorShaderProgram);
    glUniformMatrix4fv(m_axisCursorProjectionUniform, 1, false, &m_projection.m[0][0]);
    glUseProgram(m_gridShaderProgram);
    glUniformMatrix4fv(m_gridProjectionUniform, 1, false, &m_projection.m[0][0]);
    glUseProgram(m_objectPhongShaderProgram);
    glUniformMatrix4fv(m_objectPhongProjectionUniform, 1, false, &m_projection.m[0][0]);
}

void OpenGLBackend::updateGeometryWithVerticesAndColors(void **id, const std::vector<float> &vertices, const std::vector<unsigned char> &colors) const
{
    if (*id == nullptr)
    {
        GLuint vbo = 0;
        GLuint vao = 0;
        glGenBuffers (1, &vbo);
        glGenVertexArrays (1, &vao);
        *id = new RenderableGeometryData(vbo, vao);

        GLuint colorVbo = 0;

        glGenBuffers(1, &colorVbo);

        RenderableGeometryData *renderableData = (RenderableGeometryData*)(*id);
        renderableData->setColorVbo(colorVbo);
    }

    RenderableGeometryData *renderableData = (RenderableGeometryData*)(*id);

    glBindVertexArray (renderableData->getVao());

    glBindBuffer (GL_ARRAY_BUFFER, renderableData->getVbo());
    glBufferData (GL_ARRAY_BUFFER, vertices.size() * sizeof (float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, renderableData->getColorVbo());
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(unsigned char), &colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, nullptr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void OpenGLBackend::drawGridGeometry(const void * const id) const
{
    RenderableGeometryData *renderableData = (RenderableGeometryData*)(id);
    glUseProgram (m_gridShaderProgram);
    glBindVertexArray (renderableData->getVao());
    glDrawArrays (GL_LINES, 0, 252);
}

void OpenGLBackend::drawAxisCursorMove(const void * const id, const Matrix &transform, const Vector &color) const
{
    RenderableGeometryData *renderableData = (RenderableGeometryData*)(id);
    glUseProgram (m_axisCursorShaderProgram);
    glUniformMatrix4fv(m_axisCursorTransformUniform, 1, false, &transform.m[0][0]);
    glUniform3f(m_axisCursorColorUniform, color.x, color.y, color.z);
    glBindVertexArray (renderableData->getVao());
    glDrawArrays (GL_TRIANGLES, 0, 12);
    glDrawArrays (GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays(GL_LINES, 16, 2);
}

void OpenGLBackend::drawAxisCursorRotate(const void * const id, const Matrix &transform, const Vector &color) const
{
    RenderableGeometryData *renderableData = (RenderableGeometryData*)(id);
    glUseProgram (m_axisCursorShaderProgram);
    glUniformMatrix4fv(m_axisCursorTransformUniform, 1, false, &transform.m[0][0]);
    glUniform3f(m_axisCursorColorUniform, color.x, color.y, color.z);
    glBindVertexArray (renderableData->getVao());
    glDrawArrays (GL_TRIANGLES, 0, 21);
    glDrawArrays(GL_LINES, 21, 2);
}

void OpenGLBackend::drawAxisCursorScale(const void * const id, const Matrix &transform, const Vector &color) const
{
    RenderableGeometryData *renderableData = (RenderableGeometryData*)(id);
    glUseProgram (m_axisCursorShaderProgram);
    glUniformMatrix4fv(m_axisCursorTransformUniform, 1, false, &transform.m[0][0]);
    glUniform3f(m_axisCursorColorUniform, color.x, color.y, color.z);
    glBindVertexArray (renderableData->getVao());
    glDrawArrays (GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays (GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays (GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays (GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays (GL_TRIANGLE_FAN, 16, 4);
    glDrawArrays (GL_TRIANGLE_FAN, 20, 4);
    glDrawArrays(GL_LINES, 24, 2);
}

void OpenGLBackend::compileShader(GLuint &programId, GLuint &vertexShaderId, GLuint &fragmentShaderId, const char *vertexSource, const char *fragmentSource)
{
    GLint isCompiled = 0;
    vertexShaderId = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vertexShaderId, 1, &vertexSource, NULL);
    glCompileShader (vertexShaderId);
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        qDebug() << "not compiled vs";
        GLchar log[1024];
        GLsizei length;
        glGetShaderInfoLog(m_objectPhongVertexShader,1024, &length, log);
        qDebug() << log;
    }

    fragmentShaderId = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fragmentShaderId, 1, &fragmentSource, NULL);
    glCompileShader (fragmentShaderId);
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        qDebug() << "not compiled fs";
        GLchar log[1024];
        GLsizei length;
        glGetShaderInfoLog(m_objectPhongVertexShader,1024, &length, log);
        qDebug() << log;
    }

    programId = glCreateProgram ();
    glAttachShader(programId, fragmentShaderId);
    glAttachShader(programId, vertexShaderId);

    glLinkProgram(programId);
    int isLinked = 0;

    glGetProgramiv(programId, GL_LINK_STATUS, &isLinked);
    if (!isLinked)
    {
        qDebug() << "not linked";
    }
}

void OpenGLBackend::drawObjectPolygon(const std::vector<float> &vertices) const
{
    glEnable(GL_DEPTH_TEST);
    glUseProgram (m_objectPhongShaderProgram);
    glBindBuffer (GL_ARRAY_BUFFER, m_objectPolygonVbo);
    glBufferData (GL_ARRAY_BUFFER, vertices.size() * sizeof (float), &vertices[0], GL_DYNAMIC_DRAW);
    glBindVertexArray (m_objectPolygonVao);
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE,  6*sizeof(float),(void*) (3*sizeof(float)));
    glEnableVertexAttribArray (1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  6*sizeof(float),  0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size()/6);
}

