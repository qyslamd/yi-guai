#include "qxtglobalshortcut_p.h"
/****************************************************************************
** Copyright (c) 2006 - 2011, the LibQxt project.
** See the Qxt AUTHORS file for a list of authors and copyright holders.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the LibQxt project nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** <http://libqxt.org>  <foundation@libqxt.org>
*****************************************************************************/

#include <qt_windows.h>

bool QxtGlobalShortcutPrivate::nativeEventFilter(const QByteArray & eventType,
    void * message, long * result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);

    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY) {
        const quint32 keycode = HIWORD(msg->lParam);
        const quint32 modifiers = LOWORD(msg->lParam);
        activateShortcut(keycode, modifiers);
    }

    return false;
}


quint32 QxtGlobalShortcutPrivate::nativeModifiers(Qt::KeyboardModifiers modifiers)
{
    // MOD_ALT, MOD_CONTROL, (MOD_KEYUP), MOD_SHIFT, MOD_WIN
    quint32 native = 0;
    if (modifiers & Qt::ShiftModifier)
        native |= MOD_SHIFT;
    if (modifiers & Qt::ControlModifier)
        native |= MOD_CONTROL;
    if (modifiers & Qt::AltModifier)
        native |= MOD_ALT;
    if (modifiers & Qt::MetaModifier)
        native |= MOD_WIN;

    // TODO: resolve these?
    //if (modifiers & Qt::KeypadModifier)
    //if (modifiers & Qt::GroupSwitchModifier)
    return native;
}

quint32 QxtGlobalShortcutPrivate::nativeKeycode(Qt::Key key)
{
    switch (key) {
    case Qt::Key_Escape:    return VK_ESCAPE;
    case Qt::Key_Tab:
    case Qt::Key_Backtab:   return VK_TAB;
    case Qt::Key_Backspace: return VK_BACK;
    case Qt::Key_Return:    return VK_RETURN;
    case Qt::Key_Enter:     return VK_RETURN;
    case Qt::Key_Insert:    return VK_INSERT;
    case Qt::Key_Delete:    return VK_DELETE;
    case Qt::Key_Pause:     return VK_PAUSE;
    case Qt::Key_Print:     return VK_PRINT;
//    case Qt::Key_SysReq:    return key;
    case Qt::Key_Clear:     return VK_CLEAR;
    case Qt::Key_Home:      return VK_HOME;
    case Qt::Key_End:       return VK_END;
    case Qt::Key_Left:      return VK_LEFT;
    case Qt::Key_Up:        return VK_UP;
    case Qt::Key_Right:     return VK_RIGHT;
    case Qt::Key_Down:      return VK_DOWN;
    case Qt::Key_PageUp:    return VK_PRIOR;
    case Qt::Key_PageDown:  return VK_NEXT;
    case Qt::Key_Shift:     return VK_SHIFT;
    case Qt::Key_Control:   return VK_CONTROL;
    case Qt::Key_Meta:      return VK_LWIN; // or VK_RWIN ?
    case Qt::Key_Alt:       return VK_MENU;
    case Qt::Key_CapsLock:  return VK_CAPITAL;
    case Qt::Key_NumLock:   return VK_NUMLOCK;
//    case Qt::Key_ScrollLock:
    case Qt::Key_F1:        return VK_F1;
    case Qt::Key_F2:        return VK_F2;
    case Qt::Key_F3:        return VK_F3;
    case Qt::Key_F4:        return VK_F4;
    case Qt::Key_F5:        return VK_F5;
    case Qt::Key_F6:        return VK_F6;
    case Qt::Key_F7:        return VK_F7;
    case Qt::Key_F8:        return VK_F8;
    case Qt::Key_F9:        return VK_F9;
    case Qt::Key_F10:       return VK_F10;
    case Qt::Key_F11:       return VK_F11;
    case Qt::Key_F12:       return VK_F12;
    case Qt::Key_F13:       return VK_F13;
    case Qt::Key_F14:       return VK_F14;
    case Qt::Key_F15:       return VK_F15;
    case Qt::Key_F16:       return VK_F16;
    case Qt::Key_F17:       return VK_F17;
    case Qt::Key_F18:       return VK_F18;
    case Qt::Key_F19:       return VK_F19;
    case Qt::Key_F20:       return VK_F20;
    case Qt::Key_F21:       return VK_F21;
    case Qt::Key_F22:       return VK_F22;
    case Qt::Key_F23:       return VK_F23;
    case Qt::Key_F24:       return VK_F24;
//        // F25 .. F35 only on X11
//    case Qt::Key_F25:
//        ...
//    case Qt::Key_F35:

    case Qt::Key_Menu:      return VK_MENU;

    case Qt::Key_Help:      return VK_HELP;

    case Qt::Key_Space:     return VK_SPACE; // case Qt::Key_Any

    case Qt::Key_Asterisk:  return VK_MULTIPLY;
    case Qt::Key_Plus:      return VK_ADD;
    case Qt::Key_Comma:     return VK_SEPARATOR;
    case Qt::Key_Minus:     return VK_SUBTRACT;

    case Qt::Key_Slash:     return VK_DIVIDE;

    case Qt::Key_0:         return 0x30;
    case Qt::Key_1:         return 0x31;
    case Qt::Key_2:         return 0x32;
    case Qt::Key_3:         return 0x33;
    case Qt::Key_4:         return 0x34;
    case Qt::Key_5:         return 0x35;
    case Qt::Key_6:         return 0x36;
    case Qt::Key_7:         return 0x37;
    case Qt::Key_8:         return 0x38;
    case Qt::Key_9:         return 0x39;

    case Qt::Key_A:         return 0x41;
    case Qt::Key_B:         return 0x42;
    case Qt::Key_C:         return 0x43;
    case Qt::Key_D:         return 0x44;
    case Qt::Key_E:         return 0x45;
    case Qt::Key_F:         return 0x46;
    case Qt::Key_G:         return 0x47;
    case Qt::Key_H:         return 0x48;
    case Qt::Key_I:         return 0x49;
    case Qt::Key_J:         return 0x4a;
    case Qt::Key_K:         return 0x4b;
    case Qt::Key_L:         return 0x4c;
    case Qt::Key_M:         return 0x4d;
    case Qt::Key_N:         return 0x4e;
    case Qt::Key_O:         return 0x4f;
    case Qt::Key_P:         return 0x50;
    case Qt::Key_Q:         return 0x51;
    case Qt::Key_R:         return 0x52;
    case Qt::Key_S:         return 0x53;
    case Qt::Key_T:         return 0x54;
    case Qt::Key_U:         return 0x55;
    case Qt::Key_V:         return 0x56;
    case Qt::Key_W:         return 0x57;
    case Qt::Key_X:         return 0x58;
    case Qt::Key_Y:         return 0x59;
    case Qt::Key_Z:         return 0x5a;

    case Qt::Key_MediaNext: return VK_MEDIA_NEXT_TRACK;
    case Qt::Key_MediaPrevious: return VK_MEDIA_PREV_TRACK;
    case Qt::Key_MediaPlay: return VK_MEDIA_PLAY_PAUSE;
    case Qt::Key_MediaStop: return VK_MEDIA_STOP;

    case Qt::Key_VolumeDown:return VK_VOLUME_DOWN;
    case Qt::Key_VolumeUp:  return VK_VOLUME_UP;
    case Qt::Key_VolumeMute:return VK_VOLUME_MUTE;

    default:                return 0;
    }
}

bool QxtGlobalShortcutPrivate::registerShortcut(quint32 nativeKey, quint32 nativeMods)
{
    return RegisterHotKey(0, nativeMods ^ nativeKey, nativeMods, nativeKey);
}

bool QxtGlobalShortcutPrivate::unregisterShortcut(quint32 nativeKey, quint32 nativeMods)
{
    return UnregisterHotKey(0, nativeMods ^ nativeKey);
}
