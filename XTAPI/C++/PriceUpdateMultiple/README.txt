README.txt
PriceUpdateMultiple (C++)
===============================================================================

Overview
-------------------------------------------------------------------------------

This example demonstrates using the XTAPI to retrieve market data from multiple 
instruments.  The TTDropHandler object is used to resolve instrument(s) dragged 
and dropped from the X_TRADER Market Grid.  The TTInstrObj Alias property is 
used to distiguish between each instrument.


Instructions
-------------------------------------------------------------------------------

1. Drag and drop up to four Instruments from the Market Grid in X_TRADER.
2. The instrument data will populate in the text boxes.


XTAPI Objects
-------------------------------------------------------------------------------

TTDropHandler
TTInstrNotify
TTInstrObj


Revisions
-------------------------------------------------------------------------------

Version:		1.1.0
Date Created:	06/27/2005
Notes:			None

Version:		1.2.0
Date Created:	12/09/2008
Notes:			Added conformance/production warning on the form.  Added more
				descriptive comments and general source cleanup.

Version:		1.2.1
Date Created:	01/24/2013
Notes:			Updated for GitHub.  Renamed solution from 
				TTPriceUpdateMI to PriceUpdateMultiple.