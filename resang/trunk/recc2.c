#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 255
#define STRINGS_EQUAL 0
#define NUM_FIELDS 8

FILE *xml, *allobj, *allint, *fp;
char *these[NUM_FIELDS];
char *overall_name, *field_name;
int num_cat_fields;

void lookfor(char*);

int main() {

   int i, j, mapping_count;

   xml=fopen("RESA.xml", "a"); //will already have some text

   allobj=fopen("allobjects.csv", "r");
   if (!allobj) {
      printf("No objects file!\n");
      exit(1);
   }

   allint=fopen("allinteractions.csv", "r");
   if (!allint) {
      printf("No interactions file!\n");
      exit(1);
   }

   for(i=0;i<2;i++) {

      if (i==0) { //objects

         fp=allobj;
         overall_name="class";
         field_name="attribute";
         num_cat_fields=3;

      } else { //interactions

         fp=allint;
         overall_name="interaction";
         field_name="parameter";
         num_cat_fields=4;

      }

      fprintf(xml, "  <%s_enumerations>\n", field_name);

      if (i==0) { //objects

         lookfor("AIR,CRUISE_MISSILE, ,UNIT_TYPE, , ,");
         lookfor("AIR,FIXEDWING, ,EMITTER,JAMMER,TYPE,");
         lookfor("AIR,FIXEDWING, ,EMITTER,RADAR,TYPE,");
         lookfor("AIR,FIXEDWING, ,UNIT_TYPE, , ,");
         lookfor("AIR,HELICOPTER, ,UNIT_TYPE, , ,");
         lookfor("AIR,TBM, ,UNIT_TYPE, , ,");
         lookfor("GROUND,MANEUVER,RADAR,EMITTER,RADAR,TYPE,");
         lookfor("SEA,COUNTERMEASURE, ,UNIT_TYPE, , ,");
         lookfor("SEA,MINE, ,UNIT_TYPE, , ,");
         lookfor("SEA,SONOBUOY, ,UNIT_TYPE, , ,");
         lookfor("SEA,SUBSURFACE, ,UNIT_TYPE, , ,");
         lookfor("SEA,SURFACE,EMITTER,JAMMER,TYPE,");
         lookfor("SEA,SURFACE,EMITTER,RADAR,TYPE,");
         lookfor("SEA,SURFACE,BOAT,UNIT_TYPE, , ,");
         lookfor("SEA,SURFACE,SHIP,UNIT_TYPE, , ,");
         lookfor("SEA,TORPEDO, ,UNIT_TYPE, , ,");

      } else { //interactions

         lookfor("ENGAGEMENT,AIR_TO_AIR, , ,WEAPON, ,");
         lookfor("ENGAGEMENT,AIR_TO_GROUND, , ,WEAPON, ,");
         lookfor("ENGAGEMENT,AIR_TO_SHIP, , ,WEAPON, ,");
         lookfor("ENGAGEMENT,GROUND_TO_AIR, , ,WEAPON, ,");
         lookfor("ENGAGEMENT,GROUND_TO_SHIP, , ,WEAPON, ,");
         lookfor("ENGAGEMENT,SHIP_TO_AIR, , ,WEAPON, ,");
         lookfor("ENGAGEMENT,SHIP_TO_GROUND, , ,WEAPON, ,");
         lookfor("ENGAGEMENT,SHIP_TO_SHIP, , ,WEAPON, ,");
         lookfor("REPORT,ATTRITION,AIR_TO_AIR, ,WEAPON, ,");
         lookfor("REPORT,ATTRITION,AIR_TO_GROUND, ,WEAPON, ,");
         lookfor("REPORT,ATTRITION,AIR_TO_SHIP, ,WEAPON, ,");
         lookfor("REPORT,ATTRITION,GROUND_TO_AIR, ,WEAPON, ,");
         lookfor("REPORT,ATTRITION,GROUND_TO_SHIP, ,WEAPON, ,");
         lookfor("REPORT,ATTRITION,SHIP_TO_AIR, ,WEAPON, ,");
         lookfor("REPORT,ATTRITION,SHIP_TO_GROUND, ,WEAPON, ,");
         lookfor("REPORT,ATTRITION,SHIP_TO_SHIP, ,WEAPON, ,");

      }
      
      fprintf(xml, "  </%s_enumerations>\n", field_name);

   } //on loop 0 to 1, obj and int

}
void lookfor(char *stuff) {

   int i, len, mapping_count=0, write_something, write_model_value;
   char csvrec[LINE_MAX], tempstr[LINE_MAX];
   char name1[80], name2[80], currentmel[80], currentresa[80], prevmel[80],
oldmel[80];

   while (fgets(csvrec, LINE_MAX, fp)) {
      if(strstr(csvrec,stuff)) { //first of the bunch
         break;
      }
   }

   //At this point, we have a new category
   strcpy(tempstr,csvrec);
   these[0]=strtok(tempstr,",");
   for (i=1;i<NUM_FIELDS;i++) {
      these[i]=strtok(NULL,",");
   }

   name1[0]='\0';
   for (i=0;i<num_cat_fields;i++) {
      if(strlen(these[i])<2) {
         break;
      }
      if (i>0) {
         strcat(name1,",");
      } 
      strcat(name1, these[i]);
   }
   len=strlen(name1);
   if(name1[len-1]==',') {
      name1[len-1]='\0';
   }
   for (i=0;i<len;i++) {
      if(name1[i]==',') {
         name1[i]='.';
      }
   }

   name2[0]='\0';
   for (i=num_cat_fields;i<6;i++) {
      if(strlen(these[i])<2) {
         break;
      }
      if (i>num_cat_fields) {
         strcat(name2,",");
      }
      strcat(name2, these[i]);
   }
   len=strlen(name2);
   if(name2[len-1]==',') {
      name2[len-1]='\0';
   }
   for (i=0;i<len;i++) {
      if(name2[i]==',') {
         name2[i]='.';
      }
   }

   fprintf(xml, "    <%s name=\"%s\">\n", overall_name, name1);

   strcpy(prevmel,these[6]);
   strcpy(currentresa,these[7]);

   //Keep reading records that match
   while (fgets(csvrec, LINE_MAX, fp)) {
      if(!strstr(csvrec,stuff)) { 
         break;
      }
      strcpy(tempstr,csvrec);
      these[0]=strtok(tempstr,",");
      for (i=1;i<NUM_FIELDS;i++) {
         these[i]=strtok(NULL,",");
      }
      strcpy(currentmel,these[6]);
      strcpy(currentresa,these[7]);

      if (strcmp(prevmel,currentmel)==STRINGS_EQUAL) {
         mapping_count++;
         write_something=1;
         write_model_value=1;
      } else { //new MEL name
         if (mapping_count==0) {
            //printf("RESA has no match for MEL name %s\n", prevmel);
            write_something=1;
            write_model_value=0;
         } else {
            write_something=0;
            write_model_value=0;
         }
         strcpy(oldmel,prevmel);
         strcpy(prevmel,currentmel);
         mapping_count=0;
      }

      if (write_something) {
         fprintf(xml, "      <%s_set>\n", field_name);
         fprintf(xml, "        <%s name=\"%s\"", field_name, name2);
         if (write_model_value) {
            fprintf(xml, ">\n");
            fprintf(xml, "          <federationValue>%s</federationValue>\n", prevmel);
         } else {
            fprintf(xml, " not_modelled_internally=\"true\">\n");
            fprintf(xml, "          <federationValue>%s</federationValue>\n", oldmel);
         }
         fprintf(xml, "        </%s>\n", field_name);
         if (write_model_value) {
            fprintf(xml, "        <modelValue>%s</modelValue>\n", currentresa);
         }
         fprintf(xml, "        <Direction>IN_AND_OUT</Direction>\n");
         fprintf(xml, "      </%s_set>\n", field_name); 
      }
   }

   fprintf(xml, "    </%s>\n", overall_name);

   //Last thing before leaving
   rewind(fp);
}
