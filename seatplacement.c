//Author: Barış Kılıç

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROW_MAX 99
#define COLUMN_MAX 20
#define AISLE_MAX 5
#define SMALL_NUMBER -99999999999

int row = 0, column = 0, clm = 0, i = 0, x = 0, j = 0, y = 0, aisle = 0, reserve_number = 0, no_reservation = 0;
char character_set[COLUMN_MAX + AISLE_MAX] = "ABCDEFGHIJKLMNOPQRST";
char seat_map[ROW_MAX][COLUMN_MAX + AISLE_MAX];
double score_array[ROW_MAX][COLUMN_MAX + AISLE_MAX];
int reserved_seats[ROW_MAX][COLUMN_MAX + AISLE_MAX];
int current_seats[ROW_MAX];

void calculate_scores();
void layout_printer();
void reservation_maker();
double dist(int i1, int i2, int j1, int j2);
double adjusted_dist(double distance);

int main()
{
   printf("Number of rows: ");
   scanf("%d", &row);
   printf("Number of seats per row: ");
   scanf("%d", &column);
   printf("Number of aisles: ");
   scanf("%d", &aisle);

   clm = column + aisle;

   for (int i = 0; i < row; i++)
   {
      for (int j = 0; j < clm; j++)
      {
         reserved_seats[i][j] = 0;
      }
   }

   for (j = 0; j < aisle; j++)
   {
      int aisle_location;
      printf("Add aisle %d after seat: ", j + 1);
      scanf("%d", &aisle_location);
      for (i = 0; i < row; i++)
      {
         reserved_seats[i][aisle_location + j] = 3;
      }
   }

   while (reserve_number != -1)
   {
      if (reserve_number == 0)
      {
         layout_printer();
         reserve_number++;
         continue;
      }
      printf("The number of people in the reservation\n (0: print current reservations, -1: exit):");
      scanf("%d", &reserve_number);
      if (reserve_number > 0)
      {
         reservation_maker();
         if (no_reservation == 0)
         {
            printf("Reserved seats: ");
            for (i = 0; i < row; i++)
            {
               int aisle_number = 0;
               for (j = 0; j < clm; j++)
               {
                  if (reserved_seats[i][j] == 3)
                     aisle_number++;
                  if (reserved_seats[i][j] == 1)
                     printf(" %d%c", i + 1, character_set[j - aisle_number]);
               }
            }
            printf("\n\n");
            layout_printer();
            printf("\n");
         }
         no_reservation = 0;
         for (i = 0; i < row; i++)
         {
            for (j = 0; j < clm; j++)
            {
               if (reserved_seats[i][j] == 1)
                  reserved_seats[i][j] = 2;
            }
         }
      }
   }

   return EXIT_SUCCESS;
}

double dist(int i1, int i2, int j1, int j2)
{
   return (abs(i1 - i2) + (abs(j1 - j2) / 2.0)); 
}

double adjusted_dist(double distance)
{
   if (distance == 0)
      return 0;
   return (100.00 - pow(100.00, 1.0 / distance));
}


void calculate_scores()
{
   for (i = 0; i < row; i++)
   {
      for (j = 0; j < clm; j++)
      {
         score_array[i][j] = 0.0;
      }
   }

   for (x = 0; x < row; x++)
   {
      for (y = 0; y < clm; y++)
      {
         if (reserved_seats[x][y] == 0)
         {
            for (i = 0; i < row; i++)
            {
               for (int j = 0; j < clm; j++)
               {
                  if (reserved_seats[i][j] == 2)
                     score_array[x][y] += adjusted_dist(dist(x, i, y, j));
               }
            }
         }
      }
   }
}

void reservation_maker()
{
   calculate_scores();
   double max_score = SMALL_NUMBER;
   int aisle_num = 0;
   for (i = 0; i < row; i++)
   {
      for (j = 0; j < clm; j++)
      {
         double temp_score = 0;
         int aisle_temp = 0;
         int empty_seats = 0;
         int k = j;
         if (reserved_seats[i][k] == 0)
         {
            while (k < clm && empty_seats < reserve_number && reserved_seats[i][k] != 2)
            {
               if (reserved_seats[i][k] == 0)
               {
                  temp_score += score_array[i][k];
                  empty_seats++;
               }
               if (reserved_seats[i][k] == 3)
               {
                  aisle_temp++;
               }
               k++;
            }
         }
         if (empty_seats == reserve_number)
         {
            if (max_score == temp_score)
            {
               if (aisle_num > aisle_temp)
               {
                  aisle_num = aisle_temp;
                  current_seats[0] = i;
                  current_seats[1] = j;
               }
            }
            if (max_score < temp_score)
            {
               max_score = temp_score;
               aisle_num = aisle_temp;
               current_seats[0] = i;
               current_seats[1] = j;
            }
         }
      }
   }

   if (max_score == SMALL_NUMBER)
   {
      printf("\nNo available seats for the requested reservation!\n\n");
      no_reservation++;
      return;
   }

   for (int k = current_seats[1]; k < reserve_number + aisle_num + current_seats[1]; k++)
   {
      if (reserved_seats[current_seats[0]][k] != 3)
      {
         reserved_seats[current_seats[0]][k] = 1;
      }
   }
}

void layout_printer()
{
   for (i = 0; i < row; i++)
   {
      for (j = 0; j < clm; j++)
      {    
            if (reserved_seats[i][j] == 0)
               seat_map[i][j] = '-';
            else if (reserved_seats[i][j] == 1)
               seat_map[i][j] = '+';
            else if (reserved_seats[i][j] == 2)
               seat_map[i][j] = 'X';    
      }
   }
   int chr = 0;
   printf("  ");
   for (x = -1; x < row; x++)
   {
      if (x != -1)
         printf("%d ", x + 1);
      for (int y = 0; y < clm; y++)
      {
         if (x == -1)
         {
            if (reserved_seats[0][y] == 3)
            {
               printf("| |");
               continue;
            }
            printf(" %c ", character_set[chr]);
            chr++;
            continue;
         }
         if (reserved_seats[x][y] == 3)
         {
            printf("| |");
            continue;
         }
         printf(" %c ", seat_map[x][y]);
      }
      printf("\n");
   }
}