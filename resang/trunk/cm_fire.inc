###  File:     CM_Fire.INC
###  Purpose:  Defined the common block used for planning cruise missile
###            missions.  RTCommon was previously defined in wgopfir.rat,
###            but is now used by more than one file.
###  Date:     2/98 (E1864)


define ($debug,)

define ($MaxFireRatePerCycle,10)    # Max missiles that can be fired in a cycle

define (RTcommon,
        INTEGER*4  NUMsent, nord, Nargs, id_ord
        integer plan_index,
                Fire_index,
                nuclear_index,
                cruise_index,
                bol_index,
                delay_index,
                range_index,
                seeker_index,
                rbl_index,
                rbl_size,
                track_index,
                position_index,
                mission_index,
                waypoints_index,
                at_index,
                target_index,
                bridge_index,
                be_index,
                tin_index,
                base_index,
                torpedo_index,
                bearing_index,
                altitude_index,
                enableRange_index,
                torpedoes_index,
                depth_index,
                execute_index,
                ballistic_index
        LITERAL    weapon_name, Cnames[$MaxFireRatePerCycle] 

        common/RTcommon_X/  weapon_name, Cnames,
                            NUMsent, 
                            nord, Nargs, id_ord,
                            plan_index,
                            Fire_index,
                            nuclear_index,
                            cruise_index,
                            bol_index,
                            delay_index,
                            range_index,
                            seeker_index,
                            rbl_index,
                            rbl_size,
                            track_index,
                            position_index,
                            mission_index,
                            waypoints_index,
                            at_index,
                            target_index,
                            bridge_index,
                            be_index,
                            tin_index,
                            base_index,
                            torpedo_index,
                            bearing_index,
                            altitude_index,
                            enableRange_index,
                            torpedoes_index,
                            depth_index,
                            execute_index,
                            ballistic_index
     )

# RTcommon  
#            # numsent     => number to be fired/launched
#            # nord        => order number (xxxxxx$Order code)
#            # nargs       => number of arguments in command
#            # id_ord      => order id number
#            # cnames      => array of names assigned to fired/launched units
#            # waytbllat   => array of way point latitudes
#            # waytbllon   => array of way point longitudes
#            # weapon_name => literal name of weapon to be shot
#            # *_index     => lex code index of keyword
