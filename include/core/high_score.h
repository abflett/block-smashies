// high_score.h

#ifndef HIGH_SCORE_H
#define HIGH_SCORE_H

#include <stdio.h>

typedef struct
{
    char username[256];
    int score;
} HighScore;

void load_high_scores(const char *filename, HighScore *high_scores, int *count);
void save_high_scores(const char *filename, HighScore *high_scores, int count);
void add_high_score(HighScore *high_scores, int *count, const char *username, int score);

#endif