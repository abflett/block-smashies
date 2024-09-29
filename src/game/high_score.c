#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "parson.h"
#include "high_score.h"
#include "settings.h"
#include "b_utils.h"

#define MAX_HIGH_SCORES 10

// Helper function to check if a score is high enough to be added to the high scores list
bool is_high_score(const HighScore *high_scores, int count, int score)
{
    // If the current count is less than the maximum allowed, it's always a high score
    if (count < MAX_HIGH_SCORES)
    {
        return true;
    }

    // If the current count is at the maximum, check if the score is higher than the lowest score
    for (int i = 0; i < count; ++i)
    {
        if (score > high_scores[i].score)
        {
            return true;
        }
    }

    // If the score is not high enough, return false
    return false;
}

// Helper function to sort high scores in descending order
void sort_high_scores(HighScore *high_scores, int count)
{
    for (int i = 0; i < count - 1; ++i)
    {
        for (int j = i + 1; j < count; ++j)
        {
            if (high_scores[j].score > high_scores[i].score)
            {
                // Swap the scores
                HighScore temp = high_scores[i];
                high_scores[i] = high_scores[j];
                high_scores[j] = temp;
            }
        }
    }
}

void load_high_scores(HighScore *high_scores, int *count)
{
    // Initialize
    *count = 0;

    // Parse JSON file
    JSON_Value *root_value = json_parse_file(settings.file_locations.high_score_file);
    if (root_value == NULL)
    {
        log_error("Error reading JSON file");
        return;
    }

    JSON_Object *root_object = json_value_get_object(root_value);
    JSON_Array *scores_array = json_object_get_array(root_object, "high_scores");
    if (scores_array == NULL)
    {
        log_error("No 'high_scores' array found in JSON file.");
        json_value_free(root_value);
        return;
    }

    // Load scores
    size_t num_scores = json_array_get_count(scores_array);
    for (size_t i = 0; i < num_scores && i < MAX_HIGH_SCORES; ++i)
    {
        JSON_Object *score_object = json_array_get_object(scores_array, i);
        const char *username = json_object_get_string(score_object, "username");
        int score = (int)json_object_get_number(score_object, "score");

        strncpy(high_scores[i].username, username, sizeof(high_scores[i].username) - 1);
        high_scores[i].username[sizeof(high_scores[i].username) - 1] = '\0'; // Null-terminate
        high_scores[i].score = score;
        (*count)++;
    }

    json_value_free(root_value);
}

void save_high_scores(HighScore *high_scores, int count)
{
    // Create JSON object
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    // Create JSON array
    JSON_Value *scores_array_value = json_value_init_array();
    JSON_Array *scores_array = json_value_get_array(scores_array_value);
    json_object_set_value(root_object, "high_scores", scores_array_value);

    // Add high scores to array
    for (int i = 0; i < count && i < MAX_HIGH_SCORES; ++i)
    {
        JSON_Value *score_value = json_value_init_object();
        JSON_Object *score_object = json_value_get_object(score_value);

        json_object_set_string(score_object, "username", high_scores[i].username);
        json_object_set_number(score_object, "score", high_scores[i].score);

        json_array_append_value(scores_array, score_value);
    }

    json_serialize_to_file_pretty(root_value, settings.file_locations.high_score_file);
    json_value_free(root_value);
}

void add_high_score(HighScore *high_scores, int *count, const char *username, int score)
{
    if (*count < MAX_HIGH_SCORES)
    {
        // Add new high score at the end
        strncpy(high_scores[*count].username, username, sizeof(high_scores[*count].username) - 1);
        high_scores[*count].username[sizeof(high_scores[*count].username) - 1] = '\0'; // Null-terminate
        high_scores[*count].score = score;
        (*count)++;
    }
    else
    {
        // Find the lowest score and replace it if the new score is higher
        int min_index = 0;
        for (int i = 1; i < MAX_HIGH_SCORES; ++i)
        {
            if (high_scores[i].score < high_scores[min_index].score)
            {
                min_index = i;
            }
        }

        if (score > high_scores[min_index].score)
        {
            strncpy(high_scores[min_index].username, username, sizeof(high_scores[min_index].username) - 1);
            high_scores[min_index].username[sizeof(high_scores[min_index].username) - 1] = '\0'; // Null-terminate
            high_scores[min_index].score = score;
        }
    }

    // Sort the high scores after adding a new score
    sort_high_scores(high_scores, *count);
}
