//
//  HelloWorldScene.h
//  HomeWork11
//
//  Created by 王晶 on 2018/5/29.
//

#ifndef HelloWorldScene_h
#define HelloWorldScene_h

#include "cocos2d.h"
using namespace cocos2d;
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    
    void moveEvent(Ref*, char);
    bool judge_move(Point b);
    void set(Ref *);
    void actionEvent(Ref*, char);
    void stopAc();
    void updateTime(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    //void update(float dt) override;
private:
    cocos2d::Sprite* player;
    cocos2d::Vector<SpriteFrame*> attack;
    cocos2d::Vector<SpriteFrame*> dead;
    cocos2d::Vector<SpriteFrame*> run;
    cocos2d::Vector<SpriteFrame*> idle;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Label* time;
    bool en;
    int dtime;
    cocos2d::ProgressTimer* pT;
    Animate* myattack;
    Animate* move;
    Animate* mydead;
};

#endif /* HelloWorldScene_h */
