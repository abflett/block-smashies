#include "kvec.h"
#include "playing_ui_bars.h"

static PlayingUiBars bars;
float fake_stat;

static void render_playing_ui_bars(void)
{
    for (int i = 0; i < 16; i++)
    {
        PlayingUiBar bar = bars.playing_ui_bars[i];

        DrawTexturePro(
            bar.subtexture->texture_resource->texture,                                                          // The texture to draw
            bar.subtexture->src,                                                                                // Source rectangle
            (Rectangle){bar.position.x, bar.position.y, bar.subtexture->src.width, bar.subtexture->src.height}, // Destination rectangle
            bar.origin,                                                                                         // Origin for rotation
            bar.rotation,                                                                                       // Rotation angle
            WHITE                                                                                               // Tint color
        );
    }
}

static void cleanup_playing_ui_bars(void)
{
}

static void update_playing_ui_bar(PlayingUiBar *bar)
{
    bar->current_bar_level = (int)((*bar->stat * (bar->max_bar_level - 1)) + 0.5f);
    bar->subtexture = resource_manager.get_subtexture(resource_manager.bar_level_mapper->bar_level_to_subtexture_id(bar->player_index, bar->current_bar_level));
}

static void update_playing_ui_bars(float delta_time)
{

    if (fake_stat - delta_time < 0.0f)
    {
        fake_stat = 1.0f;
    }
    else
    {
        fake_stat = 1.0f;
    }

    for (int i = 0; i < 16; i++)
    {
        update_playing_ui_bar(&bars.playing_ui_bars[i]);
    }
}

static PlayingUiBar create_playing_ui_bar(float *stat, int player_index, float rotation, Vector2 position)
{
    PlayingUiBar bar;
    bar.player_index = player_index;
    bar.stat = stat;
    bar.max_bar_level = resource_manager.bar_level_mapper->bars[bar.player_index].count;
    bar.current_bar_level = (int)((*bar.stat * (bar.max_bar_level - 1)) + 0.5f);
    bar.subtexture = resource_manager.get_subtexture(resource_manager.bar_level_mapper->bar_level_to_subtexture_id(bar.player_index, bar.current_bar_level));
    bar.rotation = rotation;
    bar.origin = (Vector2){0, 0};
    bar.position = (Vector2){position.x, position.y};
    return bar;
}

PlayingUiBars *create_playing_ui_bars(void)
{
    fake_stat = 0.5f;

    // Left Bars for current energy level for players 1-4
    bars.playing_ui_bars[0] = create_playing_ui_bar(&fake_stat, 0, 0.0f, (Vector2){12, 119});
    bars.playing_ui_bars[1] = create_playing_ui_bar(&fake_stat, 1, 0.0f, (Vector2){15, 120});
    bars.playing_ui_bars[2] = create_playing_ui_bar(&fake_stat, 2, 0.0f, (Vector2){18, 122});
    bars.playing_ui_bars[3] = create_playing_ui_bar(&fake_stat, 3, 0.0f, (Vector2){21, 123});

    // Top Bars for force move timeout - uses energy to use force timeout
    bars.playing_ui_bars[4] = create_playing_ui_bar(&fake_stat, 0, 90.0f, (Vector2){61, 104});
    bars.playing_ui_bars[5] = create_playing_ui_bar(&fake_stat, 1, 90.0f, (Vector2){60, 107});
    bars.playing_ui_bars[6] = create_playing_ui_bar(&fake_stat, 2, 90.0f, (Vector2){58, 110});
    bars.playing_ui_bars[7] = create_playing_ui_bar(&fake_stat, 3, 90.0f, (Vector2){57, 113});

    // Left Bars for heat - shooting uses energy and builds up heat, max heat will disable shooting
    bars.playing_ui_bars[8] = create_playing_ui_bar(&fake_stat, 0, 180.0f, (Vector2){76, 153});
    bars.playing_ui_bars[9] = create_playing_ui_bar(&fake_stat, 1, 180.0f, (Vector2){73, 152});
    bars.playing_ui_bars[10] = create_playing_ui_bar(&fake_stat, 2, 180.0f, (Vector2){70, 150});
    bars.playing_ui_bars[11] = create_playing_ui_bar(&fake_stat, 3, 180.0f, (Vector2){67, 149});

    // Bottom bars for boost timout - dashing left or right (boost) uses energy
    bars.playing_ui_bars[12] = create_playing_ui_bar(&fake_stat, 0, 270.0f, (Vector2){27, 168});
    bars.playing_ui_bars[13] = create_playing_ui_bar(&fake_stat, 1, 270.0f, (Vector2){28, 165});
    bars.playing_ui_bars[14] = create_playing_ui_bar(&fake_stat, 2, 270.0f, (Vector2){30, 162});
    bars.playing_ui_bars[15] = create_playing_ui_bar(&fake_stat, 3, 270.0f, (Vector2){31, 159});

    bars.update = update_playing_ui_bars;
    bars.render = render_playing_ui_bars;
    bars.cleanup = cleanup_playing_ui_bars;
    return &bars;
}
