#pragma once 

#include "cameraControl.h"

class GameCameraControl :public CameraControl {
public:
	GameCameraControl();
	~GameCameraControl();

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
    float mPitch{ 0.0f };
    float mSpeed{ 0.01f };
};