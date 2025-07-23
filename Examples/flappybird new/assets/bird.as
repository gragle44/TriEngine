class BirdScript : ScriptInterface {
    void OnStart() {}

    void OnStop() {}

    void OnUpdate(float deltaTime) 
    {
        auto@ rigidBody = gameObject.GetRigidBody2DComponent();
        auto@ transform = gameObject.GetTransform2DComponent();

        m_TimeSinceSpacePressed += deltaTime;

        if (IsKeyPressed(KeyCode::Space) || IsMouseButtonPressed(MouseButton::Left)) {
            m_TimeSinceSpacePressed = 0.0f;
            m_UpVelocity += 2.6f;
            m_RotationVelocity = 20.0f;
        }
        else {
            if (m_TimeSinceSpacePressed >= TIME_UNTIL_FALL)
                m_RotationVelocity = -5.0f;
        }

        m_UpVelocity -= 0.585f;

        if (m_UpVelocity > 9.1f)
            m_UpVelocity = 9.1f;
        else if (m_UpVelocity < -13.0f)
            m_UpVelocity = -13.0f;

        if (transform.Rotation > 15.0f && m_RotationVelocity > 0.0f)
            m_RotationVelocity = 0.0f;
        else if (transform.Rotation < -90.0f && m_RotationVelocity < 0.0f)
            m_RotationVelocity = 0.0f;

        rigidBody.SetVelocity(0.0f, m_UpVelocity);
        rigidBody.SetAngularVelocity(m_RotationVelocity);
    }

    void OnCollisionStart(GameObject collider) {
        scene.Reset();
    }

    void OnCollisionStop(GameObject collider) {}

    GameObject gameObject;
    Scene@ scene;

    float TIME_UNTIL_FALL = 0.25f;

    float m_UpVelocity = 0.0f;
    float m_RotationVelocity = 0.0f;

    float m_TimeSinceSpacePressed = 0.0f;    
}

