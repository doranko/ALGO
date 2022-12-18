/* main.c
 *  ALGO で遊ぶ
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algo.h"
#include "solo_play.h"

// 場の最大の大きさ
#define MAX_Y 8
#define MAX_X 4
// キーボード入力の上限文字数
#define IN_MAX 10
// ゲームモード
#define SOLO_E 1
#define SOLO_H 2
#define RULE   3
#define EXIT   4

/* selectMode
 *  ゲームモードの選択
 *  @return ゲームモード
 */
int selectMode () {
    char input[IN_MAX];
    char str[1024];
    int select; // ゲームモード
    char *newline_code;

    do {
        system("clear");
        printf("\nALGO ~ 1 人プレイ ~\n");
        printf("  %d: 1 人プレイ (EASY)\n", SOLO_E);
        printf("  %d: 1 人プレイ (HARD)\n", SOLO_H);
        printf("  %d: ルール説明 (1 人プレイ)\n", RULE);
        printf("  %d: EXIT      \n\n", EXIT);
        printf("select number and push Enter -> ");

        fgets(input, sizeof(input), stdin);
        newline_code = strchr(input, '\n');
        if (newline_code) {
            // 改行文字の削除
            *newline_code = '\0';
        } else {
            // 標準入力のクリア
            while (getchar() != '\n');
        }

        sscanf(input, "%d%s", &select, str);
        if (strcmp(str, "") != 0)  continue;

        if (select == RULE)  show_rule("solo");
        if (select == EXIT)  exit(0);
    } while (select != SOLO_E && select != SOLO_H);

    return select;
}

/* toTitle
 *  タイトルに戻るか終了するかを選択
 *  @return 1: タイトルに戻る
 *          0: 終了
 */
int toTitle () {
    char input[IN_MAX];
    const int TO_TITLE = 1;
    const int END_GAME = 0;

    printf("\nタイトルに戻りますか ?\n");
    printf("  y: タイトルに戻る\n");
    printf("  n: 終了\n");
    while (1) {
        printf("select y/n and push Enter -> ");
        fgets(input, sizeof(input), stdin);
        // 標準入力をクリア
        if (strchr(input, '\n') == NULL) {
            while (getchar() != '\n');
        }

        if (!strcmp(input, "y\n"))
            return TO_TITLE;
        else if (!strcmp(input, "n\n"))
            return END_GAME;
    }
}

int main(int argc, char* argv[]) {
    int select;         // ゲームモードの選択
    char input[IN_MAX]; // キーボード入力
    char onemore; // ゲーム終了後, タイトルに戻るかどうか
    int i, j, game_over = 0;

    while (1) {
        // ゲームモード選択
        select = selectMode();

        // ゲームの呼び出し
        if (select == 1 || select == 2) {
            game_over = soloPlay(select);
        }

        // 結果の表示
        if (game_over) {
            printf("! GAME OVER !\n");
        } else {
            printf("! GAME CLEAR !\n");
        }

        // 再度遊ぶかどうか
        if (!toTitle())  break;
    }

    return 0;
}
