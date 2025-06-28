GameObject gameObject;
Scene@ scene;

const float MOVE_SPEED = 7.0f;

void on_start() 
{
    float displacement = RandFloat(-1.25f, 1.25f);
    gameObject.GetTransform2DComponent().Position.y += displacement;
}

void on_update(float deltaTime) 
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

void on_collision_start(GameObject collider) {
    scene.Reset();
}