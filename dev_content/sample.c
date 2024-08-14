
// paddle.h
typedef struct
{
    int x;
    int y;
    void *(*render)(void);
} Paddle;

Paddle create_paddle(void);

// padlle.c
Paddle create_paddle(void)
{
    Paddle paddle;
    paddle.x = 0;
    paddle.y = 0;
    paddle.render = render_paddle;
    return paddle;
}

void *render_paddle(Paddle *paddle)
{
}