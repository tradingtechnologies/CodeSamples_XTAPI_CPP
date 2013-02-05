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

#include "../stdafx.h"
#include "OrderSetNotifySink.h"

/* Create the instance of the TTOrderSet
 */
COrderSetNotifySink::COrderSetNotifySink()
{
	// clear out our callback till we pass it in
	m_pCallback=NULL;

	// create the notification object
	HRESULT hr=m_pOrderSetObj.CreateInstance(__uuidof(XTAPI::TTOrderSet));

	if (hr==S_OK)
	{
		// created OK, connect to it
		hr=DispEventAdvise(m_pOrderSetObj);
	}
}

/* Destroy the TTOrderSet 
 */
COrderSetNotifySink::~COrderSetNotifySink()
{
	if (m_pOrderSetObj)
	{
		// remove our connection point
		DispEventUnadvise(m_pOrderSetObj);
	}

	// de-ref the SRC
	m_pOrderSetObj=NULL;
}

/* Set the callback mechanism
 */
void COrderSetNotifySink::SetCallback(CNotify*pCallback)
{
	// save the callback pointer
	m_pCallback=pCallback;
}

/* Assign the OnOrderSetUpdate event
 */
HRESULT COrderSetNotifySink::OnOrderSetUpdate(XTAPI::ITTOrderSet* pOrderSet)
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bUpdate)
	{
		// pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_Update(pOrderSet);
	}
	return S_OK;
}

/* Assign the OnOrderFillData event
 */
HRESULT COrderSetNotifySink::OnOrderFillData(XTAPI::ITTFillObj* pFillData)
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bFills)
	{
		// pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_FillData(pFillData);
	}
	return S_OK;
}

/* Assign the OnOrderFillBlockStart event
 */
HRESULT COrderSetNotifySink::OnOrderFillBlockStart()
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bFills)
	{
		// pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_FillBlockStart();
	}
	return S_OK;
}

/* Assign the OnOrderFillBlockEnd event
 */
HRESULT COrderSetNotifySink::OnOrderFillBlockEnd()
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bFills)
	{
		// pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_FillBlockEnd();
	}
	return S_OK;
}

/* Assign the OnOrderRejected event
 */
HRESULT	COrderSetNotifySink::OnOrderRejected(XTAPI::ITTOrderObj *pRejectedOrderObj)
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bUpdate)
	{
		//pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_OnOrderRejected(pRejectedOrderObj);
	}
	return S_OK;
}

/* Assign the OnOrderSubmitted event
 */
HRESULT COrderSetNotifySink::OnOrderSubmitted(XTAPI::ITTOrderObj *pNewOrderObj, XTAPI::ITTOrderObj *pOldOrderObj, BSTR sSiteOrderKey, XTAPI::enumOrderAction eOrderAction, long wrkQty, BSTR sOrderType, BSTR sOrderTraits)
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bUpdate)
	{
		//pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_OnOrderSubmitted(pNewOrderObj, pOldOrderObj, sSiteOrderKey, eOrderAction, wrkQty, sOrderType, sOrderTraits);
	}
	return S_OK;
}

/* Assign the OnOrderHeld event
 */
HRESULT COrderSetNotifySink::OnOrderHeld(XTAPI::ITTOrderObj *pNewOrderObj, XTAPI::ITTOrderObj *pOldOrderObj, BSTR sSiteOrderKey, XTAPI::enumOrderAction eOrderAction, long ordQty)
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bUpdate)
	{
		//pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_OnOrderHeld(pNewOrderObj, pOldOrderObj, sSiteOrderKey, eOrderAction, ordQty);
	}
	return S_OK;
}

/* Assign the OnOrderUpdated event
 */
HRESULT COrderSetNotifySink::OnOrderUpdated(XTAPI::ITTOrderObj *pNewOrderObj, XTAPI::ITTOrderObj *pOldOrderObj, BSTR sSiteOrderKey, XTAPI::enumOrderNotifyState eOrderState, XTAPI::enumOrderAction eOrderAction, long updQty, BSTR sOrderType, BSTR sOrderTraits)
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bUpdate)
	{
		//pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_OnOrderUpdated(pNewOrderObj, pOldOrderObj, sSiteOrderKey, eOrderState, eOrderAction, updQty, sOrderType, sOrderTraits);
	}
	return S_OK;
}

/* Assign the OnOrderDeleted event
 */
HRESULT COrderSetNotifySink::OnOrderDeleted(XTAPI::ITTOrderObj *pNewOrderObj, XTAPI::ITTOrderObj *pOldOrderObj, BSTR sSiteOrderKey, XTAPI::enumOrderNotifyState eOrderState, XTAPI::enumOrderAction eOrderAction, long delQty)
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bUpdate)
	{
		//pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_OnOrderDeleted(pNewOrderObj, pOldOrderObj, sSiteOrderKey, eOrderState, eOrderAction, delQty);
	}
	return S_OK;
}

/* Assign the OnOrderFilled event
 */
HRESULT COrderSetNotifySink::OnOrderFilled(XTAPI::ITTOrderObj *pNewOrderObj, XTAPI::ITTOrderObj *pOldOrderObj, BSTR sSiteOrderKey, XTAPI::enumOrderNotifyState eOrderState, long fillQty)
{
	// Connection point callback function
	if (m_pCallback && m_pCallback->m_bUpdate)
	{
		//pass this onto the external event consumer
		m_pCallback->COrderSetNotifySink_OnOrderFilled(pNewOrderObj, pOldOrderObj, sSiteOrderKey, eOrderState, fillQty);
	}
	return S_OK;
}