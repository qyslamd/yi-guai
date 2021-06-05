#ifndef CAPTURER_H
#define CAPTURER_H

#include <vector>
#include <memory>
#include <utility>

#ifndef st
#define st(X) do{X}while(0)
#endif

enum Graph : uint32_t {
    NONE,
    RECTANGLE   = 0x0001,
    CIRCLE      = 0x0002,
    ELLIPSE     = CIRCLE,
    LINE        = 0x0004,
    CURVES      = 0x0008,
    MOSAIC      = 0x0010,
    ARROW       = 0x0020,
    TEXT        = 0x0040,
    ERASER      = 0x0080,
    BROKEN_LINE = 0x0100,
};

enum class PaintType : uint32_t {
    UNMODIFIED      = 0x0000,
    UPDATE_MASK     = 0x0001,
    DRAW_MODIFING   = 0x0010,
    DRAW_MODIFIED   = 0x0020 | UPDATE_MASK,
    DRAW_FINISHED   = 0x0040 | UPDATE_MASK,
    REPAINT_ALL     = 0x0100 | DRAW_MODIFIED | DRAW_FINISHED,
};

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::pair;
using std::make_pair;

#define LOAD_QSS(X, Y)			st(                                     \
                                    QFile file(Y);                      \
                                    file.open(QFile::ReadOnly);			\
                                                                        \
                                    if (file.isOpen()) {				\
                                        auto style = file.readAll();	\
                                        X->setStyleSheet(style);		\
                                        file.close();					\
                                    }                                   \
                                )

#endif // CAPTURER_H
