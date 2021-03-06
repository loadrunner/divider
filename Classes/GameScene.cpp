#include "GameScene.h"

int GameScene::NUMBER_POOL_SIZE = 21;
int GameScene::NUMBER_POOL[] = {10, 12, 14, 15, 18, 20, 21, 25, 27, 28, 32, 35, 36, 42, 45, 48, 50, 54, 56, 64, 68};
int GameScene::DIVISORS[] = {2, 5, 3, 4, 6, 7, 8, 9};
const std::string GameScene::ANALYTICS_GAME_MODE_INDEX = "dimension_1";
const std::string GameScene::ANALYTICS_LEVEL_INDEX = "dimension_2";
const std::string GameScene::ANALYTICS_WAVE_INDEX = "dimension_3";
const std::string GameScene::ANALYTICS_DIE_NUMBER_INDEX = "dimension_4";
const std::string GameScene::ANALYTICS_SCORE_INDEX = "metric_1";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const std::string GameScene::LEADERBOARD_GLOBAL_DIVIDED_ID = "CgkI9qH8t-UMEAIQEQ";
const std::string GameScene::LEADERBOARD_MODE_1_ID = "CgkI9qH8t-UMEAIQCQ";
const std::string GameScene::LEADERBOARD_MODE_2_ID = "CgkI9qH8t-UMEAIQCA";
const std::string GameScene::ACHIEVEMENT_FIRST_LEVEL = "CgkI9qH8t-UMEAIQAQ";
const std::string GameScene::ACHIEVEMENT_ALL_LEVELS = "CgkI9qH8t-UMEAIQAg";
const std::string GameScene::ACHIEVEMENT_FIRST_3_STARS = "CgkI9qH8t-UMEAIQAw";
const std::string GameScene::ACHIEVEMENT_ALL_3_STARS = "CgkI9qH8t-UMEAIQBA";
const std::string GameScene::ACHIEVEMENT_PLAY_GAME_2 = "CgkI9qH8t-UMEAIQBQ";
const std::string GameScene::ACHIEVEMENT_1_WAVE = "CgkI9qH8t-UMEAIQCg";
const std::string GameScene::ACHIEVEMENT_5_WAVES = "CgkI9qH8t-UMEAIQEA";
const std::string GameScene::ACHIEVEMENT_10_WAVES = "CgkI9qH8t-UMEAIQBg";
const std::string GameScene::ACHIEVEMENT_50_WAVES = "CgkI9qH8t-UMEAIQBw";
const std::string GameScene::ACHIEVEMENT_DIVIDE_1_NR = "CgkI9qH8t-UMEAIQDg";
const std::string GameScene::ACHIEVEMENT_DIVIDE_25_NR = "CgkI9qH8t-UMEAIQDw";
const std::string GameScene::ACHIEVEMENT_DIVIDE_100_NR = "CgkI9qH8t-UMEAIQCw";
const std::string GameScene::ACHIEVEMENT_DIVIDE_250_NR = "CgkI9qH8t-UMEAIQDA";
const std::string GameScene::ACHIEVEMENT_DIVIDE_1000_NR = "CgkI9qH8t-UMEAIQDQ";
#else
const std::string GameScene::LEADERBOARD_GLOBAL_DIVIDED_ID = "-";
const std::string GameScene::LEADERBOARD_MODE_1_ID = "-";
const std::string GameScene::LEADERBOARD_MODE_2_ID = "-";
const std::string GameScene::ACHIEVEMENT_FIRST_LEVEL = "-";
const std::string GameScene::ACHIEVEMENT_ALL_LEVELS = "-";
const std::string GameScene::ACHIEVEMENT_FIRST_3_STARS = "-";
const std::string GameScene::ACHIEVEMENT_ALL_3_STARS = "-";
const std::string GameScene::ACHIEVEMENT_PLAY_GAME_2 = "-";
const std::string GameScene::ACHIEVEMENT_1_WAVE = "-";
const std::string GameScene::ACHIEVEMENT_5_WAVES = "-";
const std::string GameScene::ACHIEVEMENT_10_WAVES = "-";
const std::string GameScene::ACHIEVEMENT_50_WAVES = "-";
const std::string GameScene::ACHIEVEMENT_DIVIDE_1_NR = "-";
const std::string GameScene::ACHIEVEMENT_DIVIDE_25_NR = "-";
const std::string GameScene::ACHIEVEMENT_DIVIDE_100_NR = "-";
const std::string GameScene::ACHIEVEMENT_DIVIDE_250_NR = "-";
const std::string GameScene::ACHIEVEMENT_DIVIDE_1000_NR = "-";
#endif

bool GameScene::init()
{
	if (!cocos2d::Scene::init())
		return false;
	
	cocos2d::UserDefault* ud = cocos2d::UserDefault::getInstance();
	
	mIsGameServicesAvailable = AppDelegate::pluginGameServices ? AppDelegate::pluginGameServices->isSignedIn() : false;
	
	mScreenSize = cocos2d::Director::getInstance()->getWinSize();
	mVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	mOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	
	cocos2d::log("size %f, %f", mScreenSize.width, mScreenSize.height);
	cocos2d::log("visible size %f, %f", mVisibleSize.width, mVisibleSize.height);
	cocos2d::log("offset %f, %f", mOrigin.x, mOrigin.y);
	
	cocos2d::Texture2D::TexParams texParams;
	texParams.magFilter = GL_LINEAR;
	texParams.minFilter = GL_LINEAR;
	texParams.wrapS = GL_REPEAT;
	texParams.wrapT = GL_REPEAT;
	
	cocos2d::Sprite* bg = cocos2d::Sprite::create("bg.png", cocos2d::Rect(mOrigin.x, mOrigin.y, mVisibleSize.width, mVisibleSize.height));
	bg->getTexture()->setTexParameters(texParams);
	bg->setPosition(cocos2d::Vec2(mOrigin.x + mVisibleSize.width/2, mOrigin.y + mVisibleSize.height/2));
	this->addChild(bg, 99);
	
	mGameLayer = cocos2d::Node::create();
//	mGameLayer->ignoreAnchorPointForPosition(false);
	mGameLayer->setPosition(cocos2d::Vec2(mOrigin.x, mOrigin.y + 20));
	mGameLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
	mGameLayer->setContentSize(cocos2d::Size(mVisibleSize.width, mVisibleSize.height - 20));
	this->addChild(mGameLayer, 100);
	
	mUILayer = cocos2d::Layer::create();
	mUILayer->ignoreAnchorPointForPosition(false);
	mUILayer->setPosition(mOrigin);
	mUILayer->setAnchorPoint(cocos2d::Vec2::ZERO);
	mUILayer->setContentSize(mVisibleSize);
	this->addChild(mUILayer, 200);
	
	mTopBar = cocos2d::LayerColor::create(cocos2d::Color4B(0, 0, 0, 30));
	mTopBar->ignoreAnchorPointForPosition(false);
	mTopBar->setPosition(0, mUILayer->getContentSize().height);
	mTopBar->setAnchorPoint(cocos2d::Vec2(0, 1));
	mTopBar->setContentSize(cocos2d::Size(mUILayer->getContentSize().width, 15));
	mUILayer->addChild(mTopBar);
	
	cocos2d::Label* scoreTitle = cocos2d::Label::createWithTTF("SCORE", "fonts/semibold.otf", 5);
	scoreTitle->setPosition(cocos2d::Vec2(mUILayer->getContentSize().width - 12, mUILayer->getContentSize().height - 1));
	scoreTitle->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
	mUILayer->addChild(scoreTitle);
	
	mScoreView = cocos2d::Label::createWithTTF("", "fonts/semibold.otf", 9);
	mScoreView->setPosition(cocos2d::Vec2(
			scoreTitle->getPosition().x - scoreTitle->getContentSize().width * scoreTitle->getAnchorPoint().x + scoreTitle->getContentSize().width * 0.5f,
			scoreTitle->getPosition().y - scoreTitle->getContentSize().height * scoreTitle->getAnchorPoint().y - 1));
	mScoreView->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
	mScoreView->enableShadow(cocos2d::Color4B::BLACK, cocos2d::Size(0.25f, -0.25f));
	mUILayer->addChild(mScoreView);
	
	mMenu = cocos2d::Menu::create();
	mMenu->ignoreAnchorPointForPosition(false);
	mMenu->setPosition(cocos2d::Vec2::ZERO);
	mMenu->setAnchorPoint(cocos2d::Vec2::ZERO);
	mMenu->setContentSize(mUILayer->getContentSize());
	mUILayer->addChild(mMenu);
	
	auto pauseBtn = cocos2d::MenuItemSprite::create(cocos2d::Sprite::createWithSpriteFrameName("pause_btn"),
			nullptr, [this] (cocos2d::Ref* btn) { pauseGame(); });
	pauseBtn->setPosition(9, 9);
	mMenu->addChild(pauseBtn);
	
	initPools();
	
	// Register Touch Event
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	auto listener2 = cocos2d::EventListenerKeyboard::create();
	listener2->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	listener2->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
	
	auto listener3 = cocos2d::EventListenerCustom::create(EVENT_COME_TO_FOREGROUND, CC_CALLBACK_0(GameScene::onComeToForeground, this));
	dispatcher->addEventListenerWithSceneGraphPriority(listener3, this);
	
	auto listener4 = cocos2d::EventListenerCustom::create(EVENT_COME_TO_BACKGROUND, CC_CALLBACK_0(GameScene::onComeToBackground, this));
	dispatcher->addEventListenerWithSceneGraphPriority(listener4, this);
	
	mGlobalScore = ud->getIntegerForKey("global_score", 0);
	
	mSceneState = GAME_SCENE_STATES::NONE;
	
	startGame();
	
	return true;
}

void GameScene::checkNumbers()
{
	cocos2d::log("Calculating number of divisors for all numbers:");
	for (int i = 0; i < NUMBER_POOL_SIZE; i++)
	{
		int m = 0;
		for (int j = 0; j < mDivisorMax; j++)
			if (NUMBER_POOL[i] % DIVISORS[j] == 0)
				m++;
		
		if (m == 0)
		{
			cocos2d::log("ERROR: number %d doesn't have any numbers in divisors!", NUMBER_POOL[i]);
			AppDelegate::closeApp();
			return;
		}
		
		cocos2d::log(" -- %d has %d divisors", NUMBER_POOL[i], m);
	}
	
	cocos2d::log("\nCounting numbers in pool for each divisor:");
	for (int i = mDivisorMin; i < mDivisorMax; i++)
	{
		int m = 0;
		for (int j = 0; j < NUMBER_POOL_SIZE; j++)
			if (NUMBER_POOL[j] % DIVISORS[i] == 0)
				m++;
		
		cocos2d::log(" -- %d can divide %d numbers from pool", DIVISORS[i], m);
	}
}

int GameScene::getNumber()
{
	return NUMBER_POOL[rand() % NUMBER_POOL_SIZE];
}

void GameScene::divideBall(Ball* ball)
{
	mGlobalScore++;
	
	if (mGlobalScore % 10 == 0)
		cocos2d::UserDefault::getInstance()->setIntegerForKey("global_score", mGlobalScore);
	
	if (mIsGameServicesAvailable)
	{
		if (mGlobalScore == 1)
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_DIVIDE_1_NR);
		else if (mGlobalScore == 25)
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_DIVIDE_25_NR);
		else if (mGlobalScore == 100)
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_DIVIDE_100_NR);
		else if (mGlobalScore == 250)
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_DIVIDE_250_NR);
		else if (mGlobalScore == 1000)
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_DIVIDE_1000_NR);
	}
}

void GameScene::missBall(Ball* ball)
{
	
}

void GameScene::initPools()
{
	mBallPool.init(100, mGameLayer);
}

void GameScene::restartGame()
{
	cocos2d::log("restart game");
	
	cocos2d::Director::getInstance()->replaceScene(clone());
}

void GameScene::startGame()
{
	if (mSceneState != GAME_SCENE_STATES::NONE
	 && mSceneState != GAME_SCENE_STATES::OVER)
	{
		cocos2d::log("WARNING! Invalid scene state!");
		return;
	}
	
	cocos2d::log("start game");
	mSceneState = GAME_SCENE_STATES::START;
	
	mBallZOrder = 999999;
	mSpawnTimer = 0;
	
	mDivisorMin = 0;
	mDivisorMax = sizeof(DIVISORS) / sizeof(int);
	
	mScore = 0;
	updateScore();
	
	checkNumbers();
	
	resumeGame();
}

void GameScene::resumeGame()
{
	if (mSceneState != GAME_SCENE_STATES::START
	 && mSceneState != GAME_SCENE_STATES::PAUSED)
	{
		cocos2d::log("WARNING! Invalid scene state!");
		return;
	}
	
	cocos2d::log("resume game");
	mSceneState = GAME_SCENE_STATES::PLAY;
	
	for (auto it = mBalls.begin(); it != mBalls.end(); it++)
		(*it)->resume();
	
	schedule(schedule_selector(GameScene::update));
	schedule(schedule_selector(GameScene::updateSlow), 1.0f);
}

void GameScene::pauseGame()
{
	if (mSceneState != GAME_SCENE_STATES::PLAY)
	{
		cocos2d::log("WARNING! Invalid scene state!");
		return;
	}
	
	cocos2d::log("pause game");
	mSceneState = GAME_SCENE_STATES::PAUSED;
	
	unschedule(schedule_selector(GameScene::update));
	unschedule(schedule_selector(GameScene::updateSlow));
	
	for (auto it = mBalls.begin(); it != mBalls.end(); it++)
		(*it)->pause();
	
	PauseOverlay* overlay = PauseOverlay::create();
	overlay->resumeCallback = CC_CALLBACK_0(GameScene::resumeGame, this);
	overlay->restartCallback = CC_CALLBACK_0(GameScene::restartGame, this);
	overlay->exitCallback = CC_CALLBACK_0(GameScene::exitGame, this);
	addChild(overlay, 1000);
}

void GameScene::endGame(int nr)
{
	if (mSceneState != GAME_SCENE_STATES::PLAY)
	{
		cocos2d::log("WARNING! Invalid scene state!");
		return;
	}
	
	cocos2d::log("end game");
	mSceneState = GAME_SCENE_STATES::OVER;
	
	unschedule(schedule_selector(GameScene::update));
	unschedule(schedule_selector(GameScene::updateSlow));
	
	for (auto it = mBalls.begin(); it != mBalls.end(); it++)
		(*it)->pause();
	
	if (mScore > 0)
	{
		cocos2d::UserDefault::getInstance()->setIntegerForKey("global_score", mGlobalScore);
		
		if (mIsGameServicesAvailable)
			AppDelegate::pluginGameServices->publishScore(LEADERBOARD_GLOBAL_DIVIDED_ID, mGlobalScore);
	}
}

void GameScene::exitGame()
{
	if (mSceneState != GAME_SCENE_STATES::PAUSED
	 && mSceneState != GAME_SCENE_STATES::OVER)
	{
		cocos2d::log("WARNING! Invalid scene state!");
		return;
	}
	
	cocos2d::log("exit game");
	
	cocos2d::Director::getInstance()->popScene();
}

void GameScene::update(float dt)
{
	mSpawnTimer += dt;
	
	if (mSpawnTimer >= mSpawnInterval)
	{
		mSpawnTimer = 0;
		
		spawnBall();
	}
}

void GameScene::updateSlow(float dt)
{
	if (AppDelegate::pluginGameServices->isSignedIn() != mIsGameServicesAvailable)
	{
		mIsGameServicesAvailable = AppDelegate::pluginGameServices->isSignedIn();
		
		if (mIsGameServicesAvailable)
		{
		//	if (mScore > 0)
		//		AppDelegate::pluginGameServices->publishScore(mScore);
			
		//	if (mScore >= 10000)
		//		AppDelegate::pluginGameServices->unlockAchievement(1);
		}
	}
}

void GameScene::updateScore()
{
	mScoreView->setString(cocos2d::__String::createWithFormat("%d", mScore)->_string);
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You touched id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	if (!mBalls.empty())
	{
		for (auto it = mBalls.begin(); it != mBalls.end(); it++)
		{
			Ball* ball = *it;
			cocos2d::Vec2 local = touch->getLocation() - mGameLayer->getPosition();
			cocos2d::Rect r = ball->getBoundingBox();
		//	r.origin += mGameLayer->getPosition(); //for squares
			
		//	if (r.containsPoint(local)) //for squares
			if (local.distance(ball->getPosition()) <= r.size.width/2)
			{
				if (mCurrentDivisor && ball->getNumber() % mCurrentDivisor == 0)
					divideBall(ball);
				else
					missBall(ball);
				
				break;
			}
		}
	}
	
	return true;
}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You moved id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You ended move id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button press %d", (int) keyCode);
	
}

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button release %d", (int) keyCode);
	
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (AppDelegate::pluginAnalytics != nullptr)
			AppDelegate::pluginAnalytics->logEvent("click_back_btn");
		
		pauseGame();
	}
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_VOLUME_DOWN)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->decreaseVolume();
	}
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_VOLUME_UP)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->increaseVolume();
	}
}

void GameScene::onComeToForeground()
{
	
}

void GameScene::onComeToBackground()
{
	if (mSceneState == GAME_SCENE_STATES::PLAY)
		pauseGame();
}

bool GameMode1Scene::init()
{
	if (!GameScene::init())
		return false;
	
	mDivisorMin = 1;
	
	float dw = (mTopBar->getContentSize().width - 20) / (mDivisorMax - mDivisorMin);
	std::function<void(cocos2d::Ref*)> callback = [this](cocos2d::Ref* node) {
		cocos2d::MenuItem* menuItem = dynamic_cast<cocos2d::MenuItem*>(node);
		if (menuItem == nullptr)
			return;
		
		updateDivisor(menuItem->getTag());
	//	cocos2d::Label* divisor = dynamic_cast<cocos2d::Label*>(menuItem->getChildren().front());
	//	if (divisor == nullptr)
	//		return;
	};
	for (int i = mDivisorMin; i < mDivisorMax; i++)
	{
		cocos2d::Label* label = cocos2d::Label::createWithTTF(cocos2d::__String::createWithFormat("%d", DIVISORS[i])->_string, "fonts/semibold.otf", 12);
		cocos2d::MenuItemLabel* divisor = cocos2d::MenuItemLabel::create(label, callback);
		label->setPosition(cocos2d::Vec2(dw/2, mTopBar->getContentSize().height/2));
		label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		divisor->setPosition((i - mDivisorMin) * dw, mMenu->getContentSize().height);
		divisor->setContentSize(cocos2d::Size(dw, mTopBar->getContentSize().height));
		divisor->setAnchorPoint(cocos2d::Vec2(0, 1));
		divisor->setTag(DIVISORS[i]);
		mMenu->addChild(divisor);
	}
	
	mSpawnInterval = 1.8f;
	
	updateDivisor(6);
	
	if (AppDelegate::pluginAnalytics != nullptr)
	{
		cocos2d::plugin::LogEventParamMap params;
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_GAME_MODE_INDEX, "mode_1"));
		AppDelegate::pluginAnalytics->logPageView("game", &params);
	}
	
	return true;
}

void GameMode1Scene::endGame(int nr)
{
	GameScene::endGame(nr);
	
	DieOverlay* overlay = DieOverlay::create(nr, mScore);
	overlay->restartCallback = CC_CALLBACK_0(GameScene::restartGame, this);
	overlay->exitCallback = CC_CALLBACK_0(GameScene::exitGame, this);
	addChild(overlay, 1000);
	
	if (AppDelegate::pluginAnalytics != nullptr)
	{
		cocos2d::plugin::LogEventParamMap params;
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_GAME_MODE_INDEX, "game_mode_1"));
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_SCORE_INDEX, helpers::String::format("%d", mScore)));
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_DIE_NUMBER_INDEX, helpers::String::format("%d", nr)));
		params.insert(cocos2d::plugin::LogEventParamPair("label", helpers::String::format("scored_%d", mScore)));
		params.insert(cocos2d::plugin::LogEventParamPair("category", "died"));
		
		AppDelegate::pluginAnalytics->logEvent("finished_level", &params);
	}
	
	int highscore = cocos2d::UserDefault::getInstance()->getIntegerForKey("highscore_mode_1", 0);
	if (highscore < mScore)
	{
		cocos2d::UserDefault::getInstance()->setIntegerForKey("highscore_mode_1", mScore);
		
		if (mIsGameServicesAvailable)
			AppDelegate::pluginGameServices->publishScore(LEADERBOARD_MODE_1_ID, mScore);
	}
}

void GameMode1Scene::updateDivisor(int d)
{
	mCurrentDivisor = d;
	
	cocos2d::Vector<cocos2d::Node*> items = mMenu->getChildren();
	if (items.size() - 1 /* pause btn */ != (mDivisorMax - mDivisorMin))
	{
		cocos2d::log("ERROR: wrong number of items in divisors menu. (%d vs %d required)", (int) items.size(), (mDivisorMax - mDivisorMin));
		AppDelegate::closeApp();
		return;
	}
	
	for (auto it = items.begin(); it != items.end(); it++)
	{
		cocos2d::Node* item = (*it);
		cocos2d::Label* label = dynamic_cast<cocos2d::Label*>(item->getChildren().front());
		
		if (label == nullptr)
			continue;
		
		if (mCurrentDivisor == (*it)->getTag())
		{
			label->setScale(1.0f);
			label->setColor(cocos2d::Color3B::WHITE);
		}
		else
		{
			label->setScale(0.7f);
			label->setColor(cocos2d::Color3B::GRAY);
		}
	}
}

void GameMode1Scene::spawnBall()
{
	Ball* ball = mBallPool.obtainPoolItem();
	ball->setPosition(rand() % (int) mScreenSize.width , rand() % (int) mScreenSize.height);
	ball->setNumber(getNumber());
	ball->setColor(cocos2d::Color3B(55+rand() % 200, 55+rand() % 200, 55+rand() % 200));
	ball->setVisible(true);
	ball->setLocalZOrder(mBallZOrder--);
	ball->runAction(BallAction::create(CC_CALLBACK_0(GameMode1Scene::ballPopCallback, this, ball)));
	mBalls.pushBack(ball);
}

void GameMode1Scene::divideBall(Ball* ball)
{
	GameScene::divideBall(ball);
	
	mBallPool.recyclePoolItem(ball);
	mBalls.eraseObject(ball);
	
	mScore++;
	updateScore();
}

void GameMode1Scene::missBall(Ball* ball)
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("die.wav");
	ball->setScale(ball->getScale() + 0.1f);
}

void GameMode1Scene::ballPopCallback(Ball* ball)
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("die.wav");
	
	endGame(ball->getNumber());
}

GameScene* GameMode1Scene::clone() const
{
	return GameMode1Scene::create();
}

bool GameMode2Scene::init()
{
	if (!GameScene::init())
		return false;
	
	cocos2d::Sprite* divisorBg = cocos2d::Sprite::createWithSpriteFrameName("divisor_bg");
	divisorBg->setPosition(mTopBar->getContentSize().width/2, mTopBar->getContentSize().height/2);
	mTopBar->addChild(divisorBg);
	
	mCurrentDivisorLabel = cocos2d::Label::createWithTTF("", "fonts/semibold.otf", 8);
	mCurrentDivisorLabel->setPosition(mTopBar->getContentSize().width * 0.5f, mTopBar->getContentSize().height * 0.52f);
	mCurrentDivisorLabel->enableShadow(cocos2d::Color4B::BLACK, cocos2d::Size(0.25f, -0.25f));
	mTopBar->addChild(mCurrentDivisorLabel);
	
	mCurrentDivisor = -1;
	mLastBallSpawnX = -1;
	
	if (mIsGameServicesAvailable)
		AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_PLAY_GAME_2);
	
	return true;
}

void GameMode2Scene::update(float dt)
{
	GameScene::update(dt);
	
	mGameLayer->setPositionY(mGameLayer->getPositionY() + dt * mBallSpeed);
}

void GameMode2Scene::updateSlow(float dt)
{
	GameScene::updateSlow(dt);
	
	if (mBalls.size() > 0)
	{
		while (!mBalls.empty())
		{
			Ball* ball = mBalls.front();
			if (ball->getPositionY() > -mGameLayer->getPositionY() + mScreenSize.height + ball->getContentSize().height/2)
			{
				mBallPool.recyclePoolItem(ball);
				mBalls.eraseObject(ball);
				
				if (ball->getNumber() % mCurrentDivisor == 0)
					missBall(ball, false);
			}
			else
				break;
		}
	}
}

void GameMode2Scene::updateDivisor(int d)
{
	mCurrentDivisor = d;
	mCurrentDivisorLabel->setString(cocos2d::__String::createWithFormat("%d", mCurrentDivisor)->_string);
}

void GameMode2Scene::spawnBall()
{
	Ball* ball = mBallPool.obtainPoolItem();
	ball->setNumber(getNumber());
	ball->setVisible(true);
	ball->setLocalZOrder(mBallZOrder--);
	mBalls.pushBack(ball);
	
	float posX = -1, posX_;
	float posY = -mGameLayer->getPositionY() - ball->getContentSize().height/2;
	int tries = 0;
	get_pos:
	posX_ = ball->getContentSize().width/2 + rand() % (int) (mGameLayer->getContentSize().width - ball->getContentSize().width);
	if (posX < 0)
		posX = posX_;
	if (mLastBallSpawnX >= 0 && std::abs(mLastBallSpawnX - posX) < 30)
	{
		if (std::abs(mLastBallSpawnX - posX) < std::abs(mLastBallSpawnX - posX_))
			posX = posX_;
		
		tries++;
		if (tries < 5)
			goto get_pos;
	}
	
	mLastBallSpawnX = posX;
	ball->setPosition(posX, posY);
}

void GameMode2Scene::divideBall(Ball* ball)
{
	GameScene::divideBall(ball);
	
	mBallPool.recyclePoolItem(ball);
	mBalls.eraseObject(ball);
}

void GameMode2Scene::missBall(Ball* ball)
{
	missBall(ball, true);
}

bool GameMode2InfiniteScene::init()
{
	if (!GameMode2Scene::init())
		return false;
	
//	mPreviousDivisorLabel = cocos2d::Label::createWithTTF("", "fonts/semibold.otf", 6);
//	mPreviousDivisorLabel->setPosition(mTopBar->getContentSize().width * 0.3f, mTopBar->getContentSize().height/2);
//	mTopBar->addChild(mPreviousDivisorLabel);
	
	mNextDivisorLabel = cocos2d::Label::createWithTTF("", "fonts/semibold.otf", 5);
	mNextDivisorLabel->setPosition(mTopBar->getContentSize().width * 0.68f, mTopBar->getContentSize().height/2);
	mNextDivisorLabel->setVisible(false);
	mTopBar->addChild(mNextDivisorLabel);
	
	mWaveTimer = 0;
	mWaveLength = 30.0f;
	
	mWaveNumber = cocos2d::UserDefault::getInstance()->getBoolForKey("skip_tutorial", false) ? 1 : 0;
	mCurrentDivisor = -1;
	mNextDivisor = -1;
	
	cocos2d::Label* waveTitle = cocos2d::Label::createWithTTF("WAVE", "fonts/semibold.otf", 5);
	waveTitle->setPosition(cocos2d::Vec2(12, mUILayer->getContentSize().height - 1));
	waveTitle->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
	mUILayer->addChild(waveTitle);
	
	mWaveLabel = cocos2d::Label::createWithTTF("", "fonts/semibold.otf", 9);
	mWaveLabel->setPosition(cocos2d::Vec2(
			waveTitle->getPosition().x - waveTitle->getContentSize().width * waveTitle->getAnchorPoint().x + waveTitle->getContentSize().width * 0.5f,
			waveTitle->getPosition().y - waveTitle->getContentSize().height * waveTitle->getAnchorPoint().y - 1.0f));
	mWaveLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
	mWaveLabel->enableShadow(cocos2d::Color4B::BLACK, cocos2d::Size(0.25f, -0.25f));
	mUILayer->addChild(mWaveLabel);
	
	setDivisorRange();
	
	if (AppDelegate::pluginAnalytics != nullptr)
	{
		cocos2d::plugin::LogEventParamMap params;
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_GAME_MODE_INDEX, "mode_2"));
		AppDelegate::pluginAnalytics->logPageView("game", &params);
	}
	
	return true;
}

void GameMode2InfiniteScene::endGame(int nr)
{
	GameMode2Scene::endGame(nr);
	
	DieOverlay* overlay = DieOverlay::create(nr, mScore);
	overlay->restartCallback = CC_CALLBACK_0(GameScene::restartGame, this);
	overlay->exitCallback = CC_CALLBACK_0(GameScene::exitGame, this);
	addChild(overlay, 1000);
	
	if (AppDelegate::pluginAnalytics != nullptr)
	{
		cocos2d::plugin::LogEventParamMap params;
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_GAME_MODE_INDEX, "game_mode_2"));
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_WAVE_INDEX, helpers::String::format("%d", mWaveNumber)));
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_SCORE_INDEX, helpers::String::format("%d", mScore)));
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_DIE_NUMBER_INDEX, helpers::String::format("%d", nr)));
		params.insert(cocos2d::plugin::LogEventParamPair("label", helpers::String::format("scored_%d", mScore)));
		params.insert(cocos2d::plugin::LogEventParamPair("category", "died"));
		
		AppDelegate::pluginAnalytics->logEvent("finished_level", &params);
	}
	
	//TODO: preload highscores, and maybe save them in bg, or in other thread .. ?
	int highscore = cocos2d::UserDefault::getInstance()->getIntegerForKey("highscore_mode_2", 0);
	if (highscore < mScore)
	{
		cocos2d::UserDefault::getInstance()->setIntegerForKey("highscore_mode_2", mScore);
		
		if (mIsGameServicesAvailable)
			AppDelegate::pluginGameServices->publishScore(LEADERBOARD_MODE_2_ID, mScore);
	}
	
	int maxWave = cocos2d::UserDefault::getInstance()->getIntegerForKey("max_wave", 0);
	if (maxWave < mWaveNumber)
		cocos2d::UserDefault::getInstance()->setIntegerForKey("max_wave", mWaveNumber);
}

void GameMode2InfiniteScene::update(float dt)
{
	if (mWaveTimer == 0) // if starting new wave, wait for all balls to go out of the screen
	{
		if (mBalls.size() == 0)
		{
			startWave();
			
			mSpawnTimer = mSpawnInterval;
			mWaveTimer += dt;
		}
		else //stall
		{
			mSpawnTimer -= mSpawnInterval;
		}
	}
	else
	{
		mWaveTimer += dt;
	}
	
	GameMode2Scene::update(dt);
	
	if (mWaveTimer >= mWaveLength)
	{
		mWaveTimer = 0;
		
		endWave();
	}
}

void GameMode2InfiniteScene::updateSlow(float dt)
{
	GameMode2Scene::updateSlow(dt);
	
}

void GameMode2InfiniteScene::startWave()
{
	if (mWaveNumber == 0)
		mWaveLength = 20;
	else
		mWaveLength = 30;
	
	if (mWaveNumber == 0)       // difficulty for wave 0
	{
		mBallSpeed = 8.0f;
		mSpawnInterval = 4.0f;
	}
	else if (mWaveNumber <= 1)  // difficulty for wave 1
	{
		mBallSpeed = 10.0f;
		mSpawnInterval = 3.0f;
	}
	else if (mWaveNumber <= 3)  // difficulty for wave 2, 3
	{
		mBallSpeed = 15.0f;
		mSpawnInterval = 2.5f;
	}
	else if (mWaveNumber <= 6)  // difficulty for wave 4, 5, 6
	{
		mBallSpeed = 20.0f;
		mSpawnInterval = 2.0f;
	}
	else if (mWaveNumber <= 10) // difficulty for wave 7, 8, 9, 10
	{
		mBallSpeed = 25.0f;
		mSpawnInterval = 1.5f;
	}
	else                        // difficulty for wave 11+
	{
		mBallSpeed = 30.0f;
		mSpawnInterval = 1.2f;
	}
	
	if (mNextDivisor == -1) // first wave
	{
		mNextDivisor = DIVISORS[mDivisorMin + rand() % (mDivisorMax - mDivisorMin)];
	}
	
	mNextDivisorLabel->setVisible(false);
	updateDivisor(mNextDivisor);
	
	mWaveLabel->setString(cocos2d::__String::createWithFormat("%d", mWaveNumber)->_string);
	
	if (mWaveNumber > 1)
	{
		mScore += 5;
		updateScore();
	}
}

void GameMode2InfiniteScene::endWave()
{
	//TODO: move end wave (inc mWaveNumber) logic after all balls are gone (maybe)
	if (mWaveNumber == 0)
		cocos2d::UserDefault::getInstance()->setBoolForKey("skip_tutorial", true);
	
	if (mIsGameServicesAvailable)
	{
		if (mWaveNumber == 0)
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_1_WAVE);
		else if (mWaveNumber == 5)
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_5_WAVES);
		else if (mWaveNumber == 10)
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_10_WAVES);
		else if (mWaveNumber == 50)
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_50_WAVES);
	}
	
	mWaveNumber++;
	
	setDivisorRange();
	
	pick_number:
	int d = DIVISORS[mDivisorMin + rand() % (mDivisorMax - mDivisorMin)];
	if ((d == mCurrentDivisor || d == mPreviousDivisor) && (mDivisorMax - mDivisorMin) > 2)
		goto pick_number;
	
	mNextDivisor = d;
	mNextDivisorLabel->setVisible(true);
	mNextDivisorLabel->setOpacity(0);
	mNextDivisorLabel->setString(cocos2d::__String::createWithFormat("Next: %d", d)->_string);
	mNextDivisorLabel->runAction(cocos2d::RepeatForever::create(cocos2d::Sequence::create(
			cocos2d::FadeIn::create(1.0f), cocos2d::FadeOut::create(0.5f), nullptr)));
	
	cocos2d::log("selected divisor %d", d);
}

void GameMode2InfiniteScene::setDivisorRange()
{
	if (mWaveNumber == 0)       // difficulty for wave 0
	{
		mDivisorMin = 0;
		mDivisorMax = 1;
	}
	else if (mWaveNumber <= 1)  // difficulty for wave 1
	{
		mDivisorMin = 0;
		mDivisorMax = 3;
	}
	else if (mWaveNumber <= 3)  // difficulty for wave 2, 3
	{
		mDivisorMin = 1;
		mDivisorMax = 4;
	}
	else if (mWaveNumber <= 6)  // difficulty for wave 4, 5, 6
	{
		mDivisorMin = 2;
		mDivisorMax = 5;
	}
	else if (mWaveNumber <= 10) // difficulty for wave 7, 8, 9, 10
	{
		mDivisorMin = 2;
		mDivisorMax = 7;
	}
	else                        // difficulty for wave 11+
	{
		mDivisorMin = 2;
		mDivisorMax = 8;
	}
}

void GameMode2InfiniteScene::updateDivisor(int d)
{
	if (mBalls.size() > 0)
		cocos2d::log("WARNING: Screen is not empty! Divisor shouldn't change while balls are still on screen!");
	
	mPreviousDivisor = mCurrentDivisor;
	
//	if (mPreviousDivisor > 0)
//		mPreviousDivisorLabel->setString(cocos2d::__String::createWithFormat("%d", mPreviousDivisor)->_string);
	mNextDivisorLabel->setVisible(false);
	mNextDivisorLabel->stopAllActions();
	
	GameMode2Scene::updateDivisor(d);
}

void GameMode2InfiniteScene::divideBall(Ball* ball)
{
	GameMode2Scene::divideBall(ball);
	
	mScore++;
	updateScore();
}

void GameMode2InfiniteScene::missBall(Ball* ball, bool manual)
{
	GameScene::missBall(ball);
	
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("die.wav");
	
	endGame(ball->getNumber());
}

GameScene* GameMode2InfiniteScene::clone() const
{
	return GameMode2InfiniteScene::create();
}

GameMode2LevelScene* GameMode2LevelScene::create(Level* level)
{
	GameMode2LevelScene* scene = new (std::nothrow) GameMode2LevelScene();
	
	if (scene && scene->initWithLevelNumber(level))
	{
		scene->autorelease();
		return scene;
	}
	else
	{
		delete scene;
		scene = nullptr;
		return nullptr;
	}
}

bool GameMode2LevelScene::initWithLevelNumber(Level* level)
{
	if (!GameMode2Scene::init())
		return false;
	
	mLevel = level;
	mLevelTimer = 60;
	
	mCurrentDivisor = mLevel->getDivisor();
	mNumbersSize = mLevel->getNrDivisible() + mLevel->getNrIndivisible();
	mNumbersIndex = 0;
	
	mNumbers = (int*) malloc(mNumbersSize * sizeof(int));
	
	if (!mNumbers)
		return false;
	
	for (int i = 0; i < mLevel->getNrDivisible();)
	{
		int x = NUMBER_POOL[rand() % NUMBER_POOL_SIZE];
		if (x % mCurrentDivisor == 0)
			mNumbers[i++] = x;
	}
	
	for (int i = 0; i < mLevel->getNrIndivisible();)
	{
		int x = NUMBER_POOL[rand() % NUMBER_POOL_SIZE];
		if (x % mCurrentDivisor != 0)
			mNumbers[mLevel->getNrDivisible() + i++] = x;
	}
	
	helpers::Number::shuffle(mNumbers, mNumbersSize);
	
	mBallSpeed = mLevel->getSpeed();
	mSpawnInterval = (mLevelTimer-10) / mNumbersSize;
	
	cocos2d::Label* timeTitle = cocos2d::Label::createWithTTF("TIME", "fonts/semibold.otf", 5);
	timeTitle->setPosition(cocos2d::Vec2(12, mUILayer->getContentSize().height - 1));
	timeTitle->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
	mUILayer->addChild(timeTitle);
	
	mTimerLabel = cocos2d::Label::createWithTTF("", "fonts/semibold.otf", 9);
	mTimerLabel->setPosition(cocos2d::Vec2(
			timeTitle->getPosition().x - timeTitle->getContentSize().width * timeTitle->getAnchorPoint().x + timeTitle->getContentSize().width * 0.5f,
			timeTitle->getPosition().y - timeTitle->getContentSize().height * timeTitle->getAnchorPoint().y - 1.0f));
	mTimerLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
	mTimerLabel->enableShadow(cocos2d::Color4B::BLACK, cocos2d::Size(0.25f, -0.25f));
	mUILayer->addChild(mTimerLabel);
	
	cocos2d::Label* tutorial1 = cocos2d::Label::createWithTTF(
			helpers::String::format("THE CURRENT DIVISOR IS\n%d", mLevel->getDivisor()),
			"fonts/semibold.otf", 6, cocos2d::Size::ZERO, cocos2d::TextHAlignment::CENTER);
	tutorial1->setLineSpacing(2.0f);
	tutorial1->setScale(0.25f);
	tutorial1->setOpacity(0);
	tutorial1->setPosition(mUILayer->getContentSize().width * 0.5f, mUILayer->getContentSize().height * 0.55f);
	tutorial1->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
	tutorial1->enableShadow(cocos2d::Color4B::BLACK, cocos2d::Size(0.25f, -0.25f));
	mUILayer->addChild(tutorial1);
	
	tutorial1->runAction(cocos2d::Sequence::create(
			cocos2d::FadeIn::create(0.5f),
			cocos2d::DelayTime::create(4.0f),
			cocos2d::FadeOut::create(0.5f),
			cocos2d::DelayTime::create(0.5f),
			cocos2d::RemoveSelf::create(),
			nullptr));
	
	tutorial1->runAction(cocos2d::Sequence::create(
			cocos2d::ScaleTo::create(0.5f, 1.0f),
			cocos2d::DelayTime::create(4.0f),
			cocos2d::ScaleTo::create(0.5f, 2.5f),
			nullptr));
	
	cocos2d::Label* tutorial2 = cocos2d::Label::createWithTTF("TAP THE NUMBERS DIVISIBLE BY IT", "fonts/semibold.otf", 5);
	tutorial2->setScale(0.25f);
	tutorial2->setOpacity(0);
	tutorial2->setPosition(mUILayer->getContentSize().width * 0.5f, mUILayer->getContentSize().height * 0.50f);
	tutorial2->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
	tutorial2->enableShadow(cocos2d::Color4B::BLACK, cocos2d::Size(0.25f, -0.25f));
	mUILayer->addChild(tutorial2);
	
	tutorial2->runAction(cocos2d::Sequence::create(
			cocos2d::DelayTime::create(1.5f),
			cocos2d::FadeIn::create(0.5f),
			cocos2d::DelayTime::create(2.6f),
			cocos2d::FadeOut::create(0.5f),
			cocos2d::DelayTime::create(0.5f),
			cocos2d::RemoveSelf::create(),
			nullptr));
	
	tutorial2->runAction(cocos2d::Sequence::create(
			cocos2d::DelayTime::create(1.5f),
			cocos2d::ScaleTo::create(0.5f, 1.0f),
			cocos2d::DelayTime::create(2.6f),
			cocos2d::ScaleTo::create(0.5f, 2.5f),
			nullptr));
	
	updateDivisor(mCurrentDivisor);
	
	if (AppDelegate::pluginAnalytics != nullptr)
	{
		cocos2d::plugin::LogEventParamMap params;
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_GAME_MODE_INDEX, "mode_3"));
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_LEVEL_INDEX, helpers::String::format("%d", mLevel->getId())));
		AppDelegate::pluginAnalytics->logPageView("game", &params);
	}
	
	return true;
}

void GameMode2LevelScene::endGame(int nr)
{
	GameMode2Scene::endGame(nr);
	
	if (nr)
	{
		DieOverlay* overlay = DieOverlay::create(nr);
		overlay->restartCallback = CC_CALLBACK_0(GameScene::restartGame, this);
		overlay->exitCallback = CC_CALLBACK_0(GameScene::exitGame, this);
		addChild(overlay, 1000);
	}
	else
	{
		FinishOverlay* overlay = FinishOverlay::create(mScore, mLevel->getId() % 100, mLevel->getStars());
		overlay->restartCallback = CC_CALLBACK_0(GameScene::restartGame, this);
		overlay->nextLevelCallback = [this] () {
			int nr = mLevel->getId() % 100;
			
			if (nr < LevelSelectScene::LEVEL_NR)
			{
				Level* level = &LevelSelectScene::LEVELS[nr];
				cocos2d::Director::getInstance()->replaceScene(GameMode2LevelScene::create(level));
			}
			else
				exitGame();
		};
		overlay->exitCallback = CC_CALLBACK_0(GameScene::exitGame, this);
		addChild(overlay, 1000);
	}
	
	if (AppDelegate::pluginAnalytics != nullptr)
	{
		cocos2d::plugin::LogEventParamMap params;
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_GAME_MODE_INDEX, "game_mode_3"));
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_LEVEL_INDEX, helpers::String::format("%d", mLevel->getId())));
		params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_SCORE_INDEX, helpers::String::format("%d", mScore)));
		params.insert(cocos2d::plugin::LogEventParamPair("label", helpers::String::format("scored_%d", mScore)));
		
		if (nr)
		{
			params.insert(cocos2d::plugin::LogEventParamPair(ANALYTICS_DIE_NUMBER_INDEX, helpers::String::format("%d", nr)));
			params.insert(cocos2d::plugin::LogEventParamPair("category", "died"));
		}
		else
			params.insert(cocos2d::plugin::LogEventParamPair("category", "finished"));
		
		AppDelegate::pluginAnalytics->logEvent("finished_level", &params);
	}
	
	if (mIsGameServicesAvailable && !nr)
	{
		if (mLevel == &LevelSelectScene::LEVELS[0])
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_FIRST_LEVEL);
		else if (mLevel == &LevelSelectScene::LEVELS[LevelSelectScene::LEVEL_NR-1])
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_ALL_LEVELS);
		
		if (mLevel->getStars() == 3)
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_FIRST_3_STARS);
		
		if (Level::getGlobalStars() >= LevelSelectScene::LEVEL_NR * 3)
			AppDelegate::pluginGameServices->unlockAchievement(ACHIEVEMENT_ALL_3_STARS);
	}
}

int GameMode2LevelScene::getNumber()
{
	if (mNumbersIndex < mNumbersSize)
		return mNumbers[mNumbersIndex++];
	
	return -1;
}

void GameMode2LevelScene::update(float dt)
{
	if (mNumbersIndex >= mNumbersSize)
		mSpawnTimer -= mSpawnInterval; //stall
	
	GameMode2Scene::update(dt);
	
	int oldTimer = mLevelTimer;
	mLevelTimer -= dt;
	if (oldTimer != (int) mLevelTimer && mLevelTimer >= 0)
		mTimerLabel->setString(cocos2d::__String::createWithFormat("%d", (int) mLevelTimer)->_string);
	
	if (mLevelTimer <= 0)
	{
		mLevel->setScore(mScore);
		
		endGame(false);
	}
}

void GameMode2LevelScene::updateSlow(float dt)
{
	GameMode2Scene::updateSlow(dt);
	
}

void GameMode2LevelScene::divideBall(Ball* ball)
{
	GameMode2Scene::divideBall(ball);
	
	mScore++;
	updateScore();
}

void GameMode2LevelScene::missBall(Ball* ball, bool manual)
{
	GameScene::missBall(ball);
	
	if (manual)
	{
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("die.wav");
		
		endGame(ball->getNumber());
	}
	else
	{
		//TODO: maybe affect score
	}
}

GameScene* GameMode2LevelScene::clone() const
{
	return GameMode2LevelScene::create(mLevel);
}
