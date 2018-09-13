#include "LoginRegisterScene.h"
#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include "json/document.h"
#include "Utils.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

USING_NS_CC;
using namespace cocos2d::network;
using namespace cocos2d::ui;
using namespace rapidjson;
using namespace std;

cocos2d::Scene * LoginRegisterScene::createScene() {
  return LoginRegisterScene::create();
}

bool LoginRegisterScene::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  auto loginButton = MenuItemFont::create("Login", CC_CALLBACK_1(LoginRegisterScene::loginButtonCallback, this));
  if (loginButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + loginButton->getContentSize().height / 2;
    loginButton->setPosition(Vec2(x, y));
  }

  auto registerButton = MenuItemFont::create("Register", CC_CALLBACK_1(LoginRegisterScene::registerButtonCallback, this));
  if (registerButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + registerButton->getContentSize().height / 2 + 100;
    registerButton->setPosition(Vec2(x, y));
  }

  auto backButton = MenuItemFont::create("Back", [] (Ref* pSender) {
    Director::getInstance()->popScene();
  });
  if (backButton) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - backButton->getContentSize().height / 2;
    backButton->setPosition(Vec2(x, y));
  }

  auto menu = Menu::create(loginButton, registerButton, backButton, NULL);
  menu->setPosition(Vec2::ZERO);
  this->addChild(menu, 1);

  usernameInput = TextField::create("username", "arial", 24);
  if (usernameInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 100.0f;
    usernameInput->setPosition(Vec2(x, y));
    this->addChild(usernameInput, 1);
  }

  passwordInput = TextField::create("password", "arial", 24);
  if (passwordInput) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 130.0f;
    passwordInput->setPosition(Vec2(x, y));
    this->addChild(passwordInput, 1);
  }

  messageBox = Label::create("", "arial", 30);
  if (messageBox) {
    float x = origin.x + visibleSize.width / 2;
    float y = origin.y + visibleSize.height - 200.0f;
    messageBox->setPosition(Vec2(x, y));
    this->addChild(messageBox, 1);
  }

  return true;
}

void LoginRegisterScene::loginButtonCallback(cocos2d::Ref * pSender) {
  // Your code here
    string username = usernameInput->getString();
    string password = passwordInput->getString();
    string postdata = getJson(username, password);
    HttpRequest* request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://127.0.0.1:8000/auth");

    request->setRequestData(postdata.c_str(), postdata.length());
    request->setResponseCallback(CC_CALLBACK_2(LoginRegisterScene::LoginCompleted, this));
    HttpClient::getInstance()->enableCookies(NULL);
    HttpClient::getInstance()->send(request);
    request->release();
}

void LoginRegisterScene::registerButtonCallback(Ref * pSender) {
  // Your code here
    string username = usernameInput->getString();
    string password = passwordInput->getString();
    string data = getJson(username, password);
    HttpRequest* request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://127.0.0.1:8000/users");
    
    request->setRequestData(data.c_str(), data.length());
    request->setResponseCallback(CC_CALLBACK_2(LoginRegisterScene::RegisterCompleted, this));
    HttpClient::getInstance()->enableCookies(NULL);
    HttpClient::getInstance()->send(request);
    request->release();
    
}

string LoginRegisterScene::getJson(string username, string password) {
    rapidjson::Document d;
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
    d.SetObject();
    rapidjson::Value str1(rapidjson::kStringType);
    str1.SetString(username.c_str(), username.length());
    rapidjson::Value str2(rapidjson::kStringType);
    str2.SetString(username.c_str(), username.length());
    d.AddMember("username", str1, allocator);
    d.AddMember("password", str2, allocator);
    StringBuffer buffer;
    rapidjson::Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    
    return buffer.GetString();
}

void LoginRegisterScene::LoginCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (!response) {
        usernameInput->setString("Wrong\n");
        return;
    }
    auto buffer = response->getResponseData();
    rapidjson::Document d;
    d.Parse(buffer->data(),buffer->size());
    if (d["status"] == true) {
        this->messageBox->setString("Login!");
    }
    else {
        this->messageBox->setString(std::string("Login Failed\n") + d["msg"].GetString());
    }
}

void LoginRegisterScene::RegisterCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if(!response){
        usernameInput->setString("Wrong\n");
        return;
    }
    auto buffer = response->getResponseData();
    rapidjson::Document d;
    d.Parse(buffer->data(), buffer->size());
    if (d["status"] == true) {
        this->messageBox->setString("Register!");
    }
    else {
        this->messageBox->setString(std::string("Register Failed\n") + d["msg"].GetString());
    }
}

