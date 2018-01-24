define (RTH$rcl$entry,7)          # size of an RCL (rothr track) entry

     ######################################################################
     #  The maximum number of dwell illumination regions (DIRs) that can be
     #  selected for track generation is limited to:
     #       (i)   16, for surface tracks during the day;
     #       (ii)   8, for surface tracks during the night;
     #       (iii) 24, for air tracks during the day;
     #       (iv)  12, for air tracks during the night.
     ######################################################################

define (maxsurf$dir$day,16)
define (maxair$dir$day,24)
define (maxsurf$dir$night,8)
define (maxair$dir$night,12)




define (air$pca$base,0)      # offset from PCA$DayLoss and PCA$NightLoss where
                             # DIR loss factors for air tracks are stored

define (surf$pca$base,48)    # offset from PCA$DayLoss and PCA$NightLoss where
                             # DIR loss factors for surface tracks are stored

define (air$pca,0)           # flag which is used for identifying a unit as
                             # being an air track

define (surf$pca,1)          # flag which is used for identifying a unit as
                             # being a surface track

define (air$dir$length,250)  # number of nautical miles from one ring to
                             # another in primary coverage area (PCA) for
                             # air tracks

define (surf$dir$length,125) # number of nautical miles between rings in
                             # PCA for surface tracks

define (night$RTH,1)         # flags which are used for identifying whether
define (day$RTH,0)           # it is day or night

define (dir$good,2)          # codes for expected Rothr performance within
define (dir$fair,1)          # a given DIR;  each DIR gets one
define (dir$poor,0)


define (dir$good$color,6)    # codes for color of DIR boundary based on
define (dir$fair$color,4)    # expected Rothr performance
define (dir$poor$color,14)
