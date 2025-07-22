class FloorScript : ScriptInterface {
    void OnUpdate(float deltaTime) 
    {
        auto@ body = gameObject.GetRigidBody2DComponent();
        auto@ transform = gameObject.GetTransform2DComponent();

        if (transform.Position.x < -15.6f) {
            Float2 newPos;
            newPos.x = transform.Position.x + 40.0f;
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

    GameObject gameObject;
    Scene@ scene;

    float MOVE_SPEED = 7.0f;
}
