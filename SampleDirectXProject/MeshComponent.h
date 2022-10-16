﻿#pragma once
#include "Component.h"

class ConstantBuffer;
class PixelShader;
class VertexShader;
class IndexBuffer;
class VertexBuffer;

class Mesh;

class MeshComponent: public Component
{
public:
	MeshComponent();
	~MeshComponent() override;

public:
	void Start() override;
	void Update(float deltaTime) override;
	void SetMesh(Mesh* inMesh);

protected:
	Mesh* mesh = nullptr;

protected:
	VertexBuffer* m_vb;
	IndexBuffer* m_ib;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;

protected:
	friend class Mesh;
};
