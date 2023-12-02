#pragma once 

#include <vector>
#include "Utils/SafeCallbackToken.h"
#include <chrono>


/****************************************************************
*			CONCEPTS AND CALLBACK TYPE DEFINITIONS
****************************************************************/
template<typename T>
concept HealthCallbackTy = requires(T t)
{
	t.OnHealthUpdate(5u);
};

// Typea erasure for health callback.
// Any class fullfilling the HealthCallbackTy concept can be a health callback
struct HealthCallback {

	template<HealthCallbackTy CallbackTy>
	HealthCallback(CallbackTy& callback) : ptr(&callback)
	{
		healthUpdate = [](void* ptr, unsigned health) { static_cast<CallbackTy*>(ptr)->OnHealthUpdate(health); };
	}

	void operator()(unsigned health) { healthUpdate(ptr, health); }
	void Trigger(unsigned health) { healthUpdate(ptr, health); }

private:
	void* ptr;
	void (*healthUpdate)(void*, unsigned);
};

/***********************************************************************************/

template<typename T>
concept ScoreCallbackTy = requires(T t)
{
	t.OnScoreUpdate(150u);
};

// Typea erasure for score callback.
// Any class fullfilling the ScoreCallbackTy concept can be a score callback
struct ScoreCallback {

	template<ScoreCallbackTy CallbackTy>
	ScoreCallback(CallbackTy& callback) : ptr(&callback)
	{
		scorehUpdate = [](void* ptr, unsigned health) { static_cast<CallbackTy*>(ptr)->OnScoreUpdate(health); };
	}

	ScoreCallback(const ScoreCallback&) = default;
	ScoreCallback& operator=(const ScoreCallback&) = default;
	ScoreCallback(ScoreCallback&&) = default;
	ScoreCallback& operator=(ScoreCallback&&) = default;

	void operator()(unsigned score) { scorehUpdate(ptr, score); }
	void Trigger(unsigned score) { scorehUpdate(ptr, score); }

private:
	void* ptr;
	void (*scorehUpdate)(void*, unsigned);
};

/***********************************************************************************/

/**
* Central storage for game related data.
*/
class GameplayManager
{
public:

	void StartGame();
	void EndGame();
	float GetGameDuration();

	void UpdateScore(int deltaScore);
	void SetScore(unsigned newScore);
	unsigned GetScore()const;
	[[nodiscard]] Utils::SafeCallbackObject RegisterScoreCallback(ScoreCallback callback);

	void UpdateHealth(int deltaHp);
	void SetHealth(unsigned hp);
	[[nodiscard]] Utils::SafeCallbackObject RegisterHealthCallback(HealthCallback callback);


private:
	/**
	* Helper struct for the listeners.
	* Wraps the ObserverCallbackObject that ensure the listener is alive.
	* Also contains the callback to call.
	*/
	template<typename Callback>
	struct CallbackObserver
	{

		CallbackObserver(const Callback& c, const Utils::SafeCallbackObject& alive) :
			callback(c),
			observer(Utils::CallbackObjectsFactory::MakeObserverCallback(alive)) {}

		CallbackObserver(const CallbackObserver<Callback>& other) = default;
		CallbackObserver<Callback>& operator=(const CallbackObserver<Callback>& other) = default;
		CallbackObserver(CallbackObserver<Callback>&& other) = default;
		CallbackObserver<Callback>& operator=(CallbackObserver<Callback>&& other) = default;

		Utils::ObserverCallbackObject observer;
		Callback callback;
		operator bool()const { return static_cast<bool>(observer); }
	};

	void TriggerScoreCallbacks();
	void TriggerHealthCallbacks();

	template<typename T, typename ... Args>
	void TriggerCallbacks(std::vector<T>& callbacks, Args&& ... args);

private:
	unsigned score = 0;
	unsigned health = 3;
	std::chrono::steady_clock::time_point gameStart;
	std::chrono::steady_clock::time_point gameEnd;
	std::vector<CallbackObserver<ScoreCallback>> scoreCallbacks;
	std::vector<CallbackObserver<HealthCallback>> healthCallbacks;
};

template<typename T, typename ...Args>
inline void GameplayManager::TriggerCallbacks(std::vector<T>& callbacks, Args && ...args)
{
	int i = static_cast<int>(callbacks.size()) - 1;

	while (i >= 0 && !callbacks.empty())
	{
		if (callbacks[i])
		{
			callbacks[i].callback.Trigger(std::forward<Args>(args)...);
		}
		else
		{
			// listener is no logner alive
			callbacks.erase(callbacks.begin() + i);
		}
		i--;
	}

}
