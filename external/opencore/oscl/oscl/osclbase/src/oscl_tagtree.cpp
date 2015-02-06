
#include "oscl_tagtree.h"

OSCL_EXPORT_REF Oscl_Tag_Base::tag_base_type Oscl_Tag_Base::tag_ancestor(tag_base_type& dest, const tag_base_type& src) const
{
    // dest and src can be the same for this call.
    // This is optimal in some cases when the src string can be modified to be the ancestor string.
    if (dest != src)
    {
        tag_copy(dest, src);
    }

    tag_base_type endptr = dest + oscl_strlen(dest);
    while (*endptr != '.' && endptr != dest) endptr--;
    *endptr = '\0';
    return dest;
}

OSCL_EXPORT_REF Oscl_Tag_Base::size_type Oscl_Tag_Base::tag_depth(const tag_base_type& t) const
{
    if (*t == '\0') return 0;

    tag_base_type ptr = t;
    size_type depth = 1;

    while (*ptr != '\0')
    {
        if (*ptr == '.') depth++;
        ptr++;
    }
    return depth;
}

