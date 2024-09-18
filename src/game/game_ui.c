#include "game_ui.h"

#include "game_ui.h"
#include "resource_manager.h"

static GameUi game_ui;

static void update_ui(float delta_time)
{
    // animations update logic
}

static void render_before_content_ui(void)
{
    DrawTexture(*game_ui.background, 0, 0, WHITE);
}

static void render_after_content_ui(void)
{
    DrawTexture(*game_ui.foreground, 0, 0, WHITE);
}

static void cleanup_ui(void)
{
}

GameUi *create_game_ui(void)
{
    game_ui.background = &resource_manager.get_texture("gameplay-bg")->texture;
    game_ui.foreground = &resource_manager.get_texture("gameplay-fg")->texture;

    game_ui.update = update_ui;
    game_ui.render_before_content = render_before_content_ui;
    game_ui.render_after_content = render_after_content_ui;
    game_ui.cleanup = cleanup_ui;

    return &game_ui;
}
