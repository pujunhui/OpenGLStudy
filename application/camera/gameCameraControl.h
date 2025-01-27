#pragma once 

#include "../../glframework/core.h"
#include "cameraControl.h"

class GameCameraControl :public CameraControl {
public:
	GameCameraControl();
	~GameCameraControl();

    void onCurosr(double xpos, double ypos) override;
    void onScroll(double offset) override;
	void update() override;

	void pitch(float angle);
	void yaw(float angle);

	void setSpeed(float speed) {
		mSpeed = speed;
	}

private:
    float mPitch{ 0.0f };
    float mSpeed{ 0.01f };
};