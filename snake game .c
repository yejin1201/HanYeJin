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

int x[100], y[100]; //x,y 좌표값을 저장 총 100개
int food_x, food_y; //food의 좌표값을 저장
int obstacle_x, obstacle_y; //obstacle의 좌표값을 저장
int hurdle_x, hurdle_y; //hurdle의 좌표값을 저장
int bawall_x, bawall_y;                 //bawall의 좌표값을 저장
int life;                               //목숨값을 저장
int obstaclex[100], obstacley[100]; // 장해물들 저장을 위한 배열


int length;         //뱀의 몸길이를 기억
int speed;          //게임 속도
int score;          //점수 저장, initialize 함수에 의해 초기화됨
int best_score = 0; //최고 점수 저장, initialize 함수에 의해 초기화 되지 않음
int last_score = 0; //마지막 점수 저장, initialize 함수에 의해 초기화 되지 않음
int ob_count = 0; //맵에 장해물을 카운트하기위한 변수


int dir; //이동방향 저장
int key; //입력받은 키 저장

int status_on = 0; //개발자용 status 표시 활성화 변수, 게임중에 S키를 누르면 활성화
int game_over = 0;

void gotoxy(int x, int y, char* s);
void title(void);      //게임 시작화면
void initialize(void); //게임 초기화
void draw_map(void);   //게임판 테두리를 그림
void move(int dir);    //뱀머리를 이동
void pause(void);      //일시정지
void results(void);    //게임종료를 확인
void food(void);       //먹이 생성
void obstacle(void);   //장애물 생성
void hurdle(void);     //한방 장애물 생성
void bawall(void);     //bawall 생성
void status(void);     //개발자용 status 표시
void record(void);     //점수를 파일로 저장

int main()
{
    title();

    while (1)
    {
        if (_kbhit())
        {
            do
                key = _getch();
            while (key == 224); //키 입력받음
        }

        Sleep(speed);

        switch (key)
        { //입력받은 키를 파악하고 실행
        case LEFT:
        case RIGHT:
        case UP:
        case DOWN:
            if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) || (dir == UP && key != DOWN) || (dir == DOWN && key != UP))
                dir = key; //180도 회전이동을 방지하기 위해 필요

            key = 0; //키값을 저장하는 함수를 initialize
            break;

        case PAUSE: //P키를 누르면 일시정지
            pause();
            break;

        case 115: //S키를 누르면 개발자용 status를 활성화 시킴
            if (status_on == 0)
                status_on = 1;
            else
                status_on = 0;
            key = 0;
            break;

        case ESC:          //ESC키를 누르면 프로그램 종료
            system("cls"); //Clear screen
            exit(0);

        default:
            break;
        }

        move(dir);

        if (status_on == 1)
            status(); //status 표시
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
    COORD pos = { 2 * x, y }; //x값을 2x로 변경
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", s); //좌표값에 바로 문자열을 입력할 수 있도록 printf 함수 추가
}

void title(void)
{ // 첫 화면 - 프로그램 소개
    while (_kbhit())
        _getch(); //버퍼에 있는 키값을 버림

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
    gotoxy(MAP_X + 2, MAP_Y + 18, " ◇ ←,→,↑,↓ : Move      ");
    gotoxy(MAP_X + 2, MAP_Y + 19, " ◇ P : Pause               ");
    gotoxy(MAP_X + 2, MAP_Y + 20, " ◇ ESC : Quit              ");

    while (1)
    {
        if (_kbhit())
        { //키입력받음
            key = _getch();
            if (key == ESC)
            {
                system("cls"); //Clear screen
                exit(0);       //프로그램 종료
            }
            else
                break; //아니면 계속 진행
        }

        gotoxy(MAP_X + 2, MAP_Y + 17, " PRESS ANY KEY TO START :   ");
        Sleep(500);
        gotoxy(MAP_X + 2, MAP_Y + 17, "                            ");
        Sleep(500);
    }

    initialize(); // 게임 초기화
}

void initialize(void)
{
    int i;

    system("cls"); //Clear screen
    draw_map();    //맵 테두리를 그림
    fflush(stdin); //버퍼에 있는 키값을 버림

    dir = LEFT;  // 방향 초기화
    speed = 180; // 속도 초기화
    length = 5;  //뱀 길이 초기화
    score = 0;   //점수 초기화
    life = 3;   //목숨값 초기화

    for (i = 0; i < length; i++)
    { //뱀 몸통값 입력

        x[i] = MAP_WIDTH / 2 + i;
        y[i] = MAP_HEIGHT / 2;
        gotoxy(MAP_X + x[i], MAP_Y + y[i], "○");
    }

    gotoxy(MAP_X + x[0], MAP_Y + y[0], "●"); //뱀 머리 그림

    food(); //food 생성
    obstacle(); // 장애물 생성
    hurdle();   // 한방 장애물 생성
    bawall();
}

void draw_map(void)
{ //맵 테두리 그리는 함수
    int i;

    for (i = 0; i < MAP_WIDTH; i++)
    {

        gotoxy(MAP_X + i, MAP_Y, "■"); // "■" etc
    }

    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++)
    {

        gotoxy(MAP_X, i, "■");
        gotoxy(MAP_X + MAP_WIDTH - 1, i, "■");
    }

    for (i = 0; i < MAP_WIDTH; i++)
    {
        gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "■");
    }
}

void move(int dir)
{
    int i;

    if (x[0] == food_x && y[0] == food_y)
    {                                  //food와 충돌했을 경우
        score += 10;                   //점수 증가
        food();                        //새로운 food 추가
        gotoxy(MAP_X + hurdle_x, MAP_Y + hurdle_y, "  ");        //기존에 있던 hurdle 제거
        gotoxy(MAP_X + bawall_x, MAP_Y + bawall_y, "  ");        //기존에 있던 bawall 제거
        bawall();                           //bawall추가
        obstacle();                           //새로운 obstacle 추가
        hurdle();                             //새로운 hurdle 추가
        length++;                      //길이증가
        x[length - 1] = x[length - 2]; //새로만든 몸통에 값 입력
        y[length - 1] = y[length - 2];
    }

    for (int i = 0; i < ob_count; i++) {
        if (x[0] == obstaclex[i] && y[0] == obstacley[i]) //뱀의 머리와 장해물들이 담겨있는 배열의 좌표를 하나하나 비교
        {                                  //obstacle와 충돌했을 경우 
            score -= 10; //점수 감소 
            obstacle(); //새로운 obstacle 추가 
            length--; //길이감소 
            speed += 5;//속도 감소
            gotoxy(MAP_X + x[length], MAP_Y + y[length], "  "); //꼬리 하나씩 제거
            break;

        }
    }

    if (x[0] == obstacle_x && y[0] == obstacle_y)  //???????????????????????????????
    {                                  //obstacle와 충돌했을 경우 
        score -= 10; //점수 감소 
        obstacle(); //새로운 obstacle 추가 
        length--; //길이감소
        speed += 5;//속도 감소
        gotoxy(MAP_X + x[length], MAP_Y + y[length], "  "); //꼬리 하나씩 제거
    }
    if (x[0] == hurdle_x && y[0] == hurdle_y) { //hurdle과 충돌했을 경우  
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
    { //벽과 충돌했을 경우
        game_over = 1;
        return;
    }

    for (i = 1; i < length; i++)
    { //자기몸과 충돌했는지 검사

        if (x[0] == x[i] && y[0] == y[i])
        {
            game_over = 1;
            return;
        }
    }

    gotoxy(MAP_X + x[length - 1], MAP_Y + y[length - 1], "  "); //몸통 마지막을 지움

    for (i = length - 1; i > 0; i--)
    { //몸통좌표를 한칸씩 옮김
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }

    gotoxy(MAP_X + x[0], MAP_Y + y[0], "○"); //머리가 있던곳을 몸통으로 고침

    if (dir == LEFT)
        --x[0]; //방향에 따라 새로운 머리좌표(x[0],y[0])값을 변경
    else if (dir == RIGHT)
        ++x[0];
    else if (dir == UP)
        --y[0];
    else if (dir == DOWN)
        ++y[0];

    gotoxy(MAP_X + x[i], MAP_Y + y[i], "●"); //새로운 머리좌표값에 머리를 그림
}

void pause(void)
{ // p키를 눌렀을 경우 게임을 일시 정지
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
{ //게임종료 함수
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
        gotoxy(MAP_X + bawall_x, MAP_Y + bawall_y, "□");
        break;
    }
}

void food(void)
{
    int i;
    int food_crush_on = 0; //food가 뱀 몸통좌표에 생길 경우 on
    int r = 0;             //난수 생성에 사용되는 변수

    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, "");
    printf("LIFE: %3d, YOUR SCORE: %3d, LAST SCORE: %3d, BEST SCORE: %3d", life, score, last_score, best_score);

    while (1)
    {
        food_crush_on = 0;

        srand((unsigned)time(NULL) + r); //난수표 생성

        food_x = (rand() % (MAP_WIDTH - 2)) + 1; //난수를 좌표값에 넣음
        food_y = (rand() % (MAP_HEIGHT - 2)) + 1;

        for (i = 0; i < length; i++)
        { //food가 뱀 몸통과 겹치는지 확인

            if (food_x == x[i] && food_y == y[i])
            {
                food_crush_on = 1; //겹치면 food_crush_on 를 on
                r++;
                break;
            }
        }

        // 장애물과 겹치면 다시 시작
        if (food_x == hurdle_x && food_y == hurdle_y) food_crush_on = 1;
        if (food_x == obstacle_x && food_y == obstacle_y) food_crush_on = 1;
        if (food_x == bawall_x && food_y == bawall_y) food_crush_on = 1;

        if (food_crush_on == 1)
            continue; //겹쳤을 경우 while문을 다시 시작

        gotoxy(MAP_X + food_x, MAP_Y + food_y, "★"); //안겹쳤을 경우 좌표값에 food를 찍고
        speed -= 5;//속도 증가
        break;
    }
}

void obstacle(void)
{
    int i;
    int obstacle_crush_on = 0; //obstacle가 뱀 몸통좌표에 생길 경우 on
    int r = 1600;             //난수 생성에 사용되는 변수

    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, "");
    printf("LIFE: %3d, YOUR SCORE: %3d, LAST SCORE: %3d, BEST SCORE: %3d", life, score, last_score, best_score);

    while (1)
    {
        obstacle_crush_on = 0;

        srand((unsigned)time(NULL) / 5 + r); //난수표 생성

        obstacle_x = (rand() % (MAP_WIDTH - 2)) + 1; //난수를 좌표값에 넣음
        obstacle_y = (rand() % (MAP_HEIGHT - 2)) + 1;

        for (i = 0; i < length; i++)
        { //obstacle가 뱀 몸통과 겹치는지 확인

            if (obstacle_x == x[i] && obstacle_y == y[i])
            {
                obstacle_crush_on = 1; //겹치면 obstacle_crush_on 를 on
                r++;
                break;
            }
        }

        // food와 겹치면 다시 시작
        if (obstacle_x == hurdle_x && obstacle_y == hurdle_y) obstacle_crush_on = 1;
        if (obstacle_x == food_x && obstacle_y == food_y) obstacle_crush_on = 1;
        if (obstacle_x == bawall_x && obstacle_y == bawall_y) obstacle_crush_on = 1;


        if (obstacle_crush_on == 1)
            continue; //겹쳤을 경우 while문을 다시 시작

        obstaclex[ob_count] = obstacle_x;
        obstacley[ob_count] = obstacle_y;

        ob_count++; //obstaclex,obstacley 의 끝을 알기위한 int 변수


        gotoxy(MAP_X + obstacle_x, MAP_Y + obstacle_y, "▣"); //안겹쳤을 경우 좌표값에 obstacle를 찍고

        break;
    }
}

void hurdle(void) {

    int i;
    int hurdle_crush_on = 0;              //hurdle이 뱀 몸통좌표에 생길 경우 on 
    int r = 150;                          //food 난수와 안겹치게 r이 100에서 -되는 형식으로 바꿈 

    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, "");
    printf("LIFE: %3d, YOUR SCORE: %3d, LAST SCORE: %3d, BEST SCORE: %3d", life, score, last_score, best_score);

    while (1) {

        hurdle_crush_on = 0;

        srand((unsigned)time(NULL) / 7 + r);            //난수표 생성 

        hurdle_x = (rand() % (MAP_WIDTH - 2)) + 1;    //난수를 좌표값에 넣음 
        hurdle_y = (rand() % (MAP_HEIGHT - 2)) + 1;

        for (i = 0; i < length; i++) {              //hurdle이 뱀 몸통과 겹치는지 확인  

            if (hurdle_x == x[i] && hurdle_y == y[i]) {
                hurdle_crush_on = 1;                  //겹치면 hurdle_crush_on 를 on 
                r++;
                break;
            }

        }

        if (hurdle_x == food_x && hurdle_y == food_y) hurdle_crush_on = 1;
        if (hurdle_x == obstacle_x && hurdle_y == obstacle_y) hurdle_crush_on = 1;
        if (hurdle_x == bawall_x && hurdle_y == bawall_y) hurdle_crush_on = 1;


        if (hurdle_crush_on == 1) continue;                //겹쳤을 경우 while문을 다시 시작 

        gotoxy(MAP_X + hurdle_x, MAP_Y + hurdle_y, "■");     //안겹쳤을 경우 좌표값에 hurdle을 찍고 
        break;

    }

}

void status(void)
{ //각종  status를 볼수 있는 함수
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