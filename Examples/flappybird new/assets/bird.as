GameObject gameObject;
Scene@ scene;

float TIME_UNTIL_FALL = 0.5f;

float m_UpVelocity = 0.0f;
float m_RotationVelocity = 0.0f;

void on_start() 
{

}

void on_update(float deltaTime) 
{
		auto@ rigidBody = gameObject.GetRigidBody2DComponent();
		auto@ transform = gameObject.GetTransform2DComponent();

		if (IsKeyPressed(KeyCode::Space) || IsMouseButtonPressed(MouseButton::Left)) {
			m_UpVelocity += 200.0f;
			m_RotationVelocity = 575.0f;
		}
		else
			m_RotationVelocity = -175.0f;

		m_UpVelocity -= 45.0f;

		if (m_UpVelocity > 700.0f)
			m_UpVelocity = 700.0f;
		else if (m_UpVelocity < -1000.0f)
			m_UpVelocity = -1000.0f;

		if (transform.Rotation > 45.0f && m_RotationVelocity > 0.0f)
			m_RotationVelocity = 0.0f;
		else if (transform.Rotation < -90.0f && m_RotationVelocity < 0.0f)
			m_RotationVelocity = 0.0f;

		rigidBody.SetVelocity(0.0f, m_UpVelocity * deltaTime);
		rigidBody.SetAngularVelocity(m_RotationVelocity * deltaTime);
}