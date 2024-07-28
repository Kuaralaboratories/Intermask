#include <napi.h>
#include "intermask.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
  return Intermask::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)