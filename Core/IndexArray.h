#pragma once
#include<vector>
#include<deque>

template<class ElementType> class IndexArray
{
private:
    std::vector<ElementType*> m_array;
    std::deque<unsigned int> m_space;

public:
    IndexArray(void)
    {
        m_array.push_back(nullptr);
    }

    IndexArray(unsigned int space)
    {
        m_space.clear();
        m_array.reserve(space+1);
        m_array.push_back(nullptr);
    }

    void clear()
    {
        for(unsigned int i=0;i<m_array.size();++i)
        {
            if(m_array[i])
            {
                delete m_array[i];
            }
        }

        m_array.clear();
        m_array.push_back(nullptr);
        m_space.clear();
    }

    void reserve(unsigned int space)
    {
        m_array.reserve(space);
    }

    void pushNull()
    {
        m_array.push_back(nullptr);
    }

    void pushNullS()
    {
        m_space.push_back(m_array.size());
        m_array.push_back(nullptr);
    }

    unsigned int addI(unsigned int ei,ElementType *element)
    {
        if(m_array[ei]==nullptr)
        {
            for(unsigned int e=0;e<m_space.size();++e)
            {
                if(m_space[e]==ei)
                {
                    m_space[e]=m_space[0];
                    m_space.pop_front();
                    m_array[ei]=element;
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

    void directPush(ElementType *element)
    {
        m_array.push_back(element);
    }

    unsigned int add(ElementType *element)
    {
        if(m_space.empty())
        {
            unsigned int tempIndex=m_array.size();
            m_array.push_back(element);
            element->m_index=tempIndex;
            return tempIndex;
        }
        else
        {
            unsigned int tempIndex=m_space[0];
            m_space.pop_front();
            m_array[tempIndex]=element;
            element->m_index=tempIndex;
            return tempIndex;
        }
    }

    void remove(unsigned int index)
    {
        if(index)
        {
            m_array[index]= nullptr;
            m_space.push_back(index);
        }
        return;
    }

    ElementType* operator [](unsigned int i)
    {
        return m_array[i];
    }

    unsigned int size()
    {
        return m_array.size();
    }

    ~IndexArray(void)
    {
        unsigned int length=m_array.size();
        for(unsigned int i=1;i<length;i++)
        {
            if(m_array[i])
            {
                delete m_array[i];
            }
        }
        m_array.clear();
    }
};
