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

    void OnEvent(const Napi::CallbackInfo &info);
    void Evaluate(const Napi::CallbackInfo &info);
    Napi::Value GetTitle(const Napi::CallbackInfo &info);
    void SetBackgroundColor(const Napi::CallbackInfo &info);
    void SetCustomUserAgent(const Napi::CallbackInfo &info);
    void OnResize(const Napi::CallbackInfo &info);
    Napi::Value GetAdditionalInfo(const Napi::CallbackInfo &info);
    Napi::Value IsDocumentReady(const Napi::CallbackInfo &info);

    webview::webview window_;

    void ThrowTypeError(const Napi::Env& env, const std::string& message);
    void CheckArguments(const Napi::CallbackInfo& info, size_t expected, const std::string& message);
    std::string GetStringArgument(const Napi::CallbackInfo& info, size_t index);
    int GetNumberArgument(const Napi::CallbackInfo& info, size_t index);
};

Napi::FunctionReference Intermask::constructor;

Napi::Object Intermask::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env,
        "Intermask",
        {
            InstanceMethod("setTitle", &Intermask::SetTitle),
            InstanceMethod("setSize", &Intermask::SetSize),
            InstanceMethod("navigate", &Intermask::Navigate),
            InstanceMethod("run", &Intermask::Run),
            InstanceMethod("destroy", &Intermask::Destroy)
        });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Intermask", func);
    return exports;
}

Intermask::Intermask(const Napi::CallbackInfo &info) 
    : Napi::ObjectWrap<Intermask>(info), window_(true, nullptr) 
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    CheckArguments(info, 0, "It does not take any arguments");
}

void Intermask::ThrowTypeError(const Napi::Env& env, const std::string& message)
{
    Napi::TypeError::New(env, message).ThrowAsJavaScriptException();
}

void Intermask::CheckArguments(const Napi::CallbackInfo& info, size_t expected, const std::string& message)
{
    if (info.Length() != expected)
    {
        ThrowTypeError(info.Env(), message);
    }
}

std::string Intermask::GetStringArgument(const Napi::CallbackInfo& info, size_t index)
{
    CheckArguments(info, index + 1, "Missing argument at index " + std::to_string(index));
    if (!info[index].IsString())
    {
        ThrowTypeError(info.Env(), "Argument at index " + std::to_string(index) + " should be string");
    }
    return info[index].ToString().Utf8Value();
}

int Intermask::GetNumberArgument(const Napi::CallbackInfo& info, size_t index)
{
    CheckArguments(info, index + 1, "Missing argument at index " + std::to_string(index));
    if (!info[index].IsNumber())
    {
        ThrowTypeError(info.Env(), "Argument at index " + std::to_string(index) + " should be number");
    }
    return static_cast<int>(info[index].As<Napi::Number>().Int64Value());
}

void Intermask::SetTitle(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    std::string title = GetStringArgument(info, 0);
    window_.set_title(title.c_str());
}

void Intermask::Navigate(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    std::string url = GetStringArgument(info, 0);
    window_.navigate(url.c_str());
}

void Intermask::SetSize(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int width = GetNumberArgument(info, 0);
    int height = GetNumberArgument(info, 1);
    window_.set_size(width, height, WEBVIEW_HINT_NONE);
}

void Intermask::Run(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    CheckArguments(info, 0, "It does not take any arguments");
    window_.run();
}

void Intermask::Destroy(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    CheckArguments(info, 0, "It does not take any arguments");
    window_.destroy();
}

void Intermask::OnEvent(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() < 1 || !info[0].IsFunction())
    {
        ThrowTypeError(env, "Callback function is required");
    }

    Napi::Function callback = info[0].As<Napi::Function>();
    // Store the callback function to call later when the event occurs
    // Implement event handling logic here
}

void Intermask::Evaluate(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() < 1 || !info[0].IsString())
    {
        ThrowTypeError(env, "JavaScript code must be a string");
    }

    std::string jsCode = GetStringArgument(info, 0);
    window_.eval(jsCode.c_str());
}


Napi::Value Intermask::GetTitle(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    std::string title = window_.get_title();
    return Napi::String::New(env, title);
}

void Intermask::SetBackgroundColor(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() < 1 || !info[0].IsString())
    {
        ThrowTypeError(env, "Color must be a string");
    }

    std::string color = GetStringArgument(info, 0);
    // Assuming the webview supports setting background color through some API
    window_.set_background_color(color.c_str());
}

void Intermask::SetCustomUserAgent(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() < 1 || !info[0].IsString())
    {
        ThrowTypeError(env, "User agent must be a string");
    }

    std::string userAgent = GetStringArgument(info, 0);
    // Assuming the webview supports setting custom user-agent
    window_.set_user_agent(userAgent.c_str());
}

void Intermask::OnResize(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() < 1 || !info[0].IsFunction())
    {
        ThrowTypeError(env, "Callback function is required");
    }

    Napi::Function callback = info[0].As<Napi::Function>();
    // Store the callback function to call later when the resize event occurs
    // Implement resize handling logic here
}

Napi::Value GetAdditionalInfo(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    // Return additional information about the webview or the application
    // Example: Window dimensions
    int width, height;
    window_.get_size(width, height);
    Napi::Object infoObj = Napi::Object::New(env);
    infoObj.Set("width", Napi::Number::New(env, width));
    infoObj.Set("height", Napi::Number::New(env, height));
    return infoObj;
}

Napi::Value IsDocumentReady(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    // Check if the document is ready (example placeholder, actual implementation needed)
    bool ready = window_.is_document_ready();
    return Napi::Boolean::New(env, ready);
}

#endif