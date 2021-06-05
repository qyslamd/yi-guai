## QxtGlobalShortcut
---
- [x] Windows 10 / Qt 5.10.1 / Visual Studio 2017 
- [x] Ubuntu 16.04 / Qt 5.5.1 / GCC 5.4.0 
- [ ] OS X

#### Usage:

```
git clone https://github.com/ffiirree/qxtglobalshortcut.git
```

add in your `.pro` file:
```
include(qxtglobalshortcut/qxt.pri)
```

example code:
```cpp
#include <QDebug>
#include "qxtglobalshortcut.h"

...

auto shortcut = new QxtGlobalShortcut(this);
shortcut->setShortcut(QKeySequence("Shift+1"));
connect(shortcut, &QxtGlobalShortcut::activated, [=](){
    qDebug() << "shortcut activated";
});
```

#### References
- [Global HotKey WinAPI in Qt5](https://evileg.com/en/post/165/)
- [Global HotKey in Linux](https://evileg.com/en/post/174/)
