#include "ModifyUserScene.h"
#include "Utils.h"
#include "network/HttpClient.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

using namespace cocos2d::network;
using namespace rapidjson;
using namespace std;


cocos2d::Scene * ModifyUserScene::createScene() {
  return ModifyUserScene::create();
}

bool ModifyUserScene::init() {
  if (!Scene::init()) return false;
  
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  auto postDeckButton = MenuItemFont::create("Post Deck", CC_CALLBACK_1(ModifyUserScene::putDeckButtonCallback, this));
  if (postDeckButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + postDeckButton->getContentSize().height / 2;
    postDeckButton->setPosition(Vec2(x, y));
  }

  auto backButton = MenuItemFont::create("Back", [](Ref* pSender) {
    Director::getInstance()->popScene();
  });
  if (backButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - backButton->getContentSize().height / 2;
    backButton->setPosition(Vec2(x, y));
  }

  auto menu = Menu::create(postDeckButton, backButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  deckInput = TextField::create("Deck json here", "arial", 24);
  if (deckInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 100.0f;
    deckInput->setPosition(Vec2(x, y));
    this->addChild(deckInput, 1);
  }

  messageBox = Label::create("", "arial", 30);
  if (messageBox) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height / 2;
    messageBox->setPosition(Vec2(x, y));
    this->addChild(messageBox, 1);
  }

  return true;
}

void ModifyUserScene::putDeckButtonCallback(Ref * pSender) {
  // Your code here
    string deck = deckInput->getStringValue();
    if(deck[0]!='['){
        this->messageBox->setString("Wrong Input!");
        return;
    }
    string data = "{\"deck\":" + deck + "}";
    HttpRequest* request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::PUT);
    request->setUrl("http://127.0.0.1:8000/users");
    
    request->setResponseCallback(CC_CALLBACK_2(ModifyUserScene::putDeckCompleted, this));
    request->setRequestData(data.c_str(), strlen(data .c_str()));
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}


void ModifyUserScene::putDeckCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    auto buffer = response->getResponseData();
    rapidjson::Document doc;
    doc.Parse(buffer->data(), buffer->size());
    if (doc["status"] == true) {
        this->messageBox->setString("PUT OK");
    }
    else {
        this->messageBox->setString(string("PUT Failed\n") + doc["msg"].GetString());
    }
}
