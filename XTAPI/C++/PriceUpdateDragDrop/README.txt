README.txt
PriceUpdateDragDrop (C++)
===============================================================================

Overview
-------------------------------------------------------------------------------

This example demonstrates using the XTAPI to retrieve market data from a single 
instrument, and provides support for filtering market data updates.  The  
TTDropHandler object is used to resolve an instrument dragged and dropped from 
the X_TRADER Market Grid.


Instructions
-------------------------------------------------------------------------------

1. Drag and drop an Instrument from the Market Grid in X_TRADER.
2. Check the "Bid Price" and/or "Last Traded Price" to filter the updates.


XTAPI Objects
-------------------------------------------------------------------------------

TTDropHandler
TTInstrNotify
TTInstrObj


Revisions
-------------------------------------------------------------------------------

Version:		1.1.0
Date Created:	06/22/2005
Notes:			Added instrument filtering.

Version:		1.2.0
Date Created:	12/08/2008
Notes:			Added conformance/production warning on the form.  Added more
				descriptive comments and general source cleanup.

Version:		1.2.1
Date Created:	01/23/2013
Notes:			Updated for GitHub.  Renamed solution from TTPriceUpdate to 
				PriceUpdateDragDrop.