README.txt
PriceUpdateManual (C++)
===============================================================================

Overview
-------------------------------------------------------------------------------

This example demonstrates using the XTAPI to retrieve market data from a single 
instrument by manually specifying the contract parameters.


Instructions
-------------------------------------------------------------------------------

1. Enter the Instrument Inforamtion.

Example:
	Exchange:		CME
	Product:		ES
	ProductType:	FUTURE
	Contract:		Mar13
		
2. Click the Connect button.
3. The Instrument Market Data will be populated.


XTAPI Objects
-------------------------------------------------------------------------------

TTInstrNotify
TTInstrObj


Revisions
-------------------------------------------------------------------------------

Version:		1.1.0
Date Created:	06/22/2005
Notes:			Added the OnNotifyNotFound callback so that a MessageBox is 
				generated if the contract is not found.

Version:		1.2.0
Date Created:	12/08/2008
Notes:			Added conformance/production warning on the form.  Added more
				descriptive comments and general source cleanup.

Version:		1.2.1
Date Created:	01/24/2013
Notes:			Updated for GitHub.  Renamed solution from 
				TTPriceUpdateMC to PriceUpdateManual.