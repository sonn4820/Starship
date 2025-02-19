#include "ParticleSystem.hpp"


Particle::Particle(Renderer* renderer, Vec3 position, float lifeTime, Texture* texture, Vec2 size)
	:m_renderer(renderer), m_position(position), m_texture(texture), m_size(size), m_lifeTime(lifeTime)
{
	Vec3 BL = Vec3(-size.x * 0.5f, -size.y * 0.5f, 0.f);
	Vec3 BR = Vec3(size.x * 0.5f, -size.y * 0.5f, 0.f);
	Vec3 TL = Vec3(-size.x * 0.5f, size.y * 0.5f, 0.f);
	Vec3 TR = Vec3(size.x * 0.5f, size.y * 0.5f, 0.f);
	AddVertsForQuad3D(m_verts, BL, BR, TL, TR);

	Mat44 transform;
	transform.AppendZRotation(-90);
	//transform.AppendYRotation(90);
	TransformVertexArray3D(m_verts, transform);

	m_buffer = m_renderer->CreateVertexBuffer(sizeof(Vertex_PCU) * (unsigned int)m_verts.size());
	m_renderer->CopyCPUToGPU(m_verts.data(), (int)(m_verts.size() * sizeof(Vertex_PCU)), m_buffer);
}

Particle::~Particle()
{
	delete m_buffer;
	m_buffer = nullptr;
}

void Particle::Update(float deltaSeconds)
{
	if (m_isGarbage)
	{
		return;
	}

	m_lifeTime -= deltaSeconds;

	if (m_lifeTime <= 0.f)
	{
		m_isGarbage = true;
		return;
	}
	m_position += m_velocity * deltaSeconds;
	m_orientation += m_angular * deltaSeconds;
	m_size += m_scale * deltaSeconds;
}

void Particle::Render(Camera* camera, BilboardType type) const
{
	UNUSED(type);

	if (m_isGarbage)
	{
		return;
	}

	Mat44 renderMatrix = GetModelMatrix();
	if (camera)
	{
		//renderMatrix = GetBillboardMatrix(type, camera->GetModelMatrix(), m_position, m_size);
	}
	m_renderer->BindTexture(m_texture);
	m_renderer->BindShader(nullptr);
	m_renderer->SetBlendMode(m_blendMode);
	m_renderer->SetDepthStencilMode(m_depthMode);
	m_renderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_NONE);
	m_renderer->SetModelConstants(renderMatrix, m_color);
	m_renderer->DrawVertexBuffer(m_buffer, m_verts.size());
}

Mat44 Particle::GetModelMatrix() const
{
	Mat44 modelMat = Mat44();
	modelMat = m_orientation.GetAsMatrix_IFwd_JLeft_KUp();
	modelMat.SetTranslation3D(m_position);
	modelMat.AppendScaleNonUniform2D(m_size);
	return modelMat;
}

Emitter::Emitter(std::string name, Vec3 position, float timer, unsigned int seed)
	: m_position(position), m_lifeTime(timer), m_name(name)
{
	m_rng = RandomNumberGenerator(seed);
	m_timer = m_lifeTime;
}

Emitter::~Emitter()
{

}

void Emitter::Update(float deltaSeconds)
{
	for (size_t i = 0; i < m_currentParticles.size(); i++)
	{
		if (m_currentParticles[i])
		{
			if (m_currentParticles[i]->m_isGarbage)
			{
				delete m_currentParticles[i];
				m_currentParticles[i] = nullptr;
				continue;
			}

			m_currentParticles[i]->Update(deltaSeconds);
		}
	}

	if (m_isStopped)
	{
		return;
	}

	m_timer -= deltaSeconds;

	if (m_timer <= 0.f)
	{
		m_isStopped = true;
	}

	m_spawnTimer -= deltaSeconds;

	if (m_spawnTimer <= 0.f)
	{
		m_spawnTimer = m_rng.RollRandomFloatInRange(m_emitSpawnTimer.m_min, m_emitSpawnTimer.m_max);
		int numberOfParticles = m_rng.RollRandomIntInRange(m_numParticleEachSpawn.m_min, m_numParticleEachSpawn.m_max);

		for (size_t i = 0; i < numberOfParticles; i++)
		{
			float liftime = m_rng.RollRandomFloatInRange(m_particleLifeTime.m_min, m_particleLifeTime.m_max);
			float size = m_rng.RollRandomFloatInRange(m_particleSize.m_min, m_particleSize.m_max);

			Texture* texture = nullptr;
			if (!m_textures.empty())
			{
				int textureIndex = m_rng.RollRandomIntInRange(0, (int)m_textures.size() - 1);
				texture = m_textures[textureIndex];
			}

			auto particle = new Particle(m_renderer, m_particlePosition, liftime, texture, Vec2(1.f, 1.f) * size);

			float speed = m_rng.RollRandomFloatInRange(m_particleSpeed.m_min, m_particleSpeed.m_max);
			Vec3 velocity = m_particleVelDirection.GetNormalized();

			if (velocity.GetLengthSquared() == 0.f)
			{
				velocity.x = m_rng.RollRandomFloatInRange(-1.f, 1.f);
				velocity.y = m_rng.RollRandomFloatInRange(-1.f, 1.f);
				velocity.z = m_rng.RollRandomFloatInRange(-1.f, 1.f);
				velocity.Normalize();
			}

			particle->m_velocity = velocity * speed;
			particle->m_angular = m_particleAngular;
			particle->m_orientation = m_particleOrientation;
			particle->m_scale = m_particleScale;
			particle->m_color = m_particleColor;
			SpawnParticle(particle);
		}
	}

}

void Emitter::Render(Camera* camera) const
{
	for (auto currentParticle : m_currentParticles)
	{
		if (currentParticle)
		{
			currentParticle->Render(camera, m_billboardType);
		}
	}
}

void Emitter::SetParticlePositionAndOrientation(Vec3 position, EulerAngles orientation)
{
	m_particlePosition = position;
	m_particleOrientation = orientation;
}

void Emitter::SpawnParticle(Particle* particle)
{
	for (size_t i = 0; i < m_currentParticles.size(); i++)
	{
		if (m_currentParticles[i] == nullptr)
		{
			m_currentParticles[i] = particle;
			return;
		}
	}

	m_currentParticles.push_back(particle);
}

void Emitter::Activate()
{
	m_isStopped = false;
	m_spawnTimer = 0.f;
	m_timer = m_lifeTime;
}

void Emitter::Stop()
{
	m_isStopped = true;
}

ParticleSystem::ParticleSystem(Renderer* renderer)
	:m_renderer(renderer)
{

}

ParticleSystem::~ParticleSystem()
{
	for (size_t i = 0; i < m_emitters.size(); i++)
	{
		delete m_emitters[i];
		m_emitters[i] = nullptr;
	}
}

void ParticleSystem::AddEmitter(Emitter* emitter)
{
	emitter->m_renderer = m_renderer;

	for (size_t i = 0; i < m_emitters.size(); i++)
	{
		if (m_emitters[i] == nullptr)
		{
			m_emitters[i] = emitter;
			return;
		}
	}

	m_emitters.push_back(emitter);
}

void ParticleSystem::Update(float deltaSeconds)
{
	for (size_t i = 0; i < m_emitters.size(); i++)
	{
		m_emitters[i]->Update(deltaSeconds);
	}
}

void ParticleSystem::Render(Camera* camera) const
{
	m_renderer->BeginCamera(*camera);
	for (size_t i = 0; i < m_emitters.size(); i++)
	{
		m_emitters[i]->Render(camera);
	}
	m_renderer->EndCamera(*camera);
}

Emitter* ParticleSystem::GetEmitter(std::string name) const
{
	for (size_t i = 0; i < m_emitters.size(); i++)
	{
		if (m_emitters[i]->m_name == name)
		{
			return m_emitters[i];
		}
	}

	return nullptr;
}

void ParticleSystem::ActivateAll()
{
	for (size_t i = 0; i < m_emitters.size(); i++)
	{
		m_emitters[i]->Activate();
	}
}

void ParticleSystem::StopAll()
{
	for (size_t i = 0; i < m_emitters.size(); i++)
	{
		m_emitters[i]->Stop();
	}
}
