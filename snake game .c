#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define PAUSE 112
#define ESC 27

#define MAP_X 3
#define MAP_Y 2
#define MAP_WIDTH 35
#define MAP_HEIGHT 20

int x[100], y[100]; //x,y ��ǥ���� ���� �� 100��
int food_x, food_y; //food�� ��ǥ���� ����
int obstacle_x, obstacle_y; //obstacle�� ��ǥ���� ����
int hurdle_x, hurdle_y; //hurdle�� ��ǥ���� ����
int bawall_x, bawall_y;                 //bawall�� ��ǥ���� ����
int life;                               //������� ����
int obstaclex[100], obstacley[100]; // ���ع��� ������ ���� �迭


int length;         //���� �����̸� ���
int speed;          //���� �ӵ�
int score;          //���� ����, initialize �Լ��� ���� �ʱ�ȭ��
int best_score = 0; //�ְ� ���� ����, initialize �Լ��� ���� �ʱ�ȭ ���� ����
int last_score = 0; //������ ���� ����, initialize �Լ��� ���� �ʱ�ȭ ���� ����
int ob_count = 0; //�ʿ� ���ع��� ī��Ʈ�ϱ����� ����


int dir; //�̵����� ����
int key; //�Է¹��� Ű ����

int status_on = 0; //�����ڿ� status ǥ�� Ȱ��ȭ ����, �����߿� SŰ�� ������ Ȱ��ȭ
int game_over = 0;

void gotoxy(int x, int y, char* s);
void title(void);      //���� ����ȭ��
void initialize(void); //���� �ʱ�ȭ
void draw_map(void);   //������ �׵θ��� �׸�
void move(int dir);    //��Ӹ��� �̵�
void pause(void);      //�Ͻ�����
void results(void);    //�������Ḧ Ȯ��
void food(void);       //���� ����
void obstacle(void);   //��ֹ� ����
void hurdle(void);     //�ѹ� ��ֹ� ����
void bawall(void);     //bawall ����
void status(void);     //�����ڿ� status ǥ��
void record(void);     //������ ���Ϸ� ����

int main()
{
    title();

    while (1)
    {
        if (_kbhit())
        {
            do
                key = _getch();
            while (key == 224); //Ű �Է¹���
        }

        Sleep(speed);

        switch (key)
        { //�Է¹��� Ű�� �ľ��ϰ� ����
        case LEFT:
        case RIGHT:
        case UP:
        case DOWN:
            if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) || (dir == UP && key != DOWN) || (dir == DOWN && key != UP))
                dir = key; //180�� ȸ���̵��� �����ϱ� ���� �ʿ�

            key = 0; //Ű���� �����ϴ� �Լ��� initialize
            break;

        case PAUSE: //PŰ�� ������ �Ͻ�����
            pause();
            break;

        case 115: //SŰ�� ������ �����ڿ� status�� Ȱ��ȭ ��Ŵ
            if (status_on == 0)
                status_on = 1;
            else
                status_on = 0;
            key = 0;
            break;

        case ESC:          //ESCŰ�� ������ ���α׷� ����
            system("cls"); //Clear screen
            exit(0);

        default:
            break;
        }

        move(dir);

        if (status_on == 1)
            status(); //status ǥ��
        if (game_over)
        {

            game_over = 0;
            results();

            key = _getch();
            system("cls"); //Clear screen

            if (key == 'y')
            {
                record();
                system("cls"); //Clear screen
            }

            title();
        }
    }
}

void gotoxy(int x, int y, char* s)
{
    COORD pos = { 2 * x, y }; //x���� 2x�� ����
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", s); //��ǥ���� �ٷ� ���ڿ��� �Է��� �� �ֵ��� printf �Լ� �߰�
}

void title(void)
{ // ù ȭ�� - ���α׷� �Ұ�
    while (_kbhit())
        _getch(); //���ۿ� �ִ� Ű���� ����

    gotoxy(MAP_X + 2, MAP_Y + 3, "* Welcome to C programming class");
    gotoxy(MAP_X + 2, MAP_Y + 6, "* Snake game instructions");
    gotoxy(MAP_X + 2, MAP_Y + 7, "1. Use arrow keys to move the snake");
    gotoxy(MAP_X + 2, MAP_Y + 8, "2. You will be provided foods at the several coordinates of the screen which you have to eat");
    gotoxy(MAP_X + 2, MAP_Y + 9, "3. Everytime you eat a food, the length of the snake will be increased by 1 element and thus the score");
    gotoxy(MAP_X + 2, MAP_Y + 10, "4. Here you are provided with three lives");
    gotoxy(MAP_X + 2, MAP_Y + 11, "5. Your life will decrease as you hit the wall or snake's body");
    gotoxy(MAP_X + 2, MAP_Y + 12, "6. You can pause the game in its middle by pressing P key");
    gotoxy(MAP_X + 2, MAP_Y + 13, "7. To continue the paused game, press any other key once again");
    gotoxy(MAP_X + 2, MAP_Y + 14, "8. If you want to exit, press ESC");

    gotoxy(MAP_X + 2, MAP_Y + 17, " PRESS ANY KEY TO START :   ");
    gotoxy(MAP_X + 2, MAP_Y + 18, " �� ��,��,��,�� : Move      ");
    gotoxy(MAP_X + 2, MAP_Y + 19, " �� P : Pause               ");
    gotoxy(MAP_X + 2, MAP_Y + 20, " �� ESC : Quit              ");

    while (1)
    {
        if (_kbhit())
        { //Ű�Է¹���
            key = _getch();
            if (key == ESC)
            {
                system("cls"); //Clear screen
                exit(0);       //���α׷� ����
            }
            else
                break; //�ƴϸ� ��� ����
        }

        gotoxy(MAP_X + 2, MAP_Y + 17, " PRESS ANY KEY TO START :   ");
        Sleep(500);
        gotoxy(MAP_X + 2, MAP_Y + 17, "                            ");
        Sleep(500);
    }

    initialize(); // ���� �ʱ�ȭ
}

void initialize(void)
{
    int i;

    system("cls"); //Clear screen
    draw_map();    //�� �׵θ��� �׸�
    fflush(stdin); //���ۿ� �ִ� Ű���� ����

    dir = LEFT;  // ���� �ʱ�ȭ
    speed = 180; // �ӵ� �ʱ�ȭ
    length = 5;  //�� ���� �ʱ�ȭ
    score = 0;   //���� �ʱ�ȭ
    life = 3;   //����� �ʱ�ȭ

    for (i = 0; i < length; i++)
    { //�� ���밪 �Է�

        x[i] = MAP_WIDTH / 2 + i;
        y[i] = MAP_HEIGHT / 2;
        gotoxy(MAP_X + x[i], MAP_Y + y[i], "��");
    }

    gotoxy(MAP_X + x[0], MAP_Y + y[0], "��"); //�� �Ӹ� �׸�

    food(); //food ����
    obstacle(); // ��ֹ� ����
    hurdle();   // �ѹ� ��ֹ� ����
    bawall();
}

void draw_map(void)
{ //�� �׵θ� �׸��� �Լ�
    int i;

    for (i = 0; i < MAP_WIDTH; i++)
    {

        gotoxy(MAP_X + i, MAP_Y, "��"); // "��" etc
    }

    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++)
    {

        gotoxy(MAP_X, i, "��");
        gotoxy(MAP_X + MAP_WIDTH - 1, i, "��");
    }

    for (i = 0; i < MAP_WIDTH; i++)
    {
        gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "��");
    }
}

void move(int dir)
{
    int i;

    if (x[0] == food_x && y[0] == food_y)
    {                                  //food�� �浹���� ���
        score += 10;                   //���� ����
        food();                        //���ο� food �߰�
        gotoxy(MAP_X + hurdle_x, MAP_Y + hurdle_y, "  ");        //������ �ִ� hurdle ����
        gotoxy(MAP_X + bawall_x, MAP_Y + bawall_y, "  ");        //������ �ִ� bawall ����
        bawall();                           //bawall�߰�
        obstacle();                           //���ο� obstacle �߰�
        hurdle();                             //���ο� hurdle �߰�
        length++;                      //��������
        x[length - 1] = x[length - 2]; //���θ��� ���뿡 �� �Է�
        y[length - 1] = y[length - 2];
    }

    for (int i = 0; i < ob_count; i++) {
        if (x[0] == obstaclex[i] && y[0] == obstacley[i]) //���� �Ӹ��� ���ع����� ����ִ� �迭�� ��ǥ�� �ϳ��ϳ� ��
        {                                  //obstacle�� �浹���� ��� 
            score -= 10; //���� ���� 
            obstacle(); //���ο� obstacle �߰� 
            length--; //���̰��� 
            speed += 5;//�ӵ� ����
            gotoxy(MAP_X + x[length], MAP_Y + y[length], "  "); //���� �ϳ��� ����
            break;

        }
    }

    if (x[0] == obstacle_x && y[0] == obstacle_y)  //???????????????????????????????
    {                                  //obstacle�� �浹���� ��� 
        score -= 10; //���� ���� 
        obstacle(); //���ο� obstacle �߰� 
        length--; //���̰���
        speed += 5;//�ӵ� ����
        gotoxy(MAP_X + x[length], MAP_Y + y[length], "  "); //���� �ϳ��� ����
    }
    if (x[0] == hurdle_x && y[0] == hurdle_y) { //hurdle�� �浹���� ���  
        game_over = 1;
        return;
    }



    if (length == 1)
    {
        game_over = 1;
        return;
    }

    if (x[0] == bawall_x && y[0] == bawall_y) {
        life = life - 1;
        bawall();
    }
    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, "");
    printf("LIFE: %3d, YOUR SCORE: %3d, LAST SCORE: %3d, BEST SCORE: %3d", life, score, last_score, best_score);

    if (life <= 0) {
        game_over = 1;
    }


    if (x[0] == 0 || x[0] == MAP_WIDTH - 1 || y[0] == 0 || y[0] == MAP_HEIGHT - 1)
    { //���� �浹���� ���
        game_over = 1;
        return;
    }

    for (i = 1; i < length; i++)
    { //�ڱ���� �浹�ߴ��� �˻�

        if (x[0] == x[i] && y[0] == y[i])
        {
            game_over = 1;
            return;
        }
    }

    gotoxy(MAP_X + x[length - 1], MAP_Y + y[length - 1], "  "); //���� �������� ����

    for (i = length - 1; i > 0; i--)
    { //������ǥ�� ��ĭ�� �ű�
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }

    gotoxy(MAP_X + x[0], MAP_Y + y[0], "��"); //�Ӹ��� �ִ����� �������� ��ħ

    if (dir == LEFT)
        --x[0]; //���⿡ ���� ���ο� �Ӹ���ǥ(x[0],y[0])���� ����
    else if (dir == RIGHT)
        ++x[0];
    else if (dir == UP)
        --y[0];
    else if (dir == DOWN)
        ++y[0];

    gotoxy(MAP_X + x[i], MAP_Y + y[i], "��"); //���ο� �Ӹ���ǥ���� �Ӹ��� �׸�
}

void pause(void)
{ // pŰ�� ������ ��� ������ �Ͻ� ����
    while (1)
    {
        if (key == PAUSE)
        {
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "< PAUSE : PRESS ANY KEY TO RESUME > ");
            Sleep(400);
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "                                    ");
            Sleep(400);
        }
        else
        {
            draw_map();
            return;
        }

        if (_kbhit())
        {
            do
            {
                key = _getch();
            } while (key == 224);
        }
    }
}

void results(void)
{ //�������� �Լ�
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 5, "+----------------------+");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 6, "|      GAME OVER       |");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 7, "+----------------------+");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 8, " YOUR SCORE : ");
    printf("%d", last_score = score);

    if (score > best_score)
    {
        best_score = score;
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 10, "Best Score! Congratulation!");
    }

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 12, "If you want to record your score,");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 8, MAP_Y + 13, "Press 'y' or any key to quit ... : ");
}

void bawall(void) {
    int f;
    int bawall_crush_on = 0;
    int g = 0;



    while (1) {

        bawall_crush_on = 0;
        srand((unsigned)time(NULL) / 2 + g);

        bawall_x = (rand() % (MAP_WIDTH - 2)) + 1;
        bawall_y = (rand() % (MAP_HEIGHT - 2)) + 1;

        for (f = 0; f < length; f++) {
            if (bawall_x == x[f] && bawall_y == y[f]) {
                bawall_crush_on = 1;
                g++;
                break;
            }
        }
        if (bawall_x == food_x && bawall_y == food_y) bawall_crush_on = 1;
        if (bawall_x == hurdle_x && bawall_y == hurdle_y) bawall_crush_on = 1;
        if (bawall_x == obstacle_x && bawall_y == obstacle_y) bawall_crush_on = 1;

        if (bawall_crush_on == 1) continue;
        gotoxy(MAP_X + bawall_x, MAP_Y + bawall_y, "��");
        break;
    }
}

void food(void)
{
    int i;
    int food_crush_on = 0; //food�� �� ������ǥ�� ���� ��� on
    int r = 0;             //���� ������ ���Ǵ� ����

    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, "");
    printf("LIFE: %3d, YOUR SCORE: %3d, LAST SCORE: %3d, BEST SCORE: %3d", life, score, last_score, best_score);

    while (1)
    {
        food_crush_on = 0;

        srand((unsigned)time(NULL) + r); //����ǥ ����

        food_x = (rand() % (MAP_WIDTH - 2)) + 1; //������ ��ǥ���� ����
        food_y = (rand() % (MAP_HEIGHT - 2)) + 1;

        for (i = 0; i < length; i++)
        { //food�� �� ����� ��ġ���� Ȯ��

            if (food_x == x[i] && food_y == y[i])
            {
                food_crush_on = 1; //��ġ�� food_crush_on �� on
                r++;
                break;
            }
        }

        // ��ֹ��� ��ġ�� �ٽ� ����
        if (food_x == hurdle_x && food_y == hurdle_y) food_crush_on = 1;
        if (food_x == obstacle_x && food_y == obstacle_y) food_crush_on = 1;
        if (food_x == bawall_x && food_y == bawall_y) food_crush_on = 1;

        if (food_crush_on == 1)
            continue; //������ ��� while���� �ٽ� ����

        gotoxy(MAP_X + food_x, MAP_Y + food_y, "��"); //�Ȱ����� ��� ��ǥ���� food�� ���
        speed -= 5;//�ӵ� ����
        break;
    }
}

void obstacle(void)
{
    int i;
    int obstacle_crush_on = 0; //obstacle�� �� ������ǥ�� ���� ��� on
    int r = 1600;             //���� ������ ���Ǵ� ����

    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, "");
    printf("LIFE: %3d, YOUR SCORE: %3d, LAST SCORE: %3d, BEST SCORE: %3d", life, score, last_score, best_score);

    while (1)
    {
        obstacle_crush_on = 0;

        srand((unsigned)time(NULL) / 5 + r); //����ǥ ����

        obstacle_x = (rand() % (MAP_WIDTH - 2)) + 1; //������ ��ǥ���� ����
        obstacle_y = (rand() % (MAP_HEIGHT - 2)) + 1;

        for (i = 0; i < length; i++)
        { //obstacle�� �� ����� ��ġ���� Ȯ��

            if (obstacle_x == x[i] && obstacle_y == y[i])
            {
                obstacle_crush_on = 1; //��ġ�� obstacle_crush_on �� on
                r++;
                break;
            }
        }

        // food�� ��ġ�� �ٽ� ����
        if (obstacle_x == hurdle_x && obstacle_y == hurdle_y) obstacle_crush_on = 1;
        if (obstacle_x == food_x && obstacle_y == food_y) obstacle_crush_on = 1;
        if (obstacle_x == bawall_x && obstacle_y == bawall_y) obstacle_crush_on = 1;


        if (obstacle_crush_on == 1)
            continue; //������ ��� while���� �ٽ� ����

        obstaclex[ob_count] = obstacle_x;
        obstacley[ob_count] = obstacle_y;

        ob_count++; //obstaclex,obstacley �� ���� �˱����� int ����


        gotoxy(MAP_X + obstacle_x, MAP_Y + obstacle_y, "��"); //�Ȱ����� ��� ��ǥ���� obstacle�� ���

        break;
    }
}

void hurdle(void) {

    int i;
    int hurdle_crush_on = 0;              //hurdle�� �� ������ǥ�� ���� ��� on 
    int r = 150;                          //food ������ �Ȱ�ġ�� r�� 100���� -�Ǵ� �������� �ٲ� 

    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, "");
    printf("LIFE: %3d, YOUR SCORE: %3d, LAST SCORE: %3d, BEST SCORE: %3d", life, score, last_score, best_score);

    while (1) {

        hurdle_crush_on = 0;

        srand((unsigned)time(NULL) / 7 + r);            //����ǥ ���� 

        hurdle_x = (rand() % (MAP_WIDTH - 2)) + 1;    //������ ��ǥ���� ���� 
        hurdle_y = (rand() % (MAP_HEIGHT - 2)) + 1;

        for (i = 0; i < length; i++) {              //hurdle�� �� ����� ��ġ���� Ȯ��  

            if (hurdle_x == x[i] && hurdle_y == y[i]) {
                hurdle_crush_on = 1;                  //��ġ�� hurdle_crush_on �� on 
                r++;
                break;
            }

        }

        if (hurdle_x == food_x && hurdle_y == food_y) hurdle_crush_on = 1;
        if (hurdle_x == obstacle_x && hurdle_y == obstacle_y) hurdle_crush_on = 1;
        if (hurdle_x == bawall_x && hurdle_y == bawall_y) hurdle_crush_on = 1;


        if (hurdle_crush_on == 1) continue;                //������ ��� while���� �ٽ� ���� 

        gotoxy(MAP_X + hurdle_x, MAP_Y + hurdle_y, "��");     //�Ȱ����� ��� ��ǥ���� hurdle�� ��� 
        break;

    }

}

void status(void)
{ //����  status�� ���� �ִ� �Լ�
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y, "head=   ");
    printf("%2d,%2d", x[0], y[0]);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 1, "food=   ");
    printf("%2d,%2d", food_x, food_y);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 2, "length= ");
    printf("%3d", length);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 3, "key=    ");
    if (dir == LEFT)
        printf("LEFT ");
    else if (dir == RIGHT)
        printf("RIGHT");
    else if (dir == UP)
        printf("UP   ");
    else if (dir == DOWN)
        printf("DOWN ");
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 4, "speed=  ");
    printf("%3d", speed);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 5, "score=  ");
    printf("%3d", score);
}

void record(void)
{
    char plname[40], nplname[40], cha, c;
    int i, j;
    FILE* info;
    time_t mytime;

    info = fopen("record.txt", "a+");
    printf("Enter your name\n");
    scanf("%s", plname); //scanf("%[^\n]", plname);
    fflush(stdin);

    for (j = 0; plname[j] != '\0'; j++) //to convert the first letter after space to capital
    {
        nplname[0] = toupper(plname[0]);
        if (plname[j - 1] == ' ')
        {
            nplname[j] = toupper(plname[j]);
            nplname[j - 1] = plname[j - 1];
        }
        else
            nplname[j] = plname[j];
    }
    nplname[j] = '\0';

    fprintf(info, "Player Name :%s\n", nplname);

    mytime = time(NULL);
    fprintf(info, "Played Date:%s", ctime(&mytime));
    fprintf(info, "Score:%d\n", score);

    for (i = 0; i <= 50; i++)
        fprintf(info, "%c", '_');
    fprintf(info, "\n");
    fclose(info);

    printf("\n\nIf you want to see past records, press 'y'\n");

    cha = _getch();
    system("cls");

    if (cha == 'y')
    {
        info = fopen("record.txt", "r");
        do
        {
            putchar(c = getc(info));
        } while (c != EOF);
        cha = _getch();
    }
    fclose(info);
}