#ifndef __COMMON_QUEUE_H__
#define __COMMON_QUEUE_H__

#include "exceptions.h"
#include "lock_guard.h"

#include <queue>

#include <tr1/memory>

namespace Network
{

  DECLARE_RUNTIME_EXCEPTION(Queue)
  
  template <typename T, typename TSyn>
  class Queue
  {
  public:
    typedef std::tr1::shared_ptr<T> ItemPtr;
    typedef std::queue<ItemPtr> ItemPool;
    
    Queue(typename ItemPool::size_type maxItemsCount)
      : MaxItemsCount(maxItemsCount)
    {
    }
    void Push(std::auto_ptr<T> item)
    {
      LockGuard Lock(Guard);
      if (Items.size() >= MaxItemsCount)
        throw QueueException("Queue is full");
      ItemPtr Item(item.get());
      item.release();
      Items.push(Item);
    }
    ItemPtr Pop()
    {
      ItemPtr Item;
      {
        LockGuard Lock(Guard);
        if (!Items.empty())
        {
          Item = Items.front();
          Items.pop();
        }
      }
      return Item;
    }
    
  private:
    typedef Common::LockGuard<TSyn> LockGuard;
    mutable TSyn Guard;
    typename ItemPool::size_type MaxItemsCount;
    ItemPool Items;
  };
  
}

#endif  // !__COMMON_QUEUE_H__
