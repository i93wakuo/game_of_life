#include <curses.h>
#include <stdlib.h>

#include "game_map.h"
#define MAX_HEIGHT	38
#define MAX_WIDTH	211	

int main(int argc, char **argv)
{
	char map[MAX_HEIGHT][MAX_WIDTH];
	char next[MAX_HEIGHT][MAX_WIDTH];
	int scr_height, scr_width;
	int map_height, map_width;
	int sell;
	int i, j, h;

	/*cursesの設定*/
	initscr();	/*端末制御を開始*/
	curs_set(0);	/*カーソルを非表示*/
	cbreak();	/*入力にreturnキーを必要としない*/
	noecho();	/*入力された文字を非表示*/
	timeout(-1);	/*キー入力を待つ*/

	/*スクリーンのサイズを取得*/
	getmaxyx(stdscr, i, j);
	if (i < MAX_HEIGHT || j < MAX_WIDTH) {
		move(0, 0);
		printw("screen size needs %dx%d\n", MAX_HEIGHT, MAX_WIDTH);
		printw("Please press any key...\n");
		getch();
		endwin();
		return 1;
	}

	/*マップの大きさを取得*/
	if (game_map_width > MAX_WIDTH || game_map_height > MAX_HEIGHT) {	/*マップの大きさが制限を超えている*/
		printw("Sorry... game_map is too big.\n");
		printw("Please press any key...\n");
		getch();
		endwin();
		return 1;
	}
	map_width = game_map_width / 8;
	if (game_map_width % 8)
		map_width++;
	map_height = game_map_height;

	/*mapの設定*/
	for (i = 0; i < MAX_HEIGHT; i++)
		for (j = 0; j < MAX_WIDTH; j++)
			map[i][j] = next[i][j] = 0;

	/*game_mapの読み込み*/
	for (i = 0; i < map_height; i++) {
		for (j = 0; j < map_width; j++) {
			for (h = 0; h < 8; h++) {
				if (game_map_bits[i*map_width+j] & 0x01)
					map[i][j*8+h] = 1;
				else
					map[i][j*8+h] = 0;
				game_map_bits[i*map_width+j] >>= 1;	/*次のビットを選択*/
			}
		}
	}

	while (1) {
		/*mapの表示*/
		clear();
		for (i = 0; i < MAX_HEIGHT; i++)
			for (j = 0; j < MAX_WIDTH; j++) {
				move(i, j);	/*カーソルを移動*/
				if (map[i][j])
					printw("O");
				else
					printw(" ");
			}

		/*細胞の生死*/
		for (i = 0; i < MAX_HEIGHT; i++) {
			for (j = 0;j < MAX_WIDTH; j++) {
				/*周りの細胞の数を数える*/
				sell = 0;
				int height, width;
				height = i;
				width = j;	/*細胞の位置*/
				int up, down, left, right;	/*計測用*/
				up = down = left = right = 1;

				/*高さに関する制限*/
				if (height == 0)
					up = 0;
				if (height == MAX_HEIGHT-1)
					down = 0;
				/*横幅に関する制限*/
				if (width == 0)
					left = 0;
				if (width == MAX_WIDTH-1)
					right = 0;

				if (up) {
					sell += map[height-1][width];
					if (left)
						sell += map[height-1][width-1];
					if (right)
						sell += map[height-1][width+1];
				}
				if (down) {
					sell += map[height+1][width];
					if (left)
						sell += map[height+1][width-1];
					if (right)
						sell += map[height+1][width+1];
				}
				if (left)
					sell += map[height][width-1];
				if (right)
					sell += map[height][width+1];

				/*細胞が生きているかの確認*/
				if (map[i][j]) {	/*細胞が生きている*/
					if (sell <= 1)	/*過疎状態*/
						next[i][j] = 0;
					else if (sell == 2 || sell == 3)	/*生存状態*/
						next[i][j] = 1;
					else if (sell >= 4)	/*過密状態*/
						next[i][j] = 0;
				} else if (sell == 3)	/*誕生*/
					next[i][j] = 1;
				else	/*細胞は死んでいるし、誕生もしない*/
					next[i][j] = 0;
			}
		}

		/*nextの中身をmapにコピー*/
		for (i = 0; i < MAX_HEIGHT; i++)
			for (j = 0; j < MAX_WIDTH; j++)
				map[i][j] = next[i][j];

		/*キー入力*/
		i = getch();	/*キー入力を待つ*/
		if (i == 'q')
			break;


	}
	endwin();	/*端末制御を終了*/
	return 0;
}

