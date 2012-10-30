#include "http_user_session.h"
#include "logger.h"

namespace Network
{

  namespace Proto
  {
    
    namespace Http
    {

      HttpUserSession::HttpUserSession(std::string const &rootDir, std::string const &defaultPage, bool useCorking)
        : Ctrl(0)
        , RootDir(rootDir)
        , DefaultPage(defaultPage)
        , UseCorking(useCorking)
      {
      }
      
      void HttpUserSession::Init(IConnectionCtrl *ctrl)
      {
        Ctrl = ctrl;
        Ctrl->GetSocketTuner().SetCorking(UseCorking);
      }
      
      void HttpUserSession::Done()
      {
        if (FileSender.get())
          FileSender.reset(0);
        if (HeadResponse.get())
          HeadResponse.reset(0);
        Ctrl = 0;
      }
      
      bool HttpUserSession::IsExpiredSession(std::time_t lastActionTime) const
      {
        return std::difftime(std::time(0), lastActionTime) > SessionTimeout;
      }
      
      unsigned HttpUserSession::GetMaxBufSizeForRead() const
      {
        return MaxBufSizeForRead;
      }
      
      void HttpUserSession::OnRecvData(void const *buf, unsigned bytes)
      {      
        struct
        {
          void operator () (IConnectionCtrl *ctrl, HttpStatusCode code) const
          {
            HttpResponse Response(ctrl);
            Response.SetConnectionType(HttpResponse::conClose);
            Response.SetStatusCode(code);
            Response.CreateResponseHeader();
            while (Response.Send());
          }
        } SendErrorPage;
        try
        {
          AssignData(buf, bytes);
          Ctrl->UpdateSessionTime();
        }
        catch (HttpRequestHeaderException const &e)
        {
          Ctrl->MarkMeForClose();
          SendErrorPage(Ctrl, static_cast<HttpStatusCode>(e.GetCode()));
          throw;
        }
        catch (HttpSrcFileHolderException const &e)
        {
          Ctrl->MarkMeForClose();
          SendErrorPage(Ctrl, static_cast<HttpStatusCode>(e.GetCode()));
          throw;
        }
        catch (std::exception const &)
        {
          Ctrl->MarkMeForClose();
          SendErrorPage(Ctrl, statInternalServerError);
          throw;
        }
      }
      
      void HttpUserSession::OnIdle()
      {
        try
        {
          if (FileSender.get())
          {
            if (!FileSender->Send())
            {
              FileSender.reset(0);
              Ctrl->MarkMeForClose();
            }
          }
          if (HeadResponse.get())
          {
            if (!HeadResponse->Send())
            {
              HeadResponse.reset(0);
              Ctrl->MarkMeForClose();
            }
          }
        }
        catch (std::exception const &e)
        {
          Ctrl->MarkMeForClose();
          Common::Log::GetLogInst() << e.what();
        }
      }
      
      void HttpUserSession::ProcessRequest(HttpRequestHeader const &header, void const *buf, unsigned bytes)
      {
        switch (header.GetMethod())
        {
        case HttpRequestHeader::mtdGet :
            if (HeadResponse.get())
              HeadResponse.reset(0);
            FileSender.reset(new HttpFileSender(Ctrl, CreateResourceName(header.GetResource())));
            FileSender->SetConnectionType(HttpResponse::conClose);
            FileSender->CreateResponseHeader();
          break;
        case HttpRequestHeader::mtdHead :
            if (FileSender.get())
              FileSender.reset(0);
            HeadResponse.reset(new HttpResponse(Ctrl));
            HeadResponse->SetConnectionType(HttpResponse::conClose);
            HeadResponse->CreateResponseHeader();
          break;
        default :
          break;
        }
      }
      
      std::string HttpUserSession::CreateResourceName(std::string const &resource) const
      {
        std::string Res = RootDir;
        if (resource.empty() || resource == "/")
        {
          Res += "/";
          Res += DefaultPage;
        }
        else
          Res += resource;
        return Res;
      }

      IUserSessionPtr CreateHttpUserSession(std::string const &rootDir,
                                            std::string const &defaultPage,
                                            bool useCorking)
      {
        return IUserSessionPtr(new HttpUserSession(rootDir, defaultPage, useCorking));
      }
      
    }
    
  }

}
