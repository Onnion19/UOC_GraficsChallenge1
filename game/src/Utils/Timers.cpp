#include "Timers.h"


/**
*           SINGLE TIMER
*/
Utils::SafeCallbackObject Utils::SingleTimer::Start(std::function<void()>&& functor)
{
	callback = std::move(functor);

	auto&& [safeCallback, observerCallback] = Utils::CallbackObjectsFactory::MakeCallbacks();
	callbackObject = std::move(observerCallback);
	return std::move(safeCallback);
}

void Utils::SingleTimer::Update(float deltaTime)
{
	if (timer < 0.f) return;

	timer -= deltaTime;
	if (timer < 0.f)
	{
		Trigger();
	}
}

void Utils::SingleTimer::Invalidate()
{
	timer = -1.f;
}

void Utils::SingleTimer::Trigger()
{
	if (callbackObject)
		callback();
}


/**
*           REPEATING TIMER
*/

void Utils::RepeatingTimer::Trigger()
{
	SingleTimer::Trigger();
	timer = initialTime;
}

/**
*           REPEATING TIMER WITH VARIATION
*/

void Utils::RepeatingTimerWithVariation::Trigger()
{
	RepeatingTimer::Trigger();
	timesTriggered++;
	timer = variationFunctor(timesTriggered, initialTime);
}
