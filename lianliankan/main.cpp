#include <conio.h>
#include <graphics.h>
#include <time.h>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<malloc.h>
#pragma comment(lib,"Winmm.lib")

COORD currentPos = { 0,0 };
IMAGE img[51];
IMAGE background[3];
int menu_choose = 0; //0：开始游戏，1：游戏设置，2：查看排行，3：退出游戏
int max_row = 10, max_col = 14;
int box_width = 50;
int blank_width = 5;
int start_x = 25;
int start_y = 25;
int previous_x = -1;
int previous_y = -1;
int* MAP = (int*)malloc(max_row * max_col * sizeof(int));
int* clear_list = (int*)malloc(max_row * max_col * sizeof(int));
int num_reminder = 2;
int score = 0;
int shuffle = 1;
clock_t t1, t2;
clock_t last_t, now_t;
int is_over = 0;
int num_hero = 13;
int is_recharge = 0;

void dataInit();
void Menu_plot();
void Menu();
void GameInit();
void ImportImage();
void GamePlay();
int is_connect(int x1, int y1, int x2, int y2, int is_draw); //判断两点是否可通
void state_upd();
int is_in_col(int x1, int y1, int x2, int y2); //两点在同一列上是否可通
int is_in_row(int x1, int y1, int x2, int y2);//两点在同一行上是否可通
int is_connect_one(int x1, int y1, int x2, int y2); //两点一次是否能连接
int is_connect_two(int x1, int y1, int x2, int y2); //两点两次是否能连接
int is_connect_three(int x1, int y1, int x2, int y2, int is_draw); //两点三次是否能连接
void clear(); //清除clear_list
int is_satisfy_10(); //判断是否满足大于10个方块相邻
void RankInit();
int is_win();
void mode_choose();
void setting();
void setting_plot();
void rank_plot();
int is_connect_solvable();
void auto_remove();

int main() {
	initgraph(900, 580);
	//mciSendString(L"open res/主题曲.mp3 alias bk", NULL, 0, NULL);
	mciSendString(_T("open music.mp3 alias mymusic"), NULL, 0, NULL);
	mciSendString(_T("play mymusic"), NULL, 0, NULL);
begin:
	dataInit();
	Menu();

	if (menu_choose == 0) {
		GameInit();
		GamePlay();
		goto begin;
	}
	else if (menu_choose == 1) {
		setting();
		goto begin;
	}
	else if (menu_choose == 2) {
		RankInit();
		goto begin;
	}
	else if (menu_choose == 3) {

	}
	//_getch();				// Press any key to continue
	//mciSendString(L"puase bk", NULL, 0, NULL);
	mciSendString(L"stop bk", NULL, 0, NULL);
	mciSendString(L"close bk", NULL, 0, NULL);
	closegraph();			// 关闭图形界面
	return 0;
}

void dataInit() {
	currentPos = { 0,0 };
	menu_choose = 0; //0：开始游戏，1：游戏设置，2：查看排行，3：退出游戏
	max_row = 10, max_col = 14;
	box_width = 50;
	blank_width = 5;
	start_x = 25;
	start_y = 25;
	previous_x = -1;
	previous_y = -1;
	for (int i = 0; i < max_col; i++)
		for (int j = 0; j < max_row; j++)
			clear_list[i + j * max_col] = 0;
	num_reminder = 2;
	score = 0;
	shuffle = 1;
	is_over = 0;
	loadimage(&background[0], L"./fig/background.jpg", 200, 150);
	//loadimage(&background[1], L"./fig/background1.jpg", 200, 400);
	//loadimage(&background[2], L"./fig/background2.jpg", 200, 400);
}

void Menu_plot()
{
	BeginBatchDraw();
	setbkcolor(WHITE); //设置窗口背景颜色
	settextcolor(BLACK);
	setbkmode(1); //设置文字背景透明
	cleardevice();
	settextstyle(15, 0, _T("Gungsuh"));
	TCHAR s[30] = { 0 };
	_stprintf_s(s, _T("SOFTWARE BY LeTian Tao"));
	settextstyle(20, 0, _T("宋体"));
	outtextxy(0, 0, s);

	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 75;                   // 设置字体高度为 100
	wcscpy_s(f.lfFaceName, _T("华文行楷"));
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);                     // 设置字体样式
	outtextxy(460, 50 + 15, _T("连连看"));

	settextstyle(40, 0, _T("宋体"));
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);

	int X1 = 340, X2 = 560, Y1 = 200, Y2 = Y1 + 60;
	rectangle(X1, Y1, X2, Y2);
	if (currentPos.X > X1 && currentPos.X < X2 && currentPos.Y>Y1 && currentPos.Y < Y2)
	{
		settextcolor(BLUE);
		line(X1 + 20, Y2 - 10, X2 - 20, Y2 - 10);
	}
	else
		settextcolor(BLACK);
	outtextxy(X1 + 30, Y1 + 10, L"开始游戏");

	Y1 += 100;
	Y2 += 100;
	rectangle(X1, Y1, X2, Y2);
	if (currentPos.X > X1 && currentPos.X < X2 && currentPos.Y>Y1 && currentPos.Y < Y2)
	{
		settextcolor(BLUE);
		line(X1 + 20, Y2 - 10, X2 - 20, Y2 - 10);
	}
	else
		settextcolor(BLACK);
	outtextxy(X1 + 30, Y1 + 10, L"游戏设置");

	Y1 += 100;
	Y2 += 100;
	rectangle(X1, Y1, X2, Y2);
	if (currentPos.X > X1 && currentPos.X < X2 && currentPos.Y>Y1 && currentPos.Y < Y2)
	{
		settextcolor(BLUE);
		line(X1 + 20, Y2 - 10, X2 - 20, Y2 - 10);
	}
	else
		settextcolor(BLACK);
	outtextxy(X1 + 10, Y1 + 10, L"查看排行榜");

	Y1 += 100;
	Y2 += 100;
	rectangle(X1, Y1, X2, Y2);
	if (currentPos.X > X1 && currentPos.X < X2 && currentPos.Y>Y1 && currentPos.Y < Y2)
	{
		settextcolor(BLUE);
		line(X1 + 20, Y2 - 10, X2 - 20, Y2 - 10);
	}
	else
		settextcolor(BLACK);
	outtextxy(X1 + 30, Y1 + 10, L"退出游戏");

	putimage(260, 30, &background[0]);
	//putimage(50, 150, &background[1]);
	//putimage(600, 400, &background[2]);

	if (currentPos.X > 780 && currentPos.X < 880 && currentPos.Y >520 && currentPos.Y < 560)
		setcolor(0x0000AA);
	else
		setcolor(0xAB5A00);
	settextstyle(50, 0, _T("华文行楷"));
	if (is_recharge == 0)
		outtextxy(780, 520, L"充值");//区间(x1,y1,x2,y2)=(750,395,850,455)
	else
		outtextxy(780, 520, L"退款");

	EndBatchDraw();
}

void Menu()
{
	int post = 1;
	while (post)
	{
		Menu_plot();
		MOUSEMSG msg = GetMouseMsg(); // 获取鼠标信息
		int x = msg.x;
		int y = msg.y;
		switch (msg.uMsg)
		{
		case WM_MOUSEMOVE:
			currentPos.X = x;
			currentPos.Y = y;
			break;
		case WM_LBUTTONUP:
			if (currentPos.X > 340 && currentPos.X < 560 && currentPos.Y>200 && currentPos.Y < 260)
			{
				menu_choose = 0;
				post = 0;
			}
			if (currentPos.X > 340 && currentPos.X < 560 && currentPos.Y>300 && currentPos.Y < 360)
			{
				menu_choose = 1;
				post = 0;
			}
			if (currentPos.X > 340 && currentPos.X < 560 && currentPos.Y>400 && currentPos.Y < 460)
			{
				menu_choose = 2;
				post = 0;
			}
			if (currentPos.X > 340 && currentPos.X < 560 && currentPos.Y>500 && currentPos.Y < 560)
			{
				menu_choose = 3;
				post = 0;
			}
			if (currentPos.X > 780 && currentPos.X < 880 && currentPos.Y >520 && currentPos.Y < 560) {
				if (is_recharge == 0) {
					MessageBox(GetHWnd(), L"充值成功！", L"提示", 0);
					is_recharge = 1;
				}
				else {
					MessageBox(GetHWnd(), L"退款成功！", L"提示", 0);
					is_recharge = 0;
				}
			}
			break;
		default:
			break;
		}
	}
}

void GameInit() {
	clearrectangle(0, 0, 900, 580);
	ImportImage();
	srand((unsigned)time(NULL));
	//随机生成
	for (int i = 0; i < max_col * max_row / 2; i++)
		MAP[i] = rand() % (num_hero)+1;
	for (int i = max_col * max_row / 2; i < max_col * max_row; i++)
		MAP[i] = MAP[i - max_col * max_row / 2];
	//随机打乱
	int i, j, T = 100, tmp;
	while (T--)
	{
		i = rand() % (max_col * max_row);
		j = rand() % (max_col * max_row);
		tmp = MAP[i];
		MAP[i] = MAP[j];
		MAP[j] = tmp;
	}
	//要求10组以上相邻
	while (is_satisfy_10() == 0) {
		//随机生成
		for (int i = 0; i < max_col * max_row / 2; i++)
			MAP[i] = rand() % 13 + 1;
		for (int i = max_col * max_row / 2; i < max_col * max_row; i++)
			MAP[i] = MAP[i - max_col * max_row / 2];
		//随机打乱
		int i, j, T = 100, tmp;
		while (T--)
		{
			i = rand() % max_col * max_row;
			j = rand() % max_col * max_row;
			tmp = MAP[i];
			MAP[i] = MAP[j];
			MAP[j] = tmp;
		}
	}
	//显示图像
	for (int i = 0; i < max_col; i++)
		for (int j = 0; j < max_row; j++) {
			putimage(start_x + i * box_width, start_y + j * box_width, &img[MAP[i + j * max_col]]);
		}
}

void ImportImage()
{
	if (is_recharge == 0) {
		loadimage(&img[0], L"./fig/0.jpg", box_width, box_width);
		loadimage(&img[1], L"./fig/1.jpg", box_width, box_width);
		loadimage(&img[2], L"./fig/2.jpg", box_width, box_width);
		loadimage(&img[3], L"./fig/3.jpg", box_width, box_width);
		loadimage(&img[4], L"./fig/4.jpg", box_width, box_width);
		loadimage(&img[5], L"./fig/5.jpg", box_width, box_width);
		loadimage(&img[6], L"./fig/6.jpg", box_width, box_width);
		loadimage(&img[7], L"./fig/7.jpg", box_width, box_width);
		loadimage(&img[8], L"./fig/8.jpg", box_width, box_width);
		loadimage(&img[9], L"./fig/9.jpg", box_width, box_width);
		loadimage(&img[10], L"./fig/10.jpg", box_width, box_width);
		loadimage(&img[11], L"./fig/11.jpg", box_width, box_width);
		loadimage(&img[12], L"./fig/12.jpg", box_width, box_width);
		loadimage(&img[13], L"./fig/13.jpg", box_width, box_width);
		loadimage(&img[14], L"./fig/14.jpg", box_width, box_width);
		loadimage(&img[15], L"./fig/15.jpg", box_width, box_width);
		loadimage(&img[16], L"./fig/16.jpg", box_width, box_width);
		loadimage(&img[17], L"./fig/17.jpg", box_width, box_width);
		loadimage(&img[18], L"./fig/18.jpg", box_width, box_width);
		loadimage(&img[19], L"./fig/19.jpg", box_width, box_width);
		loadimage(&img[20], L"./fig/20.jpg", box_width, box_width);
		loadimage(&img[21], L"./fig/21.jpg", box_width, box_width);
		loadimage(&img[22], L"./fig/22.jpg", box_width, box_width);
		loadimage(&img[23], L"./fig/23.jpg", box_width, box_width);
		loadimage(&img[24], L"./fig/24.jpg", box_width, box_width);
		loadimage(&img[25], L"./fig/25.jpg", box_width, box_width);
		loadimage(&img[26], L"./fig/26.jpg", box_width, box_width);
		loadimage(&img[27], L"./fig/27.jpg", box_width, box_width);
		loadimage(&img[28], L"./fig/28.jpg", box_width, box_width);
		loadimage(&img[29], L"./fig/29.jpg", box_width, box_width);
		loadimage(&img[30], L"./fig/30.jpg", box_width, box_width);
		loadimage(&img[31], L"./fig/31.jpg", box_width, box_width);
		loadimage(&img[32], L"./fig/32.jpg", box_width, box_width);
		loadimage(&img[33], L"./fig/33.jpg", box_width, box_width);
		loadimage(&img[34], L"./fig/34.jpg", box_width, box_width);
		loadimage(&img[35], L"./fig/35.jpg", box_width, box_width);
		loadimage(&img[36], L"./fig/36.jpg", box_width, box_width);
		loadimage(&img[37], L"./fig/37.jpg", box_width, box_width);
		loadimage(&img[38], L"./fig/38.jpg", box_width, box_width);
		loadimage(&img[39], L"./fig/39.jpg", box_width, box_width);
		loadimage(&img[40], L"./fig/40.jpg", box_width, box_width);
		loadimage(&img[41], L"./fig/41.jpg", box_width, box_width);
		loadimage(&img[42], L"./fig/42.jpg", box_width, box_width);
		loadimage(&img[43], L"./fig/43.jpg", box_width, box_width);
		loadimage(&img[44], L"./fig/44.jpg", box_width, box_width);
		loadimage(&img[45], L"./fig/45.jpg", box_width, box_width);
		loadimage(&img[46], L"./fig/46.jpg", box_width, box_width);
		loadimage(&img[47], L"./fig/47.jpg", box_width, box_width);
		loadimage(&img[48], L"./fig/48.jpg", box_width, box_width);
		loadimage(&img[49], L"./fig/49.jpg", box_width, box_width);
		loadimage(&img[50], L"./fig/50.jpg", box_width, box_width);
	}
	else {
		loadimage(&img[0], L"./fig2/0.jpg", box_width, box_width);
		loadimage(&img[1], L"./fig2/1.jpg", box_width, box_width);
		loadimage(&img[2], L"./fig2/2.jpg", box_width, box_width);
		loadimage(&img[3], L"./fig2/3.jpg", box_width, box_width);
		loadimage(&img[4], L"./fig2/4.jpg", box_width, box_width);
		loadimage(&img[5], L"./fig2/5.jpg", box_width, box_width);
		loadimage(&img[6], L"./fig2/6.jpg", box_width, box_width);
		loadimage(&img[7], L"./fig2/7.jpg", box_width, box_width);
		loadimage(&img[8], L"./fig2/8.jpg", box_width, box_width);
		loadimage(&img[9], L"./fig2/9.jpg", box_width, box_width);
		loadimage(&img[10], L"./fig2/10.jpg", box_width, box_width);
		loadimage(&img[11], L"./fig2/11.jpg", box_width, box_width);
		loadimage(&img[12], L"./fig2/12.jpg", box_width, box_width);
		loadimage(&img[13], L"./fig2/13.jpg", box_width, box_width);
		loadimage(&img[14], L"./fig2/14.jpg", box_width, box_width);
		loadimage(&img[15], L"./fig2/15.jpg", box_width, box_width);
		loadimage(&img[16], L"./fig2/16.jpg", box_width, box_width);
		loadimage(&img[17], L"./fig2/17.jpg", box_width, box_width);
		loadimage(&img[18], L"./fig2/18.jpg", box_width, box_width);
		loadimage(&img[19], L"./fig2/19.jpg", box_width, box_width);
		loadimage(&img[20], L"./fig2/20.jpg", box_width, box_width);
		loadimage(&img[21], L"./fig2/21.jpg", box_width, box_width);
		loadimage(&img[22], L"./fig2/22.jpg", box_width, box_width);
		loadimage(&img[23], L"./fig2/23.jpg", box_width, box_width);
		loadimage(&img[24], L"./fig2/24.jpg", box_width, box_width);
		loadimage(&img[25], L"./fig2/25.jpg", box_width, box_width);
		loadimage(&img[26], L"./fig2/26.jpg", box_width, box_width);
		loadimage(&img[27], L"./fig2/27.jpg", box_width, box_width);
		loadimage(&img[28], L"./fig2/28.jpg", box_width, box_width);
		loadimage(&img[29], L"./fig2/29.jpg", box_width, box_width);
		loadimage(&img[30], L"./fig2/30.jpg", box_width, box_width);
		loadimage(&img[31], L"./fig2/31.jpg", box_width, box_width);
		loadimage(&img[32], L"./fig2/32.jpg", box_width, box_width);
		loadimage(&img[33], L"./fig2/33.jpg", box_width, box_width);
		loadimage(&img[34], L"./fig2/34.jpg", box_width, box_width);
		loadimage(&img[35], L"./fig2/35.jpg", box_width, box_width);
		loadimage(&img[36], L"./fig2/36.jpg", box_width, box_width);
		loadimage(&img[37], L"./fig2/37.jpg", box_width, box_width);
		loadimage(&img[38], L"./fig2/38.jpg", box_width, box_width);
		loadimage(&img[39], L"./fig2/39.jpg", box_width, box_width);
		loadimage(&img[40], L"./fig2/40.jpg", box_width, box_width);
		loadimage(&img[41], L"./fig2/41.jpg", box_width, box_width);
		loadimage(&img[42], L"./fig2/42.jpg", box_width, box_width);
		loadimage(&img[43], L"./fig2/43.jpg", box_width, box_width);
		loadimage(&img[44], L"./fig2/44.jpg", box_width, box_width);
		loadimage(&img[45], L"./fig2/45.jpg", box_width, box_width);
		loadimage(&img[46], L"./fig2/46.jpg", box_width, box_width);
		loadimage(&img[47], L"./fig2/47.jpg", box_width, box_width);
		loadimage(&img[48], L"./fig2/48.jpg", box_width, box_width);
		loadimage(&img[49], L"./fig2/49.jpg", box_width, box_width);
		loadimage(&img[50], L"./fig2/50.jpg", box_width, box_width);
		score += 100;
	}


	int i, j, T = 1000;
	IMAGE tmp;
	while (T--)
	{
		i = rand() % 50 + 1;
		j = rand() % 50 + 1;

		tmp = img[i];
		img[i] = img[j];
		img[j] = tmp;
	}
}

void GamePlay() {
	t1 = clock();
	last_t = clock();
	//now_t = clock();
	while (1) {
		mode_choose();
		MOUSEMSG msg = GetMouseMsg(); // 获取鼠标信息
		int x = msg.x;
		int y = msg.y;
		switch (msg.uMsg)
		{
		case WM_MOUSEMOVE:
			currentPos.X = x;
			currentPos.Y = y;
			break;
		case WM_LBUTTONUP:
			if (x > start_x && x < start_x + max_col * box_width && y>start_y && y < start_y + max_row * box_width) {
				int num_x = (x - start_x) / box_width;
				int num_y = (y - start_y) / box_width;
				//之前没有格子被选中
				if (previous_x == -1 && previous_y == -1) {
					if (MAP[num_x + max_col * num_y] == 0) {
						continue;
					}
					else {
						previous_x = num_x;
						previous_y = num_y;
						//画红框
						setlinecolor(RED);
						setlinestyle(PS_SOLID, 2);
						rectangle(start_x + num_x * box_width, start_y + num_y * box_width, start_x + (num_x + 1) * box_width, start_y + (num_y + 1) * box_width);
					}
				}
				//这已经是第二个格子了
				else {
					//如果选的点为空
					if (MAP[num_x + max_col * num_y] == 0) {
						continue;
					}
					//如果两点是同一点
					else if (num_x == previous_x && num_y == previous_y) {
						//消去红框
						clearrectangle(start_x + previous_x * box_width - 1, start_y + previous_y * box_width - 1, start_x + (previous_x + 1) * box_width + 1, start_y + (previous_y + 1) * box_width + 1);
						//TODO再次显示，pre置-1
						putimage(start_x + previous_x * box_width, start_y + previous_y * box_width, &img[MAP[previous_x + max_col * previous_y]]);
						previous_x = -1;
						previous_y = -1;
					}
					//同一张图且如果两点连通，注意判断顺序不能改变
					else if (MAP[num_x + num_y * max_col] == MAP[previous_x + previous_y * max_col] && is_connect(num_x, num_y, previous_x, previous_y, 1) == 1) {
						now_t = clock();
						//得分加加
						float time = (double)(now_t - last_t) / CLOCKS_PER_SEC;
						if (time < 2.0) {
							//得分
							settextstyle(30, 0, _T("华文行楷"));
							setcolor(0x0A5FFF);
							clearrectangle(750, 75, 900, 135);
							outtextxy(760, 81, L"DOUBLE!");
							//13 2
							//15 3
							//17 4
							score += (num_hero - 13) / 2 + 2;
						}
						score += 2;
						Sleep(200);
						last_t = clock();
						clear();
						//消去红框
						clearrectangle(start_x + previous_x * box_width - 1, start_y + previous_y * box_width - 1, start_x + (previous_x + 1) * box_width + 1, start_y + (previous_y + 1) * box_width + 1);
						//动画上消去两个方块
						putimage(start_x + num_x * box_width, start_y + num_y * box_width, &img[0]);
						putimage(start_x + previous_x * box_width, start_y + previous_y * box_width, &img[0]);
						//数据上消去两个方块
						MAP[num_x + num_y * max_col] = 0;
						MAP[previous_x + previous_y * max_col] = 0;
						//前一个点置空
						previous_x = -1;
						previous_y = -1;
						//判断是否获得胜利获得胜利
						if (is_win() == 1) {
							is_over = 1;
							FILE* stream;
							errno_t err;
							err = fopen_s(&stream, "score.txt", "a+");
							fprintf(stream, "%d\n", score);
							_fcloseall();

							settextstyle(35, 0, _T("华文行楷"));
							setcolor(0x0A5FFF);
							TCHAR str1[20] = { 0 };
							_stprintf_s(str1, _T("%03d"), score);
							clearrectangle(750, 75, 900, 135);
							outtextxy(780, 81, str1);
							if (num_hero < 17) {
								num_hero += 2;
								MessageBox(GetHWnd(), L"恭喜你巅峰召唤师，你获得了游戏的胜利,难度加强！", L"恭喜", 0);
							}
							else
								MessageBox(GetHWnd(), L"恭喜你巅峰召唤师，你获得了最难关卡的游戏的胜利！", L"恭喜", 0);
							
						}
						//判断是否有可交换点
						int flag = 0;
						while (is_connect_solvable() == 0) {
							flag = 1;
							//随机打乱
							int i, j, T = 1000, tmp;
							while (T--)
							{
								i = rand() % (max_col * max_row);
								j = rand() % (max_col * max_row);
								tmp = MAP[i];
								MAP[i] = MAP[j];
								MAP[j] = tmp;
							}
						}
						if (flag == 1) MessageBox(GetHWnd(), L"没有可交换的英雄！自动打乱！", L"提示", 0);
						//显示
						for (int i = 0; i < max_col; i++)
							for (int j = 0; j < max_row; j++) {
								putimage(start_x + i * box_width, start_y + j * box_width, &img[MAP[i + j * max_col]]);
							}
					}
					//两点不连通或不是一张图
					else {
						//消去红框
						clearrectangle(start_x + previous_x * box_width - 1, start_y + previous_y * box_width - 1, start_x + (previous_x + 1) * box_width + 1, start_y + (previous_y + 1) * box_width + 1);
						//动画上补充方块
						putimage(start_x + previous_x * box_width, start_y + previous_y * box_width, &img[MAP[previous_x + max_col * previous_y]]);
						//更新先前点
						previous_x = num_x;
						previous_y = num_y;
						//画红框
						setlinecolor(RED);
						setlinestyle(PS_SOLID, 2);
						rectangle(start_x + num_x * box_width, start_y + num_y * box_width, start_x + (num_x + 1) * box_width, start_y + (num_y + 1) * box_width);
					}
				}
				break;
			}
			//点到退出
			else if (x > 760 && x < 850 && y >395 && y < 445) {
				MessageBox(GetHWnd(), L"巅峰召唤师，你放弃了你的水晶！", L"失败", 0);
				is_over = 1;
			}
			//洗牌
			else if (x > 760 && x < 850 && y >245 && y < 295) {
				if (shuffle == 0) {
					MessageBox(GetHWnd(), L"巅峰召唤师，你没有洗牌的机会了！", L"提示", 0);
				}
				else {
					shuffle--;
					//随机打乱
					int i, j, T = 1000, tmp;
					while (T--)
					{
						i = rand() % (max_col * max_row);
						j = rand() % (max_col * max_row);
						tmp = MAP[i];
						MAP[i] = MAP[j];
						MAP[j] = tmp;
					}
					//显示
					for (int i = 0; i < max_col; i++)
						for (int j = 0; j < max_row; j++) {
							putimage(start_x + i * box_width, start_y + j * box_width, &img[MAP[i + j * max_col]]);
						}
					previous_x = -1;
					previous_y = -1;
				}
			}
			//提示
			else if (x > 760 && x < 850 && y >135 && y < 185) {
				if (num_reminder == 0) {
					MessageBox(GetHWnd(), L"巅峰召唤师，你没有提示的机会了！", L"提示", 0);
				}
				else {
					num_reminder--;
					//MessageBox(GetHWnd(), L"该功能未开发！", L"提示", 0);
					auto_remove();
				}


			}
		default:
			break;
		}
		//到这里完成一次判断，对显示框的内容进行一次更新
		state_upd();
		if (is_over == 1)break;//判断游戏是否结束
	}
}

int is_connect(int x1, int y1, int x2, int y2, int is_draw) {
	if (is_connect_one(x1, y1, x2, y2) == 1) {
		if (is_draw == 1) {
			setcolor(0x0A5FFF);
			setlinestyle(PS_SOLID, 4);
			line(start_x + box_width * (x1 + 0.5), start_y + box_width * (y1 + 0.5), start_x + box_width * (x2 + 0.5), start_y + box_width * (y2 + 0.5));
			//设置清除列表
			for (int y = min(y1, y2); y <= max(y1, y2); y++)
				for (int x = min(x1, x2); x <= max(x1, x2); x++)
					clear_list[x + y * max_col] = 1;
		}
		return 1;
	}
	else if (is_connect_two(x1, y1, x2, y2) != -1) {
		if (is_draw == 1) {
			int mid_point = is_connect_two(x1, y1, x2, y2);
			int mid_point_x = mid_point % max_col;
			int mid_point_y = mid_point / max_col;
			setcolor(0x0A5FFF);
			setlinestyle(PS_SOLID, 4);
			line(start_x + box_width * (x1 + 0.5), start_y + box_width * (y1 + 0.5), start_x + box_width * (mid_point_x + 0.5), start_y + box_width * (mid_point_y + 0.5));
			line(start_x + box_width * (x2 + 0.5), start_y + box_width * (y2 + 0.5), start_x + box_width * (mid_point_x + 0.5), start_y + box_width * (mid_point_y + 0.5));
			for (int y = min(y1, mid_point_y); y <= max(y1, mid_point_y); y++)
				for (int x = min(x1, mid_point_x); x <= max(mid_point_x, x1); x++)
					clear_list[x + y * max_col] = 1;
			for (int y = min(mid_point_y, y2); y <= max(mid_point_y, y2); y++)
				for (int x = min(mid_point_x, x2); x <= max(mid_point_x, x2); x++)
					clear_list[x + y * max_col] = 1;
		}
		return 1;
	}
	else if (is_connect_three(x1, y1, x2, y2, is_draw) == 1)
		return 1;
	return 0;
}

void state_upd() {
	//得分
	settextstyle(35, 0, _T("华文行楷"));
	setcolor(0x0A5FFF);
	TCHAR str1[20] = { 0 };
	_stprintf_s(str1, _T("%03d"), score);
	//outtextxy(X1 + 30, Y1 + 6, str1); //区间(x1,y1,x2,y2)=(750,75,850,135)
	clearrectangle(750, 75, 900, 135);
	outtextxy(780, 81, str1);

	//提示次数
	settextstyle(35, 0, _T("华文行楷"));
	setcolor(0x0A5FFF);
	TCHAR str2[20] = { 0 };
	_stprintf_s(str2, _T("%d次"), num_reminder);
	//outtextxy(X1 + 30, Y1 + 6, str2);//区间(x1,y1,x2,y2)=(750,185,850,245)
	clearrectangle(750, 185, 850, 245);
	outtextxy(780, 191, str2);

	//洗牌
	settextstyle(35, 0, _T("华文行楷"));
	setcolor(0x0A5FFF);
	TCHAR str3[20] = { 0 };
	_stprintf_s(str3, _T("%d次"), shuffle);
	//outtextxy(X1 + 30, Y1 + 6, str3);//区间(x1,y1,x2,y2)=(750,295,850,355)
	clearrectangle(750, 295, 850, 355);
	outtextxy(780, 301, str3);

	//时间
	settextstyle(30, 0, _T("宋体"));
	setcolor(0xAB5A00);
	t2 = clock();
	int second = (int)(double)(t2 - t1) / CLOCKS_PER_SEC;

	if (second > 360) {
		MessageBox(GetHWnd(), L"游戏结束！", L"超时", 0);
		is_over = 1;
	}

	second = 360 - second;
	int minute = second / 60;
	second %= 60;
	TCHAR str4[20] = { 0 };
	_stprintf_s(str4, _T("%02d:%02d"), minute, second);
	clearrectangle(750, 350, 850, 390);
	outtextxy(770, 351, str4);
	//outtextxy(X1 + 25, Y1 + 6, L"360s");//区间(x1,y1,x2,y2)=(750,365,850,425);

	//对空白格子进行一次刷新
	//for(int i=0;i<max_col;i++)
	//	for (int j = 0; j < max_row; j++)
	//		if(MAP[i+j*max_col]==0)
	//			clearrectangle(start_x + box_width * i - 1, start_y + box_width * j - 1, start_x + box_width * i + box_width + 1, start_y + box_width * j + box_width + 1);
}

int is_in_col(int x1, int y1, int x2, int y2) {
	if (x1 != x2)
		return 0;
	else {
		//默认y1更小
		if (y2 < y1) {
			int temp = y2;
			y2 = y1;
			y1 = temp;
		}
		for (int y = y1 + 1; y < y2; y++)
			if (MAP[x1 + y * max_col] != 0)return 0;
	}
	return 1;
}

int is_in_row(int x1, int y1, int x2, int y2) {
	if (y1 != y2)
		return 0;
	else {
		//默认x1更小
		if (x2 < x1) {
			int temp = x2;
			x2 = x1;
			x1 = temp;
		}
		for (int x = x1 + 1; x < x2; x++)
			if (MAP[x + y1 * max_col] != 0)return 0;
	}
	return 1;
}

int is_connect_one(int x1, int y1, int x2, int y2) {
	if (is_in_col(x1, y1, x2, y2) == 1 || is_in_row(x1, y1, x2, y2) == 1)
		return 1;
	else
		return 0;
}

int is_connect_two(int x1, int y1, int x2, int y2) {
	//这里判断的顺序不能换，否则连线画图会多余显示
	if (MAP[x1 + y2 * max_col] == 0 && is_connect_one(x1, y1, x1, y2) == 1 && is_connect_one(x2, y2, x1, y2) == 1) {
		return x1 + y2 * max_col;
	}
	else if (MAP[x2 + y1 * max_col] == 0 && is_connect_one(x1, y1, x2, y1) == 1 && is_connect_one(x2, y2, x2, y1) == 1) {
		return x2 + y1 * max_col;
	}
	else
		return -1;
}

int is_connect_three(int x1, int y1, int x2, int y2, int is_draw) {
	//同列检验
	for (int i = 0; i < max_row; i++) {
		if (MAP[x1 + i * max_col] == 0 && is_connect_one(x1, y1, x1, i) == 1 && is_connect_two(x1, i, x2, y2) != -1) {
			if (is_draw == 1) {
				//画线
				int mid_point = is_connect_two(x1, i, x2, y2);
				int mid_point_x = mid_point % max_col;
				int mid_point_y = mid_point / max_col;
				setcolor(0x0A5FFF);
				setlinestyle(PS_SOLID, 4);
				line(start_x + box_width * (x1 + 0.5), start_y + box_width * (y1 + 0.5), start_x + box_width * (x1 + 0.5), start_y + box_width * (i + 0.5));
				line(start_x + box_width * (mid_point_x + 0.5), start_y + box_width * (mid_point_y + 0.5), start_x + box_width * (x1 + 0.5), start_y + box_width * (i + 0.5));
				line(start_x + box_width * (mid_point_x + 0.5), start_y + box_width * (mid_point_y + 0.5), start_x + box_width * (x2 + 0.5), start_y + box_width * (y2 + 0.5));
				//设置清除列表
				for (int y = min(y1, i); y <= max(y1, i); y++)
					clear_list[x1 + y * max_col] = 1;
				for (int y = min(mid_point_y, i); y <= max(mid_point_y, i); y++)
					for (int x = min(mid_point_x, x1); x <= max(mid_point_x, x1); x++)
						clear_list[x + y * max_col] = 1;
				for (int y = min(mid_point_y, y2); y <= max(mid_point_y, y2); y++)
					for (int x = min(mid_point_x, x2); x <= max(mid_point_x, x2); x++)
						clear_list[x + y * max_col] = 1;
			}
			return 1;
		}
	}
	for (int j = 0; j < max_col; j++) {
		if (MAP[j + y1 * max_col] == 0 && is_connect_one(x1, y1, j, y1) == 1 && is_connect_two(j, y1, x2, y2) != -1) {
			int mid_point = is_connect_two(j, y1, x2, y2);
			int mid_point_x = mid_point % max_col;
			int mid_point_y = mid_point / max_col;
			setcolor(0x0A5FFF);
			setlinestyle(PS_SOLID, 4);
			line(start_x + box_width * (x1 + 0.5), start_y + box_width * (y1 + 0.5), start_x + box_width * (j + 0.5), start_y + box_width * (y1 + 0.5));
			line(start_x + box_width * (mid_point_x + 0.5), start_y + box_width * (mid_point_y + 0.5), start_x + box_width * (j + 0.5), start_y + box_width * (y1 + 0.5));
			line(start_x + box_width * (mid_point_x + 0.5), start_y + box_width * (mid_point_y + 0.5), start_x + box_width * (x2 + 0.5), start_y + box_width * (y2 + 0.5));
			//设置清除列表
			for (int x = min(x1, j); x <= max(x1, j); x++)
				clear_list[x + y1 * max_col] = 1;
			for (int y = min(mid_point_y, y1); y <= max(mid_point_y, y1); y++)
				for (int x = min(mid_point_x, j); x <= max(mid_point_x, j); x++)
					clear_list[x + y * max_col] = 1;
			for (int y = min(mid_point_y, y2); y <= max(mid_point_y, y2); y++)
				for (int x = min(mid_point_x, x2); x <= max(mid_point_x, x2); x++)
					clear_list[x + y * max_col] = 1;
			return 1;
		}
	}
	return 0;
}

void clear() {
	for (int i = 0; i < max_col; i++)
		for (int j = 0; j < max_row; j++)
			if (clear_list[i + j * max_col] == 1) {
				clearrectangle(start_x + box_width * i, start_y + box_width * j, start_x + box_width * i + box_width, start_y + box_width * j + box_width);
				//putimage(start_x + i * box_width, start_y + j * box_width, &img[MAP[i + j * max_col]]);
				clear_list[i + j * max_col] = 0;
			}
}

int is_satisfy_10() {
	int num = 0;
	for (int x = 0; x < max_col - 1; x++) {
		for (int y = 0; y < max_row; y++) {
			if (MAP[x + y * max_col] == MAP[x + y * max_col + 1])
				num++;
		}
	}
	for (int x = 0; x < max_col; x++) {
		for (int y = 0; y < max_row - 1; y++) {
			if (MAP[x + y * max_col] == MAP[x + y * max_col + max_col])
				num++;
		}
	}
	if (num >= 10)return 1;
	return 0;
}

void RankInit() {
	clearrectangle(0, 0, 900, 580);
	FILE* stream;
	errno_t err;
	int time_rank[100] = { 0 };
	err = fopen_s(&stream, "score.txt", "a+");
	//for(int i=0;i<10;i++)
	//    fprintf(stream2, "%d\n",i);// : %s \n", name);  //写入
	char ch = 'c';
	int term = 0;
	while (ch != EOF) {
		//if (term == 10)break;
		fscanf_s(stream, "%d", &time_rank[term]);
		ch = fgetc(stream);
		term++;
	}
	//排序，一共term-1个数
	int temp;
	for (int i = 0; i < term - 2; i++) {
		int isSorted = 1;
		for (int j = 0; j < term - 2 - i; j++) {
			if (time_rank[j] < time_rank[j + 1]) {
				temp = time_rank[j];
				time_rank[j] = time_rank[j + 1];
				time_rank[j + 1] = temp;
				isSorted = 0;  //一旦需要交换数组元素，就说明剩下的元素没有排序好
			}
		}
		if (isSorted) break; //如果没有发生交换，说明剩下的元素已经排序好了
	}
	int X1 = 340, X2 = 560, Y1 = 200, Y2 = Y1 + 60;
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 80;                  // 设置字体高度为 100
	wcscpy_s(f.lfFaceName, _T("华文行楷"));
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);                     // 设置字体样式
	outtextxy(300, 50, _T("巅峰积分"));
	int Y = 150;
	setcolor(0x0A5FFF);
	settextstyle(35, 0, _T("宋体"));
	int show_num = 0;
	for (int i = 0; i < term - 1; i++) {
		if (show_num == 8)break;
		//if (score[i] == 0)break;
		TCHAR str4[20] = { 0 };
		_stprintf_s(str4, _T("%d"), time_rank[i]);
		outtextxy(400, Y, str4);
		Y += 50;
		show_num++;
	}
	_fcloseall();
	int flag = 0;
	while (flag == 0)
	{
		rank_plot();
		MOUSEMSG msg = GetMouseMsg(); // 获取鼠标信息
		int x = msg.x;
		int y = msg.y;
		switch (msg.uMsg)
		{
		case WM_MOUSEMOVE:
			currentPos.X = x;
			currentPos.Y = y;
			break;
		case WM_LBUTTONUP:
			if (x > 650 && x < 750 && y >480 && y < 520) {
				flag = 1;
			}
			break;
		default:
			break;
		}
	}
}

void rank_plot() {
	if (currentPos.X > 650 && currentPos.X < 750 && currentPos.Y >480 && currentPos.Y < 520)
		setcolor(0x0000AA);
	else
		setcolor(0xAB5A00);
	settextstyle(50, 0, _T("华文行楷"));
	outtextxy(650, 480, L"退出");//区间(x1,y1,x2,y2)=(750,395,850,455)
}

int is_win() {
	for (int i = 0; i < max_col; i++)
		for (int j = 0; j < max_row; j++) {
			if (MAP[i + max_col * j] != 0)return 0;
		}
	return 1;
}

void mode_choose() {
	//得分
	settextstyle(50, 0, _T("华文行楷"));
	setcolor(0xAB5A00);
	int X1 = 750, X2 = X1 + 100, Y1 = 25, Y2 = Y1 + 60;
	outtextxy(X1 + 10, Y1 + 6, L"得分");//(750,25,850,85)

	Y1 += 50;
	Y2 += 50;

	//提示次数
	if (currentPos.X > 760 && currentPos.X < 850 && currentPos.Y >135 && currentPos.Y < 185)
		setcolor(0x0000AA);
	else
		setcolor(0xAB5A00);
	settextstyle(50, 0, _T("华文行楷"));
	Y1 += 60;
	Y2 += 60;
	outtextxy(X1 + 10, Y1 + 6, L"提示");//(750,135,850,195)

	Y1 += 50;
	Y2 += 50;

	//洗牌
	if (currentPos.X > 760 && currentPos.X < 850 && currentPos.Y >245 && currentPos.Y < 295)
		setcolor(0x0000AA);
	else
		setcolor(0xAB5A00);
	settextstyle(50, 0, _T("华文行楷"));
	Y1 += 60;
	Y2 += 60;
	outtextxy(X1 + 10, Y1 + 6, L"洗牌");//(750,245,850,305)

	Y1 += 50;
	Y2 += 50;

	//退出
	if (currentPos.X > 760 && currentPos.X < 850 && currentPos.Y >395 && currentPos.Y < 445)
		setcolor(0x0000AA);
	else
		setcolor(0xAB5A00);

	settextstyle(50, 0, _T("华文行楷"));
	Y1 += 100;
	Y2 += 100;
	outtextxy(X1 + 10, Y1 + 6, L"退出");//区间(x1,y1,x2,y2)=(750,395,850,455)

	//EndBatchDraw();


}

void setting() {
	int flag = 0;
	while (flag == 0)
	{
		setting_plot();
		MOUSEMSG msg = GetMouseMsg(); // 获取鼠标信息
		int x = msg.x;
		int y = msg.y;
		int X1 = 400, X2 = 450, Y1 = 85, Y2 = Y1 + 60;
		int delta = 110;
		switch (msg.uMsg)
		{
		case WM_MOUSEMOVE:
			currentPos.X = x;
			currentPos.Y = y;
			break;
		case WM_LBUTTONUP:
			if (x > X1 && x < X2 && y > Y1 + delta && y < Y2 + delta) {
				num_hero = 13;
				flag = 1;
			}
			if (x > X1 && x < X2 && y > Y1 + 2 * delta && y < Y2 + 2 * delta) {
				num_hero = 15;
				flag = 1;
			}
			if (x > X1 && x < X2 && y > Y1 + 3 * delta && y < Y2 + 3 * delta) {
				num_hero = 17;
				flag = 1;
			}
			//if (x > X1 && x < X2 && y > Y1 + 4*delta && y < Y2 + 4*delta) {
			//	num_hero = 19;
			//	flag = 1;
			//}
			break;
		default:
			break;
		}
	}
}

void setting_plot() {


	clearrectangle(0, 0, 900, 580);
	BeginBatchDraw();
	settextstyle(80, 0, _T("华文行楷"));
	int X1 = 400, X2 = X1 + 50, Y1 = 85, Y2 = Y1 + 60;
	setcolor(0x000000);
	outtextxy(X1 - 200, Y1 - 40, L"请选择英雄数量");//(X1,Y1,X2,Y2)=(400,25,450,85);

	//13
	Y1 += 110;
	Y2 += 110;
	if (currentPos.X > X1 && currentPos.X < X2 && currentPos.Y >Y1 && currentPos.Y < Y2)//(X1,Y1,X2,Y2)=(400,25+110,450,85+110);
		setcolor(0x0000AA);
	else
		setcolor(0xAB5A00);
	settextstyle(50, 0, _T("华文行楷"));
	outtextxy(X1 + 10, Y1 + 6, L"13");

	//15
	Y1 += 110;
	Y2 += 110;
	if (currentPos.X > X1 && currentPos.X < X2 && currentPos.Y >Y1 && currentPos.Y < Y2)//(X1,Y1,X2,Y2)=(400,25+2*110,450,85+2*110);
		setcolor(0x0000AA);
	else
		setcolor(0xAB5A00);
	settextstyle(50, 0, _T("华文行楷"));

	outtextxy(X1 + 10, Y1 + 6, L"15");

	//17
	Y1 += 110;
	Y2 += 110;
	if (currentPos.X > X1 && currentPos.X < X2 && currentPos.Y >Y1 && currentPos.Y < Y2)//(X1,Y1,X2,Y2)=(400,25+3*110,450,85+3*110);
		setcolor(0x0000AA);
	else
		setcolor(0xAB5A00);
	settextstyle(50, 0, _T("华文行楷"));
	outtextxy(X1 + 10, Y1 + 6, L"17");

	//19
	//Y1 += 110;
	//Y2 += 110;
	//if (currentPos.X > X1 && currentPos.X < X2 && currentPos.Y >Y1 && currentPos.Y < Y2)//(X1,Y1,X2,Y2)=(400,25+4*110,450,85+4*110);
	//	setcolor(0x0000AA);
	//else
	//	setcolor(0xAB5A00);
	//settextstyle(50, 0, _T("华文行楷"));
	//outtextxy(X1 + 10, Y1 + 6, L"19");
	EndBatchDraw();
	//Sleep(5000);
}

int is_connect_solvable() {
	if (is_win() == 1)return 1;
	for (int i = 0; i < max_col * max_row - 1; i++) {
		for (int j = i + 1; j < max_col * max_row; j++) {
			int x1 = i % max_col;
			int y1 = i / max_col;
			int x2 = j % max_col;
			int y2 = j / max_col;
			if (MAP[i] == MAP[j] && MAP[i] != 0 && is_connect(x1, y1, x2, y2, 0) == 1) {
				return 1;
			}
		}
	}
	return 0;
}

void auto_remove() {
	int flag = 0;
	if (is_recharge == 0) {
		for (int i = 0; i < max_col * max_row - 1; i++) {
			for (int j = i + 1; j < max_col * max_row; j++) {
				int x1 = i % max_col;
				int y1 = i / max_col;
				int x2 = j % max_col;
				int y2 = j / max_col;
				if (MAP[i] == MAP[j] && MAP[i] != 0 && is_connect(x1, y1, x2, y2, 1) == 1) {
					score += 2;
					Sleep(200);
					clear();
					putimage(start_x + x1 * box_width, start_y + y1 * box_width, &img[0]);
					putimage(start_x + x2 * box_width, start_y + y2 * box_width, &img[0]);
					//数据上消去两个方块
					MAP[i] = 0;
					MAP[j] = 0;
					//前一个点置空
					previous_x = -1;
					previous_y = -1;
					flag = 1;
					break;
				}
			}
			if (flag == 1) {
				break;
			}
		}
	}
	else {
		for (int i = 0; i < max_col * max_row - 1; i++) {
			for (int j = i + 1; j < max_col * max_row; j++) {
				int x1 = i % max_col;
				int y1 = i / max_col;
				int x2 = j % max_col;
				int y2 = j / max_col;
				if (MAP[i] == MAP[j] && MAP[i] != 0 && is_connect(x1, y1, x2, y2, 1) == 1) {
					now_t = clock();
					//得分加加
					float time = (double)(now_t - last_t) / CLOCKS_PER_SEC;
					if (time < 2.0) {
						//得分
						settextstyle(30, 0, _T("华文行楷"));
						setcolor(0x0A5FFF);
						clearrectangle(750, 75, 900, 135);
						outtextxy(760, 81, L"DOUBLE!");
						score += (num_hero - 13) / 2 + 2;
					}
					score += 2;
					Sleep(200);
					last_t = clock();
					clear();
					putimage(start_x + x1 * box_width, start_y + y1 * box_width, &img[0]);
					putimage(start_x + x2 * box_width, start_y + y2 * box_width, &img[0]);
					//数据上消去两个方块
					MAP[i] = 0;
					MAP[j] = 0;
					//前一个点置空
					previous_x = -1;
					previous_y = -1;
				}
			}
		}
	}
	//判断是否获得胜利获得胜利
	if (is_win() == 1) {
		is_over = 1;
		FILE* stream;
		errno_t err;
		err = fopen_s(&stream, "score.txt", "a+");
		fprintf(stream, "%d\n", score);
		_fcloseall();
		settextstyle(35, 0, _T("华文行楷"));
		setcolor(0x0A5FFF);
		TCHAR str1[20] = { 0 };
		_stprintf_s(str1, _T("%03d"), score);
		//outtextxy(X1 + 30, Y1 + 6, str1); //区间(x1,y1,x2,y2)=(750,75,850,135)
		clearrectangle(750, 75, 900, 135);
		outtextxy(780, 81, str1);
		MessageBox(GetHWnd(), L"恭喜你巅峰召唤师，你获得了游戏的胜利！", L"恭喜", 0);
	}
	//判断是否有可交换点
	flag = 0;
	while (is_connect_solvable() == 0) {
		flag = 1;
		//随机打乱
		int i, j, T = 1000, tmp;
		while (T--)
		{
			i = rand() % (max_col * max_row);
			j = rand() % (max_col * max_row);
			tmp = MAP[i];
			MAP[i] = MAP[j];
			MAP[j] = tmp;
		}
	}
	if (flag == 1) MessageBox(GetHWnd(), L"没有可交换的英雄！自动打乱！", L"提示", 0);
	//显示
	for (int i = 0; i < max_col; i++)
		for (int j = 0; j < max_row; j++) {
			putimage(start_x + i * box_width, start_y + j * box_width, &img[MAP[i + j * max_col]]);
		}
}