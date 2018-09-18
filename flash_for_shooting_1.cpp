#include <windows.h>
#include <time.h>
#include <conio.h>
#include <thread>
#include <bits/stdc++.h>
using namespace std;
//-------------------------------------------------------------------------------------------------------------------------------------------------
const int N = 25;
int maze[N][N], graph[N][N], picture[N][N];
int graph_changed = 0;
struct place
{
    int a, b;
    int x, y;
};
struct node
{
    char to;
    int x, y;
};
place your_place, your_enemy_place[10];
node bullets[100];
int bullets_count = 0, enemy_killed[10], enemy_changed[10];
int enemy_value, score, checkpoint, hardrank_judge, hp, ar;
bool if_lose = false, if_endd = false;
int towards[4][2] = {0,-1,-1,0,0,1,1,0};
//------------------------------------------------------------------------------------------------------------------------------------------------
void goto_xy(int x, int y)//------------------------------------------------------位置函数
{
    COORD pos;//------------------------------------------------------------------控制台坐标封装类型 x+y
    pos.X = 2*x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);//-------------API
}
void pick_color(int a)//----------------------------------------------------------颜色函数
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);//----------------API
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void to_a_start()
{
    goto_xy(0, 0);  pick_color(15); cout << "NNNNNNNNNow youuu ccaaaaan choooooooossssse toooo ppppreeesss keeeybboarddd" << endl;
                    pick_color(15); cout << "TTTOoOo SSsSStaaartt ttthhiss vvvvvvvvvvvvvvvveeeeeeeerrrryy #lowwwww game#" << endl;
                    pick_color(8);  cout << "Explain text/ for a gameeee://////////important!! copywrong:: inceptions.me" << endl;
                                    cout << endl;
                                    cout << "xuezwohuiquzixikaolvlliangt yehaishijuedingjujuenidezhuiq feichangganxienid" << endl;
                                    cout << "uiyigxinshengdebang!zhu haimeiruxuejiujialwodeqq yizhigeiwoshuozenmeyangzen" << endl;
                                    cout << "meyang suiranbuzhidaowozhangshenmyangzidanhaishipeiwoliaodaohenwan xiexieni" << endl;
                                    cout << "qianjitianjiewohaiqingwochifan lingchensandianyehuidadianhuagwoshuonihenjim" << endl;
                                    cout << "danwobijingcaidayigangjinxuexiao nisongwodedongxiwohuihuangeinide yexiwangn" << endl;
                                    cout << "haohaoxuexi buyaozaigeiwofaduanxinqqdianhuaweixinl xiexiexuezhangnishighaor" << endl;
    goto_xy(0, N-10);pick_color(15);cout << "START GAME BY PRESSING A" << endl;
    goto_xy(0, N-5); pick_color(15);cout << "EASY: e/E                  MID:m/M                    HARD:h/H"       << endl;
                                    cout << endl;
    return ;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void init_maze()//----------------------------------------------------------------建图
{
    int maze[N+2][N+2] = {{0}};
    for(int i = 1; i <= N; i++)
        for(int j = 1; j <= N; j++)
            maze[i][j] = 1;
    pick_color(15);
    for(int i = 0; i < N+2; i++)
    {
        for(int j = 0; j < N+2; j++)
        {
            if(maze[i][j]) cout << "■";
            else           cout << "□";
        }
        cout << endl;
    }
    pick_color(15);
    goto_xy(N+3, 1);  cout << "           MOVE          " << endl;
    goto_xy(N+3, 2);  cout << "                         " << endl;
    goto_xy(N+3, 3);  cout << "            ↑↑           " << endl;
    goto_xy(N+3, 4);  cout << "         <-← →->         " << endl;
    goto_xy(N+3, 5);  cout << "            ↓↓           " << endl;
    goto_xy(N+3, 6);  cout << "                         " << endl;
    goto_xy(N+3, 7);  cout << "           SHOOT         " << endl;
    goto_xy(N+3, 8);  cout << "                         " << endl;
    goto_xy(N+3, 9);  cout << "            I↑           " << endl;
    goto_xy(N+3, 10); cout << "         <-J L->         " << endl;
    goto_xy(N+3, 11); cout << "            K↓           " << endl;
    goto_xy(N+3, 12); cout << "                         " << endl;
    goto_xy(N+3, 0);
    cout << "Control：" << endl;
    goto_xy(1, N+3);  cout << "HP: " << endl;
    goto_xy(10, N+3); cout << "AR: " << endl;
    goto_xy(19, N+3); cout << "CO: " << endl;

    your_enemy_place[0].x = rand() % N + 1;
    your_enemy_place[0].y = rand() % N + 1;
    goto_xy(your_enemy_place[0].x, your_enemy_place[0].y);
    pick_color(12);
    cout << "●" << endl;

    your_place.x = rand() % N + 1;
    your_place.y = rand() % N + 1;
    goto_xy(your_place.x, your_place.y);
    pick_color(9);
    cout << "●" << endl;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
int vis[N][N];
void ai_for_enemy_bfs(int x, int y)//---------------------------------------------从玩家开始搜敌人
{
    memset(vis, 0, sizeof vis);
    memset(enemy_changed, 0, sizeof enemy_changed);
    int enemy_value2 = enemy_value;
    queue<place> Q;
    place nowing;
    nowing.a = 0;
    nowing.b = 0;
    nowing.x = x;
    nowing.y = y;
    vis[x][y] = 1;
    Q.push(nowing);
    while(!Q.empty())
    {
        nowing = Q.front();
        Q.pop();
        int k = picture[nowing.x][nowing.y] - 1;
        if(k >= 0 && enemy_changed[k] == 0 && enemy_killed[k] == 0)
        {
            your_enemy_place[k].a = your_enemy_place[k].x;
            your_enemy_place[k].b = your_enemy_place[k].y;
            your_enemy_place[k].x = nowing.a;
            your_enemy_place[k].y = nowing.b;
            enemy_value2 -- ;
            enemy_changed[k] = 1;
            if(enemy_value2 == 0)
                return ;
        }

        place nex;
        for(int i = 0; i < 4; i++)
        {
            nex.x = nowing.x + towards[i][0];
            nex.y = nowing.y + towards[i][1];

            if(picture[nex.x][nex.y] >= 0   &&
               vis[nex.x][nex.y] == 0       &&
               nex.x >= 1                   &&
               nex.x <= N                   &&
               nex.y >= 1                   &&
               nex.y <= N                   )
            {
                nex.a = nowing.x;
                nex.b = nowing.y;
                Q.push(nex);
                vis[nex.x][nex.y] = 1;
            }
        }
    }

}
//-------------------------------------------------------------------------------------------------------------------------------------------------
void check_maze()//---------------------------------------------------------------输出集中在这个函数中，并为这个函数单独创建线程
{
    int lowbeecounts = 0, pain_time = 10;
loop:;
    for(int i = 0; i < bullets_count; i++)
    {
        if(your_place.x >= 1 &&
           your_place.x <= N &&
           your_place.y >= 1 &&
           your_place.y <= N )
        {
            goto_xy(your_place.a, your_place.b);
            pick_color(15);
            cout << "■" << endl;
            goto_xy(your_place.x, your_place.y);//--------------------------------输出玩家位置
            pick_color(9);
            cout << "●" << endl;
        }

        if(graph_changed == 1)//--------------------------------------------------输出下一关敌人位置
        {
            score *= 2;
            goto_xy(21, N+3);
            cout << score << endl;
            for(int i = 0; i < checkpoint; i++)
            {
                pick_color(12);
                goto_xy(your_enemy_place[i].x, your_enemy_place[i].y);
                cout << "●" << endl;
            }
            graph_changed = 0;
        }

        if(bullets[i].x + towards[bullets[i].to - 'i'][0] >= 1 &&//---------------输出子弹位置
           bullets[i].x + towards[bullets[i].to - 'i'][0] <= N &&
           bullets[i].y + towards[bullets[i].to - 'i'][1] >= 1 &&
           bullets[i].y + towards[bullets[i].to - 'i'][1] <= N )
        {
            goto_xy(bullets[i].x, bullets[i].y);
            pick_color(15);
            cout << "■" << endl;

            for(int j = 0; j < checkpoint; j++)//---------------------------------判断是否射中敌人
                if(bullets[i].x == your_enemy_place[j].x &&
                   bullets[i].y == your_enemy_place[j].y &&
                   enemy_killed[j] == 0)
                {
                    score += 1;
                    enemy_value -= 1;
                    goto_xy(21, N+3);  cout << score << endl;
                    enemy_killed[j] = 3;
                    break;
                }

            bullets[i].x += towards[bullets[i].to - 'i'][0];
            bullets[i].y += towards[bullets[i].to - 'i'][1];
            goto_xy(bullets[i].x, bullets[i].y);
            pick_color(14);
            cout << "★" << endl;
        }
        else//--------------------------------------------------------------------子弹触碰边界消失
        {
            goto_xy(bullets[i].x, bullets[i].y);
            pick_color(15);
            cout << "■" << endl;
        }
    }

    memset(picture, 0, sizeof picture);//-----------------------------------------空位置用0
    for(int i = 0; i < bullets_count; i++)//--------------------------------------传入BFS函数需要的的图形
        picture[bullets[i].x][bullets[i].y] = -1;//-------------------------------子弹用-1
    for(int i = 0; i < checkpoint; i++)//-----------------------------------------敌人用k+
        picture[your_enemy_place[i].x][your_enemy_place[i].y] = i+1;
    picture[your_place.x][your_place.y] = -2;//-----------------------------------玩家用-2

    Sleep(100);
    lowbeecounts++;
    if(lowbeecounts == hardrank_judge)
    {
        ai_for_enemy_bfs(your_place.x, your_place.y);
        for(int i = 0; i < checkpoint; i++)//-------------------------------------敌人选择移动方向
        {
            if(enemy_changed[i] == 1)
            {
                goto_xy(your_enemy_place[i].a, your_enemy_place[i].b);
                pick_color(15);
                cout << "■" << endl;
                goto_xy(your_enemy_place[i].x, your_enemy_place[i].y);
                pick_color(12);
                cout << "●" << endl;
            }
        }
        lowbeecounts = 0;
    }

    if(pain_time >= 100)
    {
        ar = 1;
        pain_time = 0;
    }
    for(int i = 0; i < checkpoint; i++)
    {
        if(your_enemy_place[i].x == your_place.x &&//-----------------------------游戏gaisu!
           your_enemy_place[i].y == your_place.y &&
           enemy_killed[i] == 0)
        {
            if(ar == 1)
            {
                ar = 0;
                pain_time = 0;
                hp = 1;
            }
            else if(ar == 0 && pain_time >= 50 && pain_time < 100)
            {
                hp = 0;
            }

            if(hp == 0)//---------------------------------------------------------死亡判定
            {
                system("cls");
                pick_color(15);
                goto_xy(N/2 + 1, N/2);
                cout << "   IT'S OVER!5 seconds to choose..." << endl;
                goto_xy(N/2 + 1, N/2 + 1);
                cout << "GET BACK AGAIN? if so please wait 5 sec to build the game" << endl;
                if_endd = true;
                return ;
            }
        }
    }
    pain_time++;
    goto_xy(N+1,N+1);
    cout << "----" << pain_time << endl;
    pick_color(15);
    goto_xy(3, N+3);  cout << hp << endl;
    goto_xy(12, N+3); cout << ar << endl;
goto loop;//----------------------------------------------------------------------作为独立线程不断循环
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
    int  if_cross = 1;
    score = 0, checkpoint = 1;
    int ch;
loop1:;
    to_a_start();

    switch(ch = getch())
    {
    case 69:
    case 101:
        hardrank_judge = 16;
        break;
    case 77:
    case 109:
        hardrank_judge = 8;
        break;
    case 72:
    case 104:
        hardrank_judge = 4;
        break;
    default:
        break;
    }
    system("cls");

    srand((unsigned)time(NULL));//------------------------------------------------随机化种子
    init_maze();
    bullets_count = 0,enemy_value = 1, hp = 1, ar = 1;

    thread task(check_maze);//----------------------------------------------------为图形输出函数建立线程并分离
    task.detach();

    while(1)
    {
        node nex;
        goto_xy(0, 0);
        ch = getch();
        if(isascii(ch))//---------------------------------------------------------读取上下左右和IJKL
        {
            switch(ch)
            {
            case 105:
                bullets[bullets_count].to = 'i';
                bullets[bullets_count].x = your_place.x;
                bullets[bullets_count].y = your_place.y - 1;
                goto_xy(bullets[bullets_count].x, bullets[bullets_count].y);
                bullets_count = (bullets_count+1)%100;
                break;
            case 107:
                bullets[bullets_count].to = 'k';
                bullets[bullets_count].x = your_place.x;
                bullets[bullets_count].y = your_place.y + 1;
                goto_xy(bullets[bullets_count].x, bullets[bullets_count].y);
                bullets_count = (bullets_count+1)%100;
                break;
            case 106:
                bullets[bullets_count].to = 'j';
                bullets[bullets_count].x = your_place.x - 1;
                bullets[bullets_count].y = your_place.y;
                goto_xy(bullets[bullets_count].x, bullets[bullets_count].y);
                bullets_count = (bullets_count+1)%100;
                break;
            case 108:
                bullets[bullets_count].to = 'l';
                bullets[bullets_count].x = your_place.x + 1;
                bullets[bullets_count].y = your_place.y;
                goto_xy(bullets[bullets_count].x, bullets[bullets_count].y);
                bullets_count = (bullets_count+1)%100;
                break;
            default:
                break;
            }
        }
        else
        {
            switch(ch = getch())
            {
            case 72:
                if_cross = 1;
                for(int i = 0; i < enemy_value; i++)
                    if(your_place.x == your_enemy_place[i].x &&
                       your_place.y - 1 == your_enemy_place[i].y)
                    {
                        if_cross = 0;
                        break;
                    }
                if(your_place.y - 1 >= 1 && if_cross)
                {
                    your_place.a = your_place.x;
                    your_place.b = your_place.y;
                    your_place.y -- ;
                }
                break;
            case 80:
                if_cross = 1;
                for(int i = 0; i < enemy_value; i++)
                    if(your_place.x == your_enemy_place[i].x &&
                       your_place.y + 1 == your_enemy_place[i].y)
                    {
                        if_cross = 0;
                        break;
                    }
                if(your_place.y + 1 <= N && if_cross)
                {
                    your_place.a = your_place.x;
                    your_place.b = your_place.y;
                    your_place.y ++ ;
                }
                break;
            case 75:
                if_cross = 1;
                for(int i = 0; i < enemy_value; i++)
                    if(your_place.x - 1 == your_enemy_place[i].x &&
                       your_place.y == your_enemy_place[i].y)
                    {
                        if_cross = 0;
                        break;
                    }
                if(your_place.x - 1 >= 1 && if_cross)
                {
                    your_place.a = your_place.x;
                    your_place.b = your_place.y;
                    your_place.x -- ;
                }
                break;
            case 77:
                if_cross = 1;
                for(int i = 0; i < enemy_value; i++)
                    if(your_place.x + 1 == your_enemy_place[i].x &&
                       your_place.y == your_enemy_place[i].y)
                    {
                        if_cross = 0;
                        break;
                    }
                if(your_place.x + 1 <= N && if_cross)
                {
                    your_place.a = your_place.x;
                    your_place.b = your_place.y;
                    your_place.x ++ ;
                }
                break;
            default:
                break;
            }
        }

        if( enemy_value == 0 )//--------------------------------------------------消灭所有敌人
        {
            graph_changed = 1;
            checkpoint ++;
            memset(graph, 0, sizeof graph);
            memset(your_enemy_place, 0, sizeof your_enemy_place);
            memset(enemy_killed, 0, sizeof enemy_killed);
            for(int i = 0; i < checkpoint; i++)//---------------------------------重新刷新下一关敌人位置
            {
                your_enemy_place[i].x = rand()%N + 1;
                your_enemy_place[i].y = rand()%N + 1;
                while(graph[your_enemy_place[i].x][your_enemy_place[i].y] == 1)
                {
                    your_enemy_place[i].x = rand()%N + 1;
                    your_enemy_place[i].y = rand()%N + 1;
                }
                graph[your_enemy_place[i].x][your_enemy_place[i].y] = 1;
            }
            enemy_value = checkpoint;
        }

        if(if_endd == true)
        {
            if_endd = false;
            break;
        }
    }
    switch(ch = getch())
    {
    case 89:
    case 121:
        system("cls");
        goto_xy(0, N+2);
        cout << "Y" << endl;
        goto loop1;
    case 78:
    case 110:
        system("cls");
        goto_xy(0, N+2);
        cout << "N" << endl;
        goto_xy(0, 0);
        cout << "OVER" << endl;
        break;
    default:
        break;
    }
    return 0;
}
