README.txt
PriceUpdateDepth (C++)
===============================================================================

Overview
-------------------------------------------------------------------------------

This example demonstrates using the XTAPI to retrieve market depth data from a 
single instrument.  The TTDropHandler object is used to resolve an instrument 
dragged and dropped from the X_TRADER Market Grid.  Bid and ask depth size can 
be selected as well as enabling or disabling depth updates.  The default level 
is set to "0" which means that all available depth will be displayed.  

Note: The maximum level of depth can differ by exchange.


Instructions
-------------------------------------------------------------------------------

1. Drag and drop an Instrument from the Market Grid in X_TRADER.
2. View market depth and change the depth options.


XTAPI Objects
-------------------------------------------------------------------------------

TTDropHandler
TTInstrNotify
TTInstrObj


Revisions
-------------------------------------------------------------------------------

Version:		1.1.0
Date Created:	06/22/2005
Notes:			None

Version:		1.2.0
Date Created:	12/08/2008
Notes:			Added conformance/production warning on the form.  Added more
				descriptive comments and general source cleanup.

Version:		1.2.1
Date Created:	01/15/2013
Notes:			Updated for GitHub.