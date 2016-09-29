/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/CClientDisplayManager.cpp
*  PURPOSE:     Text display manager class
*  DEVELOPERS:  Jax <>
*
*****************************************************************************/

#include "StdInc.h"

using std::list;

CClientDisplayManager::CClientDisplayManager ( void )
{
    // Init
    m_bCanRemoveFromList = true;
}


CClientDisplayManager::~CClientDisplayManager ( void )
{
    RemoveAll ();
}


CClientDisplay* CClientDisplayManager::Get ( unsigned long ulID )
{
    // Find the display with the given id
    for (auto& iter : m_List) {
        if ( iter->GetID() == ulID ) {
            return iter;
        }
    }
    return nullptr;
}


void CClientDisplayManager::DrawText2D ( const char* szCaption, const CVector& vecPosition, float fScale, RGBA rgbaColor )
{
    // Grab the resolution as a float
    float fResWidth = static_cast < float > ( g_pCore->GetGraphics ()->GetViewportWidth () );
    float fResHeight = static_cast < float > ( g_pCore->GetGraphics ()->GetViewportHeight () );

    // Render
    g_pCore->GetGraphics ()->DrawText ( static_cast < int > ( vecPosition.fX * fResWidth ),
                                        static_cast < int > ( vecPosition.fY * fResHeight ),
                                        static_cast < int > ( fResWidth ),
                                        static_cast < int > ( fResHeight ),
                                        rgbaColor, szCaption, fScale, fScale, 0 );
}


void CClientDisplayManager::AddToList ( CClientDisplay* pDisplay )
{
    m_List.push_back ( pDisplay );
}


void CClientDisplayManager::RemoveAll ( void )
{
    // Delete all the items in the list
    m_bCanRemoveFromList = false;
    for ( auto& pDisplay : m_List )
        delete pDisplay;

    // Clear the list
    m_List.clear ();
    m_bCanRemoveFromList = true;
}

void CClientDisplayManager::RemoveFromList ( CClientDisplay* pDisplay )
{
    if ( m_bCanRemoveFromList )
    {
        if ( !m_List.empty() )
        {
            m_List.remove ( pDisplay );
        }
    }
}


void CClientDisplayManager::DoPulse ( void )
{
    // Render all our displays
    m_bCanRemoveFromList = false;
    auto iter  = m_List.begin ();
    while ( iter != m_List.end () )
    {
        CClientDisplay* pObject = *iter;
        if ( pObject->IsExpired () )
        {
            // Delete it and remove it from the list
            delete pObject;
            iter = m_List.erase ( iter );
        }
        else
        {
            ++iter;
            pObject->Render ();
        }
    }

    m_bCanRemoveFromList = true;
}