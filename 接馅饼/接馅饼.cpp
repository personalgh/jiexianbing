// 接馅饼.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "graphics.h"
#include "time.h"
#include "windows.h"
#include "resource.h"

#define cooky 460
#define cookspeed 40
#define ballR 40

struct rank {
	wchar_t name[30];
	int sc;
};

struct BALL {
	int x;		//横坐标
	int clk;	//位置时间（ms）
	int y;		//纵坐标
	int spd;	//小球速度
	int judge;	//小球是什么(馅饼、炸弹)
	int status;	//状态（球、饼弹）
};

//画小球
void drawball(int x, int y, IMAGE sbll, IMAGE sbllmb) {
	putimage(x - 40, y - 40, &sbllmb, SRCPAINT);
	putimage(x - 40, y - 40, &sbll, SRCAND);
}

//画馅饼
void drawpie(int x, int y, IMAGE p, IMAGE pmb) {
	putimage(x, y, &pmb, SRCPAINT);
	putimage(x, y, &p, SRCAND);
}

//画炸弹
void drawbomb(int x, int y, IMAGE bmb, IMAGE bmbmb) {
	putimage(x, y, &bmbmb, SRCPAINT);
	putimage(x, y, &bmb, SRCAND);
}

//游戏结束
void gameover(int &mainswitch, rank rl, wchar_t username[], int score, FILE *rkfile) {
	rl.sc = score;
	wcscpy_s(rl.name, username);
	_wfopen_s(&rkfile, L"ranking.txt", L"w");
	fwprintf(rkfile, L"%30s%6d\n", rl.name, rl.sc);
	fclose(rkfile);
	settextstyle(100, 0, _T("幼圆"));
	settextcolor(BLUE);
	outtextxy(200, 200, _T("游戏结束"));
	outtextxy(170, 300, _T("按回车返回"));
	for (int ostop = 0; ostop != 1;) {
		if (GetAsyncKeyState(VK_RETURN) & 0x1) {
			mainswitch = 5;
			break;
		}
	}
}

void printusername(wchar_t *usn) {
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, _T("幼圆"));
	settextcolor(MAGENTA);
	outtextxy(200 - 30, 35, L"WELCOME,");
	outtextxy(320 - 30, 35, usn);
}

void showlife(int life) {
	settextstyle(50, 0, _T("Calibri"), 0, 0, 700, false, false, false);
	settextcolor(MAGENTA);
	wchar_t mylife[] = L"left life =";
	wchar_t clife[10] = L"life";
	_itow_s(life, clife, 10, 10);
	outtextxy(440, 20, mylife);
	outtextxy(600, 20, clife);
}

void Ps(int &mainswitch, int &timefix, unsigned int &nlasttime, int &timeswitch, int &begin, IMAGE &cntn, IMAGE &cntnmb) {
	MOUSEMSG GMMsg;
	while (MouseHit()) {
		GMMsg = GetMouseMsg();
		if (GMMsg.x >= 700 && GMMsg.x <= 700 + 50 && GMMsg.y >= 225 && GMMsg.y <= 225 + 50 && GMMsg.uMsg == WM_LBUTTONUP) {
			mainswitch = 5;
			timefix = nlasttime;
			timeswitch = 1;
			break;
		}
		if (GMMsg.x >= 630 && GMMsg.x <= 630 + 50 && GMMsg.y >= 225 && GMMsg.y <= 225 + 50 && GMMsg.uMsg == WM_LBUTTONUP) {
			time_t temptime1, temptime2;
			time(&temptime1);
			while (true) {
				BeginBatchDraw();
				putimage(630, 225, &cntnmb, SRCPAINT);
				putimage(630, 225, &cntn, SRCAND);
				EndBatchDraw();
				GMMsg = GetMouseMsg();
				if (GMMsg.x >= 700 && GMMsg.x <= 700 + 50 && GMMsg.y >= 225 && GMMsg.y <= 225 + 50 && GMMsg.uMsg == WM_LBUTTONUP) {
					mainswitch = 5;
					timefix = nlasttime;
					timeswitch = 1;
					break;
				}
				else if (GMMsg.x >= 630 && GMMsg.x <= 630 + 50 && GMMsg.y >= 225 && GMMsg.y <= 225 + 50 && GMMsg.uMsg == WM_LBUTTONUP) {
					time(&temptime2);
					begin += (temptime2 - temptime1);
					break;
				}
				else continue;
			}
		}
	}
}

void creatball(BALL ball[], int &dt, unsigned int &nlasttime, int &k) {
	if (dt != nlasttime) {
		ball[k].x = rand() % 700 + 50;
		ball[k].y = 15;
		ball[k].spd = rand() % 20 + 5;
		ball[k].clk = clock();
		ball[k].status = 0;
		dt = nlasttime;
	}
}

void showtime(time_t &nowtime, unsigned int &nlasttime, int &begin, time_t &starttime, int&ctnswitch, int &timeswitch, int&timefix) {
	settextstyle(30, 0, _T("幼圆"));
	settextcolor(MAGENTA);
	time(&nowtime);
	nlasttime = begin - (nowtime - starttime);
	if (ctnswitch == 1 && timeswitch == 1) {
		begin = timefix;
		timeswitch = 0;
	}
	wchar_t clasttime[10] = L"clasttime";
	_itow_s(nlasttime, clasttime, 10);
	outtextxy(720, 146, clasttime);
}









int main()
{
	initgraph(800, 600);
	srand((unsigned)time(NULL));
	int cookx = 350;
	int i = 0, t = 0, k = 0, n = 0;
	int NUM = 0;
	int nscore = 0, clasttime = 0;
	int direction = 0;
	unsigned int nlasttime = 0;
	int dt = 0;
	int life = 3;
	int overornot = 0;
	int overx, overy;
	int boomtime = 0;
	int mainswitch = 5, ctnswitch = 0;
	int begin = 90;
	int timefix = 0, timeswitch = 0;
	IMAGE BG, main, Rcook, Rcookm, Lcook, Lcookm;
	IMAGE pi, pimb, bmb, bmbmb;
	IMAGE boompic, boommbpic;
	IMAGE newgame1, newgame2;
	IMAGE continue1, continue2;
	IMAGE rankinglist1, rankinglist2;
	IMAGE picexit, picexitmb;
	IMAGE picstop, picstopmb;
	IMAGE quit1, quit2;
	IMAGE BLL, BLLMB;
	IMAGE cntn, cntnmb;
	BALL ball[30] = { 0 };
	MOUSEMSG GMMsg;
	wchar_t username[30] = { '\0' };
	rank rankinglist = { '\0' };
	rank rankprint[30] = { '\0' };
	FILE rkfile;


	{
		//loadimage
		loadimage(&BG, L"image", MAKEINTRESOURCE(background));
		loadimage(&pi, L"image", MAKEINTRESOURCE(pancake));
		loadimage(&pimb, L"image", MAKEINTRESOURCE(pancakemb));
		loadimage(&bmb, L"image", MAKEINTRESOURCE(bbomb));
		loadimage(&bmbmb, L"image", MAKEINTRESOURCE(bbombmb));
		loadimage(&Rcook, L"image", MAKEINTRESOURCE(RIGHT));
		loadimage(&Rcookm, L"image", MAKEINTRESOURCE(rightmb));
		loadimage(&Lcook, L"image", MAKEINTRESOURCE(LEFT));
		loadimage(&Lcookm, L"image", MAKEINTRESOURCE(leftmb));
		loadimage(&boompic, L"image", MAKEINTRESOURCE(boom));
		loadimage(&boommbpic, L"image", MAKEINTRESOURCE(boommb));
		loadimage(&main, L"image", MAKEINTRESOURCE(Main));
		loadimage(&newgame1, L"image", MAKEINTRESOURCE(NG1));
		loadimage(&newgame2, L"image", MAKEINTRESOURCE(NG2));
		loadimage(&continue1, L"image", MAKEINTRESOURCE(CTN1));
		loadimage(&continue2, L"image", MAKEINTRESOURCE(CTN2));
		loadimage(&rankinglist1, L"image", MAKEINTRESOURCE(RL1));
		loadimage(&rankinglist2, L"image", MAKEINTRESOURCE(RL2));
		loadimage(&quit1, L"image", MAKEINTRESOURCE(QT1));
		loadimage(&quit2, L"image", MAKEINTRESOURCE(QT2));
		loadimage(&picexit, L"JPG", MAKEINTRESOURCE(exit));
		loadimage(&picexitmb, L"JPG", MAKEINTRESOURCE(exitmb));
		loadimage(&picstop, L"image", MAKEINTRESOURCE(stop));
		loadimage(&picstopmb, L"image", MAKEINTRESOURCE(stopmb));
		loadimage(&BLL, L"image", MAKEINTRESOURCE(bll));
		loadimage(&BLLMB, L"image", MAKEINTRESOURCE(bllmb));
		loadimage(&cntn, L"image", MAKEINTRESOURCE(conti));
		loadimage(&cntnmb, L"image", MAKEINTRESOURCE(contimb));
	}
	//用户名
	BeginBatchDraw();
	putimage(0, 0, &main);
	EndBatchDraw();
	InputBox(username, 30, L"请输入用户名");
	if (username[0] == '\0') {
		wcscpy(username, L"UNNAMED");
	}
	while (true) {
		while (mainswitch == 5) {
			//开始主界面绘图
			BeginBatchDraw();
			putimage(0, 0, &main);

			//打印用户名
			printusername(username);

			MOUSEMSG GMMsg = GetMouseMsg();
			//主开关的新游戏判断
			if (GMMsg.x >= 207 && GMMsg.x <= 269 && GMMsg.y >= 366 && GMMsg.y <= 563 && GMMsg.uMsg == WM_LBUTTONUP) {
				mainswitch = 1;
			}
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				mainswitch = 1;
			}
			//主开关的继续判断
			if (GMMsg.x >= 327 && GMMsg.x <= 382 && GMMsg.y >= 394 && GMMsg.y <= 514 && GMMsg.uMsg == WM_LBUTTONUP) {
				mainswitch = 1;
				ctnswitch = 1;
			}
			//主开关的排行榜判断
			if (GMMsg.x >= 444 && GMMsg.x <= 504 && GMMsg.y >= 371 && GMMsg.y <= 555 && GMMsg.uMsg == WM_LBUTTONUP) {
				mainswitch = 3;
			}
			//主开关的退出判断
			if (GMMsg.x >= 564 && GMMsg.x <= 626 && GMMsg.y >= 394 && GMMsg.y <= 515 && GMMsg.uMsg == WM_LBUTTONUP) {
				mainswitch = 4;
			}

			//======================================================================================================//

			//触摸新游戏效果
			if (GMMsg.x >= 207 && GMMsg.x <= 269 && GMMsg.y >= 366 && GMMsg.y <= 563) {
				putimage(0, 0, &newgame1);
				printusername(username);
			}
			//按下新游戏效果
			if (GMMsg.x >= 207 && GMMsg.x <= 269 && GMMsg.y >= 366 && GMMsg.y <= 563 && GMMsg.uMsg == WM_LBUTTONDOWN) {
				putimage(0, 0, &newgame2);
				printusername(username);
			}
			//触摸继续效果
			if (GMMsg.x >= 327 && GMMsg.x <= 382 && GMMsg.y >= 394 && GMMsg.y <= 514) {
				putimage(0, 0, &continue1);
				printusername(username);
			}
			//按下继续效果
			if (GMMsg.x >= 327 && GMMsg.x <= 382 && GMMsg.y >= 394 && GMMsg.y <= 514 && GMMsg.uMsg == WM_LBUTTONDOWN) {
				putimage(0, 0, &continue2);
				printusername(username);
			}
			//触摸排行榜效果
			if (GMMsg.x >= 444 && GMMsg.x <= 504 && GMMsg.y >= 371 && GMMsg.y <= 555) {
				putimage(0, 0, &rankinglist1);
				printusername(username);
			}
			//按下排行榜效果
			if (GMMsg.x >= 444 && GMMsg.x <= 504 && GMMsg.y >= 371 && GMMsg.y <= 555 && GMMsg.uMsg == WM_LBUTTONDOWN) {
				putimage(0, 0, &rankinglist2);
				printusername(username);
			}
			//触摸退出效果
			if (GMMsg.x >= 564 && GMMsg.x <= 626 && GMMsg.y >= 394 && GMMsg.y <= 515) {
				putimage(0, 0, &quit1);
				printusername(username);
			}
			//按下退出效果
			if (GMMsg.x >= 564 && GMMsg.x <= 626 && GMMsg.y >= 394 && GMMsg.y <= 515 && GMMsg.uMsg == WM_LBUTTONDOWN) {
				putimage(0, 0, &quit2);
				printusername(username);
			}

			//主界面绘制结束
			EndBatchDraw();
		}

		//开关判断
		switch (mainswitch) {

			//退出部分
		case 4:
			return 0;

			//排行榜部分
		case 3:
			MessageBox(GetHWnd(), L"排行榜还在开发中，敬请期待︿(￣︶￣)︿\n\n在ranking.txt中可以查看全部分数", L"排行榜", MB_OK);
			system("notepad.exe ranking.txt");
			mainswitch = 5;
			break;

			//新游戏部分
		case 1:
			//开始计算时间
			time_t starttime, nowtime;
			time(&starttime);
			//清空
			if (mainswitch == 1 && ctnswitch == 0) {
				for (i = 0; i < 30; i++) {
					ball[i].x = 0;
					ball[i].y = 0;
				}
				nscore = 0;
				life = 3;
			}
			//游戏主循环
			while (true) {

				if (mainswitch == 5) {
					break;
				}
				//开始绘图
				BeginBatchDraw();

				//背景显示
				putimage(0, 0, &BG);
				setbkmode(TRANSPARENT);

				//打印用户名
				printusername(username);

				//得分显示
				settextstyle(30, 0, _T("幼圆"));
				settextcolor(MAGENTA);
				wchar_t cscore[10] = L"nscore";
				_itow_s(nscore, cscore, 10, 10);
				outtextxy(640, 150, cscore);

				//生命显示
				showlife(life);

				//时间显示
				showtime(nowtime, nlasttime, begin, starttime, ctnswitch, timeswitch, timefix);

				//按钮显示
				putimage(700, 225, &picexitmb, SRCPAINT);
				putimage(700, 225, &picexit, SRCAND);
				putimage(630, 225, &picstopmb, SRCPAINT);
				putimage(630, 225, &picstop, SRCAND);

				//随机掉球
				for (k = 0; k < 30; k++) {
					if (ball[k].x == 0) {
						break;
					}
				}

				//判断===》整秒掉球（创建一个坐标）
				creatball(ball, dt, nlasttime, k);

				NUM = rand() % 3;
				//随机，三分之二的概率掉馅饼
				if (NUM == 0 || NUM == 1) {
					ball[k].judge = 1;
				}
				//随机，三分之一的概率掉炸弹
				if (NUM == 2) {
					ball[k].judge = 2;
				}

				//暂停
				Ps(mainswitch, timefix, nlasttime, timeswitch, begin, cntn, cntnmb);

				//取得当前时间（毫秒）
				int mm = clock();

				//画球或者炸弹（球往下掉）
				for (i = 0; i < 30; i++) {
					if (ball[i].x != 0) {
						if (ball[i].clk + 300 >= mm) {
							if (ball[i].status % 2 == 1) {
								drawball(ball[i].x, ball[i].y, BLL, BLLMB);
							}
							else if (ball[i].status % 2 == 0) {
								if (ball[i].judge == 1) {
									drawpie(ball[i].x - 35, ball[i].y - 20, pi, pimb);
								}
								if (ball[i].judge == 2) {
									drawbomb(ball[i].x - 35, ball[i].y - 40, bmb, bmbmb);
								}
							}
						}
						else if (ball[i].clk + 300 < mm) {
							ball[i].status++;
							ball[i].clk = mm;
						}
						ball[i].y = ball[i].y + ball[i].spd;
					}

					//到底部则消失
					if (ball[i].y > 600) {
						if (ball[i].judge == 1) {
							ball[i].x = { 0 };
							ball[i].y = 0;
							ball[i].spd = 0;
							ball[i].judge = 0;
							nscore -= 10;
						}
						if (ball[i].judge == 2) {
							ball[i].x = { 0 };
							ball[i].y = 0;
							ball[i].spd = 0;
							ball[i].judge = 0;
						}
					}
				}

				//延迟
				Sleep(50);

				//厨师控制

				if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
					cookx = cookx + cookspeed;
					if (cookx > 671) {
						cookx = 671;
					}
				}
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
					cookx = cookx - cookspeed;
					if (cookx < 0) {
						cookx = 0;
					}
				}
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					return 0;
				}
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
					direction = 1;
				}
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
					direction = 2;
				}
				if (direction == 0) {
					putimage(cookx, cooky, &Rcookm, SRCPAINT);
					putimage(cookx, cooky, &Rcook, SRCAND);
				}
				if (direction == 1) {
					putimage(cookx, cooky, &Rcookm, SRCPAINT);
					putimage(cookx, cooky, &Rcook, SRCAND);
				}
				if (direction == 2) {
					putimage(cookx, cooky, &Lcookm, SRCPAINT);
					putimage(cookx, cooky, &Lcook, SRCAND);
				}

				//接球、接炸弹判断
				for (i = 0; i < 30; i++) {
					if (ball[i].x + 25 >= cookx&&ball[i].x - 25 <= cookx + 112 && ball[i].y + 20 >= 460 && ball[i].y - 10 <= cooky + 129) {
						if (ball[i].judge == 1) {
							ball[i].x = { 0 };
							ball[i].y = 0;
							ball[i].spd = 0;
							ball[i].judge = 0;
							nscore += 10;
						}
						if (ball[i].judge == 2) {
							life -= 1;
							if (life == 0) {
								overx = ball[i].x;
								overy = ball[i].y;
							}
							ball[i].x = { 0 };
							ball[i].y = 0;
							ball[i].spd = 0;
							ball[i].judge = 0;
							nscore -= 100;
							boomtime = nowtime;


						}
					}
				}

				//爆炸效果
				if (nowtime <= boomtime + 1) {
					putimage(cookx + 26, cooky - 30, &boommbpic, SRCPAINT);
					putimage(cookx + 26, cooky - 30, &boompic, SRCAND);
				}

				//死亡时更新生命数
				showlife(life);

				//结束绘图
				EndBatchDraw();

				//判断游戏结束
				if (nlasttime == 0 || life == 0) {
					if (overornot != 1) {
						overornot++;
						continue;
					}
					gameover(mainswitch, rankinglist, username, nscore, &rkfile);
				}

				//延迟
				Sleep(10);
			}
			break;

		default:
			continue;

		}
	}
	return 0;
}