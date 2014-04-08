#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h"
#include "Character.h"

using namespace cocos2d;

class Character : public CCNode
{
protected:
	int index;
	int type;

	virtual bool init(const char* spritePath,int type);

public:
	Character(void){};
	~Character(void){};
	static Character* create(const char* spritePath,int type);

	int getIndex(){ return index; }
	void setIndex(int num){ index = num; }
	int getType(){return type;}
	bool isClickOnMe(CCPoint worldPoint);
};

#endif