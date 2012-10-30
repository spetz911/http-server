#ifndef __COMMON_HTTP_USER_SESSION_H__
#define __COMMON_HTTP_USER_SESSION_H__

#include "iuser_session.h"
#include "iconnection_ctrl.h"
#include "http_request_reader.h"
#include "http_request_header.h"
#include "http_response.h"
#include "http_file_sender.h"

#include <string>
#include <memory>
#include <ctime>

namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {
      
      class HttpUserSession
        : public IUserSession
        , protected HttpRequestReader
      {
      public:
        HttpUserSession(std::string const &rootDir, std::string const &defaultPage, bool useCorking);
        virtual void Init(IConnectionCtrl *ctrl);
        virtual void Done();
        virtual bool IsExpiredSession(std::time_t lastActionTime) const;
        virtual unsigned GetMaxBufSizeForRead() const;
        virtual void OnRecvData(void const *buf, unsigned bytes);
        virtual void OnIdle();
        virtual void ProcessRequest(HttpRequestHeader const &header, void const *buf, unsigned bytes);
        
      private:
        enum { MaxBufSizeForRead = 4096 };
        enum { SessionTimeout = 3 };
        IConnectionCtrl *Ctrl;
        std::string RootDir;
        std::string DefaultPage;
        bool UseCorking;
        std::auto_ptr<HttpFileSender> FileSender;
        std::auto_ptr<HttpResponse> HeadResponse;
        
        std::string CreateResourceName(std::string const &resource) const;
      };
      
      IUserSessionPtr CreateHttpUserSession(std::string const &rootDir,
        std::string const &defaultPage, bool useCorking);
      
    }
    
  }
  
}

#endif  // !__COMMON_HTTP_USER_SESSION_H__
