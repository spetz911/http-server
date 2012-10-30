#include "http_file_sender.h"

namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {

      HttpFileSender::HttpFileSender(IConnectionCtrl *ctrl, std::string const &resourceName)
        : HttpResponse(ctrl)
        , File(resourceName)
        , HeaderHasBeenSent(false)
        , FileOffset(0)
      {
        SetResourceName(resourceName);
        SetContentLength(File.GetFileSize());
      }
      
      bool HttpFileSender::Send()
      {
        if (!HeaderHasBeenSent)
          HeaderHasBeenSent = !HttpResponse::Send();
        unsigned Bytes = File.GetFileSize() - FileOffset;
        if (!Bytes)
          return false;
        if (GetCtrl()->SendFile(File.GetHandle(), FileOffset, &Bytes))
          return false;
        FileOffset += Bytes;
        if (FileOffset >= File.GetFileSize())
          return false;
        return true;
      }
      
    }
    
  }
  
}
