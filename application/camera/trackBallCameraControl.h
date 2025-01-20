#pragma once 

#include "../../glframework/core.h"
#include "cameraControl.h"

class TrackBallCameraControl :CameraControl {
public:
    TrackBallCameraControl();
	~TrackBallCameraControl();

    void onCurosr(double xpos, double ypos) override;

private:
	void pitch(float angle);
	void yaw(float angle);


private:
	float mTranslateSpeed = 0.005f;
};