//
//  Monster.h
//  HomeWork11
//
//  Created by 王晶 on 2018/6/2.
//

#ifndef Monster_h
#define Monster_h

#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class Factory:public cocos2d::Ref {
public:
    //
    static Factory* getInstance();
    //
    Sprite* createMonster();
    //
    void moveMonster(Vec2 playerPos,float time);
    //
    void removeMonster(Sprite*);
    //
    Sprite* collider(Rect rect);
    cocos2d::Vector<Sprite*> attack_collider(Rect rect);
    //
    void initSpriteFrame();
private:
    Factory();
    Vector<Sprite*> monster;
    cocos2d::Vector<SpriteFrame*> monsterDead;
    static Factory* factory;
};

#endif /* Monster_h */
