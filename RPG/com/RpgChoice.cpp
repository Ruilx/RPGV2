#include "RpgChoice.h"



void RpgChoice::enterOrExitAnimationSetting(bool enter){
	if(this->entryAniGroup->state() != QParallelAnimationGroup::Stopped){
		this->entryAniGroup->stop();
	}
	this->entryAniGroup->clear();
	QPropertyAnimation *boxAnimation = new QPropertyAnimation(this->boxOpacityEffect, "opacity"); // 窗口的动画过渡效果
	if(enter){
		// Enter
		boxAnimation->setDuration(300);
		boxAnimation->setEasingCurve(QEasingCurve::OutQuad);
		boxAnimation->setStartValue(0.0f);
		boxAnimation->setEndValue(1.0f);
	}else{
		// Exit
		boxAnimation->setDuration(300);
		boxAnimation->setEasingCurve(QEasingCurve::OutQuad);
		boxAnimation->setStartValue(1.0f);
		boxAnimation->setEndValue(0.0f);
	}
	this->entryAniGroup->addAnimation(boxAnimation);
}

void RpgChoice::enterOrExitAnimationStart(){
	if(this->box->isVisible() == false){
		qWarning() << "Box current visible is false, cannot open animation.";
		return;
	}
	this->entryAniGroup->start(QAbstractAnimation::KeepWhenStopped);
}
