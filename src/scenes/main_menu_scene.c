#include "raylib.h"
#include "game.h"
#include "scene_manager.h"
#include "resource_manager.h"
#include "settings.h"

#define MAX_DISPLAY_CHARS 51

static Scene main_menu_scene;
static InputMapping *input;

static Texture2D *texture_stars;
static Texture2D *texture_station;
static Texture2D *texture_beam_relive;
static Texture2D *texture_beam_exit;
static Texture2D *texture_beam_historic;
static Texture2D *texture_beam_modify;
static Texture2D *texture_beam_block_smashies;
static Texture2D *texture_beam_embark;

static Subtexture *text_block_smashies[4];
static Subtexture *text_embark[4];
static Subtexture *text_relive[4];
static Subtexture *text_modify[4];
static Subtexture *text_historic[4];
static Subtexture *text_exit[4];

static Font *pixel7;
static Color beam_alpha;
static Color text_alpha1;
static Color text_alpha2;
static bool toggle_beams;
static float accumulator;

static int selected_menu_option = 0; // 0 for Embark, 1 for Relive, 2 for Modify, etc.
static int num_menu_options = 5;     // Number of options in your menu

static float min_scene_time;

static const char *flavor_texts[] = {
    "Begin your journey as a Block Smashy. Relive the legend from the start.          ",
    "Jump back to a pivotal moment. Continue the mission from a chosen point.          ",
    "Adjust simulation settings to match your preferences.          ",
    "Explore the history of the heroes and their greatest achievements.          ",
    "Leave the station and return to the mundane... for now.          "};
int flavor_text_scroll_index = 0;      // Starting index of the displayed text
float flavor_text_scroll_timer = 0.0f; // Timer for controlling scroll speed
float scroll_speed = 0.3f;             // Adjust this to change the scroll speed

void update_flavor_text_scroll(const float delta_time)
{
    flavor_text_scroll_timer += delta_time;

    // If enough time has passed, move the scroll index
    if (flavor_text_scroll_timer >= scroll_speed)
    {
        flavor_text_scroll_index++;
        flavor_text_scroll_timer = 0.0f;

        // Get the current text length for the selected menu option
        const char *current_text = flavor_texts[selected_menu_option];
        const int text_length = (int)strlen(current_text);

        // Loop the text if the scroll index goes past the text length
        if (flavor_text_scroll_index >= text_length)
        {
            flavor_text_scroll_index = 0;
        }
    }
}

void draw_flavor_text(void)
{
    // Get the current flavor text for the selected option
    const char *current_text = flavor_texts[selected_menu_option];
    const int text_length = (int)strlen(current_text);

    // Determine how many characters to display (circular scrolling)
    // int end_index = flavor_text_scroll_index + MAX_DISPLAY_CHARS;
    char displayed_text[MAX_DISPLAY_CHARS + 1];

    // Copy the substring to be displayed, considering looping around the text
    for (int i = 0; i < MAX_DISPLAY_CHARS; i++)
    {
        const int index = (flavor_text_scroll_index + i) % text_length;
        displayed_text[i] = current_text[index];
    }
    displayed_text[MAX_DISPLAY_CHARS] = '\0'; // Null-terminate the string

    // Draw the text on the screen
    DrawTextEx(*pixel7, displayed_text, (Vector2){106, 53}, 7, 0.0f, settings.colors.blue_03);
}

static void scene_init(const int arg_count, va_list args)
{
    min_scene_time = settings.game.min_screen_time;
}

static void scene_update(const float delta_time)
{
    if (min_scene_time >= 0)
    {
        min_scene_time -= delta_time;
    }

    if (IsKeyPressed(input->action_k_DOWN) || IsGamepadButtonPressed(0, input->action_DOWN))
    {
        selected_menu_option = (selected_menu_option + 1) % num_menu_options; // Loop back to the first option
    }

    if (IsKeyPressed(input->action_k_UP) || IsGamepadButtonPressed(0, input->action_UP))
    {
        selected_menu_option = (selected_menu_option - 1 + num_menu_options) % num_menu_options; // Loop to the last option
    }

    if (IsKeyPressed(input->action_k_ENTER) || IsGamepadButtonPressed(0, input->action_A) || IsGamepadButtonPressed(0, input->action_START) && min_scene_time <= 0.0f)
    {
        switch (selected_menu_option)
        {
        case 0: // Embark
            scene_manager.change(scene_manager.scenes.embark, 1, 0);
            break;
        case 1: // Relive
            scene_manager.change(scene_manager.scenes.embark, 1, 1);
            break;
        case 2: // Modify
            // Add logic for settings
            // show game_state for settings - is an overlay
            scene_manager.change(scene_manager.scenes.embark, 1, 0);
            break;
        case 3: // Historic
            // Add logic for showing credits/high scores
            scene_manager.change(scene_manager.scenes.embark, 1, 1);
            break;
        case 4: // Exit
            exit_game();
            break;
        default:
            break;
        }
    }

    if (IsKeyPressed(settings.inputs[0].action_k_ESCAPE))
    {
        exit_game();
    }

    update_flavor_text_scroll(delta_time);

    accumulator += delta_time;
    if (accumulator > 0.05)
    {
        text_alpha1 = (Color){255, 255, 255, GetRandomValue(200, 255)};
        text_alpha2 = (Color){255, 255, 255, GetRandomValue(200, 255)};
        beam_alpha = (Color){255, 255, 255, GetRandomValue(100, 155)};
        accumulator = 0;
    }
}

static void scene_render(void)
{
    DrawTexture(*texture_stars, 0, 0, WHITE);
    DrawTexture(*texture_station, 0, 0, WHITE);

    // DrawTextEx(*pixel7, "Step into the Archive, where we celebrate the legendary heroes who shaped history!", (Vector2){106, 53}, 7, 0.0f, settings.colors.blue_04);
    draw_flavor_text();
    DrawTexture(*texture_beam_relive, 194, 87, beam_alpha);
    DrawTexture(*texture_beam_exit, 228, 112, beam_alpha);
    DrawTexture(*texture_beam_historic, 51, 131, beam_alpha);
    DrawTexture(*texture_beam_modify, 22, 99, beam_alpha);
    DrawTexture(*texture_beam_block_smashies, 113, 19, beam_alpha);
    DrawTexture(*texture_beam_embark, 41, 68, beam_alpha);

    const int embark_index_a = (selected_menu_option == 0) ? 1 : 0;
    const int relive_index_a = (selected_menu_option == 1) ? 1 : 0;
    const int modify_index_a = (selected_menu_option == 2) ? 1 : 0;
    const int historic_index_a = (selected_menu_option == 3) ? 1 : 0;
    const int exit_index_a = (selected_menu_option == 4) ? 1 : 0;

    const int embark_index_b = (selected_menu_option == 0) ? 3 : 2;
    const int relive_index_b = (selected_menu_option == 1) ? 3 : 2;
    const int modify_index_b = (selected_menu_option == 2) ? 3 : 2;
    const int historic_index_b = (selected_menu_option == 3) ? 3 : 2;
    const int exit_index_b = (selected_menu_option == 4) ? 3 : 2;

    DrawTextureRec(text_block_smashies[0]->texture_resource->texture, text_block_smashies[0]->src, (Vector2){113, 19}, text_alpha1);
    DrawTextureRec(text_block_smashies[2]->texture_resource->texture, text_block_smashies[2]->src, (Vector2){113, 20}, text_alpha2);

    DrawTextureRec(text_embark[embark_index_a]->texture_resource->texture, text_embark[embark_index_a]->src, (Vector2){41, 68}, text_alpha1);
    DrawTextureRec(text_relive[relive_index_a]->texture_resource->texture, text_relive[relive_index_a]->src, (Vector2){194, 87}, text_alpha1);
    DrawTextureRec(text_modify[modify_index_a]->texture_resource->texture, text_modify[modify_index_a]->src, (Vector2){22, 99}, text_alpha1);
    DrawTextureRec(text_historic[historic_index_a]->texture_resource->texture, text_historic[historic_index_a]->src, (Vector2){51, 148}, text_alpha1);
    DrawTextureRec(text_exit[exit_index_a]->texture_resource->texture, text_exit[exit_index_a]->src, (Vector2){239, 126}, text_alpha1);

    DrawTextureRec(text_embark[embark_index_b]->texture_resource->texture, text_embark[embark_index_b]->src, (Vector2){41, 69}, text_alpha2);
    DrawTextureRec(text_relive[relive_index_b]->texture_resource->texture, text_relive[relive_index_b]->src, (Vector2){194, 88}, text_alpha2);
    DrawTextureRec(text_modify[modify_index_b]->texture_resource->texture, text_modify[modify_index_b]->src, (Vector2){22, 100}, text_alpha2);
    DrawTextureRec(text_historic[historic_index_b]->texture_resource->texture, text_historic[historic_index_b]->src, (Vector2){51, 149}, text_alpha2);
    DrawTextureRec(text_exit[exit_index_b]->texture_resource->texture, text_exit[exit_index_b]->src, (Vector2){239, 127}, text_alpha2);
}

static void scene_cleanup(void)
{
    // Cleanup resources used by the main menu
}

Scene *create_main_menu_scene()
{
    texture_stars = &resource_manager.get_texture("menu-ui-stars")->texture;
    texture_station = &resource_manager.get_texture("menu-ui-station")->texture;
    texture_beam_relive = &resource_manager.get_texture("menu-ui-beam-relive")->texture;
    texture_beam_exit = &resource_manager.get_texture("menu-ui-beam-exit")->texture;
    texture_beam_historic = &resource_manager.get_texture("menu-ui-beam-historic")->texture;
    texture_beam_modify = &resource_manager.get_texture("menu-ui-beam-modify")->texture;
    texture_beam_block_smashies = &resource_manager.get_texture("menu-ui-beam-block-smashies")->texture;
    texture_beam_embark = &resource_manager.get_texture("menu-ui-beam-embark")->texture;

    text_block_smashies[0] = resource_manager.get_subtexture("dk-holograph-block-smashies-01");
    text_block_smashies[1] = resource_manager.get_subtexture("dk-holograph-block-smashies-02");
    text_block_smashies[2] = resource_manager.get_subtexture("holograph-block-smashies-01");
    text_block_smashies[3] = resource_manager.get_subtexture("holograph-block-smashies-02");

    text_embark[0] = resource_manager.get_subtexture("dk-holograph-embark-01");
    text_embark[1] = resource_manager.get_subtexture("dk-holograph-embark-02");
    text_embark[2] = resource_manager.get_subtexture("holograph-embark-01");
    text_embark[3] = resource_manager.get_subtexture("holograph-embark-02");

    text_relive[0] = resource_manager.get_subtexture("dk-holograph-relive-01");
    text_relive[1] = resource_manager.get_subtexture("dk-holograph-relive-02");
    text_relive[2] = resource_manager.get_subtexture("holograph-relive-01");
    text_relive[3] = resource_manager.get_subtexture("holograph-relive-02");

    text_modify[0] = resource_manager.get_subtexture("dk-holograph-modify-01");
    text_modify[1] = resource_manager.get_subtexture("dk-holograph-modify-02");
    text_modify[2] = resource_manager.get_subtexture("holograph-modify-01");
    text_modify[3] = resource_manager.get_subtexture("holograph-modify-02");

    text_historic[0] = resource_manager.get_subtexture("dk-holograph-historic-01");
    text_historic[1] = resource_manager.get_subtexture("dk-holograph-historic-02");
    text_historic[2] = resource_manager.get_subtexture("holograph-historic-01");
    text_historic[3] = resource_manager.get_subtexture("holograph-historic-02");

    text_exit[0] = resource_manager.get_subtexture("dk-holograph-exit-01");
    text_exit[1] = resource_manager.get_subtexture("dk-holograph-exit-02");
    text_exit[2] = resource_manager.get_subtexture("holograph-exit-01");
    text_exit[3] = resource_manager.get_subtexture("holograph-exit-02");

    pixel7 = resource_manager.get_pixel7_font();

    toggle_beams = false;
    accumulator = 0.0f;
    beam_alpha = settings.colors.alpha_05;

    input = &settings.inputs[0];

    main_menu_scene.init = scene_init;
    main_menu_scene.update = scene_update;
    main_menu_scene.render = scene_render;
    main_menu_scene.cleanup = scene_cleanup;

    return &main_menu_scene;
};