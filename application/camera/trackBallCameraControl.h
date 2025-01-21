#pragma once 

#include "../../glframework/core.h"
#include "cameraControl.h"

class TrackBallCameraControl :public CameraControl {
public:
    TrackBallCameraControl();
	~TrackBallCameraControl();

    void onCurosr(double xpos, double ypos) override;
    void onScroll(double offset) override;

private:
	void pitch(float angle);
	void yaw(float angle);


private:
	float mMoveSpeed = 0.005f;
};