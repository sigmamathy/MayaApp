#include <Maya/value_tracker.hpp>

namespace Maya {

static float constexpr PI = 3.1415926536f;

ValueTracker::ValueTracker(float init)
	: value(init), now(0.0f), length(0.0f)
{
	tasklist.reserve(10);
}

ValueTracker& ValueTracker::operator=(float value)
{
	this->value = value;
	return *this;
}

void ValueTracker::Transform(float target, float duration, EasingFunction const& easing)
{
#if MAYA_DEBUG
	if (duration < 0.0f) {
		std::cout << "Attempting to call ValueTracker::Transform with duration < 0.0f\n";
		return;
	}
#endif
	tasklist.emplace_back(target, 0.0f, length, duration, easing, true);
	length += duration;
}

void ValueTracker::Transform(float target, EasingFunction const& easing)
{
	Transform(target, 1.0f, easing);
}

void ValueTracker::Wait(float duration)
{
#if MAYA_DEBUG
	if (duration < 0.0f) {
		std::cout << "Attempting to call ValueTracker::Wait with duration < 0.0f\n";
		return;
	}
#endif
	length += duration;
}

void ValueTracker::Update(float elapsed, bool loop)
{
	for (auto& task : tasklist)
	{
		if (now < task.begin
			|| (!task.first_call && now > task.begin + task.length))
			continue;
		if (task.first_call) task.init_value = value;
		float t = now - task.begin;
		t = t <= task.length ? (t / task.length) : 1.0f;
		float res = task.easefn(t);
		value = task.init_value + res * (task.target - task.init_value);
		task.first_call = false;
	}

	now += elapsed;
	if (loop && now > length)
	{
		now = 0.0f;
		for (auto& task : tasklist)
			task.first_call = true;
	}
}

float ValueTracker::Get() const
{
	return value;
}

float Linear(float x)
{
	return x;
}

float EaseInSine(float x)
{
	return 1 - std::cos(x * PI / 2);
}

float EaseInCubic(float x)
{
	return x * x * x;
}

float EaseInQuint(float x)
{
	return x * x * x * x * x;
}

float EaseInCirc(float x)
{
	return 1 - std::sqrt(1 - x * x);
}

float EaseInElastic(float x)
{
	constexpr float c4 = 2.0f * PI / 3;
	return x == 0.0f || x == 1.0f ? x : -std::pow(2, 10 * x - 10) * std::sin((x * 10 - 10.75f) * c4);
}

float EaseInBack(float x)
{
	constexpr float c1 = 1.70158f, c3 = c1 + 1;
	return c3 * x * x * x - c1 * x * x;
}

float EaseInBounce(float x)
{
	return 1 - EaseOutBounce(1 - x);
}

float EaseOutSine(float x)
{
	return std::sin(x * PI / 2);
}

float EaseOutCubic(float x)
{
	float y = 1 - x;
	return 1 - y * y * y;
}

float EaseOutQuint(float x)
{
	float y = 1 - x;
	return 1 - y * y * y * y * y;
}

float EaseOutCirc(float x)
{
	float y = x - 1;
	return std::sqrt(1 - y * y);
}

float EaseOutElastic(float x)
{
	constexpr float c4 = 2 * PI / 3;
	return x == 0.0f || x == 1.0f
		? x
		: std::pow(2, -10 * x) * std::sin((x * 10 - 0.75f) * c4) + 1;
}

float EaseOutBack(float x)
{
	constexpr float c1 = 1.70158f, c3 = c1 + 1;
	float y = x - 1;
	return 1 + c3 * y * y * y + c1 * y * y;
}

float EaseOutBounce(float x)
{
	constexpr float n1 = 7.5625f, d1 = 2.75f;
	if (x < 1 / d1) return n1 * x * x;
	if (x < 2 / d1) return n1 * (x -= 1.5f / d1) * x + 0.75f;
	if (x < 2.5 / d1) return n1 * (x -= 2.25f / d1) * x + 0.9375f;
	return n1 * (x -= 2.625f / d1) * x + 0.984375f;
}

float EaseInOutSine(float x)
{
	return -(std::cos(PI * x) - 1) / 2;
}

float EaseInOutCubic(float x)
{
	float y = -2 * x + 2;
	return x < 0.5f ? 4 * x * x * x : 1 - y * y * y / 2;
}

float EaseInOutQuint(float x)
{
	float y = -2 * x + 2;
	return x < 0.5f ? 16 * x * x * x * x * x : 1 - y * y * y * y * y / 2;
}

float EaseInOutCirc(float x)
{
	float y0 = 2 * x, y1 = -2 * x + 2;

	return x < 0.5f
		? (1 - std::sqrt(1 - y0 * y0)) / 2
		: (std::sqrt(1 - y1 * y1) + 1) / 2;
}

float EaseInOutElastic(float x)
{
	constexpr float c5 = 2 * PI / 4.5f;
	if (x == 0.0f || x == 1.0f) return x;
	return x < 0.5f
		? -(std::pow(2, 20 * x - 10) * std::sin((20 * x - 11.125f) * c5)) / 2
		: (std::pow(2, -20 * x + 10) * std::sin((20 * x - 11.125f) * c5)) / 2 + 1;
}

float EaseInOutBack(float x)
{
	constexpr float c2 = 1.70158f * 1.525f;
	float y0 = 2 * x, y1 = 2 * x - 2;
	return x < 0.5f
		? (y0 * y0 * ((c2 + 1) * 2 * x - c2)) / 2
		: (y1 * y1 * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

float EaseInOutBounce(float x)
{
	return x < 0.5f
		? (1 - EaseOutBounce(1 - 2 * x)) / 2
		: (1 + EaseOutBounce(2 * x - 1)) / 2;
}

}