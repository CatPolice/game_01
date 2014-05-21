#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    
    cocos2d::CCSprite *target;
    
    cocos2d::CCSize sceneSize;
    
    void myDefine(CCNode *who);
    
    void createTarget();
    
    void gameLogic();
    
    void ccTouchesEnded(cocos2d::CCSet *pTouchs, cocos2d::CCEvent *pEvent);
    
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    //menu click
    void menu_click_btn(CCObject *obj);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
