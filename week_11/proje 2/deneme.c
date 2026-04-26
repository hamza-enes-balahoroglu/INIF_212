#include <stdio.h>
#include <stdlib.h>

#define LEN(obj) (sizeof(obj) / sizeof(obj[0]))

#define GAMER_1_CHAR 'X'
#define GAMER_2_CHAR 'O'
#define TEMP_CHAR ' '

#define GAME_BOARD_SIZE 8 // Only even numbers

#define MAX_JUMPS (GAME_BOARD_SIZE / 2) * 3

/*
   CMD (General Command) Status.
*/
typedef enum
{
    CMD_ERROR,           // Command is not complated correctly.
    CMD_OK,              // Command Succesed.
    CMD_INVALID_COMMAND, // Command is invalid.
    CMD_CLOSE,           // Close the game.
} CMD_Status;

typedef enum
{
    G_ERROR,
    G_OK,
    G_Finsh,
} Game_Status;

int CMD_Get(char *cmd);
int CMD_Find_Char_Index(char *char_list, int count, char target);
int CMD_Help(char **destraptions, char *char_list, int count);
int GCMD_NewGame(char **destraptions, char *char_list, int count);
int GCMD_Quit(char **destraptions, char *char_list, int count);
void Print_Logo();
int **Creat_GameBoard();
void Kill_GameBoard(int **game_board, int count);
int Fill_GameBoard(int **game_board, int count);
int Print_GameBoard(char **destraptions, char *char_list, int cmd_count, int **game_board, int count, int *current_gamer);
void Print_Line(int count);
void Print_ABC(int count);
int Game_Finish(char **destraptions, char *char_list, int cmd_count, int **game_board, int count, int *current_gamer);
int Game_Help(char **destraptions, char *char_list, int cmd_count, int **game_board, int board_size, int *current_gamer);
int Gamer_Play(char **destraptions, char *char_list, int cmd_count, int **game_board, int board_size, int *current_gamer);
int Number_Get(int *number);
void Chance_Gamer(int *current_gamer);
int Game_ShowScore(int **game_board, int board_size);
int Calculate_TotalPions(int board_size);
void Game_Over(int winner);
void Find_Capture_Paths(int **game_board, int board_size, int current_gamer,
                        int start_r, int start_c, int current_r, int current_c,
                        int captured_count, int *temp_target_r, int *temp_target_c,
                        int *path_count, int *attackers_r, int *attackers_c,
                        int *dest_r, int *dest_c, /* YENI: Varis noktasi dizileri */
                        int target_r[][MAX_JUMPS], int target_c[][MAX_JUMPS], int *target_count);
int Capture_Piece(int **game_board, int board_size, int *current_gamer);

/*
 * General command funcions type
 *  Paramaters :
 *     char *destraptions ->  command destraption (ex: "Close The Program.").
 *     char charlist      ->  command characters list (ex: {'h', 'q'}).
 *     int count          ->  length of charlist and destraptions.
 */
typedef int (*CMD_General)(char **destraptions, char *char_list, int count);

typedef int (*CMD_Game)(char **destraptions, char *char_list, int cmd_count, int **game_board, int count, int *current_gamer);

int main(void)
{
    char *gcmd_destraption[] = {"Yardim menusu.", "Oyunu baslat.", "Programi kapat."};
    char gcmd_char[] = {'h', 'S', 'q'};
    CMD_General gcmd_functions[] = {CMD_Help, GCMD_NewGame, GCMD_Quit};

    CMD_Status status = CMD_OK;

    Print_Logo();

    printf("Dama Oyununa Hosgeldiniz. Yardim icin 'h' komutunu kullanin.\n");

    while (status != CMD_CLOSE)
    {
        char gcmd_input;
        if (CMD_Get(&gcmd_input) == CMD_OK)
        {
            int index = CMD_Find_Char_Index(gcmd_char, LEN(gcmd_char), gcmd_input);

            if (index != -1)
            {
                status = (CMD_Status)gcmd_functions[index](gcmd_destraption, gcmd_char, LEN(gcmd_char));
            }
            else
            {
                printf("Komut bulunamadi. Yardim icin : 'h'.\n");
            }
        }
    }
    return 0;
}

/*
 *  CMD_GET : Command get.
 *   The function gets input for user. The command may general or game command
 *   it does not matter.
 *
 *     Paramerters :
 *          char *cmd  -> pointer of command entered by user.
 *     Return:
 *          CMD_OK     -> oparation succed.
 *          CMD_ERROR  -> oparation failed.
 */
int CMD_Get(char *cmd)
{
    if (cmd == NULL)
    {
        return CMD_ERROR;
    }

    printf("\n=>  ");

    int input = getchar();
    if (input == EOF)
    {
        return CMD_ERROR;
    }

    *cmd = (char)input;

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    return CMD_OK;
}

/*
 *  CMD_Find_Char_Index : Command find char index
 *    The function finds that the charakter refers which command.
 *
 *    Parameters :
 *       char *char_list     -> command chars list.
 *       int count   -> size of command list.
 *       char target -> commant charakter that you looking for.
 *
 *    Return :
 *       if target find : target index  -> int
 *       else : -1 -> int
 */
int CMD_Find_Char_Index(char *char_list, int count, char target)
{
    for (int i = 0; i < count; i++)
    {
        if (char_list[i] == target)
        {
            return i;
        }
    }
    return -1;
}

int CMD_Help(char **destraptions, char *char_list, int count)
{
    printf("Yardim Menusu :\n");

    for (int i = 0; i < count; i++)
    {
        printf("\t%c  :  %s\n", char_list[i], destraptions[i]);
    }

    return CMD_OK;
}

int GCMD_NewGame(char **destraptions, char *char_list, int count)
{
    printf("Yeni Oyun Baslatilyor . . .\n");

    char *game_destraptions[] = {"Yardim menusu.", "Oyna.", "Oyun Tahtasisi goster.", "Oyunu bitir."};
    char game_char[] = {'h', 'p', 'b', 'q'};
    CMD_Game game_functions[] = {Game_Help, Gamer_Play, Print_GameBoard, Game_Finish};

    int **game_board = NULL;

    game_board = Creat_GameBoard();

    if (game_board == NULL)
    {
        return CMD_ERROR;
    }

    Game_Status status = G_OK;
    int current_gamer = 1;

    Fill_GameBoard(game_board, GAME_BOARD_SIZE);

    Print_GameBoard(game_destraptions, game_char, LEN(game_char), game_board, GAME_BOARD_SIZE, &current_gamer);

    while (status != G_Finsh)
    {
        char input;
        if (CMD_Get(&input) == CMD_OK)
        {
            int index = CMD_Find_Char_Index(game_char, LEN(game_char), input);

            if (index != -1)
            {
                status = (Game_Status)game_functions[index](game_destraptions, game_char, LEN(game_char), game_board,
                                                            GAME_BOARD_SIZE, &current_gamer);
            }
            else
            {
                printf("Komut bulunamadi. Yardim icin : 'h'.\n");
            }
        }
    }

    Kill_GameBoard(game_board, GAME_BOARD_SIZE);

    return CMD_OK;
}

int GCMD_Quit(char **destraptions, char *char_list, int count)
{
    printf("Cikis Yapiliyor . . .\n");
    return CMD_CLOSE;
}

void Print_Logo()
{
    printf("******************************************************************\n");
    printf("  ____    _    __  __    _        _____ __   __ _   _ _   _ _   _ \n");
    printf(" |  _ \\  / \\  |  \\/  |  / \\      |  _  \\\\ \\ / /| | | | \\ | | | | |\n");
    printf(" | | | |/ _ \\ | |\\/| | / _ \\     | | | | \\ V / | | | |  \\| | | | |\n");
    printf(" | |_| / ___ \\| |  | |/ ___ \\    | |_| |  | |  | |_| | |\\  | |_| |\n");
    printf(" |____/_/   \\_\\_|  |_/_/   \\_\\    \\___/   |_|   \\___/|_| \\_| \\___/ \n");
    printf("\n******************************************************************\n");
}

int **Creat_GameBoard()
{
    int **temp_board = calloc(GAME_BOARD_SIZE, sizeof(int *));
    if (temp_board == NULL)
        return NULL;
    for (int i = 0; i < GAME_BOARD_SIZE; i++)
    {
        temp_board[i] = calloc(GAME_BOARD_SIZE, sizeof(int));

        if (temp_board[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(temp_board[j]);
            free(temp_board);
            return NULL;
        }
    }
    return temp_board;
}

void Kill_GameBoard(int **game_board, int count)
{
    if (game_board == NULL)
        return;

    for (int i = 0; i < count; i++)
    {
        free(game_board[i]);
    }
    free(game_board);
    game_board = NULL;
}

int Fill_GameBoard(int **game_board, int count)
{
    if (game_board == NULL)
        return G_ERROR;

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            game_board[i][j] = 0;
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < count; j++)
        {
            if ((i + j) % 2 == 0)
            {
                game_board[i][j] = 1;
            }
        }
    }

    for (int i = count - 3; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            if ((i + j) % 2 == 0)
            {
                game_board[i][j] = 2;
            }
        }
    }

    return G_OK;
}

int Print_GameBoard(char **destraptions, char *char_list, int cmd_count, int **game_board, int count, int *current_gamer)
{
    if (game_board == NULL)
        return G_ERROR;

    printf("\n");

    Print_ABC(count);

    printf("\n");

    Print_Line(count);

    for (int i = 0; i < count; i++)
    {

        printf(" %d\t|", count - i);

        for (int j = 0; j < count; j++)
        {
            char stone;
            if (game_board[i][j] == 1)
            {
                stone = GAMER_1_CHAR;
            }

            else if (game_board[i][j] == 2)
            {
                stone = GAMER_2_CHAR;
            }

            else
            {
                stone = TEMP_CHAR;
            }
            printf(" %c |", stone);
        }

        printf("\t%d", count - i);
        Print_Line(count);
    }

    printf("\n");

    Print_ABC(count);

    printf("\n\n");
    return G_OK;
}

void Print_Line(int count)
{
    printf("\n\t+---");
    for (int i = 1; i < count; i++)
    {
        printf("+---");
    }
    printf("\n");
}

void Print_ABC(int count)
{
    printf("\t");

    for (int i = 0; i < count; i++)
    {
        printf("  %c ", 'A' + i);
    }
}

int Game_Finish(char **destraptions, char *char_list, int cmd_count, int **game_board, int count, int *current_gamer)
{
    printf("Oyunu bitirmek istedigize emin misiniz. Oyun bitince hafizadan silinir. ( y / N )  : ");

    char comman;

    CMD_Get(&comman);

    if (comman == 'y')
    {
        printf("Oyun bitirildi. Yeni oyun icin 'S' komutunu kullanini kullanin.\n");
        return G_Finsh;
    }
    else
    {
        return G_OK;
    }
}

int Game_Help(char **destraptions, char *char_list, int cmd_count, int **game_board, int board_size, int *current_gamer)
{
    printf("Oyun Yardim menusu : \n");

    for (int i = 0; i < cmd_count; i++)
    {
        printf("\t%c  :  %s\n", char_list[i], destraptions[i]);
    }

    if (current_gamer != NULL)
    {
        printf("\nSira %d. Oyuncuda (%s)\n", *current_gamer, (*current_gamer == 1) ? "X" : "O");
    }

    return G_OK;
}

int Gamer_Play(char **destraptions, char *char_list, int cmd_count, int **game_board, int board_size, int *current_gamer)
{
    int old_r, old_c, new_r, new_c;
    char col_char;
    int fork;
    int dir = (*current_gamer == 1) ? 1 : -1; // Direction

    Game_Status status = G_OK;

    printf("\nSira %d. Oyuncuda (%c)\n", *current_gamer, (*current_gamer == 1) ? GAMER_1_CHAR : GAMER_2_CHAR);

    if (Capture_Piece(game_board, board_size, current_gamer) == G_OK)
    {
        if (Game_ShowScore(game_board, board_size) == G_Finsh)
        {
            return G_Finsh;
        }

        Print_GameBoard(destraptions, char_list, LEN(char_list), game_board,
                        GAME_BOARD_SIZE, current_gamer);

        Chance_Gamer(current_gamer);
        return G_OK;
    }

    // MEVCUT TASIN SECILMESI
    while (1)
    {
        printf("Oynatilacak tasin SATIRI (1-%d): ", board_size);
        Number_Get(&old_r);
        old_r = board_size - old_r;

        printf("Oynatilacak tasin KOLONU (A-%c): ", 'A' + board_size - 1);
        CMD_Get(&col_char);
        old_c = (col_char >= 'a') ? col_char - 'a' : col_char - 'A';

        if (old_r >= 0 && old_r < board_size && old_c >= 0 && old_c < board_size)
        {

            if (game_board[old_r][old_c] == *current_gamer)
                break;
            else
                printf("HATA: Orada size ait bir tas yok! col : %d , row  : %d , owner : %d -\n",
                       old_r, old_c, game_board[old_c][old_r]);
        }
        else
        {
            printf("HATA: Tahta disinda bir koordinat girdiniz!\n");
        }
    }

    while (1)
    {
        printf("Hareket yonunu secin (1-Sola, 2-Saga, 3-Iptal) : ");
        Number_Get(&fork);

        new_r = old_r + dir; // Ileri capraz hareket (satir degisimi)

        if (fork == 1)
        {
            new_c = old_c - 1; // Sola hareket
        }
        else if (fork == 2)
        {
            new_c = old_c + 1; // Saga hareket
        }
        else if (fork == 3)
        {
            printf("Secim iptal edildi. Hamle yapmak icin 'p' komutu kullanin.\n");
            return G_ERROR;
        }
        else
        {
            printf("HATA: Gecersiz secim!\n");
            continue;
        }

        // Sinir ve doluluk kontrolu
        if (new_r >= 0 && new_r < board_size && new_c >= 0 && new_c < board_size)
        {
            if (game_board[new_r][new_c] == 0)
                break; // Gecerli hamle
            else
                printf("HATA: Hedef kare dolu!\n");
        }
        else
        {
            printf("HATA: Tahta sinirlarini asiyorsunuz!\n");
        }
    }
    // ---------------------------------------------------- //

    game_board[new_r][new_c] = game_board[old_r][old_c];
    game_board[old_r][old_c] = 0;

    if (Game_ShowScore(game_board, board_size) == G_Finsh)
    {
        status = G_Finsh;
    }

    Print_GameBoard(destraptions, char_list, LEN(char_list), game_board,
                    GAME_BOARD_SIZE, current_gamer);

    printf("Tas %d-%c konumundan %d-%c konumuna tasindi.\n",
           board_size - old_r, old_c + 'A', board_size - new_r, new_c + 'A');

    Chance_Gamer(current_gamer);

    return status;
}

int Game_ShowScore(int **game_board, int board_size)
{
    if (game_board == NULL)
    {
        return G_ERROR;
    }

    int gamer1_score = Calculate_TotalPions(board_size);
    int gamer2_score = Calculate_TotalPions(board_size);

    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            if (game_board[i][j] == 2)
            {
                gamer1_score--;
            }
            else if (game_board[i][j] == 1)
            {
                gamer2_score--;
            }
        }
    }

    printf("\n");
    printf("\t     :: SKOR TABLOSU ::\n");
    printf("\t  --------------------------\n");
    printf("\t  Oyuncu 1 (%c) Skor : %2d\n", GAMER_1_CHAR, gamer1_score);
    printf("\t  Oyuncu 2 (%c) Skor : %2d\n", GAMER_2_CHAR, gamer2_score);
    printf("\t  --------------------------\n\n");

    if (gamer1_score == Calculate_TotalPions(board_size))
    {
        Game_Over(1);
        return G_Finsh;
    }
    else if (gamer2_score == Calculate_TotalPions(board_size))
    {
        Game_Over(2);
        return G_Finsh;
    }

    return G_OK;
}

int Calculate_TotalPions(int board_size)
{
    return (board_size / 2) * 3;
}

void Game_Over(int winner)
{
    printf("\n");
    printf("    ======================================\n");
    printf("    ||          OYUN BITTI!            ||\n");
    printf("    ||      KAZANAN: OYUNCU %d ( %c )   ||\n", winner, (winner == 1) ? GAMER_1_CHAR : GAMER_2_CHAR);
    printf("    ======================================\n\n");
    printf("    Oyun kapatiliyor...\n");
}

void Chance_Gamer(int *current_gamer)
{
    *current_gamer = (*current_gamer == 1) ? 2 : 1;
}

int Number_Get(int *number)
{
    if (number == NULL)
        return G_ERROR;

    printf("\n=>  ");

    if (scanf("%d", number) != 1)
    {

        while (getchar() != '\n')
            ;
        return G_ERROR;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    return G_OK;
}

void Find_Capture_Paths(int **game_board, int board_size, int current_gamer,
                        int start_r, int start_c, int current_r, int current_c,
                        int captured_count, int *temp_target_r, int *temp_target_c,
                        int *path_count, int *attackers_r, int *attackers_c,
                        int *dest_r, int *dest_c,
                        int target_r[][MAX_JUMPS], int target_c[][MAX_JUMPS], int *target_count)
{
    int direction = (current_gamer == 1) ? 1 : -1;
    int opponent = (current_gamer == 1) ? 2 : 1;
    int has_more_captures = 0;

    // LEFT CHECK
    int left_r = current_r + direction;
    int left_c = current_c - 1;
    int left_jump_r = current_r + 2 * direction;
    int left_jump_c = current_c - 2;

    if (left_jump_r >= 0 && left_jump_r < board_size && left_jump_c >= 0 && left_jump_c < board_size)
    {
        if (game_board[left_r][left_c] == opponent && game_board[left_jump_r][left_jump_c] == 0)
        {
            has_more_captures = 1;

            /* Save current targets to the temporary list */
            temp_target_r[captured_count] = left_r;
            temp_target_c[captured_count] = left_c;

            /* Update the board virtually */
            game_board[current_r][current_c] = 0;
            game_board[left_r][left_c] = 0;
            game_board[left_jump_r][left_jump_c] = current_gamer;

            /* Continue the path (Recursive) */
            Find_Capture_Paths(game_board, board_size, current_gamer, start_r, start_c, left_jump_r, left_jump_c,
                               captured_count + 1, temp_target_r, temp_target_c,
                               path_count, attackers_r, attackers_c, dest_r, dest_c, target_r, target_c, target_count);

            /* BACKTRACKING: Restore the board */
            game_board[current_r][current_c] = current_gamer;
            game_board[left_r][left_c] = opponent;
            game_board[left_jump_r][left_jump_c] = 0;
        }
    }

    // RIGHT CHECK
    int right_r = current_r + direction;
    int right_c = current_c + 1;
    int right_jump_r = current_r + 2 * direction;
    int right_jump_c = current_c + 2;

    if (right_jump_r >= 0 && right_jump_r < board_size && right_jump_c >= 0 && right_jump_c < board_size)
    {
        if (game_board[right_r][right_c] == opponent && game_board[right_jump_r][right_jump_c] == 0)
        {
            has_more_captures = 1;

            /* Save current targets to the temporary list */
            temp_target_r[captured_count] = right_r;
            temp_target_c[captured_count] = right_c;

            /* Update the board virtually */
            game_board[current_r][current_c] = 0;
            game_board[right_r][right_c] = 0;
            game_board[right_jump_r][right_jump_c] = current_gamer;

            /* Continue the path (Recursive) */
            Find_Capture_Paths(game_board, board_size, current_gamer, start_r, start_c, right_jump_r, right_jump_c,
                               captured_count + 1, temp_target_r, temp_target_c,
                               path_count, attackers_r, attackers_c, dest_r, dest_c, target_r, target_c, target_count);

            /* BACKTRACKING: Restore the board */
            game_board[current_r][current_c] = current_gamer;
            game_board[right_r][right_c] = opponent;
            game_board[right_jump_r][right_jump_c] = 0;
        }
    }

    if (has_more_captures == 0 && captured_count > 0)
    {
        int i = *path_count;
        attackers_r[i] = start_r;
        attackers_c[i] = start_c;

        dest_r[i] = current_r;
        dest_c[i] = current_c;

        target_count[i] = captured_count;

        for (int k = 0; k < captured_count; k++)
        {
            target_r[i][k] = temp_target_r[k];
            target_c[i][k] = temp_target_c[k];
        }

        (*path_count)++;
    }
}

int Capture_Piece(int **game_board, int board_size, int *current_gamer)
{
    const int MAX_PATHS = 100;

    int attackers_r[MAX_PATHS];
    int attackers_c[MAX_PATHS];
    int target_count[MAX_PATHS];

    int target_r[MAX_PATHS][MAX_JUMPS];
    int target_c[MAX_PATHS][MAX_JUMPS];

    int dest_r[MAX_PATHS];
    int dest_c[MAX_PATHS];

    int temp_target_r[MAX_JUMPS];
    int temp_target_c[MAX_JUMPS];

    int found_path_count = 0;

    for (int i = 0; i < board_size; i++)
    {
        for (int j = 0; j < board_size; j++)
        {
            if (game_board[i][j] == *current_gamer)
            {
                Find_Capture_Paths(game_board, board_size, *current_gamer,
                                   i, j, i, j,
                                   0, temp_target_r, temp_target_c,
                                   &found_path_count,
                                   attackers_r, attackers_c, dest_r, dest_c, target_r,
                                   target_c, target_count);
            }
        }
    }

    // SHOW AND CHOISE
    if (found_path_count > 0)
    {
        int max_capture = 0;
        for (int i = 0; i < found_path_count; i++)
        {
            if (target_count[i] > max_capture)
            {
                max_capture = target_count[i];
            }
        }

        // SHOW
        printf("\n[%d. Oyuncu Icin Zorunlu Yeme Hamleleri]\n", *current_gamer);

        int greatest_captures[10] = {-1};
        int index = 0;

        for (int i = 0; i < found_path_count; i++)
        {
            if (target_count[i] == max_capture)
            {
                greatest_captures[index] = i;

                printf("%d) Saldiran: %d-%c | Varis (Son Konum): %d-%c | Hedef tas sayisi: %d\n",
                       index + 1,
                       board_size - attackers_r[i], attackers_c[i] + 'A',
                       board_size - dest_r[i], dest_c[i] + 'A',
                       target_count[i]);

                printf("    Hedef koordinatlari: ");
                for (int k = 0; k < target_count[i]; k++)
                {
                    printf("[%d-%c] ", board_size - target_r[i][k], target_c[i][k] + 'A');
                }
                printf("\n\n");

                index++;
            }
        }

        // CHOISE
        int choesen;

        do
        {
            printf("\nOynamak istediginiz yeme hamlesini seciniz. Iptal icin ( 0 ) 'test komutu' : \n");
            Number_Get(&choesen);

        } while (choesen > index || choesen < 0);

        if (choesen == 0)
        {
            return G_ERROR;
        }

        choesen = choesen - 1;

        int i = greatest_captures[choesen];

        for (int j = 0; j < target_count[i]; j++)
        {
            int t_r = target_r[i][j];
            int t_c = target_c[i][j];

            game_board[t_r][t_c] = 0;
        }

        int att_old_r = attackers_r[i];
        int att_old_c = attackers_c[i];

        game_board[att_old_r][att_old_c] = 0;

        int att_new_r = dest_r[i];
        int att_new_c = dest_c[i];

        game_board[att_new_r][att_new_c] = *current_gamer;
    }
    else
    {
        return G_ERROR;
    }

    return G_OK;
}