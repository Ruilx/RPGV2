// title.js
var init = function(){
	RpgBanner.setBackgroundColor("#000000");
    RpgBanner.setForegroundPixmap("data/images/background/lyrictest.png");
	RpgBanner.setStartOpacity(0);
	RpgBanner.setEndOpacity(1);
	RpgBanner.exec();
	RpgBanner.waitingForBannerComplete();
	RpgUtils.msleep(2000);
//    RpgBanner.setStartOpacity(1);
//	RpgBanner.setEndOpacity(0);
//	RpgBanner.execExit();
//	RpgBanner.waitingForBannerComplete();
//	RpgUtils.msleep(2000);

    RpgMusic.setLoop(1);
    RpgMusic.playMusic("nero - カンタレラ~grace edition~");

//    for(var i = 0; i < 50; i++){
//        RpgDialog.addText("<span style='color:black'>=====================================</span>");
//        RpgDialog.exec();
//        RpgDialog.waitingForDialogComplete();
//    }

    return "exit";
}

init();
