#include "PlayLayer.h"

#include "extensions/cocos-ext.h"
#include "spritebuilder/SpriteBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include "global.h"
#include "EndLayer.h"

#define PLAYER_CATEGORY 1

int map[20][22] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 5, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   
   {0, 0, 5, 5, 5, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0},
   {0, 0, 0, 0, 2, 0, 0, 0, 5, 0, 0, 0, 0, 2, 0, 0, 0, 5, 0, 0, 0, 3},
   {0, 0, 0, 0, 2, 3, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 2, 2, 0, 0},
   {0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 0},
   {0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2},
   
   {0, 0, 5, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 2},
   {0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 5, 0, 0, 0, 2, 2, 2, 0, 0, 5},
   {0, 0, 0, 0, 2, 3, 2, 0, 0, 0, 5, 5, 0, 0, 0, 2, 3, 2, 0, 0, 0, 0},
   {0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 0, 2},
   {0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0},
   
   {0, 0, 0, 0, 4, 0, 0, 0, 0, 5, 0, 0, 0, 3, 3, 0, 0, 0, 0, 5, 0, 0},
   {0, 0, 0, 0, 0, 5, 0, 0, 2, 2, 0, 0, 0, 0, 5, 5, 0, 0, 0, 4, 0, 0},
   {0, 0, 0, 0, 1, 4, 1, 0, 0, 0, 0, 1, 4, 1, 0, 0, 0, 0, 0, 1, 4, 1},
	{0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 2, 0, 0}};

Scene* PlayLayer::createScene()
{
	// 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
//	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
    // 'layer' is an autorelease object
//    auto layer = PlayLayer::create();
	auto layer = PlayLayer::createFromUI();
	
	// add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
	return scene;
}

Node* PlayLayer::createFromUI()
{
	spritebuilder::NodeLoaderLibrary * ccNodeLoaderLibrary = spritebuilder::NodeLoaderLibrary::getInstance();
	ccNodeLoaderLibrary->registerNodeLoader("PlayLayer", PlayLayerLoader::loader());
	spritebuilder::CCBReader * ccbReader = new spritebuilder::CCBReader(ccNodeLoaderLibrary);
	auto node = ccbReader->readNodeGraphFromFile("PlayLayer.ccbi");
	return node;
}

void PlayLayer::onNodeLoaded(cocos2d::Node * pNode, spritebuilder::NodeLoader * pNodeLoader) {
	visibleSize = Director::getInstance()->getVisibleSize();
	
	failCount = 0;
	curLevel = 0;
	totalLevel = 20;
	
	isStart = false;
	
	origPos = player->getPosition();
	
	setFailLabelString();
	loadLevelCube();
	
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(PlayLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(PlayLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayLayer::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	
	schedule(schedule_selector(PlayLayer::play));
}

bool PlayLayer::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
	SB_MEMBERVARIABLEASSIGNER_GLUE(this, "player", Sprite*, player);
	SB_MEMBERVARIABLEASSIGNER_GLUE(this, "line", Sprite*, line);
	SB_MEMBERVARIABLEASSIGNER_GLUE(this, "failLabel", Label*, failLabel);
	SB_MEMBERVARIABLEASSIGNER_GLUE(this, "levelLabel", Label*, levelLabel);
	SB_MEMBERVARIABLEASSIGNER_GLUE(this, "tipsLabel", Label*, tipsLabel);
	SB_MEMBERVARIABLEASSIGNER_GLUE(this, "cubes", Node*, cubes);
	return false;
}

bool PlayLayer::onAssignCCBCustomProperty(cocos2d::Ref* target, const char* memberVariableName, const cocos2d::Value& value) {
	return false;
}

cocos2d::SEL_MenuHandler PlayLayer::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
	return NULL;
}

cocos2d::SEL_CallFuncN PlayLayer::onResolveCCBCCCallFuncSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
	return NULL;
}

cocos2d::extension::Control::Handler PlayLayer::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "selector", PlayLayer::selector);
	return NULL;
}

void PlayLayer::selector(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent) {
	CCLOG("on play %d", rand());
}

void PlayLayer::setPlayerRunVelocity()
{
	player->stopAllActions();
	
	player->getPhysicsBody()->setVelocity(Point(140, 0));
}

void PlayLayer::setPlayerJumpVelocity()
{
	if (jumping > 0)
	{
		return;
	}
		
	jumping++;
	
	Point origPos = player->getPosition();
	
	Point jumpPos = Point(40, 0);
	JumpBy *jumpBy = JumpBy::create(0.5, jumpPos, 42, 1);
	player->runAction(Sequence::create(jumpBy, CallFunc::create(CC_CALLBACK_0(PlayLayer::setPlayerNoJumping, this)), NULL));
}

void PlayLayer::setPlayerNoJumping()
{
	jumping = 0;
}

void PlayLayer::setFailLabelString()
{
	failLabel->setString(StringUtils::format("%d", failCount));
}

void PlayLayer::setLevelLabelString()
{
	levelLabel->setString(StringUtils::format("%d/%d", curLevel, totalLevel));
}

void PlayLayer::play(float dt)
{
	if (!isStart)
	{
		return;
	}
	
	if (player->getPositionX() > (line->getPositionX() + line->getContentSize().width/2))
	{
		loadLevelCube();
	}
}

void PlayLayer::loadLevelCube()
{
	setPlayerNoJumping();
	player->stopAllActions();
	
	curLevel++;

	if (curLevel > totalLevel)
	{
		auto scene = EndLayer::createScene();
		Director::getInstance()->replaceScene(scene);
	}
	
	player->getPhysicsBody()->setVelocity(Point(0, 0));
	
	cubes->removeAllChildrenWithCleanup(true);
	
	setLevelLabelString();
	
	spritebuilder::NodeLoaderLibrary * ccNodeLoaderLibrary = spritebuilder::NodeLoaderLibrary::getInstance();
	spritebuilder::CCBReader * ccbReader = new spritebuilder::CCBReader(ccNodeLoaderLibrary);
	
	for (int i = 0; i < 22; i++) {
		auto cube = ccbReader->readNodeGraphFromFile("Cube.ccbi");
		
		int type = map[curLevel-1][i];
		
		float posX = 100 + i * cube->getContentSize().width;
		float posY = 0;
		
		float height;
		
		if (type == 1) {
			height = 0.3;
		}
		else if (type == 2) {
			height = 1;
		}
		else if (type == 3) {
			height = 1.5;
		}
		else if (type == 4) {
			height = 1.8;
		}
		else if (type == 5) {
			posY += 22;
			height = 0.5;
		}
		
		if (type != 0) {
			PhysicsBody *cubeBody = PhysicsBody::createBox(Size(cube->getContentSize().width, cube->getContentSize().height*height));
			cubeBody->setDynamic(false);
			cubeBody->setCategoryBitmask(2);
			cubeBody->setContactTestBitmask(1);
			cubeBody->setCollisionBitmask(1);
			
			posY += line->getPositionY() + (cube->getContentSize().height * height)/2;
			cube->setPosition(posX, posY);
			cube->setScaleY(height);
			cube->setPhysicsBody(cubeBody);
			
			cubes->addChild(cube);
		}
	}
	
	jumping = 8; // set max value to avoid jump action when it start
	player->setPosition(origPos);
	MoveTo *moveTo = MoveTo::create(0.8, Point(origPos.x, line->getPositionY() + player->getContentSize().height/2));
	if (!isStart)
	{
		player->runAction(Sequence::create(moveTo, CallFunc::create(CC_CALLBACK_0(PlayLayer::setPlayerNoJumping, this)), NULL));
	}
	else
	{
		player->runAction(Sequence::create(moveTo, CallFunc::create(CC_CALLBACK_0(PlayLayer::setPlayerNoJumping, this)), CallFunc::create(CC_CALLBACK_0(PlayLayer::setPlayerRunVelocity, this)), NULL));
	}
}

// on "init" you need to initialize your instance
//bool PlayLayer::init()
//{
//    //////////////////////////////
//    // 1. super init first
//    if ( !Layer::init() )
//    {
//        return false;
//    }
//
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Point origin = Director::getInstance()->getVisibleOrigin();
//
//    return true;
//}

//void PlayLayer::menuCloseCallback(Ref* pSender)
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
//	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//    return;
//#endif
//
//    Director::getInstance()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
//}

bool PlayLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	if ((isStart == 0) && (jumping == 0))
	{
		tipsLabel->setVisible(false);
		
		isStart = true;
		
		setPlayerRunVelocity();
	}
	
	setPlayerJumpVelocity();

	return true;
}

void PlayLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
}

void PlayLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
}

bool PlayLayer::onContactBegin(const cocos2d::PhysicsContact &contact)
{
//	auto shapea = contact.getShapeA();
//	auto shapeb = contact.getShapeB();
	
	player->stopAllActions();
	
	failCount++;
	setFailLabelString();
	
	player->setPosition(Point(origPos.x, line->getPositionY() + player->getContentSize().height/2));
	setPlayerNoJumping();
	
	return false;
}
