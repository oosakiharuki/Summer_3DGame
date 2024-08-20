#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"
#include "Input.h"

#ifdef _DEBUG
#include "imgui.h"
#endif

class Player;

class RailCamera {
public:
	void Initialize();
	void Update();


	Vector3 GetWorldTranslation();
	Vector3 GetWorldRotate();
	//Matrix4x4 GetMatView();


	const WorldTransform& GetWorldTransform() const { return worldTramsform_; } 
		
	void SetTarget(const WorldTransform* target) { target_ = target; } 

private:
	WorldTransform worldTramsform_;
	ViewProjection viewProjection_;
	MyMath* myMath_;

	const WorldTransform* target_ = nullptr;
	
	Input* input_ = nullptr;

	//Matrix4x4 matView;
	//Vector3 viewTransform;
	//Vector3 viewRotate;

	Player* player_ = nullptr;
};