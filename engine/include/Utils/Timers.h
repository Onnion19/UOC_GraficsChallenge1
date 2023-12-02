#pragma once

#include <functional>
#include "Utils/SafeCallbackToken.h"

namespace Utils {
	class SingleTimer
	{
	public:
		SingleTimer() = default;
		SingleTimer(float time) : callback(), timer(time) {}
		[[nodiscard]] virtual Utils::SafeCallbackObject Start(std::function<void()>&& functor);
		[[nodiscard]] virtual Utils::SafeCallbackObject Start(std::function<void()>&& functor, float time);

		void Update(float deltaTime);
		void Invalidate();
		bool IsActive() const;

	protected:
		virtual void Trigger();

	protected:
		Utils::ObserverCallbackObject callbackObject;
		std::function<void()> callback;
		float timer = -1.f;
	};

	class RepeatingTimer : public SingleTimer {
	public:
		RepeatingTimer(float time) : SingleTimer(time), initialTime(time) {}
	protected:
		void Trigger() override;
	protected:
		const float initialTime;
	};

	class RepeatingTimerWithVariation : public RepeatingTimer {
	public:
		RepeatingTimerWithVariation(float time, std::function<float(unsigned, float)>&& functor) : RepeatingTimer(time), variationFunctor(std::move(functor)) {}
		[[nodiscard]] Utils::SafeCallbackObject Start(std::function<void()>&& functor) override;
	protected:
		void Trigger() override;
	protected:
		std::function<float(unsigned /*times triggered*/, float/*initial time*/)> variationFunctor;
		unsigned int timesTriggered = 0;
	};
}