#include "AppDelegate.h"
#include "MainScene.h"
#include "Include.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size( 1920, 1080 );
static cocos2d::Size smallResolutionSize = cocos2d::Size( 960, 540 );
static cocos2d::Size mediumResolutionSize = cocos2d::Size( 1280, 720 );
static cocos2d::Size largeResolutionSize = cocos2d::Size( 1920, 1080 );

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs( glContextAttrs );
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	// Initialize director
	Director* director = Director::getInstance();
	GLView* glview = director->getOpenGLView();
	if( !glview ) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect( "XBalloon", Rect( 0, 0, mediumResolutionSize.width, mediumResolutionSize.height ) );
#else
		glview = GLViewImpl::create("XBalloon");
#endif
		director->setOpenGLView( glview );
	}

	// Set the design resolution
	glview->setDesignResolutionSize( designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER );

	std::vector<std::string> resOrders;
	Size realSize = glview->getFrameSize();
	if( realSize.width>mediumResolutionSize.width || realSize.height>mediumResolutionSize.height ) {
		resOrders.push_back( "res/fullhd" );
		director->setContentScaleFactor( MIN( largeResolutionSize.width/designResolutionSize.width, largeResolutionSize.height/designResolutionSize.height ) );
	}
	else if( realSize.width>smallResolutionSize.width || realSize.height>smallResolutionSize.height ) {
		resOrders.push_back( "res/hd" );
		director->setContentScaleFactor( MIN( mediumResolutionSize.width/designResolutionSize.width, mediumResolutionSize.height/designResolutionSize.height ) );
	}
	else {
		resOrders.push_back( "res/sd" );
		director->setContentScaleFactor( MIN( smallResolutionSize.width/designResolutionSize.width, smallResolutionSize.height/designResolutionSize.height ) );
	}
	resOrders.push_back( "res" );
	FileUtils::getInstance()->setSearchResolutionsOrder( resOrders );

	// Set SDKBox
	register_all_packages();

	// Preload audio
	PRELOAD_AUDIO( "bg.mp3" );
	PRELOAD_AUDIO( "pop.mp3" );

	// Run
	Scene* scene = MainScene::createScene();
	director->runWithScene( scene );

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
