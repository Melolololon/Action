#pragma once
#include<GameObject.h>

#include"Player.h"

class StageObject :public MelLib::GameObject
{
private:
	static Player* pPlayer;

private:
	// �v���C���[�̍U�����̕ϊ�
	MelLib::PlaneData SegmentToPlane();

protected:
	/// <summary>
	/// �v���C���[�̍U���Ɠ���������ؒf���������܂��B
	/// </summary>
	/// <param name="object"></param>
	/// <param name="pFront"></param>
	/// <param name="pBack"></param>
	void MeshCat(const MelLib::GameObject* const object, MelLib::ModelData*& pFront, MelLib::ModelData*& pBack);

public:
	StageObject(const MelLib::Vector3& pos, const std::string& modelName);
	StageObject(const MelLib::Vector3& pos, MelLib::ModelData* pCatModelData);
	~StageObject();

	void Update()override;
	void Draw()override;
	/// <summary>
	/// �����������̏���
	/// </summary>
	/// <param name="object">����I�u�W�F�g�̃|�C���^</param>
	/// <param name="collisionType">�����̂ǂ̔���ɓ���������</param>
	/// <param name="arrayNum">�����̉��ڂ̔���ɓ���������</param>
	/// <param name="hitObjColType">����̂ǂ̔���ɓ���������</param>
	/// <param name="hitObjArrayNum">����̉��ڂ̔���ɓ���������</param>
	void Hit
	(
		const GameObject* const  object,
		const MelLib::ShapeType3D& collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D& hitObjColType,
		const int hitObjArrayNum
	)override;

	static void SetPPlayer(Player* p) { pPlayer = p; }
};

