README.txt
ModifyOrder (C++)
===============================================================================

Overview
-------------------------------------------------------------------------------

This example demonstrates using the XTAPI to modify an order.  Modifications 
include change, cancel/replace, delete last order, delete all orders and delete
a specified range of orders.

Note:	Deleting all orders or deleting a range of orders can include orders 
		placed outside of the XTAPI application.


Instructions
-------------------------------------------------------------------------------

1. Drag and drop a contract from the X_TRADER Market Grid.
2. The instrument properties and market data will be populated and the order
   entry fields enabled.
3. To place an order select the Customer (Note: These are taken from X_TRADER 
   | Settings | Customer Defaults), enter a price and quantity and click the
   "Buy" or "Sell" button.  A limit order will be submitted and the 
   SiteOrderKey will be populated in the "Last Order" TextBox.
4. To change or cancel/replace an order, select the option in the "Modify Order"
   GroupBox.  Enter a price and quantity, and click the "Invoke" button.
5. To delete an order, select the delete option in the "Delete Order" GroupBox,
   fill out the appropriate information and click the Invoke button.


XTAPI Objects
-------------------------------------------------------------------------------

TTDropHandler
TTInstrNotify
TTInstrObj
TTOrderSet
TTOrderObj
TTOrderProfile


Revisions
-------------------------------------------------------------------------------

Version:		1.1.0
Date Created:	06/27/2005
Notes:			None

Version:		1.2.0
Date Created:	01/09/2009
Notes:			Added conformance/production warning on the form.  Added more
				descriptive comments and general source cleanup.  Added support 
				for contracts which tick in fractions

Version:		1.2.1
Date Created:	01/30/2013
Notes:			Updated for GitHub.