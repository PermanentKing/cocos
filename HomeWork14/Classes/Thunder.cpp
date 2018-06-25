#include "Thunder.h"
#include <algorithm>
#include <iostream>

USING_NS_CC;

using namespace CocosDenshion;

Scene* Thunder::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Thunder::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Thunder::init() {
	if (!Layer::init()) {
		return false;
	}
	stoneType = 0;
	isMove = false;  // 是否点击飞船
	visibleSize = Director::getInstance()->getVisibleSize();

	// 创建背景
	auto bgsprite = Sprite::create("bg.jpg");
	bgsprite->setPosition(visibleSize / 2);
	bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width,
		visibleSize.height / bgsprite->getContentSize().height);
	this->addChild(bgsprite, 0);

	// 创建飞船
	player = Sprite::create("player.png");
	player->setAnchorPoint(Vec2(0.5, 0.5));
	player->setPosition(visibleSize.width / 2, player->getContentSize().height);
	player->setName("player");
	this->addChild(player, 1);

	// 显示陨石和子弹数量
	enemysNum = Label::createWithTTF("enemys: 0", "fonts/arial.ttf", 20);
	enemysNum->setColor(Color3B(255, 255, 255));
	enemysNum->setPosition(50, 60);
	this->addChild(enemysNum, 3);
	bulletsNum = Label::createWithTTF("bullets: 0", "fonts/arial.ttf", 20);
	bulletsNum->setColor(Color3B(255, 255, 255));
	bulletsNum->setPosition(50, 30);
	this->addChild(bulletsNum, 3);

	addEnemy(5);        // 初始化陨石
	preloadMusic();     // 预加载音乐
	playBgm();          // 播放背景音乐
	explosion();        // 创建爆炸帧动画

	// 添加监听器
	addTouchListener();
	addKeyboardListener();
	addCustomListener();

	// 调度器
	schedule(schedule_selector(Thunder::update), 0.04f, kRepeatForever, 0);

	return true;
}

//预加载音乐文件
void Thunder::preloadMusic() {
	// Todo
    auto audio = SimpleAudioEngine::getInstance();
    audio->preloadBackgroundMusic("music/bgm.mp3");
    
    audio->preloadEffect("music/explore.wav");
    audio->preloadEffect("music/fire.wav");
}

//播放背景音乐
void Thunder::playBgm() {
	// Todo
    auto audio = SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("music/bgm.mp3", true);

}

//初始化陨石
void Thunder::addEnemy(int n) {
	enemys.clear();
	for (unsigned i = 0; i < 3; ++i) {
		char enemyPath[20];
		sprintf(enemyPath, "stone%d.png", 3 - i);
		double width = visibleSize.width / (n + 1.0),
			height = visibleSize.height - (50 * (i + 1));
		for (int j = 0; j < n; ++j) {
			auto enemy = Sprite::create(enemyPath);
			enemy->setAnchorPoint(Vec2(0.5, 0.5));
			enemy->setScale(0.5, 0.5);
			enemy->setPosition(width * (j + 1), height);
			enemys.push_back(enemy);
			addChild(enemy, 1);
		}
	}
}

// 陨石向下移动并生成新的一行(加分项)
void Thunder::newEnemy() {
	// Todo
    stoneType++;
    if (stoneType==4) {
        stoneType = 1;
    }
    char enemyPath[20];
    sprintf(enemyPath, "stone%d.png", stoneType);
    double tempwidth = visibleSize.width / 6;
    list<Sprite*>::iterator iter = enemys.begin();
    for (; iter!=enemys.end(); iter++) {
        (*iter)->setPosition((*iter)->getPositionX(), (*iter)->getPositionY() - 50);
    }
    for (int i = 0; i < 5; i++) {
        auto enemy = Sprite::create(enemyPath);
        enemy->setAnchorPoint(Vec2(0.5, 0.5));
        enemy->setScale(0.5, 0.5);
        enemy->setPosition(tempwidth * (i + 1) - 80 , visibleSize.height - 50);
        enemys.push_front(enemy);
        addChild(enemy, 1);
    }
}

// 移动飞船
void Thunder::movePlane(char c) {
	// Todo
    if (player == NULL || !isMove) {
        return;
    }
    if (player->getPositionX() - 10 < 0 || player->getPositionX() + 10 > visibleSize.width) {
        return;
    }
    else {
        //player->setPosition(player->getPosition() + Vec2(move, 0));
        if (movekey=='A') {
            player->setPosition(player->getPositionX() - 5, player->getPositionY());
            return;
        }else if(movekey=='D'){
            player->setPosition(player->getPositionX() + 5, player->getPositionY());
            return;
        }else{
            return;
        }
    }
    //isMove = false;
    return;
}

//发射子弹
void Thunder::fire() {
	auto bullet = Sprite::create("bullet.png");
	bullet->setAnchorPoint(Vec2(0.5, 0.5));
	bullets.push_back(bullet);
	bullet->setPosition(player->getPosition());
	this->addChild(bullet, 1);
	SimpleAudioEngine::getInstance()->playEffect("music/fire.wav", false);

	// 移除飞出屏幕外的子弹
	// Todo

}

// 切割爆炸动画帧
void Thunder::explosion() {
	// Todo
    auto texture = Director::getInstance()->getTextureCache()->addImage("explosion.png");
    explore.reserve(8);
    for (int i = 0; i < 5; i++) {
        auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(193 * i + 1, 0, 142, 142)));
        explore.pushBack(frame);
    }
    for (int i = 0; i < 3; i++) {
        auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(193 * i + 1, 193, 142, 142)));
        explore.pushBack(frame);
    }
}

void Thunder::update(float f) {
	// 实时更新页面内陨石和子弹数量(不得删除)
	// 要求数量显示正确(加分项)
	char str[15];
	sprintf(str, "enemys: %d", enemys.size());
	enemysNum->setString(str);
	sprintf(str, "bullets: %d", bullets.size());
	bulletsNum->setString(str);
    
    int rows = (visibleSize.height - (enemys.back())->getPositionY()) / 50;
    if (rows >= 13) {
        stopAc();
        return;
    }

    //更新子弹
    if (!bullets.empty()) {
        list<Sprite*>::iterator iter = bullets.begin();
        for (; iter!=bullets.end(); iter++) {
            (*iter)->setPosition((*iter)->getPositionX(), (*iter)->getPositionY() + 5);
            if ((*iter)->getPositionY() > visibleSize.height - 10) {
                (*iter)->removeFromParentAndCleanup(true);
                iter = bullets.erase(iter);
                //cout << bullets.size() << endl;
            }
        }
    }
	// 飞船移动
	if (isMove)
		this->movePlane(movekey);
    
    //先右移80，然后左移160，然后newEnemy
	static int ct = 0;
	static int dir = 4;
	++ct;
	if (ct == 120)
		ct = 40, dir = -dir;
	else if (ct == 80) {
		dir = -dir;
		newEnemy();  // 陨石向下移动并生成新的一行(加分项)
	}
	else if (ct == 20)
		ct = 40, dir = -dir;

	//陨石左右移动

	// 分发自定义事件
    EventCustom e("meet");
    e.setUserData(&dir);
    _eventDispatcher->dispatchEvent(&e);

}

// 自定义碰撞事件
//change
void Thunder::meet(EventCustom * event) {
	// 判断子弹是否打中陨石并执行对应操作
	// Todo
    int  *index = (int*)event->getUserData();
    int dir = *index;
    list<Sprite*>::iterator iter1 = enemys.begin();
    for (; iter1!=enemys.end(); iter1++) {
        if ((*iter1)!=NULL) {
            (*iter1)->setPosition((*iter1)->getPosition() + Vec2(dir, 0));
            if (player != NULL && player->getPosition().getDistance((*iter1)->getPosition()) < 50) {
                EventCustom e1("meet1");
                _eventDispatcher->dispatchEvent(&e1);
            }
            list<Sprite*>::iterator iter2 = bullets.begin();
            for (; iter2!=bullets.end(); iter2++) {
                if ((*iter2) != NULL && (*iter2)->getPosition().getDistance((*iter1)->getPosition()) < 20) {
                    auto tempAnima= Animation::createWithSpriteFrames(explore, 0.1f);
                    auto EXPLO = Animate::create(tempAnima);
                    Sequence *seq = Sequence::create(EXPLO, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, (*iter1))), NULL);
                    
                    (*iter1)->runAction(seq);

                    iter1 = enemys.erase(iter1);
                    auto audio = SimpleAudioEngine::getInstance();
                    audio->playEffect("music/explore.wav", false, 1.0f, 1.0f, 1.0f);
                    
                    (*iter2)->removeFromParentAndCleanup(true);
                    iter2 = bullets.erase(iter2);
                    break;
                }
            }
        }
    }
}


//打中玩家

void Thunder::meet1(EventCustom *event) {
    auto tempAnima= Animation::createWithSpriteFrames(explore, 0.1f);
    auto EXPLO = Animate::create(tempAnima);
    Sequence *seq = Sequence::create(EXPLO, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, player)), NULL);
    
    player->runAction(seq);
    //player->removeFromParentAndCleanup(true);
    //player = NULL;
    
    auto audio = SimpleAudioEngine::getInstance();
    audio->playEffect("music/explore.wav", false, 1.0f, 1.0f, 1.0f);
    stopAc();
}



//结束操作
void Thunder::stopAc() {
    // Todo
    auto gameover = Sprite::create("gameOver.png");
    gameover->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(gameover, 4);
    this->unscheduleAllSelectors();
    _eventDispatcher->removeAllEventListeners();
}



// 添加自定义监听器
void Thunder::addCustomListener() {
	// Todo
    auto meetListener = EventListenerCustom::create("meet", CC_CALLBACK_1(Thunder::meet, this));
    _eventDispatcher->addEventListenerWithFixedPriority(meetListener, 1);
    auto meetListener1 = EventListenerCustom::create("meet1", CC_CALLBACK_1(Thunder::meet1, this));
    _eventDispatcher->addEventListenerWithFixedPriority(meetListener1, 1);
}

// 添加键盘事件监听器
void Thunder::addKeyboardListener() {
	// Todo
    auto keyboardListener = EventListenerKeyboard::create();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Thunder::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Thunder::onKeyReleased, this);
    dispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void Thunder::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
		movekey = 'A';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
		movekey = 'D';
		isMove = true;
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		fire();
		break;
	}
}

//change
void Thunder::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
            isMove = false;
            break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
            isMove = false;
            // TODO
            break;
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
		isMove = false;
		break;
	}
}

// 添加触摸事件监听器
void Thunder::addTouchListener() {
	// Todo
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = CC_CALLBACK_2(Thunder::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Thunder::onTouchEnded, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(Thunder::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

// 鼠标点击发射炮弹
bool Thunder::onTouchBegan(Touch *touch, Event *event) {
	if (touch->getLocation().getDistance(player->getPosition()) <= 30)
        isClick = true;
    // Todo
    if (player == NULL) return true;
    Position = touch->getLocation();
    return true;
}

void Thunder::onTouchEnded(Touch *touch, Event *event) {
	isClick = false;
    if (player == NULL) return;
    fire();
}

// 当鼠标按住飞船后可控制飞船移动 (加分项)
void Thunder::onTouchMoved(Touch *touch, Event *event) {
	// Todo
    if (player == NULL) return;
    Vec2 pos = touch->getLocation();
    if (pos.x > Position.x) {
        if (player->getPositionX() + 10 > visibleSize.width) {
            return;
        }
        player->setPosition(player->getPositionX() + 5, player->getPositionY());
    }
    else if (pos.x != Position.x){
        if (player->getPositionX() - 10 < 0) {
            return;
        }
        player->setPosition(player->getPositionX() - 5, player->getPositionY());
    }
}
