#pragma once 

#include "Scenes/Scene.h"


class EndScene : public Scenes::SceneBase<EndScene> {
public:
	EndScene(Core::GameManagers& manager);
	void Activate();
	void DeActivate();
	void Update(float deltaTime);
	void Draw();
	void Finish();
private:
	float flickeringEffect = 0.f;
};

