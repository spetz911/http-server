#ifndef __COMMON_FILE_HOLDER_H__
#define __COMMON_FILE_HOLDER_H__

#include "exceptions.h"
#include "non_copyable.h"

#include <string>

namespace System
{
  
  DECLARE_RUNTIME_EXCEPTION(FileHolder)
  
  class FileHolder
    : private Common::NonCopyable
  {
  public:
    FileHolder(std::string const &fileName);
    virtual ~FileHolder();
    void Open(std::string const &fileName);
    int GetHandle() const;
    int GetSize() const;
    int Detach();
    
  private:
    int Handle;
    int Size;
  };
  
}

#endif  // !__COMMON_FILE_HOLDER_H__
