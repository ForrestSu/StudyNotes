/**
*  自旋锁（spinlock）： 是指当一个线程在获取锁的时候，如果锁已经被其它线程获取，那么该线程将循环等待，然后不断的判断锁是否能够被成功获取，直到获取到锁才会退出循环。
*  获取锁的线程一直处于活跃状态，但是并没有执行任何有效的任务，使用这种锁会造成busy-waiting。
*/
#ifndef SPIN_LOCK_H
#define SPIN_LOCK_H
#include <atomic>


namespace any{

typedef enum {
    Locked, Unlocked
} LockState;
class spin_lock {
public:
    spin_lock() :
            m_state(Unlocked)
    {
    }
    void lock()
    {
        while (m_state.exchange(Locked, std::memory_order_acquire) == Locked) {
            /* busy-wait */
        }
    }
    void unlock()
    {
        m_state.store(Unlocked, std::memory_order_release);
    }
private:
    std::atomic_bool m_state;
};
}

#endif
