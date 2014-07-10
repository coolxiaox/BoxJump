#include "EndLayer.h"

#include "extensions/cocos-ext.h"
#include "spritebuilder/SpriteBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include "global.h"

Scene* EndLayer::createScene()
{
	// 'scene' is an autorelease object
    auto scene = Scene::create();
	
    // 'layer' is an autorelease object
//    auto layer = EndLayer::create();
	auto layer = EndLayer::createFromUI();
	
	// add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
	return scene;
}

Node* EndLayer::createFromUI()
{
	spritebuilder::NodeLoaderLibrary * ccNodeLoaderLibrary = spritebuilder::NodeLoaderLibrary::getInstance();
	ccNodeLoaderLibrary->registerNodeLoader("EndLayer", EndLayerLoader::loader());
	spritebuilder::CCBReader * ccbReader = new spritebuilder::CCBReader(ccNodeLoaderLibrary);
	auto node = ccbReader->readNodeGraphFromFile("EndLayer.ccbi");
	return node;
}

void EndLayer::onNodeLoaded(cocos2d::Node * pNode, spritebuilder::NodeLoader * pNodeLoader) {
	gameTips->setString(StringUtils::format("and died %d times\ncan you do better?", failCount));
}

bool EndLayer::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode) {
	SB_MEMBERVARIABLEASSIGNER_GLUE(this, "gameTips", Label*, gameTips);
	return false;
}

bool EndLayer::onAssignCCBCustomProperty(cocos2d::Ref* target, const char* memberVariableName, const cocos2d::Value& value) {
	return false;
}

cocos2d::SEL_MenuHandler EndLayer::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
	return NULL;
}

cocos2d::SEL_CallFuncN EndLayer::onResolveCCBCCCallFuncSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
	return NULL;
}

cocos2d::extension::Control::Handler EndLayer::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "selector", EndLayer::selector);
	return NULL;
}

void EndLayer::selector(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent) {
	CCLOG("on play %d", rand());
}

// on "init" you need to initialize your instance
//bool EndLayer::init()
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

//void EndLayer::menuCloseCallback(Ref* pSender)
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
