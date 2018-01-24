# File:  StationMsg.inc
# Date:  November 1997 (E1852)
#
# Purpose:  This common block is used to retain which station orders
#           are being processed for (if within order processing).
#           Order validation error messages will be sent to the 
#           station originating the order.


define (StationMsgCommon,

    integer StationMsgWithinOrderProcessing    # Set when within wgordp
    integer StationMsgIndx                     # Station entering order

    common /StationMsg/ StationMsgWithinOrderProcessing,
                        StationMsgIndx
)
