#ifndef __COMMON_COMMAND_LINE_H__
#define __COMMON_COMMAND_LINE_H__

#include <string>
#include <map>
#include <sstream>

#include "exceptions.h"

namespace Common
{

  DECLARE_RUNTIME_EXCEPTION(CommandLine)
  
  class CommandLine
  {
  public:
    CommandLine(int argc, char const **argv);
    std::string const& GetProgramName() const;
    std::string GetStrParameter(std::string const &name) const;
    template <typename T>
    T GetParameter(std::string const &name) const
    {
      std::stringstream Stream;
      Stream << GetStrParameter(name);
      T Value = T();
      Stream >> Value;
      return Value;
    }
    
  private:
    std::string ProgramName;
    typedef std::map<std::string, std::string> ParamPool;
    ParamPool Params;
  };
  
}

#endif  // !__COMMON_COMMAND_LINE_H__
