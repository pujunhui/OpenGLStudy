#pragma once 

#include "../../glframework/core.h"
#include "cameraControl.h"

class GameCameraControl :public CameraControl {
public:
	GameCameraControl();
	~GameCameraControl();

	void onMouse(int button, int action, double xpos, double ypos) override;
	void update() override;

	void pitch(float angle);
	void yaw(float angle);

	void setSpeed(float speed) {
		mSpeed = speed;
	}

private:
	float mSpeed = 1.0f;
};