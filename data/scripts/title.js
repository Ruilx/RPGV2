// title.js
var init = function(){
    RpgBanner.setBackgroundColor("#000000");
    RpgBanner.setForegroundPixmap("kuroiduki");
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

    RpgLyric.loadLyric("demo");

    RpgMusic.setLoop(1);
    RpgMusic.playMusic("demo");
    RpgMusic.setVolume(50);
    RpgLyric.exec();

    RpgChoice.addChoiceText("第一个选项");
    RpgChoice.addChoiceText("第二个选项");
    RpgChoice.addChoiceText("第三个选项");
    RpgChoice.addChoiceText("第四个选项");
    RpgChoice.addChoiceText("第五个选项");
    RpgChoice.addChoiceText("第六个选项");
    RpgChoice.addChoiceText("第七个选项");
    RpgChoice.addChoiceText("第八个选项");
    RpgChoice.addChoiceText("第九个选项");
    RpgChoice.setChoiceEnabled(1, false);
    RpgChoice.exec();
    var c = RpgChoice.waitingForChooseComplete();

    RpgDialog.addText("你选择了第" + (c + 1) + "个选项");
    RpgDialog.exec();
    RpgDialog.waitingForDialogComplete();




//    for(var i = 0; i < 50; i++){
//        RpgDialog.addText("<span style='color:black'>=====================================</span>");
//        RpgDialog.exec();
//        RpgDialog.waitingForDialogComplete();
//    }

    return "exit";
}

init();
