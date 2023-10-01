#pragma once

namespace Scenes
{
	/*
		An updatable class is any class that implements Update function with delta time.
	*/
	template <typename T>
	concept Updatable = requires (T t) {
		t.Update(0.f);
	};

	/*
		A Scenelike class must handle all the scene life cycle from: 
		- Activation
		- Draw
		_ Reset
		- Deactivation
	*/
	template<typename T>
	concept SceneLike = requires (T t)
	{
		t.ActivateScene();
		t.DeactivateScene();
		t.DrawScene();
		t.ResetScene();
	};

	template<typename T>
	concept NonUpdatableScene = SceneLike<T> && !Updatable<T>;

	template<typename T>
	concept UpddatableScene = SceneLike<T> && Updatable<T>;

	/*
		Type erasure scene type. So any class that's scenelike can be in fact a scene. 
		This is a way to avoid interfaces and virtual functions which usually ens up indirecting pointer references.
	*/
	class SceneTE {
	public:
		// TODO: Scenes should be identified by ID's. Maybe a string hash? 
		template<NonUpdatableScene T>
		SceneTE(T* t)
		{
			_data = t;
			InitScene<T>();
		}

		template<UpddatableScene T>
		SceneTE(T* t)
		{

			_data = t;
			InitScene<T>();
			_Update = [](void* ptr, float delta) {static_cast<T*>(ptr)->Update(delta); };
		}

		~SceneTE() { _Destroy(_data); }

		void Activate() { _Activate(_data); }
		void Deactivate() { _Deactivate(_data); }
		void Draw() { _Draw(_data); }
		void Reset() { _Reset(_data); }
		void Update(float delta) { if (_Update) _Update(_data, delta); }

	private:

		template<SceneLike T>
		void InitScene()
		{
			_Destroy = [](void* ptr) { delete (static_cast<T*>(ptr)); };
			_Activate = [](void* ptr) {static_cast<T*>(ptr)->ActivateScene(); };
			_Deactivate = [](void* ptr) {static_cast<T*>(ptr)->DeactivateScene(); };
			_Draw = [](void* ptr) {static_cast<T*>(ptr)->DrawScene(); };
			_Reset = [](void* ptr) {static_cast<T*>(ptr)->ResetScene(); };
			_Update = nullptr;
		}

		void (*_Activate)(void*);
		void (*_Deactivate)(void*);
		void (*_Draw)(void*);
		void (*_Update)(void*, float);
		void (*_Reset)(void*);
		void (*_Destroy)(void*);
		void* _data;
	};

}