#include "shake_effect.h"

ShakeEffect shake_effect;

static void start_shake_effect(float intensity, float duration)
{
    shake_effect.intensity = intensity;
    shake_effect.duration = duration;
    shake_effect.elapsed = 0.0f;
    shake_effect.offset = (Vector2){0.0f, 0.0f};
}

static void update_shake_effect(float delta_time)
{
    if (shake_effect.elapsed < shake_effect.duration)
    {
        shake_effect.elapsed += delta_time;
        float progress = shake_effect.elapsed / shake_effect.duration;

        // Decrease intensity over time
        shake_effect.intensity *= (1.0f - progress);

        // Apply random offset
        shake_effect.offset.x = (GetRandomValue(-1, 1) * shake_effect.intensity);
        shake_effect.offset.y = (GetRandomValue(-1, 1) * shake_effect.intensity);
    }
    else
    {
        shake_effect.intensity = 0.0f; // Reset intensity after shake
    }
}

ShakeEffect *create_shake_effect(void)
{
    shake_effect.duration = 0.0f;
    shake_effect.elapsed = 0.0f;
    shake_effect.intensity = 0.0f;
    shake_effect.offset = (Vector2){0.0f, 0.0f};

    shake_effect.start = start_shake_effect;
    shake_effect.update = update_shake_effect;

    return &shake_effect;
}
