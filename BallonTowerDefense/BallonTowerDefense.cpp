// BallonTowerDefense.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "windows.h"
#include "conio.h"
#include "time.h"
#include "stdlib.h"

#define p 112 //일시정지 
#define P 80 //일시정지
#define ESC 27 //게임종료
#define SPACE 32 //무기사용

#define false 0
#define true 1
#define MOP1 3
#define MOP2 6
#define MOP3 9
#define MOP4 12
#define MOP5 15
#define MOP6 18
#define MOP7 21
#define MOP8 24
#define MOP9 27
#define BOSS_1 30
#define BOSS_2 31
#define BOSS_3 32
#define EMPTY 0 //빈공간을 표시
#define WEAPON1 1
#define WEAPON2 2
#define WEAPON3 3
#define WEAPON4 4
#define WEAPON5 5

#define Mx 5 //몬스터 와 무기 x좌표
#define Max_Mx 45 //몬스터 와 무기 최대이동 x좌표
#define My 17 //몬스터 와 무기 y좌표

int mx, my; //이동중인 몬스터의 게임판상의 x,y좌표를 저장
int wx, wy; //이동중인 무기의 게임판상의 x,y좌표를 저장
int hit; //총알과 몬스터의 충돌
int crash; //몬스터와 타워의 충돌
int mopz; //몬스터리젠 시간
int key; //입력받는 키값
int magic = 200; //마법 입력값
int power = 500; //파워업 입력값
int t; //증가하는 변수값
int zen = 6; //풍선 리젠 타임 (기본값 6)


int main_org[My][Max_Mx]; //게임판의 정보를 저장하는 배열 모니터에 표시후에 main_cpy로 복사됨
int main_cpy[My][Max_Mx]; //즉 map_cpy는 게임판이 모니터에 표시되기 전의 정보를 가지고 있음
						  //map의 전체를 계속 모니터에 표시하지 않고(이렇게 하면 모니터가 깜빡거림)
						  //map_cpy와 배열을 비교해서 값이 달라진 곳만 모니터에 고침

struct status {
	int Score; //점수
	int Best_Score; //최고점수
	int Gold; //소유골드
	int Hp; //타워체력
	int Weapon_level; //무기레벨
	int Weapon_speed; //무기스피드
	int Wave; //웨이브
}status;

struct monster {
	int HP; //몬스터체력
	int Speed; //몬스터속도
}monster;

void setcolor(int color);
void introduce(void);
void title(void); //게임시작화면 
void map(void);
void reset_main(void); //게임판 초기화
void reset_main_cpy(void);
void draw_main(void);
void show_monster(void); // 몬스터 등장 함수
void move_monster(void); //몬스터 이동 함수
void die_monster(void); //몬스터 죽음 처리 함수
void damage_tower(void); //타워 데미지 처리
void status_draw(void); //상태창 표시
void wave_up(void); //다음 웨이브
void board_clean(void); //상황판 지우기
void check_key(void); //키입력 함수
void game_over(void); //게임오버함수

enum {
	BLOCK,         //  0 : 까망 
	DARK_BLUE,      //  1 : 어두운 파랑 
	DARK_GREEN,      //  2 : 어두운 초록 
	DARK_SKY_BLUE,   //  3 : 어두운 하늘 
	DARK_RED,      //  4 : 어두운 빨강 
	DARK_VOILET,   //  5 : 어두운 보라 
	DARK_YELLOW,   //  6 : 어두운 노랑 
	GRAY,         //  7 : 회색 
	DARK_GRAY,      //  8 : 어두운 회색 
	BLUE,         //  9 : 파랑 
	GREEN,         // 10 : 초록 
	SKY_BLUE,      // 11 : 하늘 
	RED,         // 12 : 빨강 
	VOILET,         // 13 : 보라 
	YELLOW,         // 14 : 노랑 
	WHITE,         // 15 : 하양 
};

// 콘솔 색깔을 지정하는 함수
void setcolor(int color)//색깔함수
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y) { //gotoxy함수 
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기는 함수에 사용되는 열거형 
void setcursortype(CURSOR_TYPE c) { //커서숨기는 함수 
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

int main() {
	setcursortype(NOCURSOR);
	introduce();
	title();
	reset_main();
	system("cls");

	map();
	while (1) {
		for (t = 0; ; t++) {
			check_key();
			board_clean();
			status_draw();
			if (t % zen == 0) {
				show_monster();
			}
			move_monster();
			draw_main();
			Sleep(200);

			damage_tower();
			wave_up();
			game_over();
		}
	}

}
void introduce()
{
	int x = 0;
	int y = 0;
	gotoxy(x + 25, y + 25);
	setcolor(10);
	printf("BALOONS TOWER DEFENSE");





	while (1)
	{ //메인화면 적출현하는 애니메이션효과
		if (_kbhit() && _getch()) break; //키입력이 있으면 무한루프 종료 
	}
	system("cls");
}

void title(void)
{
	int i;
	int x = 5; //타이틀화면이 표시되는 x좌표 
	int y = 4; //타이틀화면이 표시되는 y좌표  
	setcolor(15);

	gotoxy(x, y + 0); printf("□□□□□□□□□□□□□□■□□□□"); Sleep(100);
	gotoxy(x, y + 1); printf("□□□□□□□□□□□□□■■■□□□"); Sleep(100);
	gotoxy(x, y + 2); printf("□□□□□□□□□□□□■■■■■□□"); Sleep(100);
	gotoxy(x, y + 3); printf("□□□□□□□□□□□□■      ■□□"); Sleep(100);
	gotoxy(x, y + 4); printf("□□□□□□□□□□□□■      ■□□"); Sleep(100);
	gotoxy(x, y + 5); printf("□□□□□□□□□□■■■■■■■■■"); Sleep(100);
	gotoxy(x, y + 6); printf("□□□□□□□□□□■              ■"); Sleep(100);
	gotoxy(x, y + 7); printf("□□□□□□□□□□■              ■"); Sleep(100);
	gotoxy(x, y + 8); printf("□□□□□□□□□□■              ■"); Sleep(100);
	gotoxy(x, y + 9); printf("□□□□□□□□□□■■■■■■■■■"); Sleep(100); setcolor(10);
	gotoxy(x + 14, y + 6); printf("H.T");
	gotoxy(x + 11, y + 7); printf("BALLOONS TOWER");
	gotoxy(x + 13, y + 8); printf("DEFENSE"); setcolor(15); Sleep(100);
	gotoxy(x + 8, y + 15); printf("2011244067 이동준");
	gotoxy(x, y + 21); printf("  SPACE BAR  : Attack");
	gotoxy(x, y + 22); printf("  ESC        : Quit");
	gotoxy(x, y + 24); printf("Please Enter Any Key to Start..");

	while (1) { //메인화면 적출현하는 애니메이션효과
		if (_kbhit()) break; //키입력이 있으면 무한루프 종료 

		for (i = 0; i < 10; i++) {
			if (_kbhit()) break;
			if (i == 9) {
				setcolor(12);
				gotoxy(x + i, y + 9);
				printf("★");
				Sleep(200);
				setcolor(15);
				gotoxy(x + 9, y + 9);
				printf("□");
				Sleep(200);
			}
			else {
				gotoxy(x + i, y + 9);
				printf("★");
				Sleep(200);
				gotoxy(x + i, y + 9);
				printf("□");
			}
		}
	}
	while (_kbhit()) _getch(); //버퍼에 기록된 키값을 버림 
}

void map(void)
{
	int i;

	gotoxy(4, 16); printf("▩▩");
	gotoxy(4, 17); printf("▩");
	gotoxy(4, 18); printf("▩");
	gotoxy(4, 19); printf("▩");
	for (i = 4; i < 55; i++)
	{
		gotoxy(i, 20); printf("▩");
	}
	gotoxy(50, 11); printf("■");
	gotoxy(49, 12); printf("■■■");
	gotoxy(48, 13); printf("■■■■■");
	gotoxy(48, 14); printf("■      ■");
	gotoxy(48, 15); printf("■      ■");
	gotoxy(46, 16); printf("■■■■■■■■■");
	gotoxy(46, 17); printf("■              ■");
	gotoxy(46, 18); printf("■              ■");
	gotoxy(46, 19); printf("■              ■");


	for (i = 4; i < 55; i++)
	{
		if (i == 4) { gotoxy(i, 1); printf("┏"); gotoxy(i, 7); printf("┗"); }
		else if (i == 54) { gotoxy(i, 1); printf("┓"); gotoxy(i, 7); printf("┛"); }
		else { gotoxy(i, 1); printf("━"); gotoxy(i, 7); printf("━"); }
	}
	for (i = 2; i < 7; i++) {
		gotoxy(4, i); printf("┃"); gotoxy(54, i); printf("┃");
	}
	gotoxy(5, 1); printf("[SHOP]"); setcolor(11);
	gotoxy(5, 3); printf("Weapon"); setcolor(5);
	gotoxy(22, 3); printf(" Magic"); setcolor(10);
	gotoxy(40, 3); printf("Tower"); setcolor(8);
	gotoxy(9, 3); printf("- <1> Power UP :");
	gotoxy(25, 3); printf(" - <2> Hell Fire :");
	gotoxy(43, 3); printf("- <3> Heal : 200Gold"); setcolor(14);
	gotoxy(12, 6); printf("Max power = 5  /  Hell Fire is all clear"); setcolor(15);

	for (i = 4; i < 55; i++)
	{
		if (i == 4) { gotoxy(i, 22); printf("┏"); gotoxy(i, 27); printf("┗"); }
		else if (i == 54) { gotoxy(i, 22); printf("┓"); gotoxy(i, 27); printf("┛"); }
		else { gotoxy(i, 22); printf("━"); gotoxy(i, 27); printf("━"); }
	}
	for (i = 23; i < 27; i++) {
		gotoxy(4, i); printf("┃"); gotoxy(54, i); printf("┃");
	}
	gotoxy(5, 22); printf("[STATUS]");

}

void reset_main(void) { //게임판을 초기화  
	int i, j;

	FILE *file = fopen("bestscorescore.dat", "rt"); // score.dat파일을 연결 
	if (file == 0) { status.Best_Score = 0; } //파일이 없으면 걍 최고점수에 0을 넣음 
	else {
		fscanf(file, "%d", &status.Best_Score); // 파일이 열리면 최고점수를 불러옴 
		fclose(file); //파일 닫음 
	}

	status.Score = 0;
	status.Gold = 0;
	status.Wave = 1;
	status.Weapon_level = 1;
	status.Hp = 4;

	for (i = My; i < My + 3; i++) { // 게임판을 0으로 초기화  
		for (j = Mx - 1; j<Max_Mx + 1; j++) {
			main_org[i][j] = 0;
			main_cpy[i][j] = 100;
		}
	}
}

void reset_main_cpy(void) { //main_cpy를 초기화 
	int i, j;

	for (i = My; i < My + 3; i++) {         //게임판에 게임에 사용되지 않는 숫자를 넣음 
		for (j = Mx; j<Max_Mx + 1; j++) {  //이는 main_org와 같은 숫자가 없게 하기 위함 
			main_cpy[i][j] = 100;
		}
	}
}

void check_key(void) { //키입력 함수
	key = 0; //키값 초기화
	int i, j;

	if (_kbhit()) { //키입력이 있는 경우  
		key = _getch(); //키값을 받음
		switch (key) {
		case SPACE:
			die_monster();
			break;
		case 49: //키입력1의 아스키코드값
			if (status.Weapon_level < 5) {
				if (status.Gold >= power) {
					status.Gold -= power;
					status.Weapon_level += 1;
					power += 500;
					setcolor(11);
					gotoxy(22, 12); printf("+------------------+");
					gotoxy(22, 13); printf("| Weapon Power Up!!|");
					gotoxy(22, 14); printf("+------------------+");
					setcolor(15);
				}
				else {
					setcolor(8);
					gotoxy(22, 12); printf("+---------------------+");
					gotoxy(22, 13); printf("|  You need a gold!!  |");
					gotoxy(22, 14); printf("+---------------------+");
					setcolor(15);
				}
			}
			else {
				setcolor(11);
				gotoxy(22, 12); printf("+------------------+");
				gotoxy(22, 13); printf("|    Max Level!!   |");
				gotoxy(22, 14); printf("+------------------+");
				setcolor(15);
			}
			Sleep(500);
			break;
		case 50: //키입력2의 아스키코드값
			if (status.Wave < 10) {
				if (status.Gold >= magic) {
					status.Gold -= magic;
					for (i = My; i < My + 3; i++) { // 맵 상의 모든 풍선들 제거
						for (j = Mx - 1; j < Max_Mx + 1; j++) {
							main_org[i][j] = 0;
						}
					}
					setcolor(12);
					gotoxy(22, 12); printf("+------------------+");
					gotoxy(22, 13); printf("|    Hell Fire!!   |");
					gotoxy(22, 14); printf("+------------------+");
					setcolor(15);
					magic += magic;
				}
				else {
					setcolor(8);
					gotoxy(22, 12); printf("+---------------------+");
					gotoxy(22, 13); printf("|  You need a gold!!  |");
					gotoxy(22, 14); printf("+---------------------+");
					setcolor(15);
				}
			}
			else {
				setcolor(8);
				gotoxy(22, 12); printf("+------------------+");
				gotoxy(22, 13); printf("|   It’s no use .  |");
				gotoxy(22, 14); printf("+------------------+");
				setcolor(15);
			}
			Sleep(500);
			break;
		case 51: //키입력3의 아스키코드값
			if (status.Hp < 4) {
				if (status.Gold >= 200) {
					status.Gold -= 200;
					status.Hp = 4;
					setcolor(14);
					gotoxy(22, 12); printf("+----------------------+");
					gotoxy(22, 13); printf("|     ♥♥HILL♥♥     |");
					gotoxy(22, 14); printf("+----------------------+");
					setcolor(15);
				}
				else {
					setcolor(8);
					gotoxy(22, 12); printf("+---------------------+");
					gotoxy(22, 13); printf("|  You need a gold!!  |");
					gotoxy(22, 14); printf("+---------------------+");
					setcolor(15);
				}
			}
			else {
				setcolor(14);
				gotoxy(22, 12); printf("+------------------+");
				gotoxy(22, 13); printf("|    HP is Full    |");
				gotoxy(22, 14); printf("+------------------+");
				setcolor(15);
			}
			Sleep(500);
			break;
		case 27: //ESC누르면 종료
			if (status.Score >= status.Best_Score) { //최고기록 갱신시 
				FILE* file = fopen("bestscore.dat", "wt"); //bestscore.dat에 점수 저장
				if (file == 0) { //파일 에러메세지  
					gotoxy(0, 0);
					printf("FILE ERROR: SYSTEM CANNOT WRITE BEST SCORE ON \"BESTSCORE.DAT\"");
				}
				else {
					fprintf(file, "%d", status.Best_Score);
					fclose(file);
				}
			}
			system("cls");
			gotoxy(22, 12); printf("+------------------+");
			gotoxy(22, 13); printf("|     Game Over    |");
			gotoxy(22, 14); printf("+------------------+\n\n\n");
			exit(0);

		}
	}
	while (_kbhit()) getch(); //키버퍼를 비움
}

void draw_main(void) { // 게임판 풍선그리기 함수
	int i, j;

	for (i = My; i < My + 3; i++) {
		for (j = Mx; j < Max_Mx + 1; j++) {

			if (main_cpy[i][j] != main_org[i][j]) { //cpy랑 비교해서 값이 달라진 부분만 새로 그려줌.
													//이게 없으면 게임판전체를 계속 그려서 느려지고 반짝거림

				if (main_org[i][j] <= EMPTY) {
					main_org[i][j] = EMPTY; //만약 0보다 작아지면 EMPTY로 초기화 후 그려줌.
					gotoxy(j, i); printf("  ");
				}
				else if (main_org[i][j] > EMPTY && main_org[i][j] <= MOP1) {
					gotoxy(j, i); printf("○");
				}
				else if (main_org[i][j] > MOP1 && main_org[i][j] <= MOP2) {
					gotoxy(j, i); setcolor(11); printf("●"); setcolor(15);
				}
				else if (main_org[i][j] > MOP2 && main_org[i][j] <= MOP3) {
					gotoxy(j, i); setcolor(10); printf("●"); setcolor(15);
				}
				else if (main_org[i][j] > MOP3 && main_org[i][j] <= MOP4) {
					gotoxy(j, i); setcolor(14);; printf("●"); setcolor(15);
				}
				else if (main_org[i][j] > MOP4 && main_org[i][j] <= MOP5) {
					gotoxy(j, i); setcolor(12);; printf("☆"); setcolor(15);
				}
				else if (main_org[i][j] > MOP5 && main_org[i][j] <= MOP6) {
					gotoxy(j, i); setcolor(11);; printf("★"); setcolor(15);
				}
				else if (main_org[i][j] > MOP6 && main_org[i][j] <= MOP7) {
					gotoxy(j, i); setcolor(10);; printf("★"); setcolor(15);
				}
				else if (main_org[i][j] > MOP7 && main_org[i][j] <= MOP8) {
					gotoxy(j, i); setcolor(14);; printf("★"); setcolor(15);
				}
				else if (main_org[i][j] > MOP8 && main_org[i][j] <= MOP9) {
					gotoxy(j, i); setcolor(13);; printf("◈"); setcolor(15);
				}
				else if (main_org[i][j] == BOSS_1) {
					gotoxy(j, i); setcolor(7);; printf("●"); setcolor(15);
				}
				else if (main_org[i][j] == BOSS_2) {
					gotoxy(j, i); setcolor(7);; printf("▽"); setcolor(15);
				}
				else if (main_org[i][j] > MOP9 && main_org[i][j] <= BOSS_3) {
					gotoxy(j, i); setcolor(7);; printf("●"); setcolor(15);
				}
			}
		}
	}
	for (i = My; i<My + 3; i++) { //게임판을 그린 후 main_cpy에 복사  
		for (j = Mx; j<Max_Mx; j++) {
			main_cpy[i][j] = main_org[i][j];
		}
	}
}

void show_monster(void) { //몬스터 등장 함수
	int i;

	switch (status.Wave) {
	case 1:
		main_org[My + 2][Mx - 1] = MOP1;
		break;
	case 2:
		main_org[My + 2][Mx - 1] = MOP2;
		break;
	case 3:
		main_org[My + 2][Mx - 1] = MOP3;
		break;
	case 4:
		main_org[My + 2][Mx - 1] = MOP4;
		break;
	case 5:
		main_org[My + 2][Mx - 1] = MOP5;
		break;
	case 6:
		main_org[My + 2][Mx - 1] = MOP6;
		break;
	case 7:
		main_org[My + 2][Mx - 1] = MOP7;
		break;
	case 8:
		main_org[My + 2][Mx - 1] = MOP8;
		break;
	case 9:
		main_org[My + 2][Mx - 1] = MOP9;
		break;
	case 10:
		main_org[My][Mx - 1] = BOSS_1;
		main_org[My + 1][Mx - 1] = BOSS_2;
		main_org[My + 2][Mx - 1] = BOSS_3;
		break;
	}

}

void move_monster(void) { //몬스터 이동 함수
	int i, j;
	int cnt;

	switch (status.Wave) {
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		for (i = Mx - 1; i < Max_Mx + 1; i++) {
			if (main_org[My + 2][i] != 0) {
				cnt = main_org[My + 2][i];
				main_org[My + 2][i] = main_org[My + 2][i + 1];
				main_org[My + 2][i + 1] = cnt;
				i++;
			}
		} break;
	case 10:
		for (j = My; j < My + 3; j++) {
			for (i = Mx - 1; i < Max_Mx + 1; i++) {
				if (main_org[j][i] != 0) {
					cnt = main_org[j][i];
					main_org[j][i] = main_org[My + 2][i + 1];
					main_org[j][i + 1] = cnt;
					i++;
				}

			}
		}
		for (i = Mx + 1; i < Max_Mx + 1; i++) {
			if (main_org[My + 2][i] != 0) {
				if (main_org[My + 2][i] <= 27) {
					for (j = My; j < My + 2; j++) {
						for (i = Mx - 1; i < Max_Mx + 1; i++) {
							main_org[j][i] = EMPTY;
						}
					}
				}
			}
		}
		break;
	}
}

void die_monster(void) { //몬스터 죽음 처리 함수
	int i;

	switch (status.Weapon_level) {
	case WEAPON1:
		for (i = Max_Mx; i > Mx; i--) {
			if (main_org[My + 2][i] != 0) {
				main_org[My + 2][i] -= WEAPON1;
				status.Score += 10;
				status.Best_Score = status.Score;
				status.Gold += 5;
				break;
			}
		} break;
	case WEAPON2:
		for (i = Max_Mx; i > Mx; i--) {
			if (main_org[My + 2][i] != 0) {
				main_org[My + 2][i] -= WEAPON2;
				status.Score += 20;
				status.Best_Score = status.Score;
				status.Gold += 10;
				break;
			}
		} break;
	case WEAPON3:
		for (i = Max_Mx; i > Mx; i--) {
			if (main_org[My + 2][i] != 0) {
				main_org[My + 2][i] -= WEAPON3;
				status.Score += 30;
				status.Best_Score = status.Score;
				status.Gold += 15;
				break;
			}
		} break;
	case WEAPON4:
		for (i = Max_Mx; i > Mx; i--) {
			if (main_org[My + 2][i] != 0) {
				main_org[My + 2][i] -= WEAPON4;
				status.Score += 40;
				status.Best_Score = status.Score;
				status.Gold += 20;
				break;
			}
		} break;
	case WEAPON5:
		for (i = Max_Mx; i > Mx; i--) {
			if (main_org[My + 2][i] != 0) {
				main_org[My + 2][i] -= WEAPON5;
				status.Score += 50;
				status.Best_Score = status.Score;
				status.Gold += 25;
				break;
			}
		} break;
	}
}

void damage_tower(void)
{ //타워 데미지 처리
	if (main_org[My + 2][45] != 0)
	{
		main_org[My + 2][45] = EMPTY;
		status.Hp -= 1;
	}
}

void game_over(void) {
	if (status.Hp == 0) {
		system("cls");
		gotoxy(22, 12); printf("+------------------+");
		gotoxy(22, 13); printf("|     Game Over    |");
		gotoxy(22, 14); printf("+------------------+\n\n\n");
		exit(0);
	}

}

void status_draw(void) { //상태창 표시
	gotoxy(17, 3); printf(" %dGold", power);
	gotoxy(34, 3); printf(" %dGold", magic);
	gotoxy(5, 23); setcolor(8); printf("Wave = %d", status.Wave);
	gotoxy(5, 24); printf("Comming ballons..'  '");
	gotoxy(14, 24);
	switch (status.Wave) {
	case 1:
		setcolor(15); printf("○"); break;
	case 2:
		setcolor(11); printf("●"); break;
	case 3:
		setcolor(10); printf("●"); break;
	case 4:
		setcolor(14); printf("●"); break;
	case 5:
		setcolor(12); printf("☆"); break;
	case 6:
		setcolor(11); printf("★"); break;
	case 7:
		setcolor(10); printf("★"); break;
	case 8:
		setcolor(14); printf("★"); break;
	case 9:
		setcolor(13); printf("◈"); break;
	case 10:
		setcolor(7);  printf("BOSS!!"); break;
	}

	setcolor(8); gotoxy(22, 23); printf("Weapon level = %d", status.Weapon_level);
	gotoxy(22, 24); printf("Tower HP");
	gotoxy(27, 24);
	switch (status.Hp) {
	case 1:
		setcolor(12);  printf("♥      ");   break;
	case 2:
		setcolor(12);  printf("♥♥    "); break;
	case 3:
		setcolor(12);  printf("♥♥♥  "); break;
	case 4:
		setcolor(12);  printf("♥♥♥♥"); break;
	}

	setcolor(8); gotoxy(22, 25); printf("Gold =", status.Gold);
	setcolor(14); gotoxy(26, 25); printf("%5dG", status.Gold);
	setcolor(13); gotoxy(41, 23); printf("Best Score = %5d", status.Best_Score);
	setcolor(8); gotoxy(41, 24); printf("Score = %5d", status.Score); setcolor(15);
}

void wave_up(void) { //다음 웨이브
	if (t == 60) {
		status.Wave = 2;
		setcolor(3);
		gotoxy(22, 12); printf("+-----------------+");
		gotoxy(22, 13); printf("|  Next Wave = 2  |");
		gotoxy(22, 14); printf("+-----------------+");
		setcolor(15);
		Sleep(1000);
	}
	else if (t == 120) {
		status.Wave = 3;
		setcolor(3);
		gotoxy(22, 12); printf("+-----------------+");
		gotoxy(22, 13); printf("|  Next Wave = 3  |");
		gotoxy(22, 14); printf("+-----------------+");
		setcolor(15);
		Sleep(1000);
	}
	else if (t == 180) {
		status.Wave = 4;
		setcolor(3);
		gotoxy(22, 12); printf("+-----------------+");
		gotoxy(22, 13); printf("|  Next Wave = 4  |");
		gotoxy(22, 14); printf("+-----------------+");
		setcolor(15);
		Sleep(1000);
	}
	else if (t == 240) {
		status.Wave = 5;
		setcolor(3);
		gotoxy(22, 12); printf("+-----------------+");
		gotoxy(22, 13); printf("|  Next Wave = 5  |");
		gotoxy(22, 14); printf("+-----------------+");
		setcolor(15);
		Sleep(1000);
	}
	else if (t == 300) {
		status.Wave = 6;
		setcolor(3);
		gotoxy(22, 12); printf("+-----------------+");
		gotoxy(22, 13); printf("|  Next Wave = 6  |");
		gotoxy(22, 14); printf("+-----------------+");
		setcolor(15);
		Sleep(1000);
	}
	else if (t == 360) {
		status.Wave = 7;
		setcolor(3);
		gotoxy(22, 12); printf("+-----------------+");
		gotoxy(22, 13); printf("|  Next Wave = 7  |");
		gotoxy(22, 14); printf("+-----------------+");
		setcolor(15);
		Sleep(1000);
	}
	else if (t == 420) {
		status.Wave = 8;
		setcolor(3);
		gotoxy(22, 12); printf("+-----------------+");
		gotoxy(22, 13); printf("|  Next Wave = 8  |");
		gotoxy(22, 14); printf("+-----------------+");
		setcolor(15);
		Sleep(1000);
	}
	else if (t == 480) {
		status.Wave = 9;
		setcolor(3);
		gotoxy(22, 12); printf("+-----------------+");
		gotoxy(22, 13); printf("|  Next Wave = 9  |");
		gotoxy(22, 14); printf("+-----------------+");
		setcolor(15);
		Sleep(1000);
	}
	else if (t == 530) {
		status.Wave = 10;
		zen = 1000;
		setcolor(3);
		gotoxy(22, 12); printf("+------------------+");
		gotoxy(22, 13); printf("|  Last Balloons!  |");
		gotoxy(22, 14); printf("+------------------+");
		setcolor(15);
		Sleep(1000);
	}
	else if (t == 578) {
		system("cls");
		setcolor(12);
		gotoxy(22, 12); printf("+----------------+");
		gotoxy(22, 13); printf("|   Game Clear   |");
		gotoxy(22, 14); printf("+----------------+\n\n\n");
		setcolor(15);
		exit(0);
	}
}

void board_clean(void) { //상황판 지우기
	gotoxy(22, 12); printf("                                ");
	gotoxy(22, 13); printf("                                ");
	gotoxy(22, 14); printf("                                ");
}


