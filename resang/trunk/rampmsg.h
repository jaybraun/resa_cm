/**********************************************************************
RAMPMsg.h -- these defines are shared by the RTU and the RIU
***********************************************************************
07/01/96 - SJL - fixed a bug (ghost name wasn't null terminated) found
                 while testing JCM 1047. 
09/05/96 - SJL - added GhostEngagementRAMP to RAMP_UNION overlay.
12/11/96 - SJL - added future time to TOD RAMP, added target entityID
                 to ENGAGE RAMP, removed obsolete RAMPs and other
                 obsolete defines, and changed some field names.
02/11/97 - SJL - E1732 -- added IFF RAMP, removed IFF data from INIT
                 RAMP, increased NUM_OF_UNITS to 2100. 
06/05/97 - SJL - E1812 -- added AAA_SAM_RAMP. 
**********************************************************************/

#ifndef RAMPMSG_H
#define RAMPMSG_H
#include <mdep.h>

#define NUM_OF_UNITS		2100
#define NUM_OF_MEMBERS		100
#define NUM_OF_GHOSTS		2000

#define FIRST_UNIT		1
#define LAST_UNIT		(FIRST_UNIT + NUM_OF_UNITS - 1)	
#define FIRST_MEMBER		(LAST_UNIT + 1)
#define LAST_MEMBER		(FIRST_MEMBER + NUM_OF_MEMBERS - 1)
#define FIRST_GHOST		(LAST_MEMBER + 1)
#define LAST_GHOST		(FIRST_GHOST + NUM_OF_GHOSTS - 1)

#define MARKING_SIZE 11

#define ENGAGEMENT_LAUNCH 65535

#define OTHER_STATUS	    0
#define OK_STATUS	    1
#define SMOKING_STATUS	    2
#define FLAMING_STATUS	    3
#define DELETED_STATUS	    4
#define ONDECK_STATUS	    5
#define DESTROYED_STATUS    6
#define ON_CAP_STATUS       7

typedef enum {
		INIT_REQ_RAMP =  1,
		INIT_RAMP,
		ENTITY_STATE_RAMP,
		ENGAGE_RAMP,
		JCR_RAMP,
		TIME_OF_DAY_RAMP,
		REQ_DYNAMIC_RAMP,
		END_OF_DATA_RAMP,
		GHOST_ENGAGE_RAMP,
		IFF_RAMP,
		AAA_SAM_RAMP,
		LAST_RAMP} ramp_type;


typedef struct {
	UNSIGNED_INT_16	msgType ;
	UNSIGNED_INT_16 size;
}RAMPMsgWrap ;


typedef struct {
	RAMPMsgWrap header;
	FLOAT_32 latitude;
	FLOAT_32 longitude;
	FLOAT_32 course;
	FLOAT_32 speed;
	UNSIGNED_INT_32 capabilities;     /* clk 5/97 (E1816) */
	UNSIGNED_INT_32 appearance ;
	INT_32 altitude ;
	UNSIGNED_INT_16 id ;
	UNSIGNED_INT_16 status ;
	INT_16 site;
	INT_16 application;
}EntityStateRAMP ;


typedef struct
  {
  UNSIGNED_INT_16 IFF_system_name;
  UNSIGNED_INT_8  system_on_off;
  UNSIGNED_INT_8  mode_1_code;
  UNSIGNED_INT_16 mode_2_code;
  UNSIGNED_INT_16 mode_3_code;
  UNSIGNED_INT_16 mode_C_code;
  UNSIGNED_INT_16 mode_4;
  } RESA_IFF_DATA;

typedef struct
  {
  RAMPMsgWrap header;
  UNSIGNED_INT_16 id;
  UNSIGNED_INT_16 pad;
  RESA_IFF_DATA iff_data;
  } IFFRAMPMessage ;


typedef struct {
        RAMPMsgWrap header;
	UNSIGNED_INT_16 id ;
	UNSIGNED_INT_8 kind ;
	UNSIGNED_INT_8 padding ;
}InitializationRequestRAMPMessage ;


typedef enum { REGULAR = 1, AGGREGATE} flight_type;

typedef enum { INIT = 1, TRACK_UPDATE} context_type;


typedef struct {
        RAMPMsgWrap header;
	UNSIGNED_INT_16 id ;
	UNSIGNED_INT_16 no_of_members;
	UNSIGNED_INT_16 mission_type;
	UNSIGNED_INT_16 parent_of_agg;
	UNSIGNED_INT_16 site;
	UNSIGNED_INT_16 appl;
	UNSIGNED_INT_16 entity;
	UNSIGNED_INT_16 country;
	UNSIGNED_INT_8 kind ;
	UNSIGNED_INT_8 domain ;
	UNSIGNED_INT_8 forceID ;
	UNSIGNED_INT_8 category ;
	UNSIGNED_INT_8 subcategory ;
	UNSIGNED_INT_8 specific ;
	UNSIGNED_INT_8 extra ;
	UNSIGNED_INT_8 context ;
	char marking[MARKING_SIZE+1]; /* null terminated string */
}InitializationRAMPMessage ;



typedef struct {
        RAMPMsgWrap header;
	FLOAT_32 munitionSpeed;
	FLOAT_32 indirectLat;
	FLOAT_32 indirectLon;
	FLOAT_32 indirectAlt; /* 0=RIU uses elev data */
	UNSIGNED_INT_16 firing_unit;
	UNSIGNED_INT_16 firing_entity;
	UNSIGNED_INT_16 target_site;
	UNSIGNED_INT_16 target_appl;
	UNSIGNED_INT_16 target_entity;	/* 0 = indirect Fire */
	UNSIGNED_INT_16 munition_unit;  /* 0 = not RESA unit */
	UNSIGNED_INT_16 munition_entity;
	UNSIGNED_INT_16 salvoSize ;
	UNSIGNED_INT_16 result ;
	UNSIGNED_INT_16 munitionRate ;
	UNSIGNED_INT_16 country ;
	UNSIGNED_INT_8 kind ;
	UNSIGNED_INT_8 domain ;
	UNSIGNED_INT_8 category ;
	UNSIGNED_INT_8 subcategory ;
	UNSIGNED_INT_8 specific ;
	UNSIGNED_INT_8 extra ;
}EngagementRAMPMessage ;

typedef struct {
        RAMPMsgWrap header;
	FLOAT_32 lat;
	FLOAT_32 lon;
	FLOAT_32 alt;
	UNSIGNED_INT_32 miss_distance;
	UNSIGNED_INT_16 target_unit;
	UNSIGNED_INT_16 target_entity;
	UNSIGNED_INT_16 firing_rid;
	UNSIGNED_INT_16 firing_site;
	UNSIGNED_INT_16 firing_appl;
	UNSIGNED_INT_16 firing_entity;
	UNSIGNED_INT_16 munition_rid;
	UNSIGNED_INT_16 munition_site;
	UNSIGNED_INT_16 munition_appl;
	UNSIGNED_INT_16 munition_entity;
	UNSIGNED_INT_16 salvo_size;
	UNSIGNED_INT_16 munition_rate;
	UNSIGNED_INT_16 result;
	UNSIGNED_INT_16 country;
	UNSIGNED_INT_8 kind;
	UNSIGNED_INT_8 domain;
	UNSIGNED_INT_8 category;
	UNSIGNED_INT_8 subcategory;
	UNSIGNED_INT_8 specific;
	UNSIGNED_INT_8 extra;
	UNSIGNED_INT_8 pad[2];
}GhostEngagementRAMP;



typedef struct
  {
  RAMPMsgWrap header;
  char jcr[400];
  } JCRRAMPMessage; 



typedef struct {
	RAMPMsgWrap header;
	UNSIGNED_INT_32 current_time;
	UNSIGNED_INT_32 future_time;
}TimeOfDayRAMPMessage;



typedef struct {
	RAMPMsgWrap header;
}RequestDynamicDataRAMPMessage ;



typedef struct {
	RAMPMsgWrap header;
	UNSIGNED_INT_16 RequestID ;
	UNSIGNED_INT_16 pad ;
}End_of_DataRAMPMessage ;

typedef struct
  {
  RAMPMsgWrap header;
  UNSIGNED_INT_16 illuminator_site;
  UNSIGNED_INT_16 illuminator_appl;
  UNSIGNED_INT_16 illuminator_entity;
  UNSIGNED_INT_16 target_site;
  UNSIGNED_INT_16 target_appl;
  UNSIGNED_INT_16 target_entity;
  UNSIGNED_INT_16 target_unit; /* RESA unit index */
  UNSIGNED_INT_16 status; /* 0=off, 1=on */
  } AAA_SAM_RAMPMessage ;

/********************************************************************
RAMP_UNION overlay
********************************************************************/
typedef union
  {
  RAMPMsgWrap                          header;
  EntityStateRAMP                      es;
  InitializationRequestRAMPMessage     initreq;
  InitializationRAMPMessage            init;
  EngagementRAMPMessage                eng;
  GhostEngagementRAMP                  ghost_eng;
  JCRRAMPMessage                       jcrmsg;
  TimeOfDayRAMPMessage                 tod;
  RequestDynamicDataRAMPMessage        dynreq;
  End_of_DataRAMPMessage               eod;
  IFFRAMPMessage                       iff;
  AAA_SAM_RAMPMessage                  aaa_sam;
  } RAMP_UNION;

#endif
