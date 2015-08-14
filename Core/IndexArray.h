#pragma once
#include<vector>
#include<deque>

//¼ÇÂ¼ÔªËØÏÂ±êµÄ¿É±ä³¤ÁÐ±í
template<class ElementType> class IndexArray
{
private:
	//´æ´¢ÁÐ±íÖÐÔªËØµÄÁÐ±í
	std::vector<ElementType*> theArray;
	//±£´æÊÍ·Å¿Õ¼äµÄ¶ÓÁÐ
    std::deque<unsigned int> theSpace;

public:
	//¹¹Ôìº¯Êý£¬Ê×ÏÈÏòÁÐ±íÖÐ·ÅÈë¿ÕÊý¾Ý£¬ÒòÎªÔªËØµÄÏÂ±êÈç¹ûÊÇ0±íÊ¾»¹Ã»ÓÐÌí¼Óµ½Êý×é£¬ËùÒÔÕâÀïÊý×éµÄ0ºÅÎ»²»ÄÜÕ¼ÓÃ
	IndexArray(void)
	{
        theArray.push_back(nullptr);
    }

	//´ø³õÊ¼¿Õ¼ä·ÖÅäµÄ¹¹Ôìº¯Êý
    IndexArray(unsigned int space)
	{
        theSpace.clear();
		theArray.reserve(space+1);
        theArray.push_back(nullptr);
    }

	//Çå¿ÕÕû¸öÁÐ±í
	void clear()
	{
		//±éÀúÁÐ±íÄÚµÄËùÓÐÔªËØ
        for(unsigned int i=0;i<theArray.size();++i)
		{
			//¼ì²éµ±Ç°·ÃÎÊµ½µÄÔªËØÊÇ·ñÒÑ¾­±»ÊÍ·Å¹ý
			if(theArray[i])
			{
				//Èç¹ûÃ»ÓÐ±»ÊÍ·Å¹ýÔò½øÐÐÉ¾³ý
				delete theArray[i];
			}
		}
		
		//Çå¿ÕÁÐ±í
		theArray.clear();
		//´ÓÐÂÔÚ0Î»ÌîÉÏ¿ÕÔªËØ
        theArray.push_back(nullptr);
		//Çå¿ÕÒÑ¾­ÊÍ·ÅÔªËØ±àºÅ¶ÓÁÐ
		theSpace.clear();
    }

	//ÎªÁÐ±íÔ¤Áô¿Õ¼ä
    void reserve(unsigned int space)
	{
		theArray.reserve(space);
    }

	//·ÅÈë¿ÕÖµÔªËØ
	void pushNull()
	{
        theArray.push_back(nullptr);
    }

	//ÔÚÁÐ±íµÄÄ©Î²·ÅÈëÒ»¸ö¿ÕµÄÔªËØ²¢ÇÒÔÚÒÑÊÍ·Å¶ÓÁÐÖÐ¼ÇÂ¼
	//Õâ¸öº¯ÊýÓÃÓÚ´ÓÎÄ¼þÖÐ¶ÁÈ¡µÄÊ±ºòÈç¹û³öÏÖÒÑ¾­É¾³ýµÄÔªËØµÄÊ±ºò
	void pushNullS()
	{
		theSpace.push_back(theArray.size());
        theArray.push_back(nullptr);
    }

	//ÒÔ²åÈëµÄ·½Ê½Ìí¼ÓÒ»¸öÔªËØ
	//eiÊÇ²åÈëµÄÎ»ÖÃ
	//Õâ¸öº¯ÊýÓÃÓÚ±£Ö¤¸´ÖÆµÄÒ»ÖÂÐÔ£¬Çø±ðÓÚaddº¯Êý
	//±ÈÈçÏÖÔÚÒªÉ¾³ýÒ»¸öÃæ£¬Ö®ºóundo»Ö¸´£¬Èç¹ûtheSpaceÀïÃæ¼ÇÂ¼ÁËÆäËü¿ÕÏÐµÄÎ»ÖÃ£¬Õâ¸öÃæºÜ¿ÉÄÜ±»»Ö¸´µ½ÆäËüµÄ¿ÕÏÐÎ»ÖÃÉÏÔì³ÉÊý¾Ý²»Ò»ÖÂ
    unsigned int addI(unsigned int ei,ElementType *theElement)
	{
		//Èç¹ûÖ¸¶¨µÄÎ»ÖÃÃ»ÓÐÔªËØ
        if(theArray[ei]==nullptr)
		{
			//ÔÚÒÑ¾­
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
			//Èç¹ûÃ»ÓÐÔÚtheSpaceÖÐÕÒµ½¶ÔÓ¦µÄ¿ÕÏÐÏÂ±ê
			return 0;	
		}
		else
		{
			//Èç¹ûµ±Ç°Î»ÖÃ±»Õ¼ÓÃ
			return 0;
		}
    }

	//Ö±½ÓÔÚÄ©Î²·ÅÈëÔªËØ£¬¶ÔÓÚÒÑ¾­ÓÐÏÂ±êµÄÔªËØ£¬ÇÒ²»Ï£Íû±»²åÈëµ½µ±Ç°¿ÕÏÐÎ»ÖÃµÄÊ±ºò
	void directPush(ElementType *theElement)
	{
		theArray.push_back(theElement);
    }

	//Ìí¼ÓÒ»¸öÐÂµÄÔªËØ
    unsigned int add(ElementType *theElement)
	{
		//¼ì²éµ±Ç°ÊÇ·ñÓÐ¿ÕÏÐÎ»ÖÃ
		if(theSpace.empty())
		{
			//Èç¹ûµ±Ç°Ã»ÓÐ¿ÕÏÐÎ»
			//µÃµ½¼´½«²åÈëÔªËØµÄÎ»ÖÃ(Ä©Î²)
            unsigned int tempIndex=theArray.size();
			//Ä©Î²²åÈëÔªËØ
			theArray.push_back(theElement);
			//¸üÐÂÔªËØµÄ½Å±ê
			theElement->index=tempIndex;
			//·µ»Ø²åÈëµÄÎ»ÖÃ
			return tempIndex;
		}
		else
		{
			//µÃµ½¶ÓÁÐÊ×µÄÒ»¸ö¿ÕÏÐÎ»ÖÃ
            unsigned int tempIndex=theSpace[0];
			//½«Õâ¸ö¿ÕÏÐÎ»ÖÃÏû³ý
			theSpace.pop_front();
			//¸´ÖÆÔªËØµ½¿ÕÏÐÎ»ÖÃ
			theArray[tempIndex]=theElement;
			//¸üÐÂÔªËØµÄ½Å±ê
			theElement->index=tempIndex;
			//·µ»ØÔªËØµÄ½Å±ê
			return tempIndex;
		}
    }

	//É¾³ýÒ»¸öÔªËØ
	//Õâ¸öº¯Êý²¢²»ÊÍ·ÅÔªËØµÄ¿Õ¼ä£¬ÒòÎªÒª±£Ö¤Ìí¼Óµ½ÀúÊ·¼ÇÂ¼ÖÐÔªËØµÄÕýÈ·ÐÔ
    void remove(unsigned int index)
	{
		//Èç¹ûindex²»ÊÇ0
		if(index)
		{
			//Ê×ÏÈ½«Õâ¸öÎ»ÖÃÖÃ¿Õ
            theArray[index]= nullptr;
			//ÔÚ¿ÕÏÐ¶ÓÁÐÀïÌí¼ÓÕâ¸öÎ»ÖÃ
			theSpace.push_back(index);
		}
		return;
    }

	//µÃµ½Ö¸¶¨Î»ÖÃµÄÔªËØ
    ElementType* operator [](unsigned int i)
	{
		return theArray[i];
    }

	//µÃµ½Êý×éµÄ´óÐ¡£¬Õâ¸ö´óÐ¡ÊÇ¸ö½üËÆÖµ£¬ÀïÃæ°üÀ¨ÁËÔø¾­É¾³ýµÄÔªËØÊýÄ¿
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
