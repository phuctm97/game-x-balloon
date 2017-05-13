#ifndef __INCLUDE_HEADER__
#define __INCLUDE_HEADER__

#include <cocos2d.h>
#include <AudioEngine.h>

#define DIRECTOR cocos2d::Director::getInstance()
#define SIZE_VISIBLE DIRECTOR->getVisibleSize()
#define ORIGIN_VISIBLE DIRECTOR->getVisibleOrigin()
#define EVENT_DISPATCHER DIRECTOR->getEventDispatcher()
#define SPRITE_CACHE cocos2d::SpriteFrameCache::getInstance()

#define PRELOAD_AUDIO(__FILE__) cocos2d::experimental::AudioEngine::preload(__FILE__)
#define PLAY_EFFECT(__FILE__) cocos2d::experimental::AudioEngine::play2d(__FILE__)
#define PLAY_EFFECT_VOL(__FILE__, __VOL__) cocos2d::experimental::AudioEngine::play2d(__FILE__, false, __VOL__)
#define PLAY_BACKGROUND(__FILE__) cocos2d::experimental::AudioEngine::play2d(__FILE__, true)
#define PLAY_BACKGROUND_VOL(__FILE__, __VOL__) cocos2d::experimental::AudioEngine::play2d(__FILE__, true, __VOL__)

enum Z_ORDERS
{
	BG0, BG1, MID0, MID1, MAIN0, MAIN1, UI0, UI1
};

#endif // !__INCLUDE_HEADER__
