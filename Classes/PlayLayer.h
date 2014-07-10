#ifndef __PlayLayer_H__
#define __PlayLayer_H__

#include "cocos2d.h"

#include "extensions/cocos-ext.h"
#include "spritebuilder/SpriteBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PlayLayer : public cocos2d::Layer
, public spritebuilder::CCBSelectorResolver
, public spritebuilder::CCBMemberVariableAssigner
, public spritebuilder::NodeLoaderListener
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
	
	static cocos2d::Node* createFromUI();
	
	virtual void onNodeLoaded(cocos2d::Node * pNode, spritebuilder::NodeLoader * pNodeLoader);
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, cocos2d::Node* pNode);
    virtual bool onAssignCCBCustomProperty(cocos2d::Ref* target, const char* memberVariableName, const cocos2d::Value& value);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName);
	
	void selector(cocos2d::Ref * sender, cocos2d::extension::Control::EventType pControlEvent);
	
//  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
//  virtual bool init();
//
//  // a selector callback
//  void menuCloseCallback(cocos2d::Ref* pSender);
	
	// implement the "static create()" method manually
    CREATE_FUNC(PlayLayer);
	
private:
	void play(float dt);
	
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	
	bool onContactBegin(const PhysicsContact& contact);
	
	void setPlayerRunVelocity();
	void setPlayerJumpVelocity();
	
	void setPlayerNoJumping();
	
	void setFailLabelString();
	void setLevelLabelString();
	
	void loadLevelCube();
	
	Sprite *player, *line;
	Label *failLabel, *tipsLabel, *levelLabel;
	Node *cubes;
	
	int curLevel, totalLevel, jumping;
	
	Point origPos;
	
	bool isStart;
	
	Size visibleSize;
};

CREATE_SPRITEBUILDER_LOADER_CLASS(PlayLayerLoader, PlayLayer);

#endif // __PlayLayer_H__
