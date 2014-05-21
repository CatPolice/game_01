#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
    
    /*
     
     A *p则：p->play()使用;     一个左边是指针.
     A p 则：p.paly()使用;     一个左边是实体
     
     */
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    /*
    //创建菜单
    CCMenuItemImage *item = CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", this, menu_selector(HelloWorld::menu_click_btn));
    item->setPosition(ccp(50, 50));
    
    CCMenu *menu = CCMenu::create(item,NULL);
    this->addChild(menu);
    

    //获取场景屏幕的大小
    CCSize sceneSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //创建一个精灵，并且添加到 层 上
    CCSprite *player = CCSprite::create("Icon.png");
    player->setPosition(ccp(0+40, sceneSize.height/2));
    this->addChild(player);
    
    
    //创建一个怪物精灵
    target = CCSprite::create("wtf.jpg");
    target->setPosition(ccp(sceneSize.width - 40, 40));
    this->addChild(target);
    */
    
    
    //获取场景屏幕的大小
    sceneSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //创建一个精灵，并且添加到 层 上
    CCSprite *player = CCSprite::create("Icon.png");
    player->setPosition(ccp(0+40, sceneSize.height/2));
    this->addChild(player);
    
    this->schedule(schedule_selector(HelloWorld::gameLogic),1);
    
    return true;
}


#pragma mark game logic
void HelloWorld::gameLogic(){
    this->createTarget();
}


#pragma mark create target
/**
 *
 * 创建一个怪物精灵类，并且不停的移动
 *
 */
void HelloWorld::createTarget(){
    
    //随机个Y坐标出来
    int y = rand()%(int)(sceneSize.height);
    
    
    CCSprite *targetMove = CCSprite::create("wtf.jpg");
    targetMove->setPosition(ccp(sceneSize.width - 40, y));
    this->addChild(targetMove);
    
    
    //移动怪物
    CCMoveTo *move = CCMoveTo::create(2, ccp(0, y));
    
    //自定义移动函数
    CCCallFuncN *disa = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::myDefine));
    
    CCSequence *actions = CCSequence::create(move,disa,NULL);
    
    targetMove->runAction(actions);
}


#pragma mark btn click ~~
/**
 *
 * 菜单按钮响应事件处理
 *
 */
void HelloWorld::menu_click_btn(CCObject *obj){
//    CCLog("btn click ~~");
    
    //移动怪物
    CCMoveTo *move = CCMoveTo::create(2, ccp(0, 40));

//    target->runAction(move);
    
    CCCallFuncN *disappear = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::myDefine));
    
    //CCSequence 存贮动作相应集合
    CCSequence *actions = CCSequence::create(move,disappear,NULL);
    
    target->runAction(actions);
}


#pragma mark callfunc
/**
 *
 * 自定义btn响应函数事件
 * 移除怪物精灵
 *
 */
void HelloWorld::myDefine(CCNode *who){
    who->setPosition(ccp(10, 10));
    who->setScale(4);
    
    //将当前精灵从图层里擦掉
    who->removeFromParentAndCleanup(true);
}


#pragma mark call back click ~~
/**
 *
 * 退出按钮响应事件处理
 *
 */
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
