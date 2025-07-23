class PillarScript : ScriptInterface {
    void OnStart() 
    {
        if (gameObject.GetName().findFirst("Inverted") >= 0)
            return;

        m_InvertedPillar = scene.GetGameObject("Inverted" + gameObject.GetName());

        float displacement = RandFloat(-1.5f, 1.5f);

        gameObject.GetTransform2DComponent().Position.y += displacement;
        m_InvertedPillar.GetTransform2DComponent().Position.y += displacement;
    }

    void OnStop() {}

    void OnUpdate(float deltaTime) 
    {
        auto@ body = gameObject.GetRigidBody2DComponent();
        auto@ transform = gameObject.GetTransform2DComponent();

        if (transform.Position.x < -15.0f) {
            Float2 newPos;
            newPos.x = transform.Position.x + 45.0f;
            newPos.y = transform.Position.y;
            body.SetPosition(newPos);
        }
        else {
            Float2 newPos;
            newPos.x = transform.Position.x -= MOVE_SPEED * deltaTime;
            newPos.y = transform.Position.y;

            body.SetPosition(newPos);
        }
    }

    void OnCollisionStart(GameObject collider) {}
    void OnCollisionStop(GameObject collider) {}

    GameObject gameObject;
    Scene@ scene;

    GameObject m_InvertedPillar;
    float MOVE_SPEED = 7.0f;

}