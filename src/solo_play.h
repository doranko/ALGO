/* solo_play.h
 *  ALGO の 1 人プレイルール
 *  1. シャッフルした後, 4 枚のカードを縦に並べる (7 並べの形)
 *  2. 山札から 1 枚, 裏向きで引く
 *  3. ALGO の基本ルールに乗っ取ったならびになるように予測して置く
 *      * 現状は最も右か, 最も左にしか置けない
 *      * 本来なら間にも置けたはず...?
 *  4. カードを表向きにする
 *  4.1. 基本ルール通り -> 2. に戻る
 *  4.2. 基本ルールに反する -> ゲームオーバー
 */

#ifndef SOLO_PLAY
#define SOLO_PLAY

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algo.h"

// 場の最大の大きさ
#define MAX_Y 8
#define MAX_X 4
// キーボード入力の上限文字数
#define IN_MAX 10
// ゲームをクリアしたかどうか
#define GAME_OVER 1
#define GAME_CLEAR 0
// ゲームモード
#define EASY 1
#define HARD 2

int soloPlay(int game_mode) {
    card_t cards[CARDS];
    card_t field[MAX_Y][MAX_X]; // 場に並んでいるカード
    int cols[MAX_Y];            // 場の各行のカードの枚数
    int field_X, field_Y;       // ゲームモードに則した場の大きさ
    int pick = 0;     // 次に cards から引くカード
    int put_card[2];  // カードを何段目の左から何番目に置くか
    int put_result;   // カードを置いた結果
    char input[IN_MAX]; // キーボード入力
    int i, j, game_over = 0;

    if (game_mode == EASY) {
        field_X = 3;
        field_Y = 8;
    } else if (game_mode == HARD) {
        field_X = 4;
        field_Y = 6;
    } else {
        fprintf(stderr, "ゲームの設定に失敗\n");
        return 1;
    }

    // 場の初期化とカードのシャッフル
    for (i = 0; i < MAX_Y; i++) {
        for (j = 0; j < MAX_X; j++) {
            field[i][j].color  = '\0';
            field[i][j].number = '\0';
        }
    }
    shuffle(cards);

    // 初期配置
    for (pick = 0; pick < field_Y; pick++) {
        field[pick][0].color  = cards[pick].color;
        field[pick][0].number = cards[pick].number;
        cols[pick] = 1;
    }

    // ゲームスタート
    for (pick = pick; pick < CARDS; pick++) {
        //printCards(MAX_X, MAX_Y, field);
        printCardsColor(MAX_X, MAX_Y, field);
        printf("\n");
        if (cards[pick].color == 'B') {
            printf("次のカードの色は「黒」です.\n");
        } else {
            printf("次のカードの色は「白」です.\n");
        }
        printf("何段目の左から何番目に置きますか?\n");
        printf("  e.g. 1 2 -> 1 段目の左から 2 番目\n");
        while (1) {
            printf("-> ");

            // put_card を初期化
            put_card[0] = -1;
            put_card[1] = -1;

            fgets(input, sizeof(input), stdin);
            sscanf(input, "%d%d", &put_card[0], &put_card[1]);
            // 標準入力をクリア
            if (strchr(input, '\n') == NULL) {
                while (getchar() != '\n');
            }

            if (1 <= put_card[0] && put_card[0] <= field_Y && 1 <= put_card[1] && put_card[1] <= field_X && cols[put_card[0]-1] < field_X) {
                put_result = putCard(cards[pick], MAX_X, MAX_Y, field, put_card);
                if (put_result  == -1) {
                    printf(" ! 不正な入力です !\n");
                } else {
                    cols[put_card[0]-1]++;
                    break;
                }
            } else {
                printf(" ! 不正な入力です !\n");
            }
        }
        if (put_result == 1) {
            game_over = 1;
            break;
        }
    }
    //printCards(MAX_X, MAX_Y, field);
    printCardsColor(MAX_X, MAX_Y, field);
    printf("\n");

    return (game_over) ? GAME_OVER : GAME_CLEAR ;
}

#endif
