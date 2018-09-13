#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <iostream>
using namespace std;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    auto menuItem_1 = MenuItemFont::create("Click", CC_CALLBACK_1(HelloWorld::myClick, this));
    menuItem_1->setPosition(origin.x + visibleSize.width/3,
                            origin.y + visibleSize.height/2);
    menu->addChild(menuItem_1, 2);
    
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    
    //创建词典类实例，将xml文件加载到词典中
    auto *chnStrings = Dictionary::createWithContentsOfFile("myName.xml");
    //通过xml文件中的key获取value
    const char *str1 = ((String*)chnStrings->objectForKey("name"))->getCString();
    const char *str2 = ((String*)chnStrings->objectForKey("num"))->getCString();
    
    //创建一个label
    auto label = Label::create(str1, "Arial", 24);
    auto label1 = Label::create(str2, "Arial", 24);
    //修改颜色
    label1->setColor(Color3B(249, 110, 123));
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));
        
        // add the label as a child to this layer
        this->addChild(label, 1);
        label1->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height - label1->getContentSize().height - label->getContentSize().height));
        this->addChild(label1, 2);
    }
    
    // add "HelloWorld" splash screen"
    //创建一个精灵
    auto sprite = Sprite::create("myPic.jpg");
    if (sprite == nullptr)
    {
        problemLoading("'myPic.jpg'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        
        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    return true;
}

//关闭场景
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}


void HelloWorld::myClick(Ref* pSender)
{
    cout << "now click!\n" << endl;
}
