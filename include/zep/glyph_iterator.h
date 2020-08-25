#pragma once

/*#include <functional>
#include <set>

#include "zep/mcommon/file/path.h"
#include "zep/mcommon/logger.h"
#include "zep/mcommon/string/stringutils.h"
#include "zep/mcommon/utf8/unchecked.h"
*/

#include "gap_buffer.h"

/*#include "editor.h"
#include "line_widgets.h"
#include "theme.h"
*/

namespace Zep
{

class ZepBuffer;
enum class LineLocation
{
    None, // Not any specific location
    LineFirstGraphChar, // First non blank character
    LineLastGraphChar, // Last non blank character
    LineLastNonCR, // Last character before the carriage return
    LineBegin, // Beginning of line
    BeyondLineEnd, // The line end of the buffer line (for wrapped lines).
    LineCRBegin, // The first carriage return character
};

// An iterator which is utf8-sensitve, and buffer sensitive; i.e. it knows how to
// walk the gap buffer and skip along in multi byte characters.
// This is the main 'pointer' into the text, used everywhere
// It will clamp automatically and is usually valid if not in the default uninitialized state
class GlyphIterator
{
public:
    explicit GlyphIterator(const ZepBuffer* buffer = nullptr, unsigned long offset = 0);
    GlyphIterator(const GlyphIterator& itr);

    long Index() const;
    bool Valid() const;
    void Invalidate();
    bool operator<(const GlyphIterator& rhs) const;
    bool operator<=(const GlyphIterator& rhs) const;
    bool operator>(const GlyphIterator& rhs) const;
    bool operator>=(const GlyphIterator& rhs) const;
    bool operator==(const GlyphIterator& rhs) const;
    bool operator!=(const GlyphIterator& rhs) const;
    GlyphIterator operator--(int);
    GlyphIterator operator++(int);
    void operator-=(long count);
    void operator+=(long count);

    GlyphIterator operator-(long value) const;
    GlyphIterator operator+(long value) const;

    GlyphIterator& operator=(const GlyphIterator& rhs);
    uint8_t Char() const;
    uint8_t operator*() const;
    GlyphIterator& MoveClamped(long count, LineLocation clamp = LineLocation::LineLastNonCR);
    GlyphIterator& Move(long count);
    GlyphIterator& Clamp();

    GlyphIterator Peek(long count) const;
    GlyphIterator PeekLineClamped(long count, LineLocation clamp = LineLocation::LineLastNonCR) const;
    GlyphIterator PeekByteOffset(long count) const;
    GlyphIterator Clamped() const;

private:
    const ZepBuffer* m_pBuffer = nullptr;
    long m_index = -1;
};

inline long CodePointDistance(const GlyphIterator& itr1, const GlyphIterator& itr2)
{
    long count = 0;
    auto start = itr1;
    while (start != itr2)
    {
        count++;
        start++;
    }
    return count;
}

inline long ByteDistance(const GlyphIterator& itr1, const GlyphIterator& itr2)
{
    return itr2.Index() - itr1.Index();
}

struct GlyphRange
{
    GlyphIterator first;
    GlyphIterator second;

    GlyphRange(GlyphIterator a, GlyphIterator b);
    GlyphRange();
    bool ContainsLocation(long loc) const;
    bool ContainsLocation(GlyphIterator loc) const;
};

} // namespace Zep
