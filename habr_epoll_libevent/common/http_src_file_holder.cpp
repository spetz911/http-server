#include "http_src_file_holder.h"
#include "http_status_code.h"

namespace Network
{
  
  namespace Proto
  {
    
    namespace Http
    {
      
      HttpSrcFileHolder::HttpSrcFileHolder(std::string const &fileName)
      try
        : System::FileHolder(fileName)
      {
      }
      catch (System::FileHolderException const &)
      {
        throw HttpSrcFileHolderException("Failed to open file", statNotFound);
      }
      
      int HttpSrcFileHolder::GetHandle() const
      {
        return System::FileHolder::GetHandle();
      }
      
      unsigned HttpSrcFileHolder::GetFileSize() const
      {
        return static_cast<unsigned>(System::FileHolder::GetSize());
      }
      
    }
    
  }
  
}
