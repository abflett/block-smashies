#ifndef HIGH_SCORE_H
#define HIGH_SCORE_H

typedef struct
{
    char username[10];
    int score;
} HighScore;

void load_high_scores(HighScore *high_scores, int *count);
void save_high_scores(const HighScore *high_scores, const int count);
void add_high_score(HighScore *high_scores, int *count, const char *username, const int score);
bool is_high_score(const HighScore *high_scores, const int count, const int score);

#endif
