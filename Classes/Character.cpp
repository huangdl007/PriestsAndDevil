#include "Character.h"
#include "cocos2d.h"

using namespace cocos2d;

static bool isTouchOnMe(CCNode* node, CCPoint worldPoint)
{
	CCSize contentSize = node->getContentSize();
	CCPoint localPoint = node->convertToNodeSpace(worldPoint);

	return localPoint.x >= 0 && localPoint.x < contentSize.width
		&& localPoint.y >= 0 && localPoint.y < contentSize.height;
}

bool Character::init(const char* spritePath, int type)
{
	CCNode::init();

	CCSprite* priest = CCSprite::create(spritePath);
	priest->setAnchorPoint(CCPointZero);
	priest->setPosition(CCPointZero);
	this->addChild(priest);

	this->type = type;
	this->index = 0;

	this->setContentSize(priest->getContentSize());
	return true;
}

bool Character::isClickOnMe(CCPoint worldPoint)
{
	return isTouchOnMe(this, worldPoint);
}

Character* Character::create( const char* spritePath,int type )
{
	Character* res=new Character;
	res->init(spritePath,type);
	res->autorelease();

	return res;
}