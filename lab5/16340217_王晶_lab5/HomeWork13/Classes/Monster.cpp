//
//  Monster.cpp
//  HomeWork11
//
//  Created by 王晶 on 2018/6/2.
//

#include <stdio.h>
#include"Monster.h"
USING_NS_CC;

Factory* Factory::factory = NULL;
Factory::Factory() {
    initSpriteFrame();
}
Factory* Factory::getInstance() {
    if (factory == NULL) {
        factory = new Factory();
    }
    return factory;
}
void Factory::initSpriteFrame(){
    auto texture = Director::getInstance()->getTextureCache()->addImage("Monster.png");
    monsterDead.reserve(4);
    for (int i = 0; i < 4; i++) {
        auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(258-48*i,0,42,42)));
        monsterDead.pushBack(frame);
    }
}

Sprite* Factory::createMonster() {
    Sprite* mons = Sprite::create("Monster.png", CC_RECT_PIXELS_TO_POINTS(Rect(364,0,42,42)));
    monster.pushBack(mons);
    return mons;
}

void Factory::removeMonster(Sprite* sp) {
    Animation * anim = Animation::createWithSpriteFrames(monsterDead, 0.1f);
    auto ani = Animate::create(anim);
    Sequence*seq = Sequence::create(ani, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp)), NULL);
    sp->runAction(seq);
    monster.eraseObject(sp);
}
void Factory::moveMonster(Vec2 playerPos,float time){
    auto it = monster.begin();
    for (; it != monster.end(); ) {
        Vec2 monPos = (*it)->getPosition();
        Vec2 direction = playerPos - monPos;
        direction.normalize();
        (*it)->runAction(MoveBy::create(time, direction * 30));
        it++;
    }
}

Sprite* Factory::collider(Rect rect) {
    auto fac = Factory::getInstance();
    auto m = fac->monster;
    for (auto s : m) {
        if (rect.containsPoint(s->getPosition())) {
            return s;
        }
    }
    return NULL;
}

cocos2d::Vector<Sprite*> Factory::attack_collider(Rect rect) {
    auto fac = Factory::getInstance();
    auto m = fac->monster;
    Vector<Sprite * > store;
    for (auto s : m) {
        if (rect.containsPoint(s->getPosition())) {
            store.pushBack(s);
        }
    }
    return store;
}
