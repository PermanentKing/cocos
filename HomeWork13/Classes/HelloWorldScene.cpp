//
//  HelloWorldScene.cpp
//  HomeWork11
//
//  Created by 王晶 on 2018/5/29.
//

#include "HelloWorldScene.h"
#include "Monster.h"
#include "SimpleAudioEngine.h"
#define database UserDefault::getInstance()
#pragma execution_character_set("utf-8")
USING_NS_CC;



Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    //´´½¨Ò»ÕÅÌùÍ¼
    auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
    //´ÓÌùÍ¼ÖÐÒÔÏñËØµ¥Î»ÇÐ¸î£¬´´½¨¹Ø¼üÖ¡
    auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
    //Ê¹ÓÃµÚÒ»Ö¡´´½¨¾«Áé
    player = Sprite::createWithSpriteFrame(frame0);
    player->setPosition(Vec2(origin.x + visibleSize.width / 2,
                             origin.y + visibleSize.height/2));
    addChild(player, 3);
    TMXTiledMap* tmx = TMXTiledMap::create("map.tmx");
    //ÉèÖÃÎ»ÖÃ
    tmx->setPosition(visibleSize.width / 2 , visibleSize.height / 2);
    //ÉèÖÃÃªµã
    tmx->setAnchorPoint(Vec2(0.5, 0.5));
    tmx->setScale(Director::getInstance()->getContentScaleFactor());
    //Ìí¼Óµ½ÓÎÏ·Í¼²ãÖÐ£¬ÆäÖÐ0´ú±íZÖá£¨ZÖáµÍµÄ»á±»¸ßµÄÕÚµ²£©
    this->addChild(tmx,0);
    
    TTFConfig ttfConfig;
    ttfConfig.fontFilePath = "fonts/arial.ttf";
    ttfConfig.fontSize = 36;
    
    
    
    
    
    if (database->getIntegerForKey("killNum")) {
        killNum = database->getIntegerForKey("killNum");
    }
    else killNum = 0;
    //
    time = Label::createWithTTF(ttfConfig, "0");
    char a[10];
    sprintf(a, "%d", killNum);
    time->setString(a);
    //
    schedule(schedule_selector(HelloWorld::createMonster), 3.0f, kRepeatForever, 0);
    schedule(schedule_selector(HelloWorld::moveMonster), 2.0f, kRepeatForever, 0);
    schedule(schedule_selector(HelloWorld::hitByMonster), 0.1f, kRepeatForever, 0);
    //µ¹¼ÆÊ±µÄÊý×Ö
    dtime = 180;
    time->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height - time->getContentSize().height));
    addChild(time);
    
    
    mytim = Label::createWithTTF(ttfConfig, "180");
     
     schedule(schedule_selector(HelloWorld::updateTime), 1.0f, kRepeatForever, 0);
     
     mydtime = 180;
     mytim->setPosition(Vec2(origin.x + visibleSize.width / 2,
     origin.y + visibleSize.height - time->getContentSize().height - mytim->getContentSize().height));
     addChild(mytim);
    
    
    
    //hpÌõ
    Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
    Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));
    
    //Ê¹ÓÃhpÌõÉèÖÃprogressBar
    pT = ProgressTimer::create(sp);
    pT->setScaleX(90);
    pT->setAnchorPoint(Vec2(0, 0));
    pT->setType(ProgressTimerType::BAR);
    pT->setBarChangeRate(Point(1, 0));
    pT->setMidpoint(Point(0, 1));
    pT->setPercentage(100);
    pT->setPosition(Vec2(origin.x+14*pT->getContentSize().width,origin.y + visibleSize.height - 2*pT->getContentSize().height));
    addChild(pT,1);
    sp0->setAnchorPoint(Vec2(0, 0));
    sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
    addChild(sp0,0);
    this->en = true;
    this->en1 = false;
    //¾²Ì¬¶¯»­
    idle.reserve(1);
    idle.pushBack(frame0);
    lastCid = 'D';
    this->en = true;
    //¹¥»÷¶¯»­
    attack.reserve(17);
    for (int i = 0; i < 17; i++) {
        auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
        attack.pushBack(frame);
    }
    //animation1 = Animation::createWithSpriteFrames(run, 0.5 / 4);
    //ËÀÍö¶¯»­
    auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
    dead.reserve(22);
    for (int i = 0; i < 22; i++) {
        auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
        dead.pushBack(frame);
    }
    
    //ÔË¶¯¶¯»­
    auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
    run.reserve(4);
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
        run.pushBack(frame);
    }
    Size mapSize = tmx->getMapSize();
    Size tileSize = tmx->getTileSize();
    TMXObjectGroup* objects = tmx->getObjectGroup("wall"); //´Ó¶ÔÏó²ãÖÐ»ñÈ¡¶ÔÏóÊý×é
    //Label
    auto menuLabel1 = Label::createWithTTF(ttfConfig, "W");
    auto menuLabel2 = Label::createWithTTF(ttfConfig, "S");
    auto menuLabel3 = Label::createWithTTF(ttfConfig, "A");
    auto menuLabel4 = Label::createWithTTF(ttfConfig, "D");
    auto menuLabel5 = Label::createWithTTF(ttfConfig, "X");
    auto menuLabel6 = Label::createWithTTF(ttfConfig, "Y");
    //menuItem
    auto item1 = MenuItemLabel::create(menuLabel1, CC_CALLBACK_1(HelloWorld::moveEvent, this,'W'));
    auto item2 = MenuItemLabel::create(menuLabel2, CC_CALLBACK_1(HelloWorld::moveEvent, this,'S'));
    auto item3 = MenuItemLabel::create(menuLabel3, CC_CALLBACK_1(HelloWorld::moveEvent, this,'A'));
    auto item4 = MenuItemLabel::create(menuLabel4, CC_CALLBACK_1(HelloWorld::moveEvent, this,'D'));
    auto item5 = MenuItemLabel::create(menuLabel5, CC_CALLBACK_1(HelloWorld::actionEvent, this, 'X'));
    auto item6 = MenuItemLabel::create(menuLabel6, CC_CALLBACK_1(HelloWorld::actionEvent, this, 'Y'));
    
    item3->setPosition(Vec2(origin.x+item3->getContentSize().width,origin.y+item3->getContentSize().height));
    item4->setPosition(Vec2(item3->getPosition().x + 3 * item4->getContentSize().width, item3->getPosition().y));
    item2->setPosition(Vec2(item3->getPosition().x + 1.5*item2->getContentSize().width, item3->getPosition().y));
    item1->setPosition(Vec2(item2->getPosition().x, item2->getPosition().y + item1->getContentSize().height));
    item5->setPosition(Vec2(origin.x+visibleSize.width-item5->getContentSize().width,item1->getPosition().y));
    item6->setPosition(Vec2(item5->getPosition().x-item6->getContentSize().width,item3->getPosition().y));
    
    auto menu = Menu::create(item1, item2, item3, item4, item6, NULL);
    menu->setPosition(Vec2(0, 0));
    addChild(menu, 1);
    
    return true;
}



void HelloWorld::createMonster(float a) {
    auto fac = Factory::getInstance();
    auto m = fac->createMonster();
    float x = random(origin.x, visibleSize.width);
    float y = random(origin.y, visibleSize.height);
    m->setPosition(x, y);
    addChild(m, 3);
}

void HelloWorld::moveEvent(Ref*,char cid) {
    auto action_W = MoveBy::create(0.5, Point(0, 50));
    auto action_A = MoveBy::create(0.5, Point(-50, 0));
    auto action_S = MoveBy::create(0.5, Point(0, -50));
    auto action_D = MoveBy::create(0.5, Point(50, 0));
    auto animation1 = Animation::createWithSpriteFrames(run, 0.5/4);
    auto run = Animate::create(animation1);
    auto callbackMove = CallFunc::create([&] {
        this->en = true;
    });
    switch (cid) {
        case 'W':
            if (judge_move(Point(0, 40)) && en && pT->getPercentage()) {
                auto seq = Sequence::create(action_W, callbackMove, NULL);
                player->runAction(seq);
                player->runAction(run);
                en = false;
            }
            break;
        case 'A':
            if (judge_move(Point(-40, 0)) && en &&pT->getPercentage()) {
                auto seq = Sequence::create(action_A, callbackMove, NULL);
                player->runAction(seq);
                player->runAction(run);
                en = false;
                if (lastCid == 'D') {
                    player->setFlipX(true);
                }
                lastCid = 'A';
            }
            break;
        case 'S':
            if (judge_move(Point(0, -40)) && en && pT->getPercentage()) {
                auto seq = Sequence::create(action_S, callbackMove, NULL);
                player->runAction(seq);
                player->runAction(run);
                en = false;
            }
            break;
        case 'D':
            if (judge_move(Point(40, 0)) && en && pT->getPercentage()) {
                auto seq = Sequence::create(action_D, callbackMove, NULL);
                player->runAction(seq);
                player->runAction(run);
                en = false;
                if (lastCid == 'A') {
                    player->setFlipX(false);
                }
                lastCid = 'D';
            }
            break;
    }
}

bool HelloWorld::judge_move(Point b) {
    Point p = player->getPosition() + b;
    if (p.y >= this->visibleSize.height || p.x >= this->visibleSize.width || p.y <= 0 || p.x <= this->origin.x) {
        return false;
    }
    return true;
}

void HelloWorld::set(Ref *) {
    this->en = true;
}

void HelloWorld::actionEvent(Ref*, char cid) {
    int i;
    auto animation1 = Animation::createWithSpriteFrames(dead, 0.1f);
    //人物死后，不能站起
    animation1->setRestoreOriginalFrame(false);
    auto Dead = Animate::create(animation1);
    auto callbackMove = CallFunc::create([&] {
        this->en = true;
    });
    switch (cid) {
        case 'X':
            if (pT->getPercentage()==0) {
                break;
            }
            player->stopAllActions();
            this->player->runAction(Sequence::create(Dead, callbackMove, NULL));
            this->en = false;
            i = pT->getPercentage();
            pT->setPercentage(i - 20);
            break;
        case 'Y':
            if (pT->getPercentage()==0) {
                break;
            }
            auto animation = Animation::createWithSpriteFrames(attack, 0.1f);
            animation->setRestoreOriginalFrame(true);
            myattack = Animate::create(animation);
            if (this->en) {
                int num = hitMonster();
                if (num) {
                    i = pT->getPercentage();
                    pT->setPercentage(i + 20 * num);
                    killNum += num;
                    char a[10];
                    sprintf(a, "%d", killNum);
                    time->setString(a);
                    database->setIntegerForKey("killNum", killNum);
                }
                this->player->runAction(Sequence::create(this->myattack,callbackMove, NULL));
                this->en = false;
            }
            break;
    }
}

void HelloWorld::stopAc() {
    
}

void HelloWorld::updateTime(float dt) {
    int i = atoi(mytim->getString().c_str());
    char a[10];
    sprintf(a, "%d", i-1);
    mytim->setString(a);
}

void HelloWorld::hitByMonster(float dt) {
    auto fac = Factory::getInstance();
    Sprite* collision = fac->collider(player->getBoundingBox());
    if (collision != NULL) {
        fac->removeMonster(collision);
        this->en1 = true;
        if (pT->getPercentage()!=20) {
            int i = pT->getPercentage();
            pT->setPercentage(i-20);
        }else{
            actionEvent(this, 'X');
            unscheduleAllSelectors();
        }
    }
}


int HelloWorld::hitMonster() {
    auto fac = Factory::getInstance();
    Rect playerRect = player->getBoundingBox();
    Rect attackRect = Rect(playerRect.getMinX() - 40, playerRect.getMinY(), playerRect.getMaxX() - playerRect.getMinX() + 80, playerRect.getMaxY() - playerRect.getMinY());
    Vector<Sprite *> collision = fac->attack_collider(attackRect);
    if (!collision.empty()) {
        for (auto s : collision) {
            fac->removeMonster(s);
        }
        return collision.size();
    }
    return 0;
}

void HelloWorld::moveMonster(float dt) {
    auto fac = Factory::getInstance();
    fac->moveMonster(player->getPosition(), 1);
}
