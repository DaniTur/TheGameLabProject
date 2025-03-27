#pragma once
#include <GLFW/glfw3.h>

class FPSManager {
public:

	static float Calculate() {
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrameTime;
		lastFrameTime = currentFrame;

		if (m_secondCounter <= 1) {
			m_secondCounter += deltaTime;
			m_tempFps++;
		}
		else
		{
			//"fps" are the actual fps
			fps = m_tempFps;
			m_secondCounter = 0;
			m_tempFps = 0;
		}
		return static_cast<float>(fps);
	}

	static double GetDeltaTime() {
		return deltaTime;
	}

private:
	inline static double deltaTime = 0.0;	// Time between current frame and last frame
	inline static double lastFrameTime = 0.0; // Time of last frame
	inline static double m_secondCounter = 0.0;
	inline static double m_tempFps = 0.0;	//These are not the real fps, just temporary
	inline static double fps = 0.0;	//This float is the fps we should use
};