/*c include file ptvext.h, known previously as ptvext.inc */
/*referenced in draw_center,bndry,grid,draw_bearing in dwdraw.c*/

typedef struct {
    float wxl,wyb,wxr,wyt,wybt,wytt;
    float vxl,vyb,vxr,vyt;
    float wvxm,wvxa,wvym,wvya;
    float xclp,yclp;
    short int  projty,clpm1;
} EXTcommon; 

EXTcommon EXT_;
