#ifndef __HELLOWORLDSCENE_H__
#define __HELLOWORLDSCENE_H__

#include "cocos2d.h"
#include "Boats.h"
#include "Character.h"
#include "Hint.h"
#include "string"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer
{
public:
	static bool isFirstLoad;
	CCArray* lCharacter;	//array of characters on left land
	CCArray* rCharacter;	//array of characters on right land
	CCSprite* l_sprite;		//the left sprite on boat
	CCSprite* r_sprite;		//the right sprite on boat
	Boats* boat;
	CCPoint l_pos;			//the moving range of the boat
	CCPoint r_pos;
	CCLabelAtlas* labelatlas;
	int currentTime;
	CCLabelTTF *pHint;

	HelloWorld(void);
	~HelloWorld(void);
    virtual bool init();  
	static cocos2d::CCScene* scene();
	CREATE_FUNC(HelloWorld);

	void sizeOfLC(CCArray* priestArr, CCArray* DevilArr);
	void sizeOfRC(CCArray* priestAr, CCArray* devilArr);
	void JumpOut(Character* character);
	void onGoItemCallback(CCObject* pSender);
	void menuHintCallback(CCObject* pSender);
	void onBoatReachLand();
	void collectLeftLandMan(CCArray* priestArr,CCArray* devilArr);
	void collectRightLandMan(CCArray* priestArr,CCArray* devilArr);
	bool checkLeftLandIsOK();
	bool checkRightLandIsOK();
	void doGameEnd(const char* info);
	void updateTimer(float dt);
	void checkTime();
	bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	//void ccTouchEnded(CCTouch* touch, CCEvent* event);
};

#endif
