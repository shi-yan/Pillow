#pragma once
#include<vector>
#include<deque>

//记录元素下标的可变长列表
template<class ElementType> class IndexArray
{
private:
	//存储列表中元素的列表
	std::vector<ElementType*> theArray;
	//保存释放空间的队列
    std::deque<unsigned int> theSpace;

public:
	//构造函数，首先向列表中放入空数据，因为元素的下标如果是0表示还没有添加到数组，所以这里数组的0号位不能占用
	IndexArray(void)
	{
        theArray.push_back(nullptr);
	};

	//带初始空间分配的构造函数
    IndexArray(unsigned int space)
	{
		theArray.reserve(space+1);
        theArray.push_back(nullptr);
	};

	//清空整个列表
	void clear()
	{
		//遍历列表内的所有元素
        for(unsigned int i=0;i<theArray.size();++i)
		{
			//检查当前访问到的元素是否已经被释放过
			if(theArray[i])
			{
				//如果没有被释放过则进行删除
				delete theArray[i];
			}
		}
		
		//清空列表
		theArray.clear();
		//从新在0位填上空元素
        theArray.push_back(nullptr);
		//清空已经释放元素编号队列
		theSpace.clear();
	};

	//为列表预留空间
    void reserve(unsigned int space)
	{
		theArray.reserve(space);
	};

	//放入空值元素
	void pushNull()
	{
        theArray.push_back(nullptr);
	};

	//在列表的末尾放入一个空的元素并且在已释放队列中记录
	//这个函数用于从文件中读取的时候如果出现已经删除的元素的时候
	void pushNullS()
	{
		theSpace.push_back(theArray.size());
        theArray.push_back(nullptr);
	};

	//以插入的方式添加一个元素
	//ei是插入的位置
	//这个函数用于保证复制的一致性，区别于add函数
	//比如现在要删除一个面，之后undo恢复，如果theSpace里面记录了其它空闲的位置，这个面很可能被恢复到其它的空闲位置上造成数据不一致
    unsigned int addI(unsigned int ei,ElementType *theElement)
	{
		//如果指定的位置没有元素
        if(theArray[ei]==nullptr)
		{
			//在已经
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
			//如果没有在theSpace中找到对应的空闲下标
			return 0;	
		}
		else
		{
			//如果当前位置被占用
			return 0;
		}
	};	

	//直接在末尾放入元素，对于已经有下标的元素，且不希望被插入到当前空闲位置的时候
	void directPush(ElementType *theElement)
	{
		theArray.push_back(theElement);
	};

	//添加一个新的元素
    unsigned int add(ElementType *theElement)
	{
		//检查当前是否有空闲位置
		if(theSpace.empty())
		{
			//如果当前没有空闲位
			//得到即将插入元素的位置(末尾)
            unsigned int tempIndex=theArray.size();
			//末尾插入元素
			theArray.push_back(theElement);
			//更新元素的脚标
			theElement->index=tempIndex;
			//返回插入的位置
			return tempIndex;
		}
		else
		{
			//得到队列首的一个空闲位置
            unsigned int tempIndex=theSpace[0];
			//将这个空闲位置消除
			theSpace.pop_front();
			//复制元素到空闲位置
			theArray[tempIndex]=theElement;
			//更新元素的脚标
			theElement->index=tempIndex;
			//返回元素的脚标
			return tempIndex;
		}
	};

	//删除一个元素
	//这个函数并不释放元素的空间，因为要保证添加到历史记录中元素的正确性
    void remove(unsigned int index)
	{
		//如果index不是0
		if(index)
		{
			//首先将这个位置置空
            theArray[index]= nullptr;
			//在空闲队列里添加这个位置
			theSpace.push_back(index);
		}
		return;
	};

	//得到指定位置的元素
    ElementType* operator [](unsigned int i)
	{
		return theArray[i];
	};

	//得到数组的大小，这个大小是个近似值，里面包括了曾经删除的元素数目
    unsigned int size()
	{
		return theArray.size();
	};

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
	};
};
