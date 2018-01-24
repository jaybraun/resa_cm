define ($max$map$points,250000)
define ($maptbl$size,$max$map$points/5)

define($MAP_PAGES,((($max$map$points*8 + $maptbl$size*4))/512+1)) 

define ($boundaries,    7777)           # country boundaries
define ($rivers,        8888)           # rivers
define ($land,          9999)           # land mass
define ($userHI,        9916)           # highest user-def. color
define ($userLO,        9901)           # lowest  user-def. color
define ($userBASE,      9900)           # excess over color number
define ($userHId,       9816)           # highest DASHED user-def. color
define ($userLOd,       9801)           # lowest  DASHED user-def. color
define ($userBASEd,     9800)           # excess over DASHED color number

define (GPHcommon,
        integer         grfile,mapseg,mapcnt,impseg,lmpseg,mapflg,
                        geo_display,geo_rivers,geo_boundaries,maptbl_ptr,
                        mapdef_ptr,geo_textsize,geo_radius
        real            xl,yb,xr,yt
        common/gph/     grfile,mapseg,mapcnt,impseg,lmpseg,mapflg,xl,yb,xr,yt,
                        geo_display,geo_rivers,geo_boundaries,maptbl_ptr,
                        mapdef_ptr,geo_textsize,geo_radius
        )

define (GPH1common,                     # common for current segment
        integer         kseg,lseg,iseg  # and last segment for symbols
        common /gph1/   kseg,lseg,iseg
        )

define (GPH2common,                     # common for world map data
        real            mapdef[2,$max$map$points]
        integer*4       buffer_start,
                        buffer_end
        VMSchar*512     pagefill[2]
        integer*4       maptbl[$maptbl$size]
        common/gph2/    mapdef,maptbl,  # mapdef & maptbl must be kept in order
                        pagefill, 
                        buffer_end
        equivalence     (mapdef,buffer_start)
        )
