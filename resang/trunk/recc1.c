#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 255
#define STRINGS_EQUAL 0

int main() {

   int i, j, len, entry_index, token_index_this_entry;
   FILE *rinsorted, *resaobj, *resaint, *fp;
   char rinrec[LINE_MAX], parse_me[LINE_MAX];
   char *numfields, *datafield, *fedname, *resaname, *csvtext;

   rinsorted=fopen("rin_sorted.txt", "r");
   if (!rinsorted) {
      printf("No sorted rin file!\n");
      exit(1);
   }

   resaobj=fopen("RESAobjects.txt", "w"); //unsorted
   resaint=fopen("RESAinteractions.txt", "w"); //unsorted

   while (fgets(rinrec, LINE_MAX, rinsorted)) {

      strcpy(parse_me,rinrec);
      numfields=strtok(parse_me," "); //number of fields
      datafield=strtok(NULL," "); //DATA token
      fedname=strtok(NULL," ");
      resaname=strtok(NULL," ");

      //if/else if is clearer than a select, and does not require break

      //OBJECTS

      if (strstr(rinrec,"$AIR_CRUISE_MISSILE")) {
         asprintf(&csvtext, "AIR,CRUISE_MISSILE,,UNIT_TYPE,,,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$JAMMER$CODE $AIR_FIXEDWING")) {
         asprintf(&csvtext, "AIR,FIXEDWING,,EMITTER,JAMMER,TYPE,");
         fp=resaobj;
      }
  
      else if(strstr(rinrec,"$RADAR$CODE $AIR_FIXEDWING")) {
         asprintf(&csvtext, "AIR,FIXEDWING,,EMITTER,RADAR,TYPE,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$AIR_FIXEDWING")) {
         asprintf(&csvtext, "AIR,FIXEDWING,,UNIT_TYPE,,,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$AIR_HELICOPTER")) {
         asprintf(&csvtext, "AIR,HELICOPTER,,UNIT_TYPE,,,");
         fp=resaobj;
      }
         
      else if(strstr(rinrec,"$AIR_BALLISTIC_MISSILE")) {
         asprintf(&csvtext, "AIR,TBM,,UNIT_TYPE,,,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$ALSP_CLASS_GROUND_MANEUVER_RADAR")) {
         asprintf(&csvtext, "GROUND,MANEUVER,RADAR,EMITTER,RADAR,TYPE,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$SEA_COUNTERMEASURE")) {
         asprintf(&csvtext, "SEA,COUNTERMEASURE,,UNIT_TYPE,,,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$MINE$CODE") && strcmp(numfields,"3")==STRINGS_EQUAL) {
         asprintf(&csvtext, "SEA,MINE,,UNIT_TYPE,,,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$SEA_SONOBUOY")) {
         asprintf(&csvtext, "SEA,SONOBUOY,,UNIT_TYPE,,,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$SEA_SUBSURFACE")) {
         asprintf(&csvtext, "SEA,SUBSURFACE,,UNIT_TYPE,,,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$JAMMER$CODE $SEA_SURFACE_BOAT")) {
         asprintf(&csvtext, "SEA,SURFACE,EMITTER,JAMMER,TYPE,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$RADAR$CODE $SEA_SURFACE_BOAT")) {
         asprintf(&csvtext, "SEA,SURFACE,EMITTER,RADAR,TYPE,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$SEA_SURFACE_BOAT")) {
         asprintf(&csvtext, "SEA,SURFACE,BOAT,UNIT_TYPE,,,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$JAMMER$CODE $SEA_SURFACE_SHIP")) {
         asprintf(&csvtext, "SEA,SURFACE,EMITTER,JAMMER,TYPE,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$RADAR$CODE $SEA_SURFACE_SHIP")) {
         asprintf(&csvtext, "SEA,SURFACE,EMITTER,RADAR,TYPE,");
         fp=resaobj;
      }

      else if(strstr(rinrec,"$SEA_SURFACE_SHIP")) {
         asprintf(&csvtext, "SEA,SURFACE,SHIP,UNIT_TYPE,,,");
         fp=resaobj;
      }  

      else if(strstr(rinrec,"$SEA_TORPEDO")) {
         asprintf(&csvtext, "SEA,TORPEDO,,UNIT_TYPE,,,");
         fp=resaobj;
      }

      //INTERACTIONS

      else if(strstr(rinrec,"$AIR_TO_AIR")) {
         asprintf(&csvtext, "ENGAGEMENT,AIR_TO_AIR,,,WEAPON,,");
         fp=resaint;
         //write additional record
         fprintf(fp, "%s%s,%s,,\n", csvtext, fedname, resaname);
         asprintf(&csvtext, "REPORT,ATTRITION,AIR_TO_AIR,,WEAPON,,");
      }

      else if(strstr(rinrec,"$AIR_TO_GROUND")) {
         asprintf(&csvtext, "ENGAGEMENT,AIR_TO_GROUND,,,WEAPON,,");
         fp=resaint;
         //write additional record
         fprintf(fp, "%s%s,%s,,\n", csvtext, fedname, resaname);
         asprintf(&csvtext, "REPORT,ATTRITION,AIR_TO_GROUND,,WEAPON,,");
      }

      else if(strstr(rinrec,"$AIR_TO_SHIP")) {
         asprintf(&csvtext, "ENGAGEMENT,AIR_TO_SHIP,,,WEAPON,,");
         fp=resaint;
         //write additional record
         fprintf(fp, "%s%s,%s,,\n", csvtext, fedname, resaname);
         asprintf(&csvtext, "REPORT,ATTRITION,AIR_TO_SHIP,,WEAPON,,");
      }

      else if(strstr(rinrec,"$GROUND_TO_AIR")) {
         asprintf(&csvtext, "ENGAGEMENT,GROUND_TO_AIR,,,WEAPON,,");
         fp=resaint;
         //write additional record
         fprintf(fp, "%s%s,%s,,\n", csvtext, fedname, resaname);
         asprintf(&csvtext, "REPORT,ATTRITION,GROUND_TO_AIR,,WEAPON,,");
      }

      else if(strstr(rinrec,"$GROUND_TO_SHIP")) {
         asprintf(&csvtext, "ENGAGEMENT,GROUND_TO_SHIP,,,WEAPON,,");
         fp=resaint;
         //write additional record
         fprintf(fp, "%s%s,%s,,\n", csvtext, fedname, resaname);
         asprintf(&csvtext, "REPORT,ATTRITION,GROUND_TO_SHIP,,WEAPON,,");
      }

      else if(strstr(rinrec,"$SHIP_TO_AIR")) {
         asprintf(&csvtext, "ENGAGEMENT,SHIP_TO_AIR,,,WEAPON,,");
         fp=resaint;
         //write additional record
         fprintf(fp, "%s%s,%s,,\n", csvtext, fedname, resaname);
         asprintf(&csvtext, "REPORT,ATTRITION,SHIP_TO_AIR,,WEAPON,,");
      }

      else if(strstr(rinrec,"$SHIP_TO_GROUND")) {
         asprintf(&csvtext, "ENGAGEMENT,SHIP_TO_GROUND,,,WEAPON,,");
         fp=resaint;
         //write additional record
         fprintf(fp, "%s%s,%s,,\n", csvtext, fedname, resaname);
         asprintf(&csvtext, "REPORT,ATTRITION,SHIP_TO_GROUND,,WEAPON,,");
      }

      else if(strstr(rinrec,"$SHIP_TO_SHIP")) {
         asprintf(&csvtext, "ENGAGEMENT,SHIP_TO_SHIP,,,WEAPON,,");
         fp=resaint;
         //write additional record
         fprintf(fp, "%s%s,%s,,\n", csvtext, fedname, resaname);
         asprintf(&csvtext, "REPORT,ATTRITION,SHIP_TO_SHIP,,WEAPON,,");
      }

      //FINALLY: we missed something!
      else {
         asprintf(&csvtext, "Missed this: ");
         fp=stderr;
      }

      //and finally write the record
      fprintf(fp, "%s%s,%s,,\n", csvtext, fedname, resaname);

   }

}
