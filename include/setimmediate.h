#ifndef _SETIMMEDIATE_NAPI_H
#define _SETIMMEDIATE_NAPI_H

#include <napi.h>
#include <uv.h>
#include <memory>
#include <get-uv-event-loop-napi.h>

template<typename T>
void SetImmediate(napi_env env, T&& cb) {
  struct SetImmediateData {
    explicit inline SetImmediateData(T&& cb) : callback(std::move(cb)) {}
    T callback;
    // uv_check_t + uv_idle_t is what Node.js itself does.
    uv_check_t check;
    uv_idle_t idle;
  };

  uv_loop_t* loop = napi__get_uv_event_loop(env);
  SetImmediateData* data = new SetImmediateData(std::move(cb));
  data->check.data = static_cast<void*>(data);
  data->idle.data = static_cast<void*>(data);

  uv_check_init(loop, &data->check);
  uv_idle_init(loop, &data->idle);
  uv_idle_start(&data->idle, [](uv_idle_t* idle) { /* spin the event loop */ });
  uv_check_start(&data->check, [](uv_check_t* check) {
    SetImmediateData* data = static_cast<SetImmediateData*>(check->data);
    T cb = std::move(data->callback);
    uv_close(reinterpret_cast<uv_handle_t*>(&data->check),
             [](uv_handle_t* handle) {
      SetImmediateData* data = static_cast<SetImmediateData*>(handle->data);
      uv_close(reinterpret_cast<uv_handle_t*>(&data->idle),
               [](uv_handle_t* handle) {
        SetImmediateData* data = static_cast<SetImmediateData*>(handle->data);
        delete data;
      });
    });

    Napi::HandleScope scope(env);

    try {
      cb();
    } catch (Napi::Error e) {
      // This is going to crash, but it's not like we really have a choice.
      e.ThrowAsJavaScriptException();
    }
  });
}

#endif