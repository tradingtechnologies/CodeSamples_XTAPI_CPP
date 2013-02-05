README.txt
SubmitOrder (C++)
===============================================================================


Overview
-------------------------------------------------------------------------------

This example demonstrates using the XTAPI to submit an order.  The order types
available in the application are market, limit, stop market and stop limit.  


Instructions
-------------------------------------------------------------------------------

1. Drag and drop a contract from the X_TRADER Market Grid.
2. The instrument properties and market data will be populated and the order
   entry fields enabled.
3. Select the customer the order is to be submitted under (Note: These are 
   taken from X_TRADER | Settings | Customer Defaults).
4. Select the order type.
5. Text boxes will be enabled based on the selected order type.
6. Enter a price, quantity and stop price if applicable.
7. Click the buy or sell button.
8. The quantity submitted will be printed in the right text box.


XTAPI Objects
-------------------------------------------------------------------------------

TTDropHandler
TTInstrNotify
TTInstrObj
TTOrderSet
TTOrderProfile


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
Date Created:	01/15/2013
Notes:			Updated for GitHub.  