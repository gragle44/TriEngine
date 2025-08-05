const int SPAWN_COUNT = 150;

const float TIME_UNTIL_RESET = 4.f;

class CubeSpawner : ScriptInterface {
    void OnStart() {
        for (int i = 0; i < SPAWN_COUNT; i++) {
            GameObject test = scene.InstantiatePrefab("cube.prefab");
        }
    }

    void OnUpdate(float deltaTime) {
        if (TimeElapsed > TIME_UNTIL_RESET) {
            scene.Reset();
        }
        TimeElapsed += deltaTime;
    }

    float TimeElapsed;

    GameObject gameObject;
    Scene@ scene;
}

