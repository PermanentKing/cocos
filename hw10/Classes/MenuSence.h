//
//  MenuSence.h
//  HelloWorld
//
//  Created by 王晶 on 2018/5/23.
//

#ifndef MenuSence_h
#define MenuSence_h

#include "cocos2d.h"

class MenuSence : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void menuChangeCallback(Ref * pSender);
    
    // a selector callback
    //void startMenuCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MenuSence);
};

#endif /* MenuSence_h */
