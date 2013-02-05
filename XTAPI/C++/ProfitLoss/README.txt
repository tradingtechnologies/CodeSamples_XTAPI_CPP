README.txt
ProfitLoss (C++)
===============================================================================

Overview
-------------------------------------------------------------------------------

This example demonstrates using the XTAPI to retrieve the open P&L (profit & 
loss) for multiple instruments.  The TTDropHandler object is used to resolve an 
instrument dragged and dropped from the X_TRADER Market Grid.  The TTInstrObj 
Alias property is used to distiguish between each instrument.

Note:   By default the format of the P&L from the XTAPI does not match the 
		default setting in X_TRADER.  Please see the following knowledge
		base article for further information:
    
		https://www.tradingtechnologies.com/support/knowledge-base/1/1894/


Instructions
-------------------------------------------------------------------------------

1. Drag and drop up to four instruments from the Market Grid in X_TRADER.
2. The instrument data will populate in the text boxes.
3. Profit/loss and Net Position will populate in the text boxes.


XTAPI Objects
-------------------------------------------------------------------------------

TTDropHandler
TTInstrNotify
TTInstrObj
TTOrderSet


Revisions
-------------------------------------------------------------------------------

Version:		1.2.0
Date Created:	01/09/2009
Notes:			Added conformance/production warning on the form.  Added more
				descriptive comments and general source cleanup.

Version:		1.2.1
Date Created:	01/16/2013
Notes:			Updated for GitHub.