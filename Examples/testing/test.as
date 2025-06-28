GameObject gameObject;
Scene@ scene;

void on_start()
{
    LogInfo("Hello from " + gameObject.GetTagComponent().Tag);
}

void on_update(float deltaTime)
{

}

void on_stop() 
{
    LogInfo("Stopping script");
}

void on_collision_start(GameObject gobject)
{
    LogWarn(gameObject.GetTagComponent().Tag + " -> " + gobject.GetTagComponent().Tag);
}