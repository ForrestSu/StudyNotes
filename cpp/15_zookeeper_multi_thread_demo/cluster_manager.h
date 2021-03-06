/**
 * @file cluster_manager.h
 * @brief cluster manager
 * @author shlian(连少华)
 * @version 1.0
 * @date 2019-04-23
 */
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>

#include <zookeeper.h>
#include "cluster_interface.h"

namespace uvframe {
namespace baseutils {

namespace impl {
/**
 * @brief 集群管理器
 */
class CClusterManager final :public baseutils::IClusterManager {
private:
    typedef std::shared_ptr<zhandle_t> zhandle_tPtr;
    class zk_handle_deleter {
    public:
        void operator()(zhandle_t *zk_handle)
        {
            if (zk_handle != nullptr) {
                zookeeper_close(zk_handle);
            }
        }
    };
    /**
     * @brief 运行状态
     */
    enum class EN_State {
        UnKnown = 0, Initialized = 1, Started = 4, Running = 8, Stopped = 16
    };
public:
    explicit CClusterManager();
    virtual ~CClusterManager();

    CClusterManager(const CClusterManager&) = delete;
    CClusterManager(CClusterManager &&) = delete;
    CClusterManager &operator=(const CClusterManager&) = delete;
    CClusterManager &operator=(CClusterManager&&) = delete;

    int Initialize(const std::string &hosts, const std::string &path, const std::string &node, unsigned int timeout) override;
    int Start() override;
    int Register(IEventHandlerPtr eventHandlerPtr) override;
    int Stop() override;
    int DeInitialize() override;

    EN_NodeMode getMode() override;

private:
    std::atomic<EN_State> m_runState;
    std::atomic<EN_NodeMode> m_nodeMode;

    zhandle_tPtr m_zkhandlePtr;
    std::string m_hosts;
    std::string m_tag;
    std::string m_path;
    std::string m_node;
    unsigned int m_timeout;

    std::mutex m_ehMutex;
    std::vector<baseutils::IEventHandlerPtr> m_eventHandlers;

    std::mutex m_initMutex;
    std::mutex m_startMutex;

    void clearEventHandlers();
    void triggerMasterEvent(bool isMaster);

    void onZOO_CHILD_EVENT(zhandle_t *zk_handle, int eventType, int state, const char *path);
    void onZOO_SESSION_EVENT(zhandle_t *zk_handle, int eventType, int state);

    int createNode(const std::string &path, const std::string &nodeName);
    static void watcher(zhandle_t *zk_handle, int eventType, int state, const char *path, void *context);
};
}
typedef std::shared_ptr<impl::CClusterManager> CClusterManagerPtr;

}
}

