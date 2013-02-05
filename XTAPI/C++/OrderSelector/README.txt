README.txt
OrderSelector (C++)
===============================================================================


Overview
-------------------------------------------------------------------------------

This example demonstrates using the XTAPI to filter the order updates using the
TTOrderSelector object.  

Note:	Separate TTOrderSet objects are created for submitting orders and
		receiving order updates.  This is the recommended practice, as orders 
		sent on a filtered TTOrderSet must match the filter, or will be 
		rejected.


Instructions
-------------------------------------------------------------------------------

1. Drag and drop an Instrument from the Market Grid in X_TRADER.
2. When the contract has loaded, the instrument properties will be populated.
3. To place an order select the Customer (Note: These are taken from X_TRADER 
   | Settings | Customer Defaults), enter a price and quantity and click the
   "Buy" or "Sell" button.  A limit order will be submitted.
4. Select the Order Selector items to filter.
5. Click the "Set Selector" button.
6. Orders that match the filter criteria will be displayed in the text box.

Note:	"All Matches Required" places the boolean AND operator on all items.
		"Allow Any Matches" places the boolean OR operator on all items.



XTAPI Objects
-------------------------------------------------------------------------------

TTDropHandler
TTInstrNotify
TTInstrObj
TTOrderSet
TTOrderProfile
TTOrderSelector
TTOrderTracker


Revisions
-------------------------------------------------------------------------------

Version:		1.1.0
Date Created:	06/27/2005
Notes:			None

Version:		1.2.0
Date Created:	01/09/2009
Notes:			Added conformance/production warning on the form.  Added more
				descriptive comments and general source cleanup.

Version:		1.2.1
Date Created:	01/15/2013
Notes:			Updated for GitHub.