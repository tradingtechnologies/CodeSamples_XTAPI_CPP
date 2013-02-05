README.txt
OrderUpdate (C++)
===============================================================================

Overview
-------------------------------------------------------------------------------

This example demonstrates using the XTAPI to retrieve order status updates from
the TTOrderSet.  The TTDropHandler object is used to resolve an instrument 
dragged and dropped from the X_TRADER Market Grid.


Instructions
-------------------------------------------------------------------------------

1. Drag and drop an Instrument from the Market Grid in X_TRADER.
2. Place an order either from this sample or from X_TRADER.
3. Modify orders from X_TRADER.
4. Observe events fired in the application.


XTAPI Objects
-------------------------------------------------------------------------------

TTDropHandler
TTInstrNotify
TTInstrObj
TTOrderSetObj


Revisions
-------------------------------------------------------------------------------

Version:		1.2.0
Date Created:	12/08/2009
Notes:			Added conformance/production warning on the form.  Added more
				descriptive comments and general source cleanup.

Version:		1.2.1
Date Created:	01/15/2013
Notes:			Updated for GitHub.  Renamed from OrderStatus to OrderUpdate.			