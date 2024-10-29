#include "shake_effect.h"
#include "settings.h"

ShakeEffect shake_effect;

static void start_shake_effect(const float intensity, const float duration)
{
    shake_effect.intensity = intensity;
    shake_effect.duration = duration;
    shake_effect.elapsed = 0.0f;
    shake_effect.offset = (Vector2){0.0f, 0.0f};
}

static void update_shake_effect(const float delta_time)
{
    if (shake_effect.elapsed < shake_effect.duration && settings.config.shake_screen)
    {
        shake_effect.elapsed += delta_time;
        const float progress = shake_effect.elapsed / shake_effect.duration;

        // Decrease intensity over time
        shake_effect.intensity *= (1.0f - progress);

        // Apply random offset
        shake_effect.offset.x = (float)GetRandomValue(-1, 1) * shake_effect.intensity;
        shake_effect.offset.y = (float)GetRandomValue(-1, 1) * shake_effect.intensity;
    }
    else
    {
        shake_effect.intensity = 0.0f; // Reset intensity after shake
    }
}

ShakeEffect *get_shake_effect(void)
{
    return &shake_effect;
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
