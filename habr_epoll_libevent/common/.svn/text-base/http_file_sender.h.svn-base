#ifndef __COMMON_HTTP_FILE_SENDER_H__
#define __COMMON_HTTP_FILE_SENDER_H__

#include "exceptions.h"
#include "http_response.h"
#include "http_src_file_holder.h"
#include "iconnection_ctrl.h"

#include <string>

namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {
      
      DECLARE_RUNTIME_EXCEPTION(HttpFileSender)
      
      class HttpFileSender
        : public HttpResponse
      {
      public:
        HttpFileSender(IConnectionCtrl *ctrl, std::string const &resourceName);
        bool Send();
        
      private:
        HttpSrcFileHolder File;
        bool HeaderHasBeenSent;
        unsigned FileOffset;
      };
      
    }
    
  }
  
}

#endif  // !__COMMON_HTTP_FILE_SENDER_H__
