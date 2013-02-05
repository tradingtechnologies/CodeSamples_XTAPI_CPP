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

#include "stdafx.h"
#include "MEAPISink.h"

CMeapiSink::CMeapiSink()
{
	m_pMarketExplorerObj.CreateInstance(__uuidof(MEAPI::TTMarketExplorer));
}

CMeapiSink::~CMeapiSink()
{
	if (m_pMarketExplorerObj)
	{
		// TODO : Unregister event handlers??
	}

	m_pMarketExplorerObj = NULL;
}

HRESULT CMeapiSink::OnGateway(MEAPI::ITTGateway* pGateway)
{
	this->CMeapiSink_OnGateway(pGateway);

	return S_OK;
}

HRESULT CMeapiSink::OnGatewayAttributes(MEAPI::ITTGateway* pGateway, BSTR attributeList, MEAPI::enumServiceType service)
{
	this->CMeapiSink_OnGatewayAttributes(pGateway, attributeList, service);

	return S_OK;
}

HRESULT CMeapiSink::OnProducts(MEAPI::ITTGateway* pGateway, MEAPI::ITTProductCollection* products)
{
	this->CMeapiSink_OnProducts(pGateway, products);

	return S_OK;
}

HRESULT CMeapiSink::OnRequestProductsTimeout(MEAPI::ITTGateway* pGateway) 
{
	this->CMeapiSink_OnRequestProductsTimeout(pGateway);

	return S_OK;
}

HRESULT CMeapiSink::OnServerStatus(MEAPI::ITTGateway* pGateway, MEAPI::enumServiceType service, long isAvailable, long isLoggedIn) 
{
	this->CMeapiSink_OnServerStatus(pGateway, service, isAvailable, isLoggedIn);

	return S_OK;
}

HRESULT CMeapiSink::OnProductTypes(MEAPI::ITTGateway* pGateway, VARIANT productTypes, VARIANT productTypesAsString) 
{
	this->CMeapiSink_OnProductTypes(pGateway, productTypes, productTypesAsString);

	return S_OK;
}

HRESULT CMeapiSink::OnNewProduct(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct) 
{
	this->CMeapiSink_OnNewProduct(pGateway, pProduct);

	return S_OK;
}

HRESULT CMeapiSink::OnNewContract(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractInfo* pContractInfo) 
{
	this->CMeapiSink_OnNewContract(pGateway, pProduct, pContractInfo);

	return S_OK;
}

HRESULT CMeapiSink::OnContracts(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractCollection* contracts) 
{
	this->CMeapiSink_OnContracts(pGateway, pProduct, contracts);

	return S_OK;
}

HRESULT CMeapiSink::OnNewContractA(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct, MEAPI::ITTContractInfo* pContractInfo) 
{
	this->CMeapiSink_OnNewContractA(pGateway, pProduct, pContractInfo);

	return S_OK;
}

HRESULT CMeapiSink::OnRequestContractsTimeout(MEAPI::ITTGateway* pGateway, MEAPI::ITTProduct* pProduct) 
{
	this->CMeapiSink_OnRequestContractsTimeout(pGateway, pProduct);

	return S_OK;
}