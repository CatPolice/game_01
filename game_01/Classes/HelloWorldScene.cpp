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
    
    //cocos2dx中 Class::create() 这种工厂方法是不需要手动释放内存的，在coco2d底层中，自动加入到自动释放池中了。
    // new 这种形式的，需要手动释放掉内存。一般放到析构函数中释放掉所占的内存
    
    
    //初始化精灵数组
    _targets = new CCArray;
    _projs = new CCArray;
    
    //获取场景屏幕的大小
    sceneSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //创建一个精灵，并且添加到 层 上
    CCSprite *player = CCSprite::create("Icon.png");
    player->setPosition(ccp(0+40, sceneSize.height/2));
    this->addChild(player);
    
    this->schedule(schedule_selector(HelloWorld::gameLogic),2);//第一个参数响应方法第二个时间间隔
//    this->schedule(schedule_selector(HelloWorld::createTarget));//返回参数不一致最好用方法过度下
    
    this->schedule(schedule_selector(HelloWorld::updateArr), 0.5);
    
    
    this->setTouchEnabled(true);//打开layer的点击响应事件
    
    
    return true;
}

#pragma mark touch click
void HelloWorld::ccTouchesEnded(CCSet *pTouchs, CCEvent *pEvent){
    CCTouch *touch = (CCTouch *)pTouchs->anyObject();
    CCPoint localInView = touch->getLocationInView();//获取到的是视图view的坐标
    
    CCPoint loc = CCDirector::sharedDirector()->convertToGL(localInView);//视图view坐标和我们GL的坐标 不是同一个起点，需要转换下
    
    //如果飞镖过来了，就不做处理
    if (loc.x < 20) {
        return;
    }
    
    //创建个飞镖
    CCSprite *proj = CCSprite::create("Icon-Small.png");
    proj->setPosition(ccp(20, sceneSize.height/2.0));
    this->addChild(proj);
    
    _projs->addObject(proj);
    proj->setTag(1);
    
    
    //-------------------------------------------------------------------------------------------------------------------
    //两点一线，根据相似三角形，计算出飞镖飞出屏幕外面的点的坐标值，要保证飞镖飞出屏幕的距离都一致，才能保证速度相同
    double dx = loc.x - 20;
    double dy = loc.y - sceneSize.height/2.0;
    double d = sqrt(dx*dx + dy*dy);
    
    double D = sqrt(sceneSize.width*sceneSize.width + sceneSize.height*sceneSize.height);
    
    double ratio = D/d;
    
    double endx = ratio * dx + 20;
    double endy = ratio * dy + sceneSize.height / 2.0;
    //-------------------------------------------------------------------------------------------------------------------
    
    //移动方法
    CCMoveTo *move = CCMoveTo::create(D/320, ccp(endx, endy));
    CCCallFuncN *moveFinish = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::myDefine));
    CCSequence *actions = CCSequence::create(move,moveFinish,NULL);
    proj->runAction(actions);
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
    
    //创建个怪物出来
    CCSprite *targetMove = CCSprite::create("wtf.jpg");
    targetMove->setPosition(ccp(sceneSize.width - 40, y));
    this->addChild(targetMove);
    
    _targets->addObject(targetMove);
    targetMove->setTag(2);
    
    
    //移动怪物
    CCMoveTo *move = CCMoveTo::create(2, ccp(0, y));
    
    //自定义移动函数
    CCCallFuncN *disa = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::myDefine));
    
    CCSequence *actions = CCSequence::create(move,disa,NULL);
    
    targetMove->runAction(actions);
}


#pragma mark callfunc
/**
 *
 * 自定义btn响应函数事件
 * 移除怪物精灵
 *
 */
void HelloWorld::myDefine(CCNode *who){
//    who->setPosition(ccp(10, 10));
//    who->setScale(4);
    
    //将当前精灵从图层里擦掉
    who->removeFromParentAndCleanup(true);
    
    
    //根据设置的tag去判断删除哪个arr里面元素
    switch (who->getTag()) {
        case 1:
            _projs->removeObject(who);
            break;
        case 2:
            _targets->removeObject(who);
            break;
        default:
            break;
    }
}

#pragma mark update
void HelloWorld::updateArr(float dalta){ //dalta = 1.0 / fps
    
    CCObject *i_targets;
    CCObject *i_projs;
    
    CCArray *targetToDelete = new CCArray;//存放要删除的元素数组
    CCArray *projsToDelete = new CCArray;
    
    //2dx 提供了遍历数组的方法
    //CCARRAY_FOREACH 遍历出来的为object，得需要强转下
    CCARRAY_FOREACH(_targets, i_targets){
        CCSprite *target = (CCSprite*)i_targets;
        
        CCRect targetZone = CCRectMake(target->getPositionX(), target->getPositionY(), target->getContentSize().width, target->getContentSize().height);
        
        
        CCARRAY_FOREACH(_projs, i_projs){
            CCSprite *proj = (CCSprite *)i_projs;
            CCRect projZone = CCRectMake(proj->getPositionX(), proj->getPositionY(), proj->getContentSize().width, proj->getContentSize().height);
            
            if (projZone.intersectsRect(targetZone)) {
                projsToDelete->addObject(i_projs);
                targetToDelete->addObject(i_targets);
            }
        }
    }
    
    
    CCARRAY_FOREACH(projsToDelete, i_projs){
        _projs->removeObject(i_projs);
        
        CCSprite *proj = (CCSprite *)i_projs;
        proj->removeFromParentAndCleanup(true);
    }
    
    CCARRAY_FOREACH(targetToDelete, i_targets){
        _targets->removeObject(i_targets);
        
        CCSprite *targ = (CCSprite *)i_targets;
        targ->removeFromParentAndCleanup(true);
    }
    
    
}

#pragma mark 析构函数
HelloWorld::~HelloWorld(){
    if (_projs != NULL) {
        _projs->release();
    }
    
    if (_targets != NULL) {
        _targets->release();
    }
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
