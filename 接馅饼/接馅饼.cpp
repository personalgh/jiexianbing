// ���ڱ�.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "graphics.h"
#include "time.h"
#include "windows.h"
#include "resource.h"

#define cooky 460
#define cookspeed 40
#define ballR 40


struct BALL {
	int x;		//������
	int clk;	//λ��ʱ�䣨ms��
	int y;		//������
	int spd;	//С���ٶ�
	int judge;	//С����ʲô(�ڱ���ը��)
	int status;	//״̬���򡢱�����
};

//��С��
void drawball(int x, int y, IMAGE sbll, IMAGE sbllmb) {
	putimage(x-40, y-40, &sbllmb, SRCPAINT);
	putimage(x-40, y-40, &sbll, SRCAND);
}

//���ڱ�
void drawpie(int x, int y, IMAGE p, IMAGE pmb) {
	putimage(x, y, &pmb, SRCPAINT);
	putimage(x, y, &p, SRCAND);
}

//��ը��
void drawbomb(int x, int y, IMAGE bmb, IMAGE bmbmb) {
	putimage(x, y, &bmbmb, SRCPAINT);
	putimage(x, y, &bmb, SRCAND);
}

//��Ϸ����
void gameover() {
	settextstyle(100, 0, _T("��Բ"));
	settextcolor(BLUE);
	outtextxy(200, 200, _T("��Ϸ����"));
	for (int ostop = 0; ostop != 1;) {
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			ostop++;
		}
	}
}

void printusername(wchar_t *usn) {
	setbkmode(TRANSPARENT);
	settextstyle(30, 0, _T("��Բ"));
	settextcolor(MAGENTA);
	outtextxy(200, 35, L"WELCOME,");
	outtextxy(320, 35, usn);
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
	unsigned int nlasttime;
	int dt = 0;
	int life = 3;
	int overornot = 0;
	int overx, overy;
	int boomtime = 0;
	int mainswitch = 5;
	int begin = 90;
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
	BALL ball[30] = { 0 };
	MOUSEMSG GMMsg;
	wchar_t username[30];


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
	}
	//�û���
	BeginBatchDraw();
	putimage(0, 0, &main);
	EndBatchDraw();
	InputBox(username, 30, L"�������û���");
	while (true) {
		while (mainswitch == 5) {
			//��ʼ�������ͼ
			BeginBatchDraw();
			putimage(0, 0, &main);

			//��ӡ�û���
			printusername(username);

			MOUSEMSG GMMsg = GetMouseMsg();
			//�����ص�����Ϸ�ж�
			if (GMMsg.x >= 207 && GMMsg.x <= 269 && GMMsg.y >= 366 && GMMsg.y <= 563 && GMMsg.uMsg == WM_LBUTTONUP) {
				mainswitch = 1;
			}
			//�����صļ����ж�
			if (GMMsg.x >= 327 && GMMsg.x <= 382 && GMMsg.y >= 394 && GMMsg.y <= 514 && GMMsg.uMsg == WM_LBUTTONUP) {
				mainswitch = 2;
			}
			//�����ص����а��ж�
			if (GMMsg.x >= 444 && GMMsg.x <= 504 && GMMsg.y >= 371 && GMMsg.y <= 555 && GMMsg.uMsg == WM_LBUTTONUP) {
				mainswitch = 3;
			}
			//�����ص��˳��ж�
			if (GMMsg.x >= 564 && GMMsg.x <= 626 && GMMsg.y >= 394 && GMMsg.y <= 515 && GMMsg.uMsg == WM_LBUTTONUP) {
				mainswitch = 4;
			}

			//======================================================================================================//

			//��������ϷЧ��
			if (GMMsg.x >= 207 && GMMsg.x <= 269 && GMMsg.y >= 366 && GMMsg.y <= 563) {
				putimage(0, 0, &newgame1);
				printusername(username);
			}
			//��������ϷЧ��
			if (GMMsg.x >= 207 && GMMsg.x <= 269 && GMMsg.y >= 366 && GMMsg.y <= 563 && GMMsg.uMsg == WM_LBUTTONDOWN) {
				putimage(0, 0, &newgame2);
				printusername(username);
			}
			//��������Ч��
			if (GMMsg.x >= 327 && GMMsg.x <= 382 && GMMsg.y >= 394 && GMMsg.y <= 514) {
				putimage(0, 0, &continue1);
				printusername(username);
			}
			//���¼���Ч��
			if (GMMsg.x >= 327 && GMMsg.x <= 382 && GMMsg.y >= 394 && GMMsg.y <= 514 && GMMsg.uMsg == WM_LBUTTONDOWN) {
				putimage(0, 0, &continue2);
				printusername(username);
			}
			//�������а�Ч��
			if (GMMsg.x >= 444 && GMMsg.x <= 504 && GMMsg.y >= 371 && GMMsg.y <= 555) {
				putimage(0, 0, &rankinglist1);
				printusername(username);
			}
			//�������а�Ч��
			if (GMMsg.x >= 444 && GMMsg.x <= 504 && GMMsg.y >= 371 && GMMsg.y <= 555 && GMMsg.uMsg == WM_LBUTTONDOWN) {
				putimage(0, 0, &rankinglist2);
				printusername(username);
			}
			//�����˳�Ч��
			if (GMMsg.x >= 564 && GMMsg.x <= 626 && GMMsg.y >= 394 && GMMsg.y <= 515) {
				putimage(0, 0, &quit1);
				printusername(username);
			}
			//�����˳�Ч��
			if (GMMsg.x >= 564 && GMMsg.x <= 626 && GMMsg.y >= 394 && GMMsg.y <= 515 && GMMsg.uMsg == WM_LBUTTONDOWN) {
				putimage(0, 0, &quit2);
				printusername(username);
			}

			//��������ƽ���
			EndBatchDraw();
		}

		//�����ж�
		switch (mainswitch) {

			//�˳�����
		case 4:
			return 0;

			//���а񲿷�
		case 3:
			MessageBox(GetHWnd(), L"�����У������ڴ���(�����)��", L"���а�", MB_OK);
			break;

			//��������
		case 2:
			MessageBox(GetHWnd(), L"�����У������ڴ���(�����)��", L"������Ϸ", MB_OK);
			break;

			//����Ϸ����
		case 1:
			//��ʼ����ʱ��
			time_t starttime, nowtime;
			time(&starttime);

			//���
			for (i = 0; i < 30; i++) {
				ball[i].x =  0;
				ball[i].y = 0;
			}
			nscore = 0;
			life = 3;
			//��Ϸ��ѭ��
			while (true) {



				if (mainswitch==5) {
					break;
				}
				//��ʼ��ͼ
				BeginBatchDraw();

				//������ʾ
				putimage(0, 0, &BG);
				setbkmode(TRANSPARENT);

				//����ʱ��ʾ����ը��
				if (overornot == 1) {
					drawbomb(overx, overy, bmb, bmbmb);
				}

				//��ӡ�û���
				printusername(username);

				//�÷���ʾ
				settextstyle(30, 0, _T("��Բ"));
				settextcolor(MAGENTA);
				wchar_t cscore[10] = L"nscore";
				_itow_s(nscore, cscore, 10, 10);
				outtextxy(640, 150, cscore);

				//������ʾ
				settextstyle(50, 0, _T("Calibri"), 0, 0, 700, false, false, false);
				settextcolor(MAGENTA);
				wchar_t mylife[] = L"left life =";
				wchar_t clife[10] = L"life";
				_itow_s(life, clife, 10, 10);
				outtextxy(440, 20, mylife);
				outtextxy(600, 20, clife);

				//ʱ����ʾ
				settextstyle(30, 0, _T("��Բ"));
				settextcolor(MAGENTA);
				time(&nowtime);
				nlasttime = begin - (nowtime - starttime);
				wchar_t clasttime[10] = L"clasttime";
				_itow_s(nlasttime, clasttime, 10);
				outtextxy(720, 146, clasttime);

				//�������
				NUM = rand() % 3;
				for (k = 0; k < 30; k++) {
					if (ball[k].x == 0) {
						break;									//30����ȫ����ռ��ʱ���ֵ�30������״��------���case�жϵ�30�ŵ������
					}
				}

				//��ͣ
				putimage(700, 225, &picexitmb, SRCPAINT);
				putimage(700, 225, &picexit, SRCAND);
				putimage(630, 225, &picstopmb, SRCPAINT);
				putimage(630, 225, &picstop, SRCAND);
				while (MouseHit()) {
					GMMsg = GetMouseMsg();
					if (GMMsg.x >= 700 && GMMsg.x <= 700 + 50 && GMMsg.y >= 225 && GMMsg.y <= 225 + 50 && GMMsg.uMsg == WM_LBUTTONUP) {
						mainswitch = 5;
						break;
					}
					if (GMMsg.x >= 630 && GMMsg.x <= 630 + 50 && GMMsg.y >= 225 && GMMsg.y <= 225 + 50 && GMMsg.uMsg == WM_LBUTTONUP) {
						time_t temptime1, temptime2;
						time(&temptime1);
						while (true) {
							GMMsg = GetMouseMsg();
							if (GMMsg.x >= 700 && GMMsg.x <= 700 + 50 && GMMsg.y >= 225 && GMMsg.y <= 225 + 50 && GMMsg.uMsg == WM_LBUTTONUP) {
								mainswitch = 5;
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

				//�ж�===��������򣨴���һ�����꣩
				if (dt != nlasttime) {
					ball[k].x = rand() % 700 + 50;
					ball[k].y = 15;
					ball[k].spd = rand() % 20 + 5;
					ball[k].clk = clock();
					ball[k].status = 0;
					dt = nlasttime;
				}
				//���������֮���ĸ��ʵ��ڱ�
				if (NUM == 0 || NUM == 1) {
					ball[k].judge = 1;
				}
				//���������֮һ�ĸ��ʵ�ը��
				if (NUM == 2) {
					ball[k].judge = 2;
				}

				//ȡ�õ�ǰʱ�䣨���룩
				int mm = clock();

				//�������ը���������µ���
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

					//���ײ�����ʧ
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

				//�ӳ�
				Sleep(50);

				//��ʦ����
				{
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
				}
				//���򡢽�ը���ж�
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

				//��ըЧ��
				if (nowtime <= boomtime + 1) {
					putimage(cookx + 26, cooky - 30, &boommbpic, SRCPAINT);
					putimage(cookx + 26, cooky - 30, &boompic, SRCAND);
				}

				//����ʱ����������
				settextstyle(50, 0, _T("Calibri"), 0, 0, 700, false, false, false);
				_itow_s(life, clife, 10, 10);
				outtextxy(600, 20, clife);

				//������ͼ
				EndBatchDraw();

				//�ж���Ϸ����
				if (nlasttime == 0 || life == 0) {
					if (overornot != 1) {
						overornot++;
						continue;
					}
					gameover();
				}

				//�ӳ�
				Sleep(10);
			}
			break;

		default:
			continue;

		}
	}
	return 0;
}