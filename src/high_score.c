#include <string.h>
#include <stdio.h>
#include "parson.h"
#include "high_score.h"

#define MAX_HIGH_SCORES 10

void load_high_scores(const char *filename, HighScore *high_scores, int *count)
{
    // Initialize
    *count = 0;

    // Parse JSON file
    JSON_Value *root_value = json_parse_file(filename);
    if (root_value == NULL)
    {
        fprintf(stderr, "Error reading JSON file: %s\n", filename);
        return;
    }

    JSON_Object *root_object = json_value_get_object(root_value);
    JSON_Array *scores_array = json_object_get_array(root_object, "high_scores");
    if (scores_array == NULL)
    {
        fprintf(stderr, "No 'high_scores' array found in JSON file.\n");
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

void save_high_scores(const char *filename, HighScore *high_scores, int count)
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

    // Write JSON to file
    json_serialize_to_file(root_value, filename);
    json_value_free(root_value);
}

void add_high_score(HighScore *high_scores, int *count, const char *username, int score)
{
    if (*count < MAX_HIGH_SCORES)
    {
        // Add new high score
        strncpy(high_scores[*count].username, username, sizeof(high_scores[*count].username) - 1);
        high_scores[*count].username[sizeof(high_scores[*count].username) - 1] = '\0'; // Null-terminate
        high_scores[*count].score = score;
        (*count)++;
    }
    else
    {
        // Find the lowest score and replace it if necessary
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
}
