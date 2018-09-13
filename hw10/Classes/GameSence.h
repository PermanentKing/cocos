//
//  GameSence.h
//  HelloWorld
//
//  Created by 王晶 on 2018/5/23.
//

#ifndef GameSence_h
#define GameSence_h

#include "cocos2d.h"
USING_NS_CC;

class GameSence : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    void menu(Ref * pSender);
    
    //virtual void shootMenuCallback(Ref* pSender);
    
    CREATE_FUNC(GameSence);
    
private:
    Sprite* mouse;
    
    Sprite* stone;
    
    Layer* mouseLayer;
    
    Layer* stoneLayer;
    
};

#endif /* GameSence_h */
