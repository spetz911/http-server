#ifndef __COMMON_LOCK_GUARD_H__
#define __COMMON_LOCK_GUARD_H__

namespace Common
{

  template <typename T>
  class LockGuard
    : private NonCopyable
  {
  public:
    LockGuard(T &obj)
      : Obj(obj)
    {
      Obj.Lock();
    }
    ~LockGuard()
    {
      Obj.Unlock();
    }
    
  private:
    T &Obj;
  };

}

#endif  // !__COMMON_LOCK_GUARD_H__
