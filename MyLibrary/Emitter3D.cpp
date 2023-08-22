#include "Emitter3D.h"

#include"LibMath.h"

void MelLib::Emitter3D::CheckChangeUpdateDrawFlag()
{
	if (isStop)
	{
		return;
	}
	if (releaseTimer.GetMaxOverFlag())
	{
		const size_t PAR_SIZE = particles.size();
		for (int i = 0; i < PAR_SIZE; i++)
		{
			if (reShotStop)return;

			if (!particles[i]->GetUpdateDrawFlag())
			{
				particles[i]->SetUpdateDrawFlag(true);
				releaseTimer.ResetTimeZero();

				// 放出時間が0じゃなかったらreturn
				// 0だったら全部出す
				if(releaseTimer.GetMaxTime() != 0)return;
				
			}

		}
	}
}

void MelLib::Emitter3D::ParticleUpdate()
{

	for (int i = 0;i < particles.size(); i++)
	{
		if (!particles[i]->GetUpdateDrawFlag())return;
		if (reShotStop)return;

		particles[i]->Update();

		if (particles[i]->GetIsDead())
		{
			if (!allShotDead)
			{
				particles[i]->ResetParametor();
				particles[i]->Initialize();


				if (isStop)
				{
					particles[i]->SetUpdateDrawFlag(false);
				}

				const size_t PAR_SIZE = particles.size();
				if (i == PAR_SIZE - 1 && !reShot)
				{
					reShotStop = true;
				}
			}
		}
		
	}

	if (particles[particles.size() - 1]->GetIsDead() && allShotDead)eraseManager = true;
}

MelLib::Emitter3D::Emitter3D(const Particle3D& pParticle, const bool allShotDead, const unsigned int particleNum, unsigned int releaseTime, const Vector3& pos, const std::string& name)
	:GameObject(name)
	, allShotDead(allShotDead)
{
	//pParticleを元にパーティクルを生成
	//particles.resize(particleNum, pParticle);
	SetPosition(pos);

	particles.resize(particleNum);

	for (auto& particle : particles)
	{
		particle = pParticle.GetNewPtr();
		particle->Initialize();
		particle->SetStartPosition(GetPosition());
	}

	releaseTimer.SetMaxTime(releaseTime);
	releaseTimer.SetStopFlag(false);

}

void MelLib::Emitter3D::Update()
{
	CheckChangeUpdateDrawFlag();
	ParticleUpdate();
}

void MelLib::Emitter3D::Draw()
{
	const Vector3 cameraPos = MelLib::RenderTarget::Get()->GetCamera()->GetCameraPosition();
	std::sort(particles.begin(), particles.end(), [&cameraPos](const std::shared_ptr<Particle3D>& obj1, const std::shared_ptr<Particle3D>& obj2)
		{
			Vector3 pos1 = obj1->GetRefSprite().GetPosition();
			Vector3 pos2 = obj2->GetRefSprite().GetPosition();
			float dis1 = MelLib::LibMath::CalcDistance3D(pos1, cameraPos);
			float dis2 = MelLib::LibMath::CalcDistance3D(pos2, cameraPos);
			return dis1 < dis2;
		});


	for (int i = 0;i < particles.size(); i++)
	{
		if (!particles[i]->GetUpdateDrawFlag())return;
		if (reShotStop)return;

		particles[i]->Draw();
	}
}

void MelLib::Emitter3D::SetPosition(const Vector3& pos)
{
	SetPositionDefault(pos);
	// ここにパーティクルのStartPosを変更する処理を書く

	for (auto& par : particles) 
	{
		par->SetStartPosition(pos);
	}
}

void MelLib::Emitter3D::SetReShotFlag(bool flag)
{
	reShot = flag;

	if (flag)reShotStop = false;
}
