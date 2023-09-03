#pragma once

#include "./core.hpp"

namespace Maya {

// An Easing function, both parameter and return value should be in range of [0, 1]
using EasingFunction = std::function<float(float)>;

// Linear function
float Linear(float x);

// Ease in functions
float EaseInSine(float x);
float EaseInCubic(float x);
float EaseInQuint(float x);
float EaseInCirc(float x);
float EaseInElastic(float x);
float EaseInBack(float x);
float EaseInBounce(float x);

// Ease out functions
float EaseOutSine(float x);
float EaseOutCubic(float x);
float EaseOutQuint(float x);
float EaseOutCirc(float x);
float EaseOutElastic(float x);
float EaseOutBack(float x);

// Ease in out functions
float EaseOutBounce(float x);
float EaseInOutSine(float x);
float EaseInOutCubic(float x);
float EaseInOutQuint(float x);
float EaseInOutCirc(float x);
float EaseInOutElastic(float x);
float EaseInOutBack(float x);
float EaseInOutBounce(float x);

// Tracks and update a float value
class ValueTracker
{
public:
	// Constructs and initialize value
	ValueTracker(float init = 0.0f);

	// Directly set the value instantly
	ValueTracker& operator=(float value);

	// Add a value transform function into the tracker
	// @param target: the target value that will be reached
	// @param duration: time duration of the whole task (in seconds)
	// @param easing: the easing function
	void Transform(float target, float duration = 1.0f, EasingFunction const& easing = Linear);

	// Add a value transform function into the tracker
	// @param target: the target value that will be reached
	// @param easing: the easing function
	void Transform(float target, EasingFunction const& easing);

	// Wait for some duration (in seconds)
	void Wait(float duration = 1.0f);

	// Update the tracker by time (in seconds)
	// @param loop: action loops infinity if true
	void Update(float elapsed, bool loop = false);

	// Get the current value
	float Get() const;

private:
	// All information of a single task
	struct Task
	{
		float target, init_value, begin, length;
		EasingFunction easefn;
		bool first_call; // true if the task haven't called
	};
	float value, now, length;
	std::vector<Task> tasklist;
};

}