#pragma once
#include<vector>
#include<deque>

template<class ElementType> class IndexArray
{
private:
    std::vector<ElementType*> theArray;
    std::deque<unsigned int> theSpace;

public:
    IndexArray(void)
    {
        theArray.push_back(nullptr);
    }

    IndexArray(unsigned int space)
    {
        theSpace.clear();
        theArray.reserve(space+1);
        theArray.push_back(nullptr);
    }

    void clear()
    {
        for(unsigned int i=0;i<theArray.size();++i)
        {
            if(theArray[i])
            {
                delete theArray[i];
            }
        }

        theArray.clear();
        theArray.push_back(nullptr);
        theSpace.clear();
    }

    void reserve(unsigned int space)
    {
        theArray.reserve(space);
    }

    void pushNull()
    {
        theArray.push_back(nullptr);
    }

    void pushNullS()
    {
        theSpace.push_back(theArray.size());
        theArray.push_back(nullptr);
    }

    unsigned int addI(unsigned int ei,ElementType *theElement)
    {
        if(theArray[ei]==nullptr)
        {
            for(unsigned int e=0;e<theSpace.size();++e)
            {
                if(theSpace[e]==ei)
                {
                    theSpace[e]=theSpace[0];
                    theSpace.pop_front();
                    theArray[ei]=theElement;
                    return ei;
                }
            }
            return 0;
        }
        else
        {
            return 0;
        }
    }

    void directPush(ElementType *theElement)
    {
        theArray.push_back(theElement);
    }

    unsigned int add(ElementType *theElement)
    {
        if(theSpace.empty())
        {
            unsigned int tempIndex=theArray.size();
            theArray.push_back(theElement);
            theElement->m_index=tempIndex;
            return tempIndex;
        }
        else
        {
            unsigned int tempIndex=theSpace[0];
            theSpace.pop_front();
            theArray[tempIndex]=theElement;
            theElement->m_index=tempIndex;
            return tempIndex;
        }
    }

    void remove(unsigned int index)
    {
        if(index)
        {
            theArray[index]= nullptr;
            theSpace.push_back(index);
        }
        return;
    }

    ElementType* operator [](unsigned int i)
    {
        return theArray[i];
    }

    unsigned int size()
    {
        return theArray.size();
    }

    ~IndexArray(void)
    {
        unsigned int length=theArray.size();
        for(unsigned int i=1;i<length;i++)
        {
            if(theArray[i])
            {
                delete theArray[i];
            }
        }
        theArray.clear();
    }
};
