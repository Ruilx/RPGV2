// title.js
var init = function(){
//    RpgBanner.setBackgroundColor("#000000");
//    RpgBanner.setForegroundPixmap("kuroiduki");
//    RpgBanner.setStartOpacity(0);
//    RpgBanner.setEndOpacity(1);
//    RpgBanner.exec();
//    RpgBanner.waitingForBannerComplete();
//    RpgUtils.msleep(2000);
//    RpgBanner.setStartOpacity(1);
//	RpgBanner.setEndOpacity(0);
//	RpgBanner.execExit();
//	RpgBanner.waitingForBannerComplete();
//	RpgUtils.msleep(2000);

    RpgLyric.loadLyric("demo");

    RpgMusic.setLoop(1);
    RpgMusic.setVolume(50);
    RpgMusic.playMusic("demo");
    RpgLyric.exec();

//    RpgUtils.msleep(180000);

//    RpgChoice.addChoiceText("はじめから");
//    RpgChoice.addChoiceText("つづけから");
//    RpgChoice.addChoiceText("画像<sup>がぞう</sup>閲覧<sup>えいらん</sup>室<sup>しつ</sup>");
//    RpgChoice.addChoiceText("終わる");
//    RpgChoice.addChoiceText("第五个选项");
//    RpgChoice.addChoiceText("第六个选项");
//    RpgChoice.addChoiceText("第七个选项");
//    RpgChoice.addChoiceText("第八个选项");
//    RpgChoice.addChoiceText("第九个选项");
//    RpgChoice.setChoiceEnabled(1, false);
//    RpgChoice.exec();
//    var c = RpgChoice.waitingForChooseComplete();

    RpgDialog.addText("准备好了?");
    RpgDialog.exec();
    RpgDialog.waitingForDialogComplete();

    RpgScene.setScenePos(15 *32, 18 * 32);
    RpgScene.moveSceneToLocation(5 * 32, 18 * 32);
    RpgScene.waitingForMoveSceneComplete();
    RpgScene.moveSceneToLocation(5 * 32, 5 * 32);
    RpgScene.waitingForMoveSceneComplete();
    RpgScene.moveSceneToLocation(15 * 32, 5 * 32);
    RpgScene.waitingForMoveSceneComplete();
    RpgScene.moveSceneToLocation(15 * 32, 18 * 32);
    RpgScene.waitingForMoveSceneComplete();
    RpgScene.moveSceneToLocation(-5 * 32, -5 * 32);
    RpgScene.waitingForMoveSceneComplete();
    RpgScene.moveSceneToLocation(45 * 32, 45 * 32);
    RpgScene.waitingForMoveSceneComplete();

    RpgUtils.msleep(5000000);

//    for(var i = 0; i < 50; i++){
//        RpgDialog.addText("<span style='color:black'>=====================================</span>");
//        RpgDialog.exec();
//        RpgDialog.waitingForDialogComplete();
//    }

    return "exit";
}

init();
