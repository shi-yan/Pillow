#include "Grid.h"
#include <vector>

Grid *Grid::grid = 0;

Grid::Grid(OpenGLBackend *backend)
    :m_backend(backend),
      m_xyRenderable(nullptr),
      m_xzRenderable(nullptr),
      m_yzRenderable(nullptr)
{
}

Grid::~Grid(void)
{
    if (m_xyRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_xyRenderable);
        m_xyRenderable = nullptr;
    }
    if (m_xzRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_xzRenderable);
        m_xzRenderable = nullptr;
    }
    if (m_yzRenderable)
    {
        m_backend->deleteRenderableGeometryData(m_yzRenderable);
        m_yzRenderable = nullptr;
    }
}

void Grid::initialize()
{
    std::vector<float> xyVertices;
    std::vector<float> xyColors;

    xyColors.push_back(172.0/255.0);
    xyColors.push_back(43.0/255.0);
    xyColors.push_back(40.0/255.0);

    xyColors.push_back(180.0/255.0);
    xyColors.push_back(180.0/255.0);
    xyColors.push_back(180.0/255.0);

    xyVertices.push_back(200);
    xyVertices.push_back(0);
    xyVertices.push_back(0);

    xyVertices.push_back(-200);
    xyVertices.push_back(0);
    xyVertices.push_back(0);


    xyColors.push_back(91.0/255.0);
    xyColors.push_back(103.0/255.0);
    xyColors.push_back(147.0/255.0);

    xyColors.push_back(180.0/255.0);
    xyColors.push_back(180.0/255.0);
    xyColors.push_back(180.0/255.0);

    xyVertices.push_back(0);
    xyVertices.push_back(200);
    xyVertices.push_back(0);

    xyVertices.push_back(0);
    xyVertices.push_back(-200);
    xyVertices.push_back(0);

    for (int x = 20; x <= 200; x += 20)
    {
        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);

        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);

        xyVertices.push_back(x);
        xyVertices.push_back(200);
        xyVertices.push_back(0);

        xyVertices.push_back(x);
        xyVertices.push_back(-200);
        xyVertices.push_back(0);

        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);

        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);

        xyVertices.push_back(-x);
        xyVertices.push_back(200);
        xyVertices.push_back(0);

        xyVertices.push_back(-x);
        xyVertices.push_back(-200);
        xyVertices.push_back(0);
    }

    for(int y=20;y<=200;y+=20)
    {
        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);

        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);

        xyVertices.push_back(200);
        xyVertices.push_back(y);
        xyVertices.push_back(0);

        xyVertices.push_back(-200);
        xyVertices.push_back(y);
        xyVertices.push_back(0);

        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);

        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);
        xyColors.push_back(180.0/255.0);

        xyVertices.push_back(200);
        xyVertices.push_back(-y);
        xyVertices.push_back(0);

        xyVertices.push_back(-200);
        xyVertices.push_back(-y);
        xyVertices.push_back(0);
    }

    m_backend->updateGridGeometry(&m_xyRenderable, xyVertices, xyColors);

    std::vector<float> xzVertices;
    std::vector<float> xzColors;

    xzColors.push_back(172.0/255.0);
    xzColors.push_back(43.0/255.0);
    xzColors.push_back(40.0/255.0);

    xzColors.push_back(180.0/255.0);
    xzColors.push_back(180.0/255.0);
    xzColors.push_back(180.0/255.0);

    xzVertices.push_back(200);
    xzVertices.push_back(0);
    xzVertices.push_back(0);

    xzVertices.push_back(-200);
    xzVertices.push_back(0);
    xzVertices.push_back(0);


    xzColors.push_back(255.0/255.0);
    xzColors.push_back(255.0/255.0);
    xzColors.push_back(128.0/255.0);

    xzColors.push_back(180.0/255.0);
    xzColors.push_back(180.0/255.0);
    xzColors.push_back(180.0/255.0);

    xzVertices.push_back(0);
    xzVertices.push_back(0);
    xzVertices.push_back(200);

    xzVertices.push_back(0);
    xzVertices.push_back(0);
    xzVertices.push_back(-200);

    for (int x = 20; x <= 200; x += 20)
    {
        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);

        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);

        xzVertices.push_back(x);
        xzVertices.push_back(0);
        xzVertices.push_back(200);

        xzVertices.push_back(x);
        xzVertices.push_back(0);
        xzVertices.push_back(-200);

        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);

        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);

        xzVertices.push_back(-x);
        xzVertices.push_back(0);
        xzVertices.push_back(200);

        xzVertices.push_back(-x);
        xzVertices.push_back(0);
        xzVertices.push_back(-200);
    }

    for(int z=20;z<=200;z+=20)
    {
        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);

        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);

        xzVertices.push_back(200);
        xzVertices.push_back(0);
        xzVertices.push_back(z);

        xzVertices.push_back(-200);
        xzVertices.push_back(0);
        xzVertices.push_back(z);

        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);

        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);
        xzColors.push_back(180.0/255.0);

        xzVertices.push_back(200);
        xzVertices.push_back(0);
        xzVertices.push_back(-z);

        xzVertices.push_back(-200);
        xzVertices.push_back(0);
        xzVertices.push_back(-z);
    }

    m_backend->updateGridGeometry(&m_xzRenderable, xzVertices, xzColors);

    std::vector<float> yzVertices;
    std::vector<float> yzColors;

    yzColors.push_back(91.0/255.0);
    yzColors.push_back(103.0/255.0);
    yzColors.push_back(147.0/255.0);

    yzColors.push_back(180.0/255.0);
    yzColors.push_back(180.0/255.0);
    yzColors.push_back(180.0/255.0);

    yzVertices.push_back(0);
    yzVertices.push_back(200);
    yzVertices.push_back(0);

    yzVertices.push_back(0);
    yzVertices.push_back(-200);
    yzVertices.push_back(0);

    yzColors.push_back(255.0/255.0);
    yzColors.push_back(255.0/255.0);
    yzColors.push_back(128.0/255.0);

    yzColors.push_back(180.0/255.0);
    yzColors.push_back(180.0/255.0);
    yzColors.push_back(180.0/255.0);

    yzVertices.push_back(0);
    yzVertices.push_back(0);
    yzVertices.push_back(200);

    yzVertices.push_back(0);
    yzVertices.push_back(0);
    yzVertices.push_back(-200);

    for (int y = 20; y <= 200; y += 20)
    {
        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);

        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);

        yzVertices.push_back(0);
        yzVertices.push_back(y);
        yzVertices.push_back(200);

        yzVertices.push_back(0);
        yzVertices.push_back(y);
        yzVertices.push_back(-200);

        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);

        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);

        yzVertices.push_back(0);
        yzVertices.push_back(-y);
        yzVertices.push_back(200);

        yzVertices.push_back(0);
        yzVertices.push_back(-y);
        yzVertices.push_back(-200);
    }

    for(int z=20;z<=200;z+=20)
    {
        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);

        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);

        yzVertices.push_back(0);
        yzVertices.push_back(200);
        yzVertices.push_back(z);

        yzVertices.push_back(0);
        yzVertices.push_back(-200);
        yzVertices.push_back(z);

        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);

        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);
        yzColors.push_back(180.0/255.0);

        yzVertices.push_back(0);
        yzVertices.push_back(200);
        yzVertices.push_back(-z);

        yzVertices.push_back(0);
        yzVertices.push_back(-200);
        yzVertices.push_back(-z);
    }

    m_backend->updateGridGeometry(&m_yzRenderable, yzVertices, yzColors);
}

void Grid::drawXY() const
{
    m_backend->drawGridGeometry(m_xyRenderable);
}

void Grid::drawXZ() const
{
    m_backend->drawGridGeometry(m_xzRenderable);
}

void Grid::drawYZ() const
{
    m_backend->drawGridGeometry(m_yzRenderable);
}
