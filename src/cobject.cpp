#include "cobject.h"

CObject::~CObject() {
    disconnectAll();
}

void CObject::addConnection(CEventBase* event, size_t connectionId) {
    m_connections.push_back({event, connectionId});
}

void CObject::disconnectAll() {
    for (auto& conn : m_connections) {
        if (conn.event) {
            conn.event->disconnect(conn.connectionId);
        }
    }
    m_connections.clear();
}

void CObject::disconnect(CEventBase *p_event, size_t id){
    for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
        if (it->connectionId == id && it->event == p_event) {
            delete &(*it);
            break;
        }
    }
}