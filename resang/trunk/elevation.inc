define ($MaxElevHeight,17)	# Max height of elevation region (deg or cells)
define ($MaxElevWidth,17)	# Max width of elevation region (deg or cell)
define ($MaxElevRegions,1)	# Max elevation regions
define ($DTEDCellDim,1200)      # Number of points in row/column of dted cell
define ($DTEDAreaDim,300)       # Number of points in row/column of dted area
define ($DTEDArrayDim,4080)     # Number of point in row or column of dted arra
# 57600 = 240*240 points in a 1200 by 1200 point cell with 500 meter resolution

define($points_word_count,(($DTEDArrayDim+1)*($DTEDArrayDim+1))/2)
define(DTED_pages,((elevation_region_size + elevation_common_prefix_size +
       $points_word_count)/128) +100)
define($ELEVATIONS$PER$RECORD, 256)		# 512 byte record with int*2 array
                        

define (ElevationCommon,
#STRUCTURE /elevation_region_struct/
#    real	swlat		# Southwest latitude of elevation region: rad
#    real	swlon		# Southwest longitude of elevation region: rad
#    integer	height		# Height of elevation region: deg
#    integer	width		# Width of elevation region: deg
#    integer	resolution	# Meter resolution between elevation points: m
#    integer	elevation_sub1  # Beginning address of latitude points
#    integer	elevation_sub2  # Beginning address of longitude points
#    integer	max_region_elevation # Max elevation in region (ft)
#    STRUCTURE /max_elevation_struct/ max_elevation[0:$MaxElevHeight,0:$MaxElevWidth]
#	integer	cell		# Max elevation of cell (ft)
#	integer	area[16]	# Max elevation of 15'x15' areas within cell
#                                # (ft). Areas in cell configured as in diagram 
#				#    4   8  12  16
#                                #    3   7  11  15
#                                #    2   6  10  14
#				#    1   5   9  13
#    END STRUCTURE
#END STRUCTURE
#record /elevation_region_struct/ elevation_region[$MaxElevRegions]

real    elevation_region_swlat[$MaxElevRegions]           # Southwest latitude of elevation region: rad
real    elevation_region_swlon[$MaxElevRegions]           # Southwest longitude of elevation region: rad
integer elevation_region_height[$MaxElevRegions]          # Height of elevation region: deg
integer elevation_region_width[$MaxElevRegions]           # Width of elevation region: deg
integer elevation_region_resolution[$MaxElevRegions]      # Meter resolution between elevation points: m
integer elevation_region_elevation_sub1[$MaxElevRegions]  # Beginning address of latitude points
integer elevation_region_elevation_sub2[$MaxElevRegions]  # Beginning address of longitude points
integer elevation_region_max_region_elevation[$MaxElevRegions] # Max elevation in region (ft)
integer elevation_region_max_elevation_cell[0:$MaxElevHeight,0:$MaxElevWidth,$MaxElevRegions]  # Max elevation of cell (ft)
integer elevation_region_max_elevation_area[16,0:$MaxElevHeight,0:$MaxElevWidth,$MaxElevRegions]           # Max elevation of 15'x15' areas within cell
#                                # (ft). Areas in cell configured as in diagram 
#               #    4   8  12  16
#               #    3   7  11  15
#               #    2   6  10  14
#               #    1   5   9  13

define (elevation_region_size,( 8 + 17 * ($MaxElevHeight+1) * ($MaxElevWidth+1)))
define (elevation_common_prefix_size,(1 + wet$slots))
                        

integer nat2elev[wet$slots]	# Index array by wet$index to get
    				#  elevation_region index
integer N_elev_regions
integer*2 elevation_points[0:$DTEDArrayDim,0:$DTEDArrayDim]	# 4080 gives 1000x1000 nm
                                                # grid at 400 meter resolution

integer elevation_common_overlay[ARB]

    common /DTEDboard/  N_elev_regions, nat2elev,
                        elevation_region_swlat,
                        elevation_region_swlon,
                        elevation_region_height,
                        elevation_region_width,
                        elevation_region_resolution,
                        elevation_region_elevation_sub1,
                        elevation_region_elevation_sub2,
                        elevation_region_max_region_elevation,
                        elevation_region_max_elevation_cell,
                        elevation_region_max_elevation_area,
                        elevation_points

%cpar$ private / DTEDboard / ! no FORTRAN assigning a global section

equivalence (N_elev_regions, elevation_common_overlay)
)


define (DTED_MAP,call map_dted(ok_to_write))
