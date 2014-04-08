#ifndef __GAMEOVERSCENE__H
#define __GMAEOVERSCENE__H
#include "cocos2d.h"

using namespace cocos2d;

//class HelloWorld;

class GameOverScene:public cocos2d::CCScene
{
public:
	GameOverScene();

	~GameOverScene(){};

	void newGame();

	void onGoItemCallback(CCObject* pSender);
};

#endif