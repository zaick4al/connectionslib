#ifndef CONLIB_COBJECT_H
#define CONLIB_COBJECT_H

#include <vector>

#pragma once
#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>

class CEventBase;

class CObject {
public:
    explicit CObject() {};
    virtual ~CObject();

    CObject(const CObject&) = delete;
    CObject& operator=(const CObject&) = delete;

protected:
    template<typename... Args>
    friend class CEvent;

    void addConnection(CEventBase* p_event, size_t p_connectionId);
    
    void disconnectAll();

    void disconnect(CEventBase *p_event, size_t p_id);

private:
    struct CConnection {
        CEventBase* event;
        size_t connectionId;
    };

    std::vector<CConnection> m_connections;
};

class CEventBase {
public:
    virtual ~CEventBase() = default;
    virtual void disconnect(size_t p_id) = 0;
};

template<typename... Args>
class CEvent : public CEventBase {
public:
    using Callback = std::function<void(Args...)>;
    using CallbackID = size_t;

    ~CEvent() {
        for (auto& [obj, ids] : m_objectConnections) {
            if (auto ptr = obj.lock()) {
                for (auto id : ids) {
                    ptr->disconnect(this, id);
                }
            }
        }
    }

    CallbackID connect(const Callback& p_cb) {
        CallbackID id = m_nextId++;
        m_callbacks[id] = p_cb;
        return id;
    }

    CallbackID connect(std::weak_ptr<CObject> p_obj, Callback p_cb) {
        CallbackID id = connect(p_cb);
        if (auto ptr = p_obj.lock()) {
            ptr->addConnection(this, id);
            m_objectConnections[p_obj].push_back(id);
        }
        return id;
    }

    void disconnect(CallbackID id) override {
        m_callbacks.erase(id);
        // Удаляем из objectConnections...
    }

    void emit(Args... args) {
        for (auto& [id, cb] : m_callbacks) {
            cb(std::forward<Args>(args)...);
        }
    }

private:
    std::unordered_map<CallbackID, Callback> m_callbacks;
    std::unordered_map<std::weak_ptr<CObject>, std::vector<CallbackID>, 
                      std::owner_less<std::weak_ptr<CObject>>> m_objectConnections;
    static inline CallbackID m_nextId = 0;
};

#endif