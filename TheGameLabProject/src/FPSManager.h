#pragma once
#include <GLFW/glfw3.h>

class FPSManager {
public:

	static float Calculate() {
		double currentFrame = glfwGetTime();
		s_DeltaTime = currentFrame - s_LastFrameTime;
		s_LastFrameTime = currentFrame;

		if (s_SecondCounter <= 1) {
			s_SecondCounter += s_DeltaTime;
			s_TempFps++;
		}
		else
		{
			//"fps" are the actual fps
			s_Fps = s_TempFps;
			s_SecondCounter = 0;
			s_TempFps = 0;
		}
		return static_cast<float>(s_Fps);
	}

	static double GetDeltaTime() {
		return s_DeltaTime;
	}

private:
	inline static double s_DeltaTime = 0.0;	// Time between current frame and last frame
	inline static double s_LastFrameTime = 0.0; // Time of last frame
	inline static double s_SecondCounter = 0.0;
	inline static double s_TempFps = 0.0;	//These are not the real fps, just temporary
	inline static double s_Fps = 0.0;	//This float is the fps we should use
};