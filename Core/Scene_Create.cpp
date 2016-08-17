#include <Core/Scene.h>

static Vector positionAAxis(AxisMode::__Enum axis, float x, float y, float z, float ax, float ay, float az)
{
    Vector result(x,y,z);

    switch (axis)
    {
    case AxisMode::axisZ:
    {
        Vector add(ax,ay,az);
        result += add;
    }
        break;
    case AxisMode::axisX:
    {
        Vector add(az,ax,ay);
        result += add;
    }
        break;
    case AxisMode::axisY:
    {
        Vector add(ax,az,ay);
        result += add;
    }
        break;
    }
    return result;
}

unsigned int Scene::newCube(float x, float y, float z, float lx, float ly, float lz, unsigned int sx, unsigned int sy, unsigned int sz)
{
    Object *cube=new Object("Cube");
    newCube(cube, x, y, z, lx, ly, lz, sx, sy, sz);
    m_objectList.add(cube);
    return cube->m_index;
}

unsigned int Scene::newPlane(float x, float y, float z, float length, float width, AxisMode::__Enum axis, unsigned int sl, unsigned int sw)
{
    Object *plane=new Object("Plane");
    newPlane(plane, x, y, z, length, width, axis, sl, sw);
    m_objectList.add(plane);
    return plane->m_index;
}

void Scene::newPlane(Object *plane, float x, float y, float z, float length, float width, AxisMode::__Enum axis, unsigned int sl, unsigned int sw)
{
    plane->m_center.x=x;
    plane->m_center.y=y;
    plane->m_center.z=z;

    {
        float halfLength = length * 0.5f;
        float halfWidth = width * 0.5f;
        float segmentL = length / sl;
        float segmentW = length / sw;

        unsigned int *tempVertexListRight = new unsigned int[sl+1];
        unsigned int *tempVertexListLeft = new unsigned int[sl+1];
        unsigned int *rightEdge = new unsigned int[sl];
        unsigned int *leftEdge = new unsigned int[sl];

        Vector positionAAxisV(positionAAxis(axis, x, y, z, -halfLength, -halfWidth, 0));
        tempVertexListRight[0] = plane->addVertex(positionAAxisV);

        for(unsigned int e = 1; e <= sl; ++e)
        {
            positionAAxisV = positionAAxis(axis, x, y, z, -halfLength + e * segmentL, -halfWidth, 0);
            tempVertexListRight[e] = plane->addVertex(positionAAxisV);
            rightEdge[e - 1] = plane->addEdge(tempVertexListRight[e - 1], tempVertexListRight[e]);
        }

        for(unsigned int i = 1; i <= sw; ++i)
        {
            positionAAxisV = positionAAxis(axis, x, y, z, -halfLength, -halfWidth + i * segmentW, 0);
            tempVertexListLeft[0] = plane->addVertex(positionAAxisV);
            for(unsigned int e = 1; e <= sl; ++e)
            {
                positionAAxisV=positionAAxis(axis, x, y, z, -halfLength + e * segmentL, -halfWidth + i * segmentW, 0);
                tempVertexListLeft[e] = plane->addVertex(positionAAxisV);
                leftEdge[e - 1] = plane->addEdge(tempVertexListLeft[e - 1], tempVertexListLeft[e]);
            }

            unsigned int bottomEdge = plane->addEdge(tempVertexListRight[0], tempVertexListLeft[0]);

            for(unsigned int e = 0; e < sl; ++e)
            {
                unsigned int topEdge = plane->addEdge(tempVertexListRight[e + 1], tempVertexListLeft[e + 1]);
                unsigned int tempEdges[4];
                tempEdges[0] = bottomEdge;
                tempEdges[1] = leftEdge[e];
                tempEdges[2] = topEdge;
                tempEdges[3] = rightEdge[e];

                plane->addFace(tempEdges, 4);
                bottomEdge = topEdge;
            }

            for(unsigned int e = 0; e < sl; ++e)
            {
                rightEdge[e] = leftEdge[e];
                tempVertexListRight[e] = tempVertexListLeft[e];
            }
            tempVertexListRight[sl] = tempVertexListLeft[sl];
        }

        delete leftEdge;
        delete rightEdge;
        delete tempVertexListLeft;
        delete tempVertexListRight;
    }
}

void Scene::newCube(Object *cube, float x, float y, float z, float lx, float ly, float lz, unsigned int sx, unsigned int sy, unsigned int sz)
{
    cube->m_center.x = x;
    cube->m_center.y = y;
    cube->m_center.z = z;

    float hlx = lx / 2.0f;
    float hly = ly / 2.0f;
    float hlz = lz / 2.0f;

    float slx = lx / sx;
    float sly = ly / sy;
    float slz = lz / sz;

    unsigned int *edge1 = new unsigned int[sz];
    unsigned int *vertex1 = new unsigned int[sz + 1];
    unsigned int *edge2 = new unsigned int[sz];
    unsigned int *vertex2 = new unsigned int[sz + 1];
    unsigned int *edge3 = new unsigned int[sz];
    unsigned int *vertex3 = new unsigned int[sz + 1];
    unsigned int *edge4 = new unsigned int[sz];
    unsigned int *vertex4 = new unsigned int[sz + 1];
    unsigned int *edge5 = new unsigned int[sy];
    unsigned int *vertex5 = new unsigned int[sy + 1];
    unsigned int *edge6 = new unsigned int[sy];
    unsigned int *vertex6 = new unsigned int[sy + 1];
    unsigned int *edge7 = new unsigned int[sx];
    unsigned int *vertex7 = new unsigned int[sx + 1];
    unsigned int *edge8 = new unsigned int[sx];
    unsigned int *vertex8 = new unsigned int[sx + 1];
    unsigned int *edge9 = new unsigned int[sy];
    unsigned int *vertex9 = new unsigned int[sy + 1];
    unsigned int *edge10 = new unsigned int[sy];
    unsigned int *vertex10 = new unsigned int[sy + 1];

    unsigned int *edge11 = new unsigned int[sx];
    unsigned int *vertex11 = new unsigned int[sx + 1];
    unsigned int *edge12 = new unsigned int[sx];
    unsigned int *vertex12 = new unsigned int[sx + 1];

    vertex12[sx] = vertex1[0] = vertex6[0] = cube->addVertex(x+hlx,y+hly,z-hlz);
    vertex6[sy] = vertex8[0] = vertex2[0] = cube->addVertex(x+hlx,y-hly,z-hlz);
    vertex8[sx] = vertex10[0] = vertex3[0] = cube->addVertex(x-hlx,y-hly,z-hlz);
    vertex10[sy] = vertex12[0] = vertex4[0] = cube->addVertex(x-hlx,y+hly,z-hlz);

    vertex11[sx] = vertex5[0] = vertex1[sz] = cube->addVertex(x+hlx,y+hly,z+hlz);
    vertex5[sy] = vertex7[0] = vertex2[sz] = cube->addVertex(x+hlx,y-hly,z+hlz);
    vertex7[sx] = vertex9[0] = vertex3[sz] = cube->addVertex(x-hlx,y-hly,z+hlz);
    vertex9[sy] = vertex11[0] = vertex4[sz] = cube->addVertex(x-hlx,y+hly,z+hlz);

    for(unsigned int i = 1; i < sz; ++i)
    {
        vertex1[i]=cube->addVertex(x+hlx,y+hly,z-hlz+i*slz);
        vertex2[i]=cube->addVertex(x+hlx,y-hly,z-hlz+i*slz);
        vertex3[i]=cube->addVertex(x-hlx,y-hly,z-hlz+i*slz);
        vertex4[i]=cube->addVertex(x-hlx,y+hly,z-hlz+i*slz);
    }

    for(unsigned int i = 1; i < sy; ++i)
    {
        vertex5[i]=cube->addVertex(x+hlx,y+hly-i*sly,z+hlz);
        vertex6[i]=cube->addVertex(x+hlx,y+hly-i*sly,z-hlz);
        vertex9[i]=cube->addVertex(x-hlx,y-hly+i*sly,z+hlz);
        vertex10[i]=cube->addVertex(x-hlx,y-hly+i*sly,z-hlz);
    }

    for(unsigned int i = 1; i < sx; ++i)
    {
        vertex7[i]=cube->addVertex(x+hlx-i*slx,y-hly,z+hlz);
        vertex8[i]=cube->addVertex(x+hlx-i*slx,y-hly,z-hlz);
        vertex11[i]=cube->addVertex(x-hlx+i*slx,y+hly,z+hlz);
        vertex12[i]=cube->addVertex(x-hlx+i*slx,y+hly,z-hlz);
    }

    for(unsigned int i = 0; i < sz; ++i)
    {
        edge1[i]=cube->addEdge(vertex1[i],vertex1[i+1]);
        edge2[i]=cube->addEdge(vertex2[i],vertex2[i+1]);
        edge3[i]=cube->addEdge(vertex3[i],vertex3[i+1]);
        edge4[i]=cube->addEdge(vertex4[i],vertex4[i+1]);
    }

    for(unsigned int i = 0; i < sx; ++i)
    {
        edge7[i]=cube->addEdge(vertex7[i],vertex7[i+1]);
        edge8[i]=cube->addEdge(vertex8[i],vertex8[i+1]);
        edge11[i]=cube->addEdge(vertex11[i],vertex11[i+1]);
        edge12[i]=cube->addEdge(vertex12[i],vertex12[i+1]);
    }

    for(unsigned int i = 0; i < sy; ++i)
    {
        edge5[i]=cube->addEdge(vertex5[i],vertex5[i+1]);
        edge6[i]=cube->addEdge(vertex6[i],vertex6[i+1]);
        edge9[i]=cube->addEdge(vertex9[i],vertex9[i+1]);
        edge10[i]=cube->addEdge(vertex10[i],vertex10[i+1]);
    }

    {
        unsigned int *rightEdge1=new unsigned int[sz];
        unsigned int *rightEdge3=new unsigned int[sz];
        unsigned int *rightVertex1=new unsigned int[sz];
        unsigned int *rightVertex3=new unsigned int[sz];
        for(unsigned int i=0;i<sz;++i)
        {
            rightEdge1[i]=edge1[i];
            rightEdge3[i]=edge3[i];
            rightVertex1[i]=vertex1[i+1];
            rightVertex3[i]=vertex3[i+1];
        }
        for(unsigned int i = 0; i < sy-1; ++i)
        {
            unsigned int lowEdge1=edge6[i];
            unsigned int lowEdge3=edge10[i];
            unsigned int lowLeftVertex1=vertex6[i+1];
            unsigned int lowLeftVertex3=vertex10[i+1];
            unsigned int *leftEdge1=new unsigned int[sz];
            unsigned int *leftEdge3=new unsigned int[sz];
            for(unsigned int e = 0; e < sz-1; ++e)
            {
                unsigned int newVertex1=cube->addVertex(x+hlx,y+hly-(i+1)*sly,z-hlz+(e+1)*slz);
                unsigned int newVertex3=cube->addVertex(x-hlx,y-hly+(i+1)*sly,z-hlz+(e+1)*slz);
                leftEdge1[e]=cube->addEdge(lowLeftVertex1,newVertex1);
                leftEdge3[e]=cube->addEdge(lowLeftVertex3,newVertex3);
                unsigned int topEdge1=cube->addEdge(newVertex1,rightVertex1[e]);
                unsigned int topEdge3=cube->addEdge(newVertex3,rightVertex3[e]);
                unsigned int edgeList1[4]={0};
                unsigned int edgeList3[4]={0};
                edgeList1[0]=leftEdge1[e];
                edgeList3[0]=leftEdge3[e];
                edgeList1[1]=topEdge1;
                edgeList3[1]=topEdge3;
                edgeList1[2]=rightEdge1[e];
                edgeList3[2]=rightEdge3[e];
                edgeList1[3]=lowEdge1;
                edgeList3[3]=lowEdge3;
                cube->addFace(edgeList1,4);
                cube->addFace(edgeList3,4);
                lowEdge1=topEdge1;
                lowEdge3=topEdge3;
                lowLeftVertex1=newVertex1;
                lowLeftVertex3=newVertex3;
                rightVertex1[e]=newVertex1;
                rightVertex3[e]=newVertex3;
            }
            {
                leftEdge1[sz-1]=cube->addEdge(lowLeftVertex1,vertex5[i+1]);
                leftEdge3[sz-1]=cube->addEdge(lowLeftVertex3,vertex9[i+1]);
                unsigned int topEdge1=edge5[i];
                unsigned int topEdge3=edge9[i];
                unsigned int edgeList1[4]={0};
                unsigned int edgeList3[4]={0};
                edgeList1[0]=leftEdge1[sz-1];
                edgeList3[0]=leftEdge3[sz-1];
                edgeList1[1]=topEdge1;
                edgeList3[1]=topEdge3;
                edgeList1[2]=rightEdge1[sz-1];
                edgeList3[2]=rightEdge3[sz-1];
                edgeList1[3]=lowEdge1;
                edgeList3[3]=lowEdge3;
                cube->addFace(edgeList1,4);
                cube->addFace(edgeList3,4);
                rightVertex1[sz-1]=vertex5[i+1];
                rightVertex3[sz-1]=vertex9[i+1];
            }
            for(unsigned int e=0;e<sz;++e)
            {
                rightEdge1[e]=leftEdge1[e];
                rightEdge3[e]=leftEdge3[e];
            }
            delete leftEdge1;
            delete leftEdge3;
        }
        unsigned int lowEdge1=edge6[sy-1];
        unsigned int lowEdge3=edge10[sy-1];
        unsigned int lowLeftVertex1=vertex2[0];
        unsigned int lowLeftVertex3=vertex4[0];
        for(unsigned int e=0;e<sz-1;++e)
        {
            unsigned int newVertex1=vertex2[e+1];
            unsigned int newVertex3=vertex4[e+1];
            unsigned int topEdge1=cube->addEdge(newVertex1,rightVertex1[e]);
            unsigned int topEdge3=cube->addEdge(newVertex3,rightVertex3[e]);
            unsigned int edgeList1[4]={0};
            unsigned int edgeList3[4]={0};
            edgeList1[0]=edge2[e];
            edgeList3[0]=edge4[e];
            edgeList1[1]=topEdge1;
            edgeList3[1]=topEdge3;
            edgeList1[2]=rightEdge1[e];
            edgeList3[2]=rightEdge3[e];
            edgeList1[3]=lowEdge1;
            edgeList3[3]=lowEdge3;
            cube->addFace(edgeList1,4);
            cube->addFace(edgeList3,4);
            lowEdge1=topEdge1;
            lowEdge3=topEdge3;
            lowLeftVertex1=newVertex1;
            lowLeftVertex3=newVertex3;
        }
        {
            unsigned int edgeList1[4]={0};
            unsigned int edgeList3[4]={0};
            edgeList1[0]=edge2[sz-1];
            edgeList3[0]=edge4[sz-1];
            edgeList1[1]=edge5[sy-1];
            edgeList3[1]=edge9[sy-1];
            edgeList1[2]=rightEdge1[sz-1];
            edgeList3[2]=rightEdge3[sz-1];
            edgeList1[3]=lowEdge1;
            edgeList3[3]=lowEdge3;
            cube->addFace(edgeList1,4);
            cube->addFace(edgeList3,4);
        }
        delete rightEdge1;
        delete rightEdge3;
        delete rightVertex1;
        delete rightVertex3;
    }
    {
        unsigned int *rightEdge2=new unsigned int[sz];
        unsigned int *rightEdge4=new unsigned int[sz];
        unsigned int *rightVertex2=new unsigned int[sz];
        unsigned int *rightVertex4=new unsigned int[sz];
        for(unsigned int i=0;i<sz;++i)
        {
            rightEdge2[i]=edge2[i];
            rightEdge4[i]=edge4[i];
            rightVertex2[i]=vertex2[i+1];
            rightVertex4[i]=vertex4[i+1];
        }
        for(unsigned int i=0;i<sx-1;++i)
        {
            unsigned int lowEdge2=edge8[i];
            unsigned int lowEdge4=edge12[i];
            unsigned int lowLeftVertex2=vertex8[i+1];
            unsigned int lowLeftVertex4=vertex12[i+1];
            unsigned int *leftEdge2=new unsigned int[sz];
            unsigned int *leftEdge4=new unsigned int[sz];
            for(unsigned int e=0;e<sz-1;++e)
            {
                unsigned int newVertex2=cube->addVertex(x+hlx-(i+1)*slx,y-hly,z-hlz+(e+1)*slz);
                unsigned int newVertex4=cube->addVertex(x-hlx+(i+1)*slx,y+hly,z-hlz+(e+1)*slz);
                leftEdge2[e]=cube->addEdge(lowLeftVertex2,newVertex2);
                leftEdge4[e]=cube->addEdge(lowLeftVertex4,newVertex4);
                unsigned int topEdge2=cube->addEdge(newVertex2,rightVertex2[e]);
                unsigned int topEdge4=cube->addEdge(newVertex4,rightVertex4[e]);
                unsigned int edgeList2[4]={0};
                unsigned int edgeList4[4]={0};
                edgeList2[0]=leftEdge2[e];
                edgeList4[0]=leftEdge4[e];
                edgeList2[1]=topEdge2;
                edgeList4[1]=topEdge4;
                edgeList2[2]=rightEdge2[e];
                edgeList4[2]=rightEdge4[e];
                edgeList2[3]=lowEdge2;
                edgeList4[3]=lowEdge4;
                cube->addFace(edgeList2,4);
                cube->addFace(edgeList4,4);
                lowEdge2=topEdge2;
                lowEdge4=topEdge4;
                lowLeftVertex2=newVertex2;
                lowLeftVertex4=newVertex4;
                rightVertex2[e]=newVertex2;
                rightVertex4[e]=newVertex4;
            }
            {
                leftEdge2[sz-1]=cube->addEdge(lowLeftVertex2,vertex7[i+1]);
                leftEdge4[sz-1]=cube->addEdge(lowLeftVertex4,vertex11[i+1]);
                unsigned int topEdge2=edge7[i];
                unsigned int topEdge4=edge11[i];
                unsigned int edgeList2[4]={0};
                unsigned int edgeList4[4]={0};
                edgeList2[0]=leftEdge2[sz-1];
                edgeList4[0]=leftEdge4[sz-1];
                edgeList2[1]=topEdge2;
                edgeList4[1]=topEdge4;
                edgeList2[2]=rightEdge2[sz-1];
                edgeList4[2]=rightEdge4[sz-1];
                edgeList2[3]=lowEdge2;
                edgeList4[3]=lowEdge4;
                cube->addFace(edgeList2,4);
                cube->addFace(edgeList4,4);
                rightVertex2[sz-1]=vertex7[i+1];
                rightVertex4[sz-1]=vertex11[i+1];
            }
            for(unsigned int e=0;e<sz;++e)
            {
                rightEdge2[e]=leftEdge2[e];
                rightEdge4[e]=leftEdge4[e];
            }
            delete leftEdge2;
            delete leftEdge4;
        }
        unsigned int lowEdge2=edge8[sx-1];
        unsigned int lowEdge4=edge12[sx-1];
        unsigned int lowLeftVertex2=vertex3[0];
        unsigned int lowLeftVertex4=vertex1[0];
        for(unsigned int e = 0; e < sz-1; e++)
        {
            unsigned int newVertex2=vertex3[e+1];
            unsigned int newVertex4=vertex1[e+1];
            unsigned int topEdge2=cube->addEdge(newVertex2,rightVertex2[e]);
            unsigned int topEdge4=cube->addEdge(newVertex4,rightVertex4[e]);
            unsigned int edgeList2[4]={0};
            unsigned int edgeList4[4]={0};
            edgeList2[0]=edge3[e];
            edgeList4[0]=edge1[e];
            edgeList2[1]=topEdge2;
            edgeList4[1]=topEdge4;
            edgeList2[2]=rightEdge2[e];
            edgeList4[2]=rightEdge4[e];
            edgeList2[3]=lowEdge2;
            edgeList4[3]=lowEdge4;
            cube->addFace(edgeList2,4);
            cube->addFace(edgeList4,4);
            lowEdge2=topEdge2;
            lowEdge4=topEdge4;
            lowLeftVertex2=newVertex2;
            lowLeftVertex4=newVertex4;
        }

        {
            unsigned int edgeList2[4]={0};
            unsigned int edgeList4[4]={0};
            edgeList2[0]=edge3[sz-1];
            edgeList4[0]=edge1[sz-1];
            edgeList2[1]=edge7[sx-1];
            edgeList4[1]=edge11[sx-1];
            edgeList2[2]=rightEdge2[sz-1];
            edgeList4[2]=rightEdge4[sz-1];
            edgeList2[3]=lowEdge2;
            edgeList4[3]=lowEdge4;
            cube->addFace(edgeList2,4);
            cube->addFace(edgeList4,4);
        }
        delete rightEdge2;
        delete rightEdge4;
        delete rightVertex2;
        delete rightVertex4;
    }
    {
        unsigned int *rightEdge5=new unsigned int[sx];
        unsigned int *rightVertex5=new unsigned int[sx];
        for(unsigned int i=0;i<sx;++i)
        {
            rightEdge5[i]=edge7[i];
            rightVertex5[i]=vertex7[i+1];
        }
        for(unsigned int i=0;i<sy-1;++i)
        {
            unsigned int lowEdge5=edge5[sy-1-i];
            unsigned int lowLeftVertex5=vertex5[sy-1-i];
            unsigned int *leftEdge5=new unsigned int[sx];
            for(unsigned int e=0;e<sx-1;++e)
            {
                unsigned int newVertex5=cube->addVertex(x+hlx-(e+1)*slx,y-hly+(i+1)*sly,z+hlz);
                leftEdge5[e]=cube->addEdge(lowLeftVertex5,newVertex5);
                unsigned int topEdge5=cube->addEdge(newVertex5,rightVertex5[e]);
                unsigned int edgeList5[4]={0};
                edgeList5[3]=leftEdge5[e];
                edgeList5[2]=topEdge5;
                edgeList5[1]=rightEdge5[e];
                edgeList5[0]=lowEdge5;
                cube->addFace(edgeList5,4);
                lowEdge5=topEdge5;
                lowLeftVertex5=newVertex5;
                rightVertex5[e]=newVertex5;
            }
            {
                leftEdge5[sx-1]=cube->addEdge(lowLeftVertex5,vertex9[i+1]);
                unsigned int topEdge5=edge9[i];
                unsigned int edgeList5[4]={0};
                edgeList5[3]=leftEdge5[sx-1];
                edgeList5[2]=topEdge5;
                edgeList5[1]=rightEdge5[sx-1];
                edgeList5[0]=lowEdge5;
                cube->addFace(edgeList5,4);
                rightVertex5[sx-1]=vertex9[i+1];
            }
            for(unsigned int e=0;e<sx;++e)
            {
                rightEdge5[e]=leftEdge5[e];
            }
            delete leftEdge5;
        }
        unsigned int lowEdge5=edge5[0];
        unsigned int lowLeftVertex5=vertex11[sx];
        for(unsigned int e=0;e<sx-1;++e)
        {
            unsigned int newVertex5=vertex11[sx-1-e];
            unsigned int topEdge5=cube->addEdge(newVertex5,rightVertex5[e]);
            unsigned int edgeList5[4]={0};
            edgeList5[3]=edge11[sx-1-e];
            edgeList5[2]=topEdge5;
            edgeList5[1]=rightEdge5[e];
            edgeList5[0]=lowEdge5;
            cube->addFace(edgeList5,4);
            lowEdge5=topEdge5;
            lowLeftVertex5=newVertex5;
        }
        {
            unsigned int edgeList5[4]={0};
            edgeList5[3]=edge11[0];
            edgeList5[2]=edge9[sy-1];
            edgeList5[1]=rightEdge5[sx-1];
            edgeList5[0]=lowEdge5;
            cube->addFace(edgeList5,4);
        }
        delete rightEdge5;
        delete rightVertex5;
    }
    {
        unsigned int *rightEdge6=new unsigned int[sy];
        unsigned int *rightVertex6=new unsigned int[sy];
        for(unsigned int i=0;i<sy;++i)
        {
            rightEdge6[i]=edge6[i];
            rightVertex6[i]=vertex6[i+1];
        }
        for(unsigned int i=0;i<sx-1;++i)
        {
            unsigned int lowEdge6=edge12[sx-1-i];
            unsigned int lowLeftVertex6=vertex12[sx-1-i];
            unsigned int *leftEdge6=new unsigned int[sy];
            for(unsigned int e=0;e<sy-1;++e)
            {
                unsigned int newVertex6=cube->addVertex(x+hlx-(i+1)*slx,y+hly-(e+1)*sly,z-hlz);
                leftEdge6[e]=cube->addEdge(lowLeftVertex6,newVertex6);
                unsigned int topEdge6=cube->addEdge(newVertex6,rightVertex6[e]);
                unsigned int edgeList6[4]={0};
                edgeList6[0]=leftEdge6[e];
                edgeList6[1]=topEdge6;
                edgeList6[2]=rightEdge6[e];
                edgeList6[3]=lowEdge6;
                cube->addFace(edgeList6,4);
                lowEdge6=topEdge6;
                lowLeftVertex6=newVertex6;
                rightVertex6[e]=newVertex6;
            }
            {
                leftEdge6[sy-1]=cube->addEdge(lowLeftVertex6,vertex8[i+1]);
                unsigned int topEdge6=edge8[i];
                unsigned int edgeList6[4]={0};
                edgeList6[0]=leftEdge6[sy-1];
                edgeList6[1]=topEdge6;
                edgeList6[2]=rightEdge6[sy-1];
                edgeList6[3]=lowEdge6;
                cube->addFace(edgeList6,4);
                rightVertex6[sy-1]=vertex8[i+1];
            }
            for(unsigned int e=0;e<sy;++e)
            {
                rightEdge6[e]=leftEdge6[e];
            }
            delete leftEdge6;
        }

        unsigned int lowEdge6=edge12[0];
        unsigned int lowLeftVertex6=vertex10[sy];
        for(unsigned int e=0;e<sy-1;++e)
        {
            unsigned int newVertex6=vertex10[sy-1-e];
            unsigned int topEdge6=cube->addEdge(newVertex6,rightVertex6[e]);
            unsigned int edgeList6[4]={0};
            edgeList6[0]=edge10[sy-1-e];
            edgeList6[1]=topEdge6;
            edgeList6[2]=rightEdge6[e];
            edgeList6[3]=lowEdge6;
            cube->addFace(edgeList6,4);
            lowEdge6=topEdge6;
            lowLeftVertex6=newVertex6;
        }
        {
            unsigned int edgeList6[4]={0};
            edgeList6[0]=edge10[0];
            edgeList6[1]=edge8[sx-1];
            edgeList6[2]=rightEdge6[sy-1];
            edgeList6[3]=lowEdge6;
            cube->addFace(edgeList6,4);
        }
        delete rightEdge6;
        delete rightVertex6;
    }
    delete edge1;
    delete vertex1;
    delete edge2;
    delete vertex2;
    delete edge3;
    delete vertex3;
    delete edge4;
    delete vertex4;

    delete edge5;
    delete vertex5;
    delete edge6;
    delete vertex6;

    delete edge7;
    delete vertex7;
    delete edge8;
    delete vertex8;

    delete edge9;
    delete vertex9;
    delete edge10;
    delete vertex10;

    delete edge11;
    delete vertex11;
    delete edge12;
    delete vertex12;
}

unsigned int Scene::newCylinder(float x,float y,float z,float r,float h,AxisMode::__Enum axis,unsigned int sa,unsigned int sr,unsigned int sh)
{
    Object *cylinder=new Object("Clinder");
    newCylinder(cylinder,x,y,z,r,h,axis,sa,sr,sh);
    m_objectList.add(cylinder);
    return cylinder->m_index;
}
void Scene::newCylinder(Object *cylinder,float x,float y,float z,float r,float h,AxisMode::__Enum axis,unsigned int sa,unsigned int sr,unsigned int sh)
{
    cylinder->m_center.x=x;
    cylinder->m_center.y=y;
    cylinder->m_center.z=z;

    {
        Vector positionAAxisV(positionAAxis(axis,x,y,z,0,0,0));
        unsigned int center=cylinder->addVertex(positionAAxisV);
        positionAAxisV=positionAAxis(axis,x,y,z,0,0,h);
        unsigned int centerH=cylinder->addVertex(positionAAxisV);

        float da=ps2PI/sa;
        float dr=r/sr;
        float dh=h/sh;

        unsigned int *startHEdge=new unsigned int[sh];
        unsigned int *startHVertex=new unsigned int[sh+1];

        unsigned int *startUEdge=new unsigned int[sr];
        unsigned int *startUVertex=new unsigned int[sr+1];

        unsigned int *startLEdge=new unsigned int[sr];
        unsigned int *startLVertex=new unsigned int[sr+1];

        startUVertex[0]=centerH;
        startLVertex[0]=center;

        positionAAxisV=positionAAxis(axis,x,y,z,r,0,h);
        startUVertex[sr]=startHVertex[sh]=cylinder->addVertex(positionAAxisV);
        positionAAxisV=positionAAxis(axis,x,y,z,r,0,0);
        startLVertex[sr]=startHVertex[0]=cylinder->addVertex(positionAAxisV);

        for(unsigned int i=1;i<sr;++i)
        {
            positionAAxisV=positionAAxis(axis,x,y,z,dr*i,0,h);
            startUVertex[i]=cylinder->addVertex(positionAAxisV);
            startUEdge[i-1]=cylinder->addEdge(startUVertex[i-1],startUVertex[i]);
            positionAAxisV=positionAAxis(axis,x,y,z,dr*i,0,0);
            startLVertex[i]=cylinder->addVertex(positionAAxisV);
            startLEdge[i-1]=cylinder->addEdge(startLVertex[i-1],startLVertex[i]);
        }
        startUEdge[sr-1]=cylinder->addEdge(startUVertex[sr-1],startUVertex[sr]);
        startLEdge[sr-1]=cylinder->addEdge(startLVertex[sr-1],startLVertex[sr]);

        for(unsigned int i=1;i<sh;++i)
        {
            positionAAxisV=positionAAxis(axis,x,y,z,r,0,dh*i);
            startHVertex[i]=cylinder->addVertex(positionAAxisV);
            startHEdge[i-1]=cylinder->addEdge(startHVertex[i-1],startHVertex[i]);
        }
        startHEdge[sh-1]=cylinder->addEdge(startHVertex[sh-1],startHVertex[sh]);

        unsigned int *endHEdgeC=new unsigned int[sh];
        unsigned int *endHVertexC=new unsigned int[sh+1];

        for(unsigned int i=0;i<sh;++i)
        {
            endHEdgeC[i]=startHEdge[i];
            endHVertexC[i]=startHVertex[i];
        }
        endHVertexC[sh]=startHVertex[sh];

        unsigned int *endUEdgeC=new unsigned int[sr];
        unsigned int *endUVertexC=new unsigned int[sr+1];
        unsigned int *endLEdgeC=new unsigned int[sr];
        unsigned int *endLVertexC=new unsigned int[sr+1];

        for(unsigned int i=0;i<sr;++i)
        {
            endUEdgeC[i]=startUEdge[i];
            endUVertexC[i]=startUVertex[i];
            endLEdgeC[i]=startLEdge[i];
            endLVertexC[i]=startLVertex[i];
        }
        endUVertexC[sr]=startUVertex[sr];
        endLVertexC[sr]=startLVertex[sr];

        for(unsigned int e=1;e<sa;++e)
        {
            unsigned int *endHEdge=new unsigned int[sh];
            unsigned int *endHVertex=new unsigned int[sh+1];
            unsigned int *endUEdge=new unsigned int[sr];
            unsigned int *endUVertex=new unsigned int[sr+1];
            unsigned int *endLEdge=new unsigned int[sr];
            unsigned int *endLVertex=new unsigned int[sr+1];

            endUVertex[0]=centerH;
            endLVertex[0]=center;

            positionAAxisV=positionAAxis(axis,x,y,z,r*cos(da*e),r*sin(da*e),h);
            endUVertex[sr]=endHVertex[sh]=cylinder->addVertex(positionAAxisV);
            positionAAxisV=positionAAxis(axis,x,y,z,r*cos(da*e),r*sin(da*e),0);
            endLVertex[sr]=endHVertex[0]=cylinder->addVertex(positionAAxisV);

            for(unsigned int i=1;i<sr;++i)
            {
                positionAAxisV=positionAAxis(axis,x,y,z,dr*i*cos(da*e),dr*i*sin(da*e),h);
                endUVertex[i]=cylinder->addVertex(positionAAxisV);
                endUEdge[i-1]=cylinder->addEdge(endUVertex[i-1],endUVertex[i]);
                positionAAxisV=positionAAxis(axis,x,y,z,dr*i*cos(da*e),dr*i*sin(da*e),0);
                endLVertex[i]=cylinder->addVertex(positionAAxisV);
                endLEdge[i-1]=cylinder->addEdge(endLVertex[i-1],endLVertex[i]);
            }
            endUEdge[sr-1]=cylinder->addEdge(endUVertex[sr-1],endUVertex[sr]);
            endLEdge[sr-1]=cylinder->addEdge(endLVertex[sr-1],endLVertex[sr]);

            for(unsigned int i=1;i<sh;++i)
            {
                positionAAxisV=positionAAxis(axis,x,y,z,r*cos(da*e),r*sin(da*e),dh*i);
                endHVertex[i]=cylinder->addVertex(positionAAxisV);
                endHEdge[i-1]=cylinder->addEdge(endHVertex[i-1],endHVertex[i]);
            }
            endHEdge[sh-1]=cylinder->addEdge(endHVertex[sh-1],endHVertex[sh]);

            unsigned int LEdge=cylinder->addEdge(endHVertex[0],startHVertex[0]);
            unsigned int UEdge=cylinder->addEdge(endHVertex[sh],startHVertex[sh]);
            unsigned int HLEdge=LEdge;
            unsigned int HUEdge=UEdge;
            for(unsigned int i=sr-1;i>0;--i)
            {
                unsigned int tempLEdge=cylinder->addEdge(endLVertex[i],startLVertex[i]);
                unsigned int tempUEdge=cylinder->addEdge(endUVertex[i],startUVertex[i]);

                unsigned int edgeListL[4]={0};
                edgeListL[3]=tempLEdge;
                edgeListL[2]=endLEdge[i];
                edgeListL[1]=LEdge;
                edgeListL[0]=startLEdge[i];
                cylinder->addFace(edgeListL,4);

                unsigned int edgeListU[4]={0};
                edgeListU[3]=tempUEdge;
                edgeListU[2]=startUEdge[i];
                edgeListU[1]=UEdge;
                edgeListU[0]=endUEdge[i];
                cylinder->addFace(edgeListU,4);

                LEdge=tempLEdge;
                UEdge=tempUEdge;
            }
            unsigned int edgeListL[3]={0};
            edgeListL[2]=endLEdge[0];
            edgeListL[1]=LEdge;
            edgeListL[0]=startLEdge[0];
            cylinder->addFace(edgeListL,3);

            unsigned int edgeListU[3]={0};
            edgeListU[2]=startUEdge[0];
            edgeListU[1]=UEdge;
            edgeListU[0]=endUEdge[0];
            cylinder->addFace(edgeListU,3);

            for(unsigned int i=1;i<sh;i++)
            {
                unsigned int tempHLEdge=cylinder->addEdge(endHVertex[i],startHVertex[i]);
                unsigned int edgeListH[4]={0};
                edgeListH[3]=tempHLEdge;
                edgeListH[2]=startHEdge[i-1];
                edgeListH[1]=HLEdge;
                edgeListH[0]=endHEdge[i-1];
                cylinder->addFace(edgeListH,4);
                HLEdge=tempHLEdge;
            }
                unsigned int edgeListH[4]={0};
                edgeListH[3]=HUEdge;
                edgeListH[2]=startHEdge[sh-1];
                edgeListH[1]=HLEdge;
                edgeListH[0]=endHEdge[sh-1];
                cylinder->addFace(edgeListH,4);

            delete startHEdge;
            delete startHVertex;
            delete startUEdge;
            delete startUVertex;
            delete startLEdge;
            delete startLVertex;

            startHEdge=endHEdge;
            startHVertex=endHVertex;
            startUEdge=endUEdge;
            startUVertex=endUVertex;
            startLEdge=endLEdge;
            startLVertex=endLVertex;
        }

            unsigned int LEdge=cylinder->addEdge(endHVertexC[0],startHVertex[0]);
            unsigned int UEdge=cylinder->addEdge(endHVertexC[sh],startHVertex[sh]);
            unsigned int HLEdge=LEdge;
            unsigned int HUEdge=UEdge;
            for(unsigned int i=sr-1;i>0;--i)
            {
                unsigned int tempLEdge=cylinder->addEdge(endLVertexC[i],startLVertex[i]);
                unsigned int tempUEdge=cylinder->addEdge(endUVertexC[i],startUVertex[i]);

                unsigned int edgeListL[4]={0};
                edgeListL[3]=tempLEdge;
                edgeListL[2]=endLEdgeC[i];
                edgeListL[1]=LEdge;
                edgeListL[0]=startLEdge[i];
                cylinder->addFace(edgeListL,4);

                unsigned int edgeListU[4]={0};
                edgeListU[3]=tempUEdge;
                edgeListU[2]=startUEdge[i];
                edgeListU[1]=UEdge;
                edgeListU[0]=endUEdgeC[i];
                cylinder->addFace(edgeListU,4);

                LEdge=tempLEdge;
                UEdge=tempUEdge;
            }
            unsigned int edgeListL[3]={0};
            edgeListL[2]=endLEdgeC[0];
            edgeListL[1]=LEdge;
            edgeListL[0]=startLEdge[0];
            cylinder->addFace(edgeListL,3);

            unsigned int edgeListU[3]={0};
            edgeListU[2]=startUEdge[0];
            edgeListU[1]=UEdge;
            edgeListU[0]=endUEdgeC[0];
            cylinder->addFace(edgeListU,3);

            for(unsigned int i=1;i<sh;++i)
            {
                unsigned int tempHLEdge=cylinder->addEdge(endHVertexC[i],startHVertex[i]);
                unsigned int edgeListH[4]={0};
                edgeListH[3]=tempHLEdge;
                edgeListH[2]=startHEdge[i-1];
                edgeListH[1]=HLEdge;
                edgeListH[0]=endHEdgeC[i-1];
                cylinder->addFace(edgeListH,4);
                HLEdge=tempHLEdge;
            }
                unsigned int edgeListH[4]={0};
                edgeListH[3]=HUEdge;
                edgeListH[2]=startHEdge[sh-1];
                edgeListH[1]=HLEdge;
                edgeListH[0]=endHEdgeC[sh-1];
                cylinder->addFace(edgeListH,4);

            delete startHEdge;
            delete startHVertex;
            delete startUEdge;
            delete startUVertex;
            delete startLEdge;
            delete startLVertex;

            delete endHEdgeC;
            delete endHVertexC;
            delete endUEdgeC;
            delete endUVertexC;
            delete endLEdgeC;
            delete endLVertexC;
    }

}

unsigned int Scene::newSphere(float x, float y, float z, float r, AxisMode::__Enum axis, unsigned int sa, unsigned int sr)
{
    Object *sphere=new Object("Sphere");
    newSphere(sphere,x,y,z,r,axis,sa,sr);
    m_objectList.add(sphere);
    return sphere->m_index;
}

void Scene::newSphere(Object *sphere,float x,float y,float z,float r,AxisMode::__Enum axis,unsigned int sa,unsigned int sr)
{
    sphere->m_center.x=x;
    sphere->m_center.y=y;
    sphere->m_center.z=z;

    Vector positionAAxisV(positionAAxis(axis,x,y,z,0,0,r));
    unsigned int centerU=sphere->addVertex(positionAAxisV);
    positionAAxisV=positionAAxis(axis,x,y,z,0,0,-r);
    unsigned int centerL=sphere->addVertex(positionAAxisV);

    float dr=psPI2/sr;
    float da=ps2PI/sa;

    unsigned int *startUEdge=new unsigned int[sr];
    unsigned int *startUVertex=new unsigned int[sr+1];

    unsigned int *startLEdge=new unsigned int[sr];
    unsigned int *startLVertex=new unsigned int[sr+1];

    unsigned int *endUEdgeC=new unsigned int[sr];
    unsigned int *endUVertexC=new unsigned int[sr+1];

    unsigned int *endLEdgeC=new unsigned int[sr];
    unsigned int *endLVertexC=new unsigned int[sr+1];

    startUVertex[0]=endUVertexC[0]=centerU;
    startLVertex[0]=endLVertexC[0]=centerL;

    positionAAxisV=positionAAxis(axis,x,y,z,r,0,0);
    startUVertex[sr]=endUVertexC[sr]=startLVertex[sr]=endLVertexC[sr]=sphere->addVertex(positionAAxisV);

    for(unsigned int i=1;i<sr;++i)
    {
        positionAAxisV=positionAAxis(axis,x,y,z,r*cos((sr-i)*dr),0,r*sin((sr-i)*dr));
        startUVertex[i]=endUVertexC[i]=sphere->addVertex(positionAAxisV);
        positionAAxisV=positionAAxis(axis,x,y,z,r*cos((sr-i)*dr),0,-r*sin((sr-i)*dr));
        startLVertex[i]=endLVertexC[i]=sphere->addVertex(positionAAxisV);
    }

    for(unsigned int i=0;i<sr;++i)
    {
        startUEdge[i]=endUEdgeC[i]=sphere->addEdge(startUVertex[i],startUVertex[i+1]);
        startLEdge[i]=endLEdgeC[i]=sphere->addEdge(startLVertex[i],startLVertex[i+1]);
    }

    for(unsigned int e=0;e<sa-1;++e)
    {
        unsigned int *endUEdge=new unsigned int[sr];
        unsigned int *endUVertex=new unsigned int[sr+1];
        unsigned int *endLEdge=new unsigned int[sr];
        unsigned int *endLVertex=new unsigned int[sr+1];
        endUVertex[0]=centerU;
        endLVertex[0]=centerL;
        positionAAxisV=positionAAxis(axis,x,y,z,r*cos((e+1)*da),r*sin((e+1)*da),0);
        endUVertex[sr]=endLVertex[sr]=sphere->addVertex(positionAAxisV);

        for(unsigned int i=1;i<sr;++i)
        {
            positionAAxisV=positionAAxis(axis,x,y,z,r*cos((sr-i)*dr)*cos((e+1)*da),r*cos((sr-i)*dr)*sin((e+1)*da),r*sin((sr-i)*dr));
            endUVertex[i]=sphere->addVertex(positionAAxisV);
            positionAAxisV=positionAAxis(axis,x,y,z,r*cos((sr-i)*dr)*cos((e+1)*da),r*cos((sr-i)*dr)*sin((e+1)*da),-r*sin((sr-i)*dr));
            endLVertex[i]=sphere->addVertex(positionAAxisV);
        }

        for(unsigned int i=0;i<sr;++i)
        {
            endUEdge[i]=sphere->addEdge(endUVertex[i],endUVertex[i+1]);
            endLEdge[i]=sphere->addEdge(endLVertex[i],endLVertex[i+1]);
        }

        //ÕýÊ½Éú³ÉÃæ
        unsigned int middleEdge=sphere->addEdge(endUVertex[sr],startUVertex[sr]);
        unsigned int upEdge=sphere->addEdge(endUVertex[1],startUVertex[1]);
        unsigned int lowEdge=sphere->addEdge(endLVertex[1],startLVertex[1]);
        unsigned int tempUpEdge=upEdge;
        unsigned int tempLowEdge=lowEdge;
        for(unsigned int i=1;i<sr-1;++i)
        {
            unsigned int tempEdgeU=sphere->addEdge(endUVertex[i+1],startUVertex[i+1]);
            unsigned int edgeListU[4]={0};
            edgeListU[3]=endUEdge[i];
            edgeListU[2]=tempUpEdge;
            edgeListU[1]=startUEdge[i];
            edgeListU[0]=tempEdgeU;
            sphere->addFace(edgeListU,4);

            unsigned int tempEdgeL=sphere->addEdge(endLVertex[i+1],startLVertex[i+1]);
            unsigned int edgeListL[4]={0};
            edgeListL[3]=endLEdge[i];
            edgeListL[2]=tempEdgeL;
            edgeListL[1]=startLEdge[i];
            edgeListL[0]=tempLowEdge;
            sphere->addFace(edgeListL,4);

            tempUpEdge=tempEdgeU;
            tempLowEdge=tempEdgeL;
        }

        unsigned int edgeListU[4]={0};
        edgeListU[3]=endUEdge[sr-1];
        edgeListU[2]=tempUpEdge;
        edgeListU[1]=startUEdge[sr-1];
        edgeListU[0]=middleEdge;
        sphere->addFace(edgeListU,4);

        unsigned int edgeListL[4]={0};
        edgeListL[3]=endLEdge[sr-1];
        edgeListL[2]=middleEdge;
        edgeListL[1]=startLEdge[sr-1];
        edgeListL[0]=tempLowEdge;
        sphere->addFace(edgeListL,4);

        edgeListU[2]=endUEdge[0];
        edgeListU[1]=startUEdge[0];
        edgeListU[0]=upEdge;
        sphere->addFace(edgeListU,3);

        edgeListL[2]=endLEdge[0];
        edgeListL[1]=lowEdge;
        edgeListL[0]=startLEdge[0];
        sphere->addFace(edgeListL,3);

        delete startUEdge;
        delete startUVertex;
        delete startLEdge;
        delete startLVertex;

        startUEdge=endUEdge;
        startUVertex=endUVertex;
        startLEdge=endLEdge;
        startLVertex=endLVertex;
    }


        unsigned int middleEdge=sphere->addEdge(endUVertexC[sr],startUVertex[sr]);
        unsigned int upEdge=sphere->addEdge(endUVertexC[1],startUVertex[1]);
        unsigned int lowEdge=sphere->addEdge(endLVertexC[1],startLVertex[1]);
        unsigned int tempUpEdge=upEdge;
        unsigned int tempLowEdge=lowEdge;
        for(unsigned int i=1;i<sr-1;++i)
        {
            unsigned int tempEdgeU=sphere->addEdge(endUVertexC[i+1],startUVertex[i+1]);
            unsigned int edgeListU[4]={0};
            edgeListU[3]=endUEdgeC[i];
            edgeListU[2]=tempUpEdge;
            edgeListU[1]=startUEdge[i];
            edgeListU[0]=tempEdgeU;
            sphere->addFace(edgeListU,4);

            unsigned int tempEdgeL=sphere->addEdge(endLVertexC[i+1],startLVertex[i+1]);
            unsigned int edgeListL[4]={0};
            edgeListL[3]=endLEdgeC[i];
            edgeListL[2]=tempEdgeL;
            edgeListL[1]=startLEdge[i];
            edgeListL[0]=tempLowEdge;
            sphere->addFace(edgeListL,4);

            tempUpEdge=tempEdgeU;
            tempLowEdge=tempEdgeL;
        }

        unsigned int edgeListU[4]={0};
        edgeListU[3]=endUEdgeC[sr-1];
        edgeListU[2]=tempUpEdge;
        edgeListU[1]=startUEdge[sr-1];
        edgeListU[0]=middleEdge;
        sphere->addFace(edgeListU,4);

        unsigned int edgeListL[4]={0};
        edgeListL[3]=endLEdgeC[sr-1];
        edgeListL[2]=middleEdge;
        edgeListL[1]=startLEdge[sr-1];
        edgeListL[0]=tempLowEdge;
        sphere->addFace(edgeListL,4);

        edgeListU[2]=endUEdgeC[0];
        edgeListU[1]=startUEdge[0];
        edgeListU[0]=upEdge;
        sphere->addFace(edgeListU,3);

        edgeListL[2]=endLEdgeC[0];
        edgeListL[1]=lowEdge;
        edgeListL[0]=startLEdge[0];
        sphere->addFace(edgeListL,3);

        delete startUEdge;
        delete startUVertex;
        delete startLEdge;
        delete startLVertex;

        delete endUEdgeC;
        delete endUVertexC;
        delete endLEdgeC;
        delete endLVertexC;
}
