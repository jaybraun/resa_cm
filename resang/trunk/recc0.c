#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEWLINE 10
#define LINE_MAX 255
#define MAX_ENTRIES 20000
#define MAX_TOKENS_PER_ENTRY 5

int main() {

   int i, j, len, entry_index, token_index_this_entry;
   FILE *rinfile1;
   char rinrec[LINE_MAX];
   char *token;
   char *tokens[MAX_ENTRIES][MAX_TOKENS_PER_ENTRY];
   int highest_entry_index;
   int highest_token_index[MAX_ENTRIES];

   rinfile1=fopen("alsp_enumeration_data_jlcctc.rin", "r");
   if (!rinfile1) {
      printf("No rin file!\n");
      exit(1);
   }

   entry_index=0;
   while (fgets(rinrec, LINE_MAX, rinfile1)) {

      if ( rinrec[0]=='#' || rinrec[0]==NEWLINE )
         continue;

      len=strlen(rinrec);

      if (strlen(rinrec)<5 || !strstr(rinrec,"Data$"))
         continue;

      for (i=0;i<len;i++) {
         if(rinrec[i]==',' || rinrec[i]=='\'' || rinrec[i]=='\"' || rinrec[i]=='(' || rinrec[i]==')'
            || rinrec[i]==NEWLINE)
            rinrec[i]=' ';
         //always:
         rinrec[i]=toupper(rinrec[i]);
      }

      token_index_this_entry=0;
      token=strtok(rinrec," ");
      while(token!=NULL) {
         asprintf(&tokens[entry_index][token_index_this_entry],token);
         token_index_this_entry++;
         token=strtok(NULL," ");
      }

      highest_token_index[entry_index]=token_index_this_entry-1;
      entry_index++;

   }   

   highest_entry_index=entry_index-1;
   for (i=0;i<=highest_entry_index;i++) {
      printf("%i ", highest_token_index[i]);
      for (j=0;j<=highest_token_index[i];j++) {
         printf("%s ", tokens[i][j]);
      }
      printf("\n");
   }

}
