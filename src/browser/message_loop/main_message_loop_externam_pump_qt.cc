// Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "main_message_loop_external_pump.h"

#include <errno.h>
#include <fcntl.h>
#include <math.h>

#include <memory>

#include <QTimer>
#include <QApplication>
#include <QThread>

#include "include/base/cef_logging.h"
#include "include/cef_app.h"

namespace client {

    namespace {

    class MainMessageLoopExternalPumpQt : public MainMessageLoopExternalPump {
    public:
        MainMessageLoopExternalPumpQt();
        ~MainMessageLoopExternalPumpQt();

        // MainMessageLoopStd methods:
        void Quit() override;
        int Run() override;

        // MainMessageLoopExternalPump methods:
        void OnScheduleMessagePumpWork(int64 delay_ms) override;

        // Internal methods used for processing the pump callbacks. They are public
        // for simplicity but should not be used directly. HandlePrepare is called
        // during the prepare step of glib, and returns a timeout that will be passed
        // to the poll. HandleCheck is called after the poll has completed, and
        // returns whether or not HandleDispatch should be called. HandleDispatch is
        // called if HandleCheck returned true.
        //        int HandlePrepare();
        //        bool HandleCheck();
        //        void HandleDispatch();

    protected:
        // MainMessageLoopExternalPump methods:
        void SetTimer(int64 delay_ms) override;
        void KillTimer() override;
        bool IsTimerPending() override;

    private:
        // Used to flag that the Run() invocation should return ASAP.
        bool should_quit_;

        // The time when we need to do delayed work.
        CefTime delayed_work_time_;
    };

    // Return a timeout suitable for the glib loop, -1 to block forever,
    // 0 to return right away, or a timeout in milliseconds from now.
    int GetTimeIntervalMilliseconds(const CefTime& from) {
        if (from.GetDoubleT() == 0.0)
            return -1;

        CefTime now;
        now.Now();

        // Be careful here. CefTime has a precision of microseconds, but we want a
        // value in milliseconds. If there are 5.5ms left, should the delay be 5 or
        // 6?  It should be 6 to avoid executing delayed work too early.
        int delay =
            static_cast<int>(ceil((from.GetDoubleT() - now.GetDoubleT()) * 1000.0));

        // If this value is negative, then we need to run delayed work soon.
        return delay < 0 ? 0 : delay;
    }

    MainMessageLoopExternalPumpQt::MainMessageLoopExternalPumpQt()
        : should_quit_(false)
    {
        auto timer = new QTimer(qApp);
        QObject::connect(timer, &QTimer::timeout, []
                         {
            for (int i = 0; i < 10; ++i) {
                // Do some work.
                CefDoMessageLoopWork();
            }
        });
        timer->start(28);
    }

    MainMessageLoopExternalPumpQt::~MainMessageLoopExternalPumpQt() {

    }

    void MainMessageLoopExternalPumpQt::Quit() {
        should_quit_ = true;
    }

    int MainMessageLoopExternalPumpQt::Run() {
        // We really only do a single task for each iteration of the loop. If we
        // have done something, assume there is likely something more to do. This
        // will mean that we don't block on the message pump until there was nothing
        // more to do. We also set this to true to make sure not to block on the
        // first iteration of the loop.
        bool more_work_is_plausible = true;

        // We run our own loop instead of using g_main_loop_quit in one of the
        // callbacks. This is so we only quit our own loops, and we don't quit
        // nested loops run by others.
    //    for (;;) {
    //        // Don't block if we think we have more work to do.
    //        bool block = !more_work_is_plausible;

    //        more_work_is_plausible = g_main_context_iteration(context_, block);
    //        if (should_quit_)
    //            break;
    //    }

        // We need to run the message pump until it is idle. However we don't have
        // that information here so we run the message loop "for a while".
        for (int i = 0; i < 10; ++i) {
            // Do some work.
            CefDoMessageLoopWork();

            // Sleep to allow the CEF proc to do work.
            QThread::usleep(50000);
        }

        return 0;
    }

    void MainMessageLoopExternalPumpQt::OnScheduleMessagePumpWork(
        int64 delay_ms) {
        // TODO
    }

    void MainMessageLoopExternalPumpQt::SetTimer(int64 delay_ms) {
        DCHECK_GT(delay_ms, 0);

        CefTime now;
        now.Now();

        delayed_work_time_ =
            CefTime(now.GetDoubleT() + static_cast<double>(delay_ms) / 1000.0);
    }

    void MainMessageLoopExternalPumpQt::KillTimer() {
        delayed_work_time_ = CefTime();
    }

    bool MainMessageLoopExternalPumpQt::IsTimerPending() {
        return GetTimeIntervalMilliseconds(delayed_work_time_) > 0;
    }

    }  // namespace

    // static
    scoped_ptr<MainMessageLoopExternalPump> MainMessageLoopExternalPump::Create() {
      return scoped_ptr<MainMessageLoopExternalPump>(
          new MainMessageLoopExternalPumpQt());
    }

}  // namespace client
