//
//  HelloWorldScene.cpp
//  HomeWork11
//
//  Created by 王晶 on 2018/5/29.
//

#include "HelloWorldScene.h"
//#include "Monster.h"
//#include "cocostudio/CocoStudio.h"
//#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#pragma execution_character_set("utf-8")

USING_NS_CC;

//using namespace cocostudio::timeline;

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
    
    
    TTFConfig ttfConfig;
    ttfConfig.fontFilePath = "fonts/arial.ttf";
    ttfConfig.fontSize = 36;
    
    //时间
    time = Label::createWithTTF(ttfConfig, "180");
    //倒计时周期性调用调度器
    schedule(schedule_selector(HelloWorld::updateTime), 1.0f, kRepeatForever, 0);
    //倒计时数字
    dtime = 180;
    time->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height - time->getContentSize().height));
    addChild(time);
    
    //hp条
    Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
    Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));
    
    //使用hp条设置progressBar
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
    //静态动画
    idle.reserve(1);
    idle.pushBack(frame0);
    
    //攻击动画
    attack.reserve(17);
    for (int i = 0; i < 17; i++) {
        auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
        attack.pushBack(frame);
    }
    //animation1 = Animation::createWithSpriteFrames(run, 0.5 / 4);
    //死亡动画
    auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
    dead.reserve(22);
    for (int i = 0; i < 22; i++) {
        auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
        dead.pushBack(frame);
    }
    
    //运动动画
    auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
    run.reserve(4);
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
        run.pushBack(frame);
    }
    
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
    
    auto menu = Menu::create(item1, item2, item3, item4, item5, item6, NULL);
    menu->setPosition(Vec2(0, 0));
    addChild(menu, 1);
    
    return true;
}


void HelloWorld::moveEvent(Ref*,char cid) {
    auto action_W = MoveBy::create(0.5, Point(0, 50));
    auto action_A = MoveBy::create(0.5, Point(-50, 0));
    auto action_S = MoveBy::create(0.5, Point(0, -50));
    auto action_D = MoveBy::create(0.5, Point(50, 0));
    auto animation1 = Animation::createWithSpriteFrames(run, 0.5/4);
    //animation1->setRestoreOriginalFrame(true);
    auto run = Animate::create(animation1);
    auto callbackMove = CallFunc::create([&] {
        this->en = true;
    });
    switch (cid) {
        case 'W':
            if (judge_move(Point(0, 40)) && en) {
                auto seq = Sequence::create(action_W, callbackMove, NULL);
                player->runAction(seq);
                player->runAction(run);
                en = false;
            }
            break;
        case 'A':
            if (judge_move(Point(-50, 0)) && en) {
                auto seq = Sequence::create(action_A, callbackMove, NULL);
                player->runAction(seq);
                player->runAction(run);
                en = false;
            }
            break;
        case 'S':
            if (judge_move(Point(0, -50)) && en) {
                auto seq = Sequence::create(action_S, callbackMove, NULL);
                player->runAction(seq);
                player->runAction(run);
                en = false;
            }
            break;
        case 'D':
            if (judge_move(Point(50, 0)) && en) {
                auto seq = Sequence::create(action_D, callbackMove, NULL);
                player->runAction(seq);
                player->runAction(run);
                en = false;
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
    animation1->setRestoreOriginalFrame(true);
    auto Dead = Animate::create(animation1);
    auto callbackMove = CallFunc::create([&] {
        this->en = true;
    });
    switch (cid) {
        case 'X':
            if (this->en) {
                this->player->runAction(Sequence::create(Dead, callbackMove, NULL));
                this->en = false;
                i = pT->getPercentage();
                pT->setPercentage(i - 20);
            }
            break;
        case 'Y':
            auto animation = Animation::createWithSpriteFrames(attack, 0.1f);
            animation->setRestoreOriginalFrame(true);
            myattack = Animate::create(animation);
            if (this->en) {
                this->player->runAction(Sequence::create(this->myattack,callbackMove, NULL));
                this->en = false;
                i = pT->getPercentage();
                pT->setPercentage(i + 20);
            }
            break;
    }
}

void HelloWorld::stopAc() {
    
}

void HelloWorld::updateTime(float dt) {
    dtime = dtime - 1 >= 0 ? dtime-1 : 0;
    char str[5] = { '0' };
    sprintf(str, "%d", dtime);
    time->setString(str);
}
