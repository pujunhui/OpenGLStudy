#pragma once 

#include "cameraControl.h"

class TrackBallCameraControl :public CameraControl {
public:
    TrackBallCameraControl();
	~TrackBallCameraControl();

    void onCursor(double xpos, double ypos) override;
    void onScroll(double offset) override;
	void update() override;

	void setSpeed(float speed) {
		mSpeed = speed;
	}

private:
	void pitch(float angle);
	void yaw(float angle);

private:
	float mMoveSpeed = 0.005f;
	float mSpeed{ 0.01f };
};