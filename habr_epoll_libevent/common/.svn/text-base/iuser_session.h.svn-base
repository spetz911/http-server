#ifndef __COMMON_IUSER_SESSION_H__
#define __COMMON_IUSER_SESSION_H__

#include "iconnection_ctrl.h"

#include <ctime>

#include <tr1/memory>

namespace Network
{

  struct IUserSession
  {
    virtual ~IUserSession()
    {
    }
    virtual void Init(IConnectionCtrl *ctrl) = 0;
    virtual void Done() = 0;
    virtual unsigned GetMaxBufSizeForRead() const = 0;
    virtual bool IsExpiredSession(std::time_t lastActionTime) const = 0;
    virtual void OnRecvData(void const *buf, unsigned bytes) = 0;
    virtual void OnIdle() = 0;
  };

  typedef std::tr1::shared_ptr<IUserSession> IUserSessionPtr;

}

#endif  // !__COMMON_IUSER_SESSION_H__
