/* algo.h
 *  カードゲーム ALGO 
 */

#ifndef ALGO
#define ALGO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// カード枚数
#define CARDS 24

// カード
typedef struct {
    char color; // 黒(B) or 白(W)
    int number; // 0 <= number <= 11
} card_t;

/* ALGO のルールを説明する
 * @param mode: ゲームモード
 */
void show_rule (char *mode) {
    FILE *rule_file;
    char line[1024];

    // 基本ルール
    if ((rule_file = fopen("./Rule_book/basic_rule.txt", "r")) == NULL) {
        fprintf(stderr, "基本ルールのファイルが見つかりません.\n");
        sleep(2);
        return;
    }
    system("clear");
    while (fgets(line, sizeof(line), rule_file) != NULL) {
        printf("%s", line);
    }
    fclose(rule_file);
    printf(">> next [Enter]");
    getchar();

    // 1 人プレイのルール
    if (!strcmp(mode, "solo")) {
        if ((rule_file = fopen("./Rule_book/solo_rule.txt", "r")) == NULL) {
            fprintf(stderr, "1 人プレイルールのファイルが見つかりません.\n");
            sleep(2);
            return;
        }
        system("clear");
        while (fgets(line, sizeof(line), rule_file) != NULL) {
            printf("%s", line);
        }
        fclose(rule_file);
        printf(">> next [Enter]");
        getchar();

        if ((rule_file = fopen("./Rule_book/solo_inputEX.txt", "r")) == NULL) {
            fprintf(stderr, "1 人プレイルールの入力例のファイルが見つかりません.\n");
            sleep(2);
            return;
        }
        system("clear");
        while (fgets(line, sizeof(line), rule_file) != NULL) {
            printf("%s", line);
        }
        fclose(rule_file);
        printf(">> end [Enter]");
        getchar();
    }

    return;
}

/* カードをシャッフルする
 * @param cards: シャッフルした後の山札の格納先
 */
void shuffle(card_t *cards) {
    card_t tmp[CARDS];     // ここからランダムに選択して山札とする
    int remaining = CARDS; // tmp の残り枚数
    int choice;
    int i, j;

    for (i = 0; i < (CARDS / 2); i++) {
        tmp[i].color  = 'B';
        tmp[i].number = i;
        tmp[i+12].color  = 'W';
        tmp[i+12].number = i;
    }

    // シードの設定
    srand((unsigned)time(NULL));
    // カードをシャッフル
    for (i = 0; i < CARDS; i++) {
        // tmp の中からランダムに 1 枚選択
        choice = rand() % remaining;
        (cards+i)->color  = tmp[choice].color;
        (cards+i)->number = tmp[choice].number;
        // 選んだカードを tmp から除去
        for (j = choice; j < CARDS - 1; j++) {
            tmp[j].color  = tmp[j+1].color;
            tmp[j].number = tmp[j+1].number;
        }
        remaining--;
    }
    return;
}

/* 場のカードを表示する
 * @param x    : 場の横幅
 * @param y    : 場の縦幅
 * @param field: 場のカード
 */
void printCards(int x, int y, card_t field[y][x]) {
    int has_card; // その段がカードを持つか
    int i, j;

    system("clear");
    for (i = 0; i < y; i++) {
        has_card = 0;
        for (j = 0; j < x; j++) {
            if ( field[i][j].color != '\0' ) {
                if (has_card == 0)  has_card = 1;
                if (field[i][j].number < 10) {
                    printf("  %c%d", field[i][j].color, field[i][j].number);
                } else {
                    printf(" %c%2d", field[i][j].color, field[i][j].number);
                }
            } else {
                break;
            }
        }
        if (has_card) {
            printf("\n");
        }
    }
    return;
}

/* 場のカードをより実物的に表示する
 *  カード色をそのまま表現
 *  黒: 背景「黒」, 数字「白」
 *  白: 背景「白」, 数字「黒」
 * @param x    : 場の横幅
 * @param y    : 場の縦幅
 * @param field: 場のカード
 */
void printCardsColor(int x, int y, card_t field[y][x]) {
    int has_card; // その段がカードを持つか
    int i, j;

    system("clear");
    for (i = 0; i < y; i++) {
        has_card = 0;
        for (j = 0; j < x; j++) {
            if ( field[i][j].color != '\0' ) {
                if (has_card == 0) {
                    has_card = 1;
                    printf("%d:", i + 1); // 段数の表示
                }
                printf(" ");
                (field[i][j].color == 'B') ? printf("\x1b[40m\x1b[37m") : printf("\x1b[47m\x1b[30m") ;
                if (field[i][j].number < 10) {
                    printf(" ");
                }
                printf("%d\x1b[49m\x1b[39m", field[i][j].number);
            } else {
                break;
            }
        }
        if (has_card) {
            printf("\n");
        }
    }
    return;
}

/* 2 枚のカードの大小関係を確認
 *  基準となるカードと, それと比較するカードを受け取り,
 *  その並びが基本ルールに則しているか (card < pair となっているか) を確認する.
 * @param card: 基準のカード
 * @param pair: 比較対象のカード
 * @return 0: card < pair, 1: card > pair
 */
int checkOrder (card_t card, card_t pair) {
    int result;
    if (card.number < pair.number) {
        result = 0;
    } else if (card.number == pair.number && card.color == 'B') {
        result = 0;
    } else {
        result = 1;
    }
    return result;
}

/* 場にカードを置く
 * @param cards  : 置くカード
 * @param x      : 場の横幅
 * @param y      : 場の縦幅
 * @param field  : 場のカード
 * @param put_pos: カードを置く位置
 *                 put_pos[0]: 場の何段目に置くか
 *                 put_pos[1]: 左から何番目に置くか
 * @return カードの並びが基本ルールに則しているか
 *         0: 則している, 1: 則していない, -1: カード位置の指定が無効
 */
int putCard(card_t card, int x, int y, card_t field[y][x], int put_pos[]) {
    int result = 0;
    int col = -1;
    int y_pos = put_pos[0] - 1; // カードが何段目に挿入されるか
    int x_pos = put_pos[1] - 1; // カードが左から何番目に挿入されるか
    int add_pos; // カードの挿入された位置
                 // -1: 左端, 0: 間 (左右に別のカードがある), 1: 右端
    int i, j;

    for (i = 0; i < x; i++) {
        if (x_pos < 0) {
            // カードを左端より更に左に置こうとした場合は無効
            return -1;
        } else if (field[y_pos][i].color == 'B' || field[y_pos][i].color == 'W') {
            col++;
        } else if (field[y_pos][i].color == '\0' && col <= x_pos - 2) {
            // 右端より更に右にカードを置こうとした場合は無効
            //   e.g.「W3」 の場合に, 左から 3 番目に置こうとした
            return -1;
        } else if (field[y_pos][i].color == '\0' && col == x_pos - 1) {
            // 右端にカードを置く場合
            field[y_pos][i].color  = card.color;
            field[y_pos][i].number = card.number;
            add_pos = 1;
            break;
        }

        if (col == x_pos) {
            add_pos = (x_pos == 0) ? -1 : 0 ;
            // カード挿入位置より右のカードを全て 1 つ右にずらす
            for (j = x-1; j > x_pos; j--) {
                field[y_pos][j].color  = field[y_pos][j-1].color;
                field[y_pos][j].number = field[y_pos][j-1].number;
            }
            // x 番目にカードを挿入
            field[y_pos][x_pos].color  = card.color;
            field[y_pos][x_pos].number = card.number;
            break;
        }
    }

    // 予測結果の判定
    switch (add_pos) {
    case -1:
        // 左端に置いた場合
        result = checkOrder(field[y_pos][x_pos], field[y_pos][x_pos+1]);
        break;
    case 0:
        // ある 2 枚のカードの間に置いた場合
        if (checkOrder(field[y_pos][x_pos-1], field[y_pos][x_pos]) == 0
                && checkOrder(field[y_pos][x_pos], field[y_pos][x_pos+1]) == 0) {
            result = 0;
        } else {
            result = 1;
        }
        break;
    case 1:
        // 右端に置いた場合
        result = checkOrder(field[y_pos][x_pos-1], field[y_pos][x_pos]);
        break;
    default:
        fprintf(stderr, "想定外の分岐\n");
        break;
    }

    return result;
}

#endif
