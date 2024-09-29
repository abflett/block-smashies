#include "raylib.h"
#include "main_menu_scene.h"
#include "game.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "game_data.h"
#include "settings.h"

static Scene main_menu_scene;

static Texture2D *texture_stars;
static Texture2D *texture_station;
static Texture2D *texture_beam_relive;
static Texture2D *texture_beam_exit;
static Texture2D *texture_beam_historic;
static Texture2D *texture_beam_modify;
static Texture2D *texture_beam_block_smashies;
static Texture2D *texture_beam_embark;

static Subtexture *block_smashies_holos[4];
static Subtexture *block_smashies_embark[4];
static Subtexture *block_smashies_relive[4];
static Subtexture *block_smashies_modify[4];
static Subtexture *block_smashies_historic[4];
static Subtexture *block_smashies_exit[4];

static Font *pixel7;
static Color beam_alpha;

static bool toggle_beams;
float accumalator;

static void scene_init(int arg_count, va_list args)
{
}

static void scene_update(float delta_time)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        scene_manager.change(scene_manager.scenes.gameplay, 1, create_game_data("Team1"));
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        exit_game();
    }

    beam_alpha = settings.colors.alpha_07;
    accumalator += delta_time;
    if (accumalator > 0.1)
    {
        beam_alpha = settings.colors.alpha_05;
        accumalator = 0;
    }
}

static void scene_render(void)
{
    DrawTexture(*texture_stars, 0, 0, WHITE);
    DrawTexture(*texture_station, 0, 0, WHITE);
    DrawTextEx(*pixel7, "Begin your journey as a Block Smashy. Relive the leg", (Vector2){106, 53}, 7, 0.0f, settings.colors.blue_04);

    DrawTexture(*texture_beam_relive, 184, 97, beam_alpha);
    DrawTexture(*texture_beam_exit, 228, 112, beam_alpha);
    DrawTexture(*texture_beam_historic, 51, 131, beam_alpha);
    DrawTexture(*texture_beam_modify, 22, 99, beam_alpha);
    DrawTexture(*texture_beam_block_smashies, 113, 19, beam_alpha);
    DrawTexture(*texture_beam_embark, 41, 68, beam_alpha);
}

static void scene_cleanup(void)
{
    // Cleanup resources used by the main menu
}

Scene *create_main_menu_scene()
{
    TraceLog(LOG_INFO, "[[[[CREATED]]]] -");
    texture_stars = &resource_manager.get_texture("menu-ui-stars")->texture;     // d
    texture_station = &resource_manager.get_texture("menu-ui-station")->texture; // d
    texture_beam_relive = &resource_manager.get_texture("menu-ui-beam-relive")->texture;
    texture_beam_exit = &resource_manager.get_texture("menu-ui-beam-exit")->texture;
    texture_beam_historic = &resource_manager.get_texture("menu-ui-beam-historic")->texture;
    texture_beam_modify = &resource_manager.get_texture("menu-ui-beam-modify")->texture;
    texture_beam_block_smashies = &resource_manager.get_texture("menu-ui-beam-block-smashies")->texture;
    texture_beam_embark = &resource_manager.get_texture("menu-ui-beam-embark")->texture;

    block_smashies_holos[0] = resource_manager.get_subtexture("dk-holograph-block-smashies-01");
    block_smashies_holos[1] = resource_manager.get_subtexture("dk-holograph-block-smashies-02");
    block_smashies_holos[2] = resource_manager.get_subtexture("holograph-block-smashies-01");
    block_smashies_holos[3] = resource_manager.get_subtexture("holograph-block-smashies-02");

    block_smashies_embark[0] = resource_manager.get_subtexture("dk-holograph-embark-01");
    block_smashies_embark[1] = resource_manager.get_subtexture("dk-holograph-embark-02");
    block_smashies_embark[2] = resource_manager.get_subtexture("holograph-embark-01");
    block_smashies_embark[3] = resource_manager.get_subtexture("holograph-embark-02");

    block_smashies_relive[0] = resource_manager.get_subtexture("dk-holograph-relive-01");
    block_smashies_relive[1] = resource_manager.get_subtexture("dk-holograph-relive-02");
    block_smashies_relive[2] = resource_manager.get_subtexture("holograph-relive-01");
    block_smashies_relive[3] = resource_manager.get_subtexture("holograph-relive-02");

    block_smashies_modify[0] = resource_manager.get_subtexture("dk-holograph-modify-01");
    block_smashies_modify[1] = resource_manager.get_subtexture("dk-holograph-modify-02");
    block_smashies_modify[2] = resource_manager.get_subtexture("holograph-modify-01");
    block_smashies_modify[3] = resource_manager.get_subtexture("holograph-modify-02");

    block_smashies_historic[0] = resource_manager.get_subtexture("dk-holograph-historic-01");
    block_smashies_historic[1] = resource_manager.get_subtexture("dk-holograph-historic-02");
    block_smashies_historic[2] = resource_manager.get_subtexture("holograph-historic-01");
    block_smashies_historic[3] = resource_manager.get_subtexture("holograph-historic-02");

    block_smashies_exit[0] = resource_manager.get_subtexture("dk-holograph-exit-01");
    block_smashies_exit[1] = resource_manager.get_subtexture("dk-holograph-exit-02");
    block_smashies_exit[2] = resource_manager.get_subtexture("holograph-exit-01");
    block_smashies_exit[3] = resource_manager.get_subtexture("holograph-exit-02");

    pixel7 = resource_manager.get_pixel7_font();

    toggle_beams = false;
    accumalator = 0.0f;
    beam_alpha = settings.colors.alpha_05;

    main_menu_scene.init = scene_init;
    main_menu_scene.update = scene_update;
    main_menu_scene.render = scene_render;
    main_menu_scene.cleanup = scene_cleanup;

    return &main_menu_scene;
};