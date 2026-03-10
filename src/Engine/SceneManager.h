#pragma once

class Scene;

class SceneManager
{
private:
	std::vector<Scene*> m_pScenes;
	Scene* m_pCurrentScene = nullptr;
public:

	void Init();
	void Update(float _dt);
	void Draw();

	void SetCurrentScene(Scene* _currScene);

	void AddScene(Scene* _scene);
	Scene* GetCurrentScene();
};

