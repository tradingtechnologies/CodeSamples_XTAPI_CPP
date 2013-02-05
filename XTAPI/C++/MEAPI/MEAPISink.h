/***************************************************************************
 *    
 *      Copyright (c) 2012 Trading Technologies International, Inc.
 *                     All Rights Reserved Worldwide
 *
 *        * * *   S T R I C T L Y   P R O P R I E T A R Y   * * *
 *
 * WARNING:  This file is the confidential property of Trading Technologies
 * International, Inc. and is to be maintained in strict confidence.  For
 * use only by those with the express written permission and license from
 * Trading Technologies International, Inc.  Unauthorized reproduction,
 * distribution, use or disclosure of this file or any program (or document)
 * derived from it is prohibited by State and Federal law, and by local law
 * outside of the U.S. 
 *
 ***************************************************************************/

#pragma once
#ifndef MEAPISINK_H
#define MEAPISINK_H

class CMeapiSink;

typedef IDispEventImpl<0, CMeapiSink, &MEAPI::DIID_ITTMarketExplorerEvents, &MEAPI::LIBID_MEAPI, 1, 0> CMarketExplorerEventsSink;
typedef IDispEventImpl<1, CMeapiSink, &MEAPI::DIID_ITTGatewayEvents, &MEAPI::LIBID_MEAPI, 1, 0> CGatewayEventsSink;
typedef IDispEventImpl<2, CMeapiSink, &MEAPI::DIID_ITTProductEvents, &MEAPI::LIBID_MEAPI, 1, 0> CProductEventsSink;

class CMeapiSink : public CMarketExplorerEventsSink, public CGatewayEventsSink, public CProductEventsSink
{
private:
	MEAPI::ITTMarketExplorerPtr m_pMarketExplorerObj;

public:
	CMeapiSink();
	virtual ~CMeapiSink();
	
	MEAPI::ITTMarketExplorerPtr& Obj()
	{
		return m_pMarketExplorerObj;
	}

	BEGIN_SINK_MAP(CMeapiSink)
		// ITTMarketExplorerEvents
		SINK_ENTRY_EX(0, MEAPI::DIID_ITTMarketExplorerEvents, 1, OnGateway)

		// ITTGatewayEvents
		SINK_ENTRY_EX(1, MEAPI::DIID_ITTGatewayEvents, 1, OnGatewayAttributes)
		SINK_ENTRY_EX(1, MEAPI::DIID_ITTGatewayEvents, 2, OnProducts)
		SINK_ENTRY_EX(1, MEAPI::DIID_ITTGatewayEvents, 3, OnRequestProductsTimeout)
		SINK_ENTRY_EX(1, MEAPI::DIID_ITTGatewayEvents, 4, OnServerStatus)
		SINK_ENTRY_EX(1, MEAPI::DIID_ITTGatewayEvents, 5, OnProductTypes)
		SINK_ENTRY_EX(1, MEAPI::DIID_ITTGatewayEvents, 6, OnNewProduct)
		SINK_ENTRY_EX(1, MEAPI::DIID_ITTGatewayEvents, 7, OnNewContract)

		// ITTProductEvents
		SINK_ENTRY_EX(2, MEAPI::DIID_ITTProductEvents, 1, OnContracts)
		SINK_ENTRY_EX(2, MEAPI::DIID_ITTProductEvents, 2, OnNewContractA)
		SINK_ENTRY_EX(2, MEAPI::DIID_ITTProductEvents, 3, OnRequestContractsTimeout)
	END_SINK_MAP()

	// ITTMarketExplorerEvents
	HRESULT __stdcall OnGateway(MEAPI::ITTGateway* pGateway);

	// ITTGatewayEvents
	HRESULT __stdcall OnGatewayAttributes(MEAPI::ITTGateway* pGateway, BSTR attributeList, MEAPI::enumServiceType service);
	HRESULT __stdcall OnProducts(MEAPI::ITTGateway* pGateway, MEAPI::ITTProductCollection* products);
	HRESULT __stdcall OnRequestProductsTimeout(MEAPI::ITTGateway* pGateway);
	HRESULT __stdcall OnServerStatus(MEAPI::ITTGateway* pGateway, MEAPI::enumServiceType service, long isAvailable, long isLoggedIn);
	HRESULT __stdcall OnProductTypes(MEAPI::ITTGateway* pGateway, VARIANT productTypes, VARIANT productTypesAsString);
	HRESULT __stdcall OnNewProduct(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct);
	HRESULT __stdcall OnNewContract(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractInfo* pContractInfo);

	// ITTProductEvents
	HRESULT __stdcall OnContracts(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractCollection* contracts);
	HRESULT __stdcall OnNewContractA(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractInfo* pContractInfo);
	HRESULT __stdcall OnRequestContractsTimeout(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct);
	
protected:
	// ITTMarketExplorerEvents
	virtual void CMeapiSink_OnGateway(MEAPI::ITTGateway* pGateway) {};

	// ITTGatewayEvents
	virtual void CMeapiSink_OnGatewayAttributes(MEAPI::ITTGateway* pGateway, BSTR attributeList, MEAPI::enumServiceType service) {};
	virtual void CMeapiSink_OnProducts(MEAPI::ITTGateway* pGateway, MEAPI::ITTProductCollection* products) {};
	virtual void CMeapiSink_OnRequestProductsTimeout(MEAPI::ITTGateway* pGateway) {};
	virtual void CMeapiSink_OnServerStatus(MEAPI::ITTGateway* pGateway, MEAPI::enumServiceType service, long isAvailable, long isLoggedIn) {};
	virtual void CMeapiSink_OnProductTypes(MEAPI::ITTGateway* pGateway, VARIANT productTypes, VARIANT productTypesAsString) {};
	virtual void CMeapiSink_OnNewProduct(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct) {};
	virtual void CMeapiSink_OnNewContract(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractInfo* pContractInfo) {};

	// ITTProductEvents
	virtual void CMeapiSink_OnContracts(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractCollection* contracts) {};
	virtual void CMeapiSink_OnNewContractA(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractInfo* pContractInfo) {};
	virtual void CMeapiSink_OnRequestContractsTimeout(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct) {};
};

#endif /* MEAPISINK_H */