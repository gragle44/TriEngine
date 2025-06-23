void on_start()
{
    log_info("Hello world!");
}

void on_update(float deltaTime)
{
    log_info(formatInt(RandInt(1, 100), 'l', 10));
}