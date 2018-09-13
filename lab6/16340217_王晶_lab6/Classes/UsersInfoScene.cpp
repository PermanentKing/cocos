#include "UsersInfoScene.h"
#include "network/HttpClient.h"
#include "json/document.h"
#include "Utils.h"
#include <iostream>

using namespace cocos2d::network;
using namespace rapidjson;
using namespace std;


cocos2d::Scene * UsersInfoScene::createScene() {
  return UsersInfoScene::create();
}

bool UsersInfoScene::init() {
  if (!Scene::init()) return false;

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  auto getUserButton = MenuItemFont::create("Get User", CC_CALLBACK_1(UsersInfoScene::getUserButtonCallback, this));
  if (getUserButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + getUserButton->getContentSize().height / 2;
    getUserButton->setPosition(Vec2(x, y));
  }

  auto backButton = MenuItemFont::create("Back", [](Ref* pSender) {
    Director::getInstance()->popScene();
  });
  if (backButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - backButton->getContentSize().height / 2;
    backButton->setPosition(Vec2(x, y));
  }

  auto menu = Menu::create(getUserButton, backButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  limitInput = TextField::create("limit", "arial", 24);
  if (limitInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 100.0f;
    limitInput->setPosition(Vec2(x, y));
    this->addChild(limitInput, 1);
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

void UsersInfoScene::getUserButtonCallback(Ref * pSender) {
  // Your code here
    string num = limitInput->getString();
    HttpRequest* request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl("http://127.0.0.1:8000/users?limit=" + num);
    
    request->setResponseCallback(CC_CALLBACK_2(UsersInfoScene::getUserCompleted, this));
    HttpClient::getInstance()->send(request);
    request->release();
}


void UsersInfoScene::getUserCompleted(HttpClient *sender, HttpResponse *response)
{
    if (!response) {
        this->messageBox->setString("Wrong\n");
        return;
    }
    auto buffer = response->getResponseData();
    rapidjson::Document d;
    d.Parse(buffer->data(), buffer->size());

    if (d["status"] == true) {
        string output;
        for (int i = 0; i < d["data"].Size(); i++) {
            output += "Username : ";
            output += d["data"][i]["username"].GetString();
            output += "\nDeck : \n";
            for (int j = 0; j < d["data"][i]["deck"].Size(); j++) {
                for (auto& temp : d["data"][i]["deck"][j].GetObject()) {
                    output += "  ";
                    output += temp.name.GetString();
                    output += ":";
                    int a = temp.value.GetInt();
                    char str[100];
                    sprintf(str, "%d", a);
                    output += string(str);
                    output += "\n";
                }
                output += " *******\n";
            }
            output += "----------\n";
        }
        this->messageBox->setString(output);
    }
    else {
        this->messageBox->setString(string("Failed\n") + d["msg"].GetString());
    }
}
