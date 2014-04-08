#include "GameOverScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

GameOverScene::GameOverScene()
{
	CCLayer* myLayer = CCLayer::create();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCLabelTTF* label = CCLabelTTF::create("Game Over~", "Arial",30);
	label->setPosition(ccp(size.width/2,size.height/2));
	myLayer->addChild(label);
	this->addChild(myLayer);	

	CCMenuItem* goItem=CCMenuItemImage::create("start.png", "start.png",this, 
		menu_selector(GameOverScene::onGoItemCallback));
	
	//goItem->setAnchorPoint(ccp(0.5,1));
	goItem->setPosition(ccp(size.width/2,size.height/2-100));
	//5goItem->setContentSize(CCSizeMake(200,200));
	CCMenu* menu=CCMenu::create(goItem,NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);

	this->addChild(menu, 1000);
};

void GameOverScene::newGame()
{
	CCScene* gameOver = HelloWorld::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, gameOver));
}

void GameOverScene::onGoItemCallback( CCObject* pSender )
{
	this->newGame();
}