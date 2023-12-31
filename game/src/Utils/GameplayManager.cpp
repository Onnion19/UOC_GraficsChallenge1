#include "GameplayManager.h"

void GameplayManager::StartGame()
{
	gameStart = std::chrono::steady_clock::now();
}

void GameplayManager::EndGame()
{
	gameEnd = std::chrono::steady_clock::now();
}

float GameplayManager::GetGameDuration()
{
	using DurationInSeconds = std::chrono::duration<float>;
	return std::chrono::duration_cast<DurationInSeconds>(gameEnd - gameStart).count();
}

void GameplayManager::UpdateScore(int deltaScore)
{
	score += deltaScore;
	TriggerScoreCallbacks();
}

void GameplayManager::SetScore(unsigned newScore)
{
	score = newScore;
	TriggerScoreCallbacks();
}

unsigned GameplayManager::GetScore() const
{
	return score;
}

Utils::SafeCallbackObject GameplayManager::RegisterScoreCallback(ScoreCallback callback)
{
	auto safeObject = Utils::CallbackObjectsFactory::MakeSafeCallbackObject();
	scoreCallbacks.emplace_back(callback, safeObject);
	return safeObject;
}

void GameplayManager::UpdateHealth(int deltaHp)
{
	health += deltaHp;
	TriggerHealthCallbacks();
}

void GameplayManager::SetHealth(unsigned hp)
{
	health = hp;
	TriggerHealthCallbacks();
}

Utils::SafeCallbackObject GameplayManager::RegisterHealthCallback(HealthCallback callback)
{
	auto safeObject = Utils::CallbackObjectsFactory::MakeSafeCallbackObject();
	healthCallbacks.emplace_back(callback, safeObject);
	return safeObject;
}

void GameplayManager::TriggerScoreCallbacks()
{
	using CallbackType = decltype(scoreCallbacks)::value_type;
	TriggerCallbacks<CallbackType>(scoreCallbacks, score);
}

void GameplayManager::TriggerHealthCallbacks()
{
	using CallbackType = decltype(healthCallbacks)::value_type;
	TriggerCallbacks<CallbackType>(healthCallbacks, health);
}
