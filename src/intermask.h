#ifndef INTERMASK_H
#define INTERMASK_H

#include "webview.h"
#include <napi.h>

class Intermask : public Napi::ObjectWrap<Intermask>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Intermask(const Napi::CallbackInfo &info);

private:
  static Napi::FunctionReference constructor;

  void SetTitle(const Napi::CallbackInfo &info);
  void SetSize(const Napi::CallbackInfo &info);
  void Navigate(const Napi::CallbackInfo &info);
  void Run(const Napi::CallbackInfo &info);
  void Destroy(const Napi::CallbackInfo &info);

  webview::webview window_;
};


Napi::FunctionReference Intermask::constructor;

Napi::Object Intermask::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env,
                  "Intermask",
                  {InstanceMethod("setTitle", &Intermask::SetTitle),
                   InstanceMethod("setSize", &Intermask::SetSize),
                   InstanceMethod("navigate", &Intermask::Navigate),
                   InstanceMethod("run", &Intermask::Run),
                   InstanceMethod("destroy", &Intermask::Destroy)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Intermask", func);
  return exports;
}

Intermask::Intermask(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Intermask>(info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() > 0)
  {
    Napi::TypeError::New(env, "It does not take any arguments")
        .ThrowAsJavaScriptException();
  }

  webview::webview window(true, nullptr);
  this->window_ = window;
}

void Intermask::SetTitle(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() < 1)
  {

    Napi::TypeError::New(env, "You have to supply title")
        .ThrowAsJavaScriptException();
  }

  if (!info[0].IsString())
  {
    Napi::TypeError::New(env, "title should be string").ThrowAsJavaScriptException();
  }

  std::string str = info[0].ToString().Utf8Value();
  const char *title = str.c_str();
  this->window_.set_title(title);
}

void Intermask::Navigate(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() < 1)
  {

    Napi::TypeError::New(env, "You have to supply URL")
        .ThrowAsJavaScriptException();
  }

  if (!info[0].IsString())
  {
    Napi::TypeError::New(env, "URL should be string").ThrowAsJavaScriptException();
  }

  std::string str = info[0].ToString().Utf8Value();
  const char *url = str.c_str();
  this->window_.navigate(url);
}

void Intermask::SetSize(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() < 2)
  {
    Napi::TypeError::New(env, "You need to give width and height")
        .ThrowAsJavaScriptException();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber())
  {
    Napi::TypeError::New(env, "width and height should be numbers").ThrowAsJavaScriptException();
  }

  int width = (int)info[0].As<Napi::Number>().Int64Value();
  int height = info[1].As<Napi::Number>().Int64Value();
  this->window_.set_size(width, height, WEBVIEW_HINT_NONE);
}

void Intermask::Run(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() > 0)
  {
    Napi::TypeError::New(env, "It does not take any arguments")
        .ThrowAsJavaScriptException();
  }

  this->window_.run();
}

void Intermask::Destroy(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() > 0)
  {
    Napi::TypeError::New(env, "It does not take any arguments")
        .ThrowAsJavaScriptException();
  }

  this->window_.destroy();
}


#endif