#include "SceneCamera.h"
#include "Camera.h"
#include "CameraComponent.h"
#include "CameraManager.h"
#include "AppWindow.h"
#include "EngineTime.h"
#include "InputSystem.h"
#include "Camera.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

#include <iostream>

SceneCamera::SceneCamera() : GameObject()
{
	camera = new CameraComponent();
	camera->SetActive();
	AttachComponent(camera);
}

SceneCamera::~SceneCamera()
{
	GameObject::~GameObject();
}

void SceneCamera::Start()
{
	GameObject::Start();

}

void SceneCamera::Update(float deltaTime)
{
	GameObject::Update(deltaTime);

	if (transform)
	{
		Vector3 moveInput = input;
		moveInput.Normalize();

		Vector3 move = moveInput * cameraSpeed;

		const Matrix& mat = transform->GetTransformationMatrix();

		Vector3 new_pos = mat.Translation() + mat.Forward() * (move.z);

		new_pos = new_pos + mat.Right() * (move.x);

		new_pos = new_pos + mat.Up() * (move.y);

		transform->SetPosition(new_pos);
	}
}

void SceneCamera::onKeyDown(int key)
{
	if (key == 'W')
	{
		input.z = 1.0f;
	}

	else if (key == 'S')
	{
		input.z = -1.0f;
	}

	else if (key == 'A')
	{
		input.x = -1.0f;
	}

	else if (key == 'D')
	{
		input.x = 1.0f;
	}

	else if (key == 'Q')
	{
		input.y = -1.0f;
	}

	else if (key == 'E')
	{
		input.y = 1.0f;
	}

	/*if (key == '1')
	{
		if (camera)
		{
			camera->GetCamera()->SetCameraProjection(CameraProjectionType::Orthographic);
		}
	}
	else if (key == '2')
	{
		if (camera)
		{
			camera->GetCamera()->SetCameraProjection(CameraProjectionType::Perspective);
		}
	}*/

}

void SceneCamera::onKeyUp(int key)
{
	input = Vector3::Zero;
}

void SceneCamera::onMouseMove(const Point& delta_mouse)
{
	if (!rmbDown)
		return;

	float width, height;
	AppWindow::Get()->GetWindowSize(width, height);

	Vector3 euler = transform->GetEulerAngles();

	const float deltaTime = EngineTime::getDeltaTime();

	euler.y -= (delta_mouse.x - (width / 2.0f)) * deltaTime * cameraRotSpeed;
	euler.x -= (delta_mouse.y - (height / 2.0f)) * deltaTime * cameraRotSpeed;

	transform->SetEulerAngles(euler);

	InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
}

void SceneCamera::onLeftMouseButtonDown(const Point& mouse_pos)
{
	if (!lmbDown)
	{
		lmbDown = true;
	}
}

bool CheckIntersect(Vector3 rayOrigin, Vector3 rayDirection, float radius)
{
	float a, b, c, discriminant;

	// Calculate the a, b, and c coefficients.
	a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	// Find the discriminant.
	discriminant = (b * b) - (4 * a * c);

	// if discriminant is negative the picking ray missed the sphere, otherwise it intersected the sphere.
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}

void SceneCamera::onLeftMouseButtonUp(const Point& mouse_pos)
{
	if (lmbDown)
	{
		lmbDown = false;

		Point p = InputSystem::get()->GetMousePositionInWindow();
		//std::cout << "X: " << p.x << " Y: " << p.y << std::endl;

		Ray r = camera->GetCamera()->ScreenPointToRay(Vector3(p.x, p.y, 0));
		//std::cout << "X: " << r.direction.x << " Y: " << r.direction.y << " Z: " << r.direction.z << std::endl;

		const Vector3& pos = r.position;

		for (GameObject* obj : AppWindow::Get()->GetGameObjects())
		{
			if (MeshComponent* mesh = obj->GetComponent<MeshComponent>())
			{
				const auto& bounds = mesh->GetBounds();

				float dist = 100.0f;

				if (bounds.Intersects(r.position, r.direction, dist))
				{
					std::cout << obj->GetName() << std::endl;
				}

				/*if (CheckIntersect(r.position, r.direction, 1.0f))
				{
					std::cout << obj->GetName() << std::endl;
				}*/

				//std::cout << dist << std::endl;

				//std::cout << "X: " << r.position.x << " Y: " << r.position.y << " Z: " << r.position.z << std::endl;
			}
		}
	}
}


void SceneCamera::onRightMouseButtonDown(const Point& mouse_pos)
{
	rmbDown = true;
	InputSystem::get()->showCursor(false);
}

void SceneCamera::onRightMouseButtonUp(const Point& mouse_pos)
{
	rmbDown = false;
	InputSystem::get()->showCursor(true);
}
