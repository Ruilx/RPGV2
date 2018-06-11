// title.js
var init = function(){
	RpgBanner.setBackgroundColor("#000000");
	RpgBanner.setForegroundPixmap("data/images/background/title2.png");
	RpgBanner.setStartOpacity(0);
	RpgBanner.setEndOpacity(1);
	RpgBanner.exec();
	RpgBanner.waitingForBannerComplete();
	RpgUtils.msleep(2000);
	RpgBanner.setStartOpacity(1);
	RpgBanner.setEndOpacity(0);
	RpgBanner.execExit();
	RpgBanner.waitingForBannerComplete();
	RpgUtils.msleep(2000);

	RpgMusic.playMusic("op1");
	return "exit";
}

init();
