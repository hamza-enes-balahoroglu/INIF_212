#include <stdio.h>
#include <stdlib.h>

#define LEN(obj) (sizeof(obj) / sizeof(obj[0]))

#define GAMER_1_CHAR 'O'
#define GAMER_2_CHAR 'X'
#define TEMP_CHAR ' '

#define GAME_BOARD_SIZE 10

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

/*
 * General command funcions type
 *  Paramaters :
 *     char *destraptions ->  command destraption (ex: "Close The Program.").
 *     char charlist      ->  command characters list (ex: {'h', 'q'}).
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

   Print_ABC(count);

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

      Print_Line(count);
   }

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
   printf("\n\t");

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

   printf("\nSira %d. Oyuncuda (%s)\n", *current_gamer, (*current_gamer == 1) ? "X" : "O");

   // MEVCUT TAŞIN SEÇİLMESİ
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

         if (game_board[old_c][old_r] == *current_gamer)
            break;
         else
            printf("HATA: Orada size ait bir tas yok! col : %d row  : %d gamer : %d -\n", old_r, old_c, game_board[old_c][old_r]);
      }
      else
      {
         printf("HATA: Tahta disinda bir koordinat girdiniz!\n");
      }
   }

   // HEDEF KARENİN SEÇİLMESİ
   while (1)
   {
      printf("Hedef kare SATIRI (1-%d): ", board_size);
      Number_Get(&new_r);
      new_r = board_size - new_r;

      printf("Hedef kare KOLONU (A-%c): ", 'A' + board_size - 1);
      CMD_Get(&col_char);
      new_c = (col_char >= 'a') ? col_char - 'a' : col_char - 'A';

      if (new_r >= 0 && new_r < board_size && new_c >= 0 && new_c < board_size)
      {
         if (game_board[new_r][new_c] == 0)
            break;
         else
            printf("HATA: Hedef kare dolu!\n");
      }
      else
      {
         printf("HATA: Tahta disinda bir koordinat girdiniz!\n");
      }
   }

   game_board[new_r][new_c] = game_board[old_r][old_c];
   game_board[old_r][old_c] = 0;
   Print_GameBoard(destraptions, char_list, LEN(char_list), game_board,
                   GAME_BOARD_SIZE, current_gamer);

   printf("Tas %d-%c konumundan %d-%c konumuna tasindi.\n",
          board_size - old_r, old_c + 'A', board_size - new_r, new_c + 'A');

   return CMD_OK;
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