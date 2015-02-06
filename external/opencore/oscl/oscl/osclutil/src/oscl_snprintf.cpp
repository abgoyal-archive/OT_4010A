


/* #define HAVE_SNPRINTF */

/* #define PREFER_PORTABLE_SNPRINTF */

/* #define SNPRINTF_LONGLONG_SUPPORT */

/* #define NEED_SNPRINTF_ONLY */

/* #define NEED_ASPRINTF   */
/* #define NEED_ASNPRINTF  */
/* #define NEED_VASPRINTF  */
/* #define NEED_VASNPRINTF */





/* ============================================= */
/* NO USER SERVICABLE PARTS FOLLOWING THIS POINT */
/* ============================================= */
#include "oscl_snprintf.h"

#ifndef OSCLCONFIG_UTIL_H_INCLUDED
#include "osclconfig_util.h"
#endif

#ifndef OSCL_STRING_UTILS_H
#include "oscl_string_utils.h"
#endif

#ifndef OSCL_STDSTRING_H_INCLUDED
#include "oscl_stdstring.h"
#endif

#ifndef OSCL_STRING_UTILS_H_INCLUDED
#include "oscl_string_utils.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#define PORTABLE_SNPRINTF_VERSION_MAJOR 2
#define PORTABLE_SNPRINTF_VERSION_MINOR 2

#define NUMERIC_CONV_BUF_SIZE 32

// configure the portable sprintf code
#define PREFER_PORTABLE_SNPRINTF
#define NEED_SNPRINTF_ONLY

#if defined(NEED_ASPRINTF) || defined(NEED_ASNPRINTF) || defined(NEED_VASPRINTF) || defined(NEED_VASNPRINTF)
# if defined(NEED_SNPRINTF_ONLY)
# undef NEED_SNPRINTF_ONLY
# endif
# if !defined(PREFER_PORTABLE_SNPRINTF)
# define PREFER_PORTABLE_SNPRINTF
# endif
#endif

#if defined(SOLARIS_BUG_COMPATIBLE) && !defined(SOLARIS_COMPATIBLE)
#define SOLARIS_COMPATIBLE
#endif

#if defined(HPUX_BUG_COMPATIBLE) && !defined(HPUX_COMPATIBLE)
#define HPUX_COMPATIBLE
#endif

#if defined(DIGITAL_UNIX_BUG_COMPATIBLE) && !defined(DIGITAL_UNIX_COMPATIBLE)
#define DIGITAL_UNIX_COMPATIBLE
#endif

#if defined(PERL_BUG_COMPATIBLE) && !defined(PERL_COMPATIBLE)
#define PERL_COMPATIBLE
#endif

#if defined(LINUX_BUG_COMPATIBLE) && !defined(LINUX_COMPATIBLE)
#define LINUX_COMPATIBLE
#endif



//An implementation of swprintf for platforms that have native sprintf, but not swprintf.
//Note this routine converts input oscl_wchar strings to single-wide without any
//Unicode-to-UTF8 conversion.  This is sufficient for the usage in this module, since
//we know that format strings are ASCII character only, and there are no string arguments
//in the argument list passed to this routine.
static int32 oscl_default_swprintf(oscl_wchar *buffer, int32 bufferlen, const oscl_wchar *format, ...)
{
    OSCL_UNUSED_ARG(bufferlen);

    int32 result;
    int32 nformat = oscl_strlen(format);
    char* sformat = (char*)format;
    char* sbuffer = (char*)buffer;

    //Compress the format-string to single-wide.
    int32 i;
    for (i = 0; i < nformat; i++)
        sformat[i] = (char)format[i];
    sformat[nformat] = '\0';

    va_list args;
    va_start(args, format);
    result = vsprintf(sbuffer, sformat, args);
    va_end(args);

    //Restore the original format string.  Probably unnecessary.
    oscl_wchar* oformat = (oscl_wchar*)format;
    oformat[nformat] = '\0';
    for (i = 0; i < nformat; i++)
        oformat[nformat-i-1] = (oscl_wchar)sformat[nformat-i-1];

    //Stretch the result string from single-wide to wide.
    int32 nsbuffer = oscl_strlen(sbuffer);
    buffer[nsbuffer] = '\0';
    for (i = 0; i < nsbuffer; i++)
        buffer[nsbuffer-i-1] = (oscl_wchar)sbuffer[nsbuffer-i-1];

    return result;
}

OSCL_IMPORT_REF int32 oscl_UnicodeToUTF8(const oscl_wchar *input, int32 inLength, char *output, int32 outLength);

OsclAny * oscl_memchr(const OsclAny * str, int32 c, int32 count)
{
    uint8* p = (uint8 *)str;
    int32 j = 1;
    while (j <= count)
    {
        if (*p == c)
        {
            return p;
        }
        j++;
        p++;
    }
    return NULL;
}

/* prototypes */

#if defined(NEED_ASPRINTF)
int asprintf(char **ptr, const char *fmt, /*args*/ ...);
#endif
#if defined(NEED_VASPRINTF)
int vasprintf(char **ptr, const char *fmt, va_list ap);
#endif
#if defined(NEED_ASNPRINTF)
int asnprintf(char **ptr, size_t str_m, const char *fmt, /*args*/ ...);
#endif
#if defined(NEED_VASNPRINTF)
int vasnprintf(char **ptr, size_t str_m, const char *fmt, va_list ap);
#endif

//#if defined(HAVE_SNPRINTF)
/* declare our portable snprintf  routine under name portable_snprintf  */
/* declare our portable vsnprintf routine under name portable_vsnprintf */
//#else
/* declare our portable routines under names snprintf and vsnprintf */
//#define portable_snprintf snprintf
//#if !defined(NEED_SNPRINTF_ONLY)
//#define portable_vsnprintf vsnprintf
//#endif
//#endif

//#if !defined(HAVE_SNPRINTF) || defined(PREFER_PORTABLE_SNPRINTF)
//int portable_snprintf(char *str, size_t str_m, const char *fmt, /*args*/ ...);
//#if !defined(NEED_SNPRINTF_ONLY)
//int portable_vsnprintf(char *str, size_t str_m, const char *fmt, va_list ap);
//#endif
//#endif

/* declarations */


#if defined(NEED_ASPRINTF)
int asprintf(char **ptr, const char *fmt, /*args*/ ...)
{
    va_list ap;
    size_t str_m;
    int str_l;

    *ptr = NULL;
    va_start(ap, fmt);                            /* measure the required size */
    str_l = portable_vsnprintf(NULL, (size_t)0, fmt, ap);
    va_end(ap);
    OSCL_ASSERT(str_l >= 0);        /* possible integer overflow if str_m > INT_MAX */
    *ptr = (char *) malloc(str_m = (size_t)str_l + 1);
    if (*ptr == NULL)
    {
        errno = ENOMEM;
        str_l = -1;
    }
    else
    {
        int str_l2;
        va_start(ap, fmt);
        str_l2 = portable_vsnprintf(*ptr, str_m, fmt, ap);
        va_end(ap);
        OSCL_ASSERT(str_l2 == str_l);
    }
    return str_l;
}
#endif

#if defined(NEED_VASPRINTF)
int vasprintf(char **ptr, const char *fmt, va_list ap)
{
    size_t str_m;
    int str_l;

    *ptr = NULL;
    {
        va_list ap2;
        va_copy(ap2, ap);  /* don't consume the original ap, we'll need it again */
        str_l = portable_vsnprintf(NULL, (size_t)0, fmt, ap2);/*get required size*/
        va_end(ap2);
    }
    OSCL_ASSERT(str_l >= 0);        /* possible integer overflow if str_m > INT_MAX */
    *ptr = (char *) malloc(str_m = (size_t)str_l + 1);
    if (*ptr == NULL)
    {
        errno = ENOMEM;
        str_l = -1;
    }
    else
    {
        int str_l2 = portable_vsnprintf(*ptr, str_m, fmt, ap);
        OSCL_ASSERT(str_l2 == str_l);
    }
    return str_l;
}
#endif

#if defined(NEED_ASNPRINTF)
int asnprintf(char **ptr, size_t str_m, const char *fmt, /*args*/ ...)
{
    va_list ap;
    int str_l;

    *ptr = NULL;
    va_start(ap, fmt);                            /* measure the required size */
    str_l = portable_vsnprintf(NULL, (size_t)0, fmt, ap);
    va_end(ap);
    OSCL_ASSERT(str_l >= 0);        /* possible integer overflow if str_m > INT_MAX */
    if ((size_t)str_l + 1 < str_m) str_m = (size_t)str_l + 1;      /* truncate */
    /* if str_m is 0, no buffer is allocated, just set *ptr to NULL */
    if (str_m == 0)    /* not interested in resulting string, just return size */
    {
    }
    else
    {
        *ptr = (char *) malloc(str_m);
        if (*ptr == NULL)
        {
            errno = ENOMEM;
            str_l = -1;
        }
        else
        {
            int str_l2;
            va_start(ap, fmt);
            str_l2 = portable_vsnprintf(*ptr, str_m, fmt, ap);
            va_end(ap);
            OSCL_ASSERT(str_l2 == str_l);
        }
    }
    return str_l;
}
#endif

#if defined(NEED_VASNPRINTF)
int vasnprintf(char **ptr, size_t str_m, const char *fmt, va_list ap)
{
    int str_l;

    *ptr = NULL;
    {
        va_list ap2;
        va_copy(ap2, ap);  /* don't consume the original ap, we'll need it again */
        str_l = portable_vsnprintf(NULL, (size_t)0, fmt, ap2);/*get required size*/
        va_end(ap2);
    }
    OSCL_ASSERT(str_l >= 0);        /* possible integer overflow if str_m > INT_MAX */
    if ((size_t)str_l + 1 < str_m) str_m = (size_t)str_l + 1;      /* truncate */
    /* if str_m is 0, no buffer is allocated, just set *ptr to NULL */
    if (str_m == 0)    /* not interested in resulting string, just return size */
    {
    }
    else
    {
        *ptr = (char *) malloc(str_m);
        if (*ptr == NULL)
        {
            errno = ENOMEM;
            str_l = -1;
        }
        else
        {
            int str_l2 = portable_vsnprintf(*ptr, str_m, fmt, ap);
            OSCL_ASSERT(str_l2 == str_l);
        }
    }
    return str_l;
}
#endif

#if !defined(HAVE_SNPRINTF) || defined(PREFER_PORTABLE_SNPRINTF)

#if defined(NEED_SNPRINTF_ONLY)
int32 portable_snprintf(char *str, size_t str_m, const char *fmt, va_list ap)   // changed elipsis to va_list-jg
{
#else
int portable_vsnprintf(char *str, size_t str_m, const char *fmt, va_list ap)
{
#endif

#if defined(NEED_SNPRINTF_ONLY)
    //va_list ap; jg
#endif
    size_t str_l = 0;
    const char *p = fmt;

    /* In contrast with POSIX, the ISO C99 now says
     * that str can be NULL and str_m can be 0.
     * This is more useful than the old:  if (str_m < 1) return -1; */

#if defined(NEED_SNPRINTF_ONLY)
    //va_start(ap, fmt); jg
#endif
    if (!p) p = "";
    while (*p)
    {
        if (*p != '%')
        {
            /* if (str_l < str_m) str[str_l++] = *p++;    -- this would be sufficient */
            /* but the following code achieves better performance for cases
             * where format string is long and contains few conversions */
            const char *q = oscl_strchr(p + 1, '%');
            size_t n = !q ? oscl_strlen(p) : (q - p);
            if (str_l < str_m)
            {
                size_t avail = str_m - str_l;
                oscl_strncpy(str + str_l, p, (n > avail ? avail : n));
            }
            p += n;
            str_l += n;
        }
        else
        {
#if defined(PERL_COMPATIBLE) || defined(LINUX_COMPATIBLE)
            const char *starting_p;
#endif
            size_t min_field_width = 0, precision = 0;
            int zero_padding = 0, precision_specified = 0, justify_left = 0;
            int alternate_form = 0, force_sign = 0;
            int space_for_positive = 1; /* If both the ' ' and '+' flags appear,
                                     the ' ' flag should be ignored. */
            char length_modifier = '\0';            /* allowed values: \0, h, l, L */
            char tmp[NUMERIC_CONV_BUF_SIZE];/* temporary buffer for simple numeric->string conversion */

            const char *str_arg;      /* string address in case of string argument */
            size_t str_arg_l;         /* natural field width of arg without padding
                                   and sign */
            unsigned char uchar_arg;
            /* unsigned char argument value - only defined for c conversion.
               N.B. standard explicitly states the char argument for
               the c conversion is unsigned */

            size_t number_of_zeros_to_pad = 0;
            /* number of zeros to be inserted for numeric conversions
               as required by the precision or minimal field width */

            size_t zero_padding_insertion_ind = 0;
            /* index into tmp where zero padding is to be inserted */

            char fmt_spec = '\0';
            /* current conversion specifier character */

            str_arg = NULL;
#if defined(PERL_COMPATIBLE) || defined(LINUX_COMPATIBLE)
            starting_p = p;
#endif
            p++;  /* skip '%' */

            /* parse flags */
            while (*p == '0' || *p == '-' || *p == '+' ||
                    *p == ' ' || *p == '#' || *p == '\'')
            {
                switch (*p)
                {
                    case '0':
                        zero_padding = 1;
                        break;
                    case '-':
                        justify_left = 1;
                        break;
                    case '+':
                        force_sign = 1;
                        space_for_positive = 0;
                        break;
                    case ' ':
                        force_sign = 1;
                        /* If both the ' ' and '+' flags appear, the ' ' flag should be ignored */
#ifdef PERL_COMPATIBLE
                        /* ... but in Perl the last of ' ' and '+' applies */
                        space_for_positive = 1;
#endif
                        break;
                    case '#':
                        alternate_form = 1;
                        break;
                    case '\'':
                        break;
                }
                p++;
            }
            /* If the '0' and '-' flags both appear, the '0' flag should be ignored. */

            /* parse field width */
            if (*p == '*')
            {
                int j;
                p++;
                j = va_arg(ap, int);
                if (j >= 0) min_field_width = j;
                else
                {
                    min_field_width = -j;
                    justify_left = 1;
                }
            }
            else if (oscl_isdigit((int)(*p)))
            {
                /* size_t could be wider than unsigned int;
                   make sure we treat argument like common implementations do */
                unsigned int uj = *p++ - '0';
                while (oscl_isdigit((int)(*p))) uj = 10 * uj + (unsigned int)(*p++ - '0');
                min_field_width = uj;
            }
            /* parse precision */
            if (*p == '.')
            {
                p++;
                precision_specified = 1;
                if (*p == '*')
                {
                    int j = va_arg(ap, int);
                    p++;
                    if (j >= 0) precision = j;
                    else
                    {
                        precision_specified = 0;
                        precision = 0;
                        /* NOTE:
                         *   Solaris 2.6 man page claims that in this case the precision
                         *   should be set to 0.  Digital Unix 4.0, HPUX 10 and BSD man page
                         *   claim that this case should be treated as unspecified precision,
                         *   which is what we do here.
                         */
                    }
                }
                else if (oscl_isdigit((int)(*p)))
                {
                    /* size_t could be wider than unsigned int;
                       make sure we treat argument like common implementations do */
                    unsigned int uj = *p++ - '0';
                    while (oscl_isdigit((int)(*p))) uj = 10 * uj + (unsigned int)(*p++ - '0');
                    precision = uj;
                }
            }
            /* parse 'h', 'l' and 'll' length modifiers */
            if (*p == 'h' || *p == 'l')
            {
                length_modifier = *p;
                p++;
                if (length_modifier == 'l' && *p == 'l')     /* double l = long long */
                {
#ifdef SNPRINTF_LONGLONG_SUPPORT
                    length_modifier = '2';                  /* double l encoded as '2' */
#else
                    length_modifier = 'l';                 /* treat it as a single 'l' */
#endif
                    p++;
                }
            }
            fmt_spec = *p;
            /* common synonyms: */
            switch (fmt_spec)
            {
                case 'i':
                    fmt_spec = 'd';
                    break;
                case 'D':
                    fmt_spec = 'd';
                    length_modifier = 'l';
                    break;
                case 'U':
                    fmt_spec = 'u';
                    length_modifier = 'l';
                    break;
                case 'O':
                    fmt_spec = 'o';
                    length_modifier = 'l';
                    break;
                default:
                    break;
            }
            /* get parameter value, do initial processing */
            switch (fmt_spec)
            {
                case '%': /* % behaves similar to 's' regarding flags and field widths */
                case 'c': /* c behaves similar to 's' regarding flags and field widths */
                case 's':
                    length_modifier = '\0';          /* wint_t and wchar_t not supported */
                    /* the result of zero padding flag with non-numeric conversion specifier*/
                    /* is undefined. Solaris and HPUX 10 does zero padding in this case,    */
                    /* Digital Unix and Linux does not. */
#if !defined(SOLARIS_COMPATIBLE) && !defined(HPUX_COMPATIBLE)
                    zero_padding = 0;    /* turn zero padding off for string conversions */
#endif
                    str_arg_l = 1;
                    switch (fmt_spec)
                    {
                        case '%':
                            str_arg = p;
                            break;
                        case 'c':
                        {
                            int j = va_arg(ap, int);
                            uchar_arg = (unsigned char) j;   /* standard demands unsigned char */
                            str_arg = (const char *) & uchar_arg;
                            break;
                        }
                        case 's':
                            str_arg = va_arg(ap, const char *);
                            if (!str_arg) str_arg_l = 0;
                            /* make sure not to address string beyond the specified precision !!! */
                            else if (!precision_specified) str_arg_l = oscl_strlen(str_arg);
                            /* truncate string if necessary as requested by precision */
                            else if (precision == 0) str_arg_l = 0;
                            else
                            {
                                /* oscl_memchr on HP does not like n > 2^31  !!! */ // cast on memchar - jg
                                const char *q = (const char*)oscl_memchr(str_arg, '\0',
                                                precision <= 0x7fffffff ? precision : 0x7fffffff);
                                str_arg_l = !q ? precision : (q - str_arg);
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case 'd':
                case 'u':
                case 'o':
                case 'x':
                case 'X':
                case 'p':
                {
                    /* NOTE: the u, o, x, X and p conversion specifiers imply
                             the value is unsigned;  d implies a signed value */

                    int arg_sign = 0;
                    /* 0 if numeric argument is zero (or if pointer is NULL for 'p'),
                      +1 if greater than zero (or nonzero for unsigned arguments),
                      -1 if negative (unsigned argument is never negative) */

                    int int_arg = 0;
                    unsigned int uint_arg = 0;
                    /* only defined for length modifier h, or for no length modifiers */

                    long int long_arg = 0;
                    unsigned long int ulong_arg = 0;
                    /* only defined for length modifier l */

                    void *ptr_arg = NULL;
                    /* pointer argument value -only defined for p conversion */

#ifdef SNPRINTF_LONGLONG_SUPPORT
                    long long int long_long_arg = 0;
                    unsigned long long int ulong_long_arg = 0;
                    /* only defined for length modifier ll */
#endif
                    if (fmt_spec == 'p')
                    {
                        /* HPUX 10: An l, h, ll or L before any other conversion character
                         *   (other than d, i, u, o, x, or X) is ignored.
                         * Digital Unix:
                         *   not specified, but seems to behave as HPUX does.
                         * Solaris: If an h, l, or L appears before any other conversion
                         *   specifier (other than d, i, u, o, x, or X), the behavior
                         *   is undefined. (Actually %hp converts only 16-bits of address
                         *   and %llp treats address as 64-bit data which is incompatible
                         *   with (void *) argument on a 32-bit system).
                         */
#ifdef SOLARIS_COMPATIBLE
#  ifdef SOLARIS_BUG_COMPATIBLE
                        /* keep length modifiers even if it represents 'll' */
#  else
                        if (length_modifier == '2') length_modifier = '\0';
#  endif
#else
                        length_modifier = '\0';
#endif
                        ptr_arg = va_arg(ap, void *);
                        if (ptr_arg != NULL) arg_sign = 1;
                    }
                    else if (fmt_spec == 'd')    /* signed */
                    {
                        switch (length_modifier)
                        {
                            case '\0':
                            case 'h':
                                /* It is non-portable to specify a second argument of char or short
                                 * to va_arg, because arguments seen by the called function
                                 * are not char or short.  C converts char and short arguments
                                 * to int before passing them to a function.
                                 */
                                int_arg = va_arg(ap, int);
                                if (int_arg > 0) arg_sign =  1;
                                else if (int_arg < 0) arg_sign = -1;
                                break;
                            case 'l':
                                long_arg = va_arg(ap, long int);
                                if (long_arg > 0) arg_sign =  1;
                                else if (long_arg < 0) arg_sign = -1;
                                break;
#ifdef SNPRINTF_LONGLONG_SUPPORT
                            case '2':
                                long_long_arg = va_arg(ap, long long int);
                                if (long_long_arg > 0) arg_sign =  1;
                                else if (long_long_arg < 0) arg_sign = -1;
                                break;
#endif
                        }
                    }
                    else    /* unsigned */
                    {
                        switch (length_modifier)
                        {
                            case '\0':
                            case 'h':
                                uint_arg = va_arg(ap, unsigned int);
                                if (uint_arg) arg_sign = 1;
                                break;
                            case 'l':
                                ulong_arg = va_arg(ap, unsigned long int);
                                if (ulong_arg) arg_sign = 1;
                                break;
#ifdef SNPRINTF_LONGLONG_SUPPORT
                            case '2':
                                ulong_long_arg = va_arg(ap, unsigned long long int);
                                if (ulong_long_arg) arg_sign = 1;
                                break;
#endif
                        }
                    }
                    str_arg = tmp;
                    str_arg_l = 0;
                    /* NOTE:
                     *   For d, i, u, o, x, and X conversions, if precision is specified,
                     *   the '0' flag should be ignored. This is so with Solaris 2.6,
                     *   Digital UNIX 4.0, HPUX 10, Linux, FreeBSD, NetBSD; but not with Perl.
                     */
#ifndef PERL_COMPATIBLE
                    if (precision_specified) zero_padding = 0;
#endif
                    if (fmt_spec == 'd')
                    {
                        if (force_sign && arg_sign >= 0)
                            tmp[str_arg_l++] = space_for_positive ? ' ' : '+';
                        /* leave negative numbers for sprintf to handle,
                           to avoid handling tricky cases like (short int)(-32768) */
#ifdef LINUX_COMPATIBLE
                    }
                    else if (fmt_spec == 'p' && force_sign && arg_sign > 0)
                    {
                        tmp[str_arg_l++] = space_for_positive ? ' ' : '+';
#endif
                    }
                    else if (alternate_form)
                    {
                        if (arg_sign != 0 && (fmt_spec == 'x' || fmt_spec == 'X'))
                        {
                            tmp[str_arg_l++] = '0';
                            tmp[str_arg_l++] = fmt_spec;
                        }
                        /* alternate form should have no effect for p conversion, but ... */
#ifdef HPUX_COMPATIBLE
                        else if (fmt_spec == 'p'
                                 /* HPUX 10: for an alternate form of p conversion,
                                  *          a nonzero result is prefixed by 0x. */
#ifndef HPUX_BUG_COMPATIBLE
                                 /* Actually it uses 0x prefix even for a zero value. */
                                 && arg_sign != 0
#endif
                                )
                        {
                            tmp[str_arg_l++] = '0';
                            tmp[str_arg_l++] = 'x';
                        }
#endif
                    }
                    zero_padding_insertion_ind = str_arg_l;
                    if (!precision_specified) precision = 1;   /* default precision is 1 */
                    if (precision == 0 && arg_sign == 0
#if defined(HPUX_BUG_COMPATIBLE) || defined(LINUX_COMPATIBLE)
                            && fmt_spec != 'p'
                            /* HPUX 10 man page claims: With conversion character p the result of
                             * converting a zero value with a precision of zero is a null string.
                             * Actually HP returns all zeroes, and Linux returns "(nil)". */
#endif
                       )
                    {
                        /* converted to null string */
                        /* When zero value is formatted with an explicit precision 0,
                           the resulting formatted string is empty (d, i, u, o, x, X, p).   */
                    }
                    else
                    {
                        char f[5];
                        int f_l = 0;
                        f[f_l++] = '%';    /* construct a simple format string for sprintf */
                        if (!length_modifier) { }
                        else if (length_modifier == '2')
                        {
                            f[f_l++] = 'l';
                            f[f_l++] = 'l';
                        }
#if  defined(EKA2)
                        else
                        {
                            if (length_modifier == 'l')
                            {
                                // ignore this because there is a mismatch because symbian
                                // treats "%ld" as TInt64, and will expect 8 bytes of
                                // data as the argument when we only give it 4.
                            }
                            else
                            {
                                f[f_l++] = length_modifier;
                            }
                        }
#else
                        else f[f_l++] = length_modifier;
#endif
                        f[f_l++] = fmt_spec;
                        f[f_l++] = '\0';
                        if (fmt_spec == 'p')
                            str_arg_l += sprintf(tmp + str_arg_l, f, ptr_arg);
                        else if (fmt_spec == 'd')    /* signed */
                        {
                            switch (length_modifier)
                            {
                                case '\0':
                                case 'h':
                                    str_arg_l += sprintf(tmp + str_arg_l, f, int_arg);
                                    break;
                                case 'l':
                                    str_arg_l += sprintf(tmp + str_arg_l, f, long_arg);
                                    break;
#ifdef SNPRINTF_LONGLONG_SUPPORT
                                case '2':
                                    str_arg_l += sprintf(tmp + str_arg_l, f, long_long_arg);
                                    break;
#endif
                            }
                        }
                        else    /* unsigned */
                        {
                            switch (length_modifier)
                            {
                                case '\0':
                                case 'h':
                                    str_arg_l += sprintf(tmp + str_arg_l, f, uint_arg);
                                    break;
                                case 'l':
                                    str_arg_l += sprintf(tmp + str_arg_l, f, ulong_arg);
                                    break;
#ifdef SNPRINTF_LONGLONG_SUPPORT
                                case '2':
                                    str_arg_l += sprintf(tmp + str_arg_l, f, ulong_long_arg);
                                    break;
#endif
                            }
                        }
                        /* include the optional minus sign and possible "0x"
                           in the region before the zero padding insertion point */
                        if (zero_padding_insertion_ind < str_arg_l &&
                                tmp[zero_padding_insertion_ind] == '-')
                        {
                            zero_padding_insertion_ind++;
                        }
                        if (zero_padding_insertion_ind + 1 < str_arg_l &&
                                tmp[zero_padding_insertion_ind]   == '0' &&
                                (tmp[zero_padding_insertion_ind+1] == 'x' ||
                                 tmp[zero_padding_insertion_ind+1] == 'X'))
                        {
                            zero_padding_insertion_ind += 2;
                        }
                    }
                    {
                        size_t num_of_digits = str_arg_l - zero_padding_insertion_ind;
                        if (alternate_form && fmt_spec == 'o'
#ifdef HPUX_COMPATIBLE                                  /* ("%#.o",0) -> ""  */
                                && (str_arg_l > 0)
#endif
#ifdef DIGITAL_UNIX_BUG_COMPATIBLE                      /* ("%#o",0) -> "00" */
#else
                                /* unless zero is already the first character */
                                && !(zero_padding_insertion_ind < str_arg_l
                                     && tmp[zero_padding_insertion_ind] == '0')
#endif
                           )          /* assure leading zero for alternate-form octal numbers */
                        {
                            if (!precision_specified || precision < num_of_digits + 1)
                            {
                                /* precision is increased to force the first character to be zero,
                                   except if a zero value is formatted with an explicit precision
                                   of zero */
                                precision = num_of_digits + 1;
                                precision_specified = 1;
                            }
                        }
                        /* zero padding to specified precision? */
                        if (num_of_digits < precision)
                            number_of_zeros_to_pad = precision - num_of_digits;
                    }
                    /* zero padding to specified minimal field width? */
                    if (!justify_left && zero_padding)
                    {
                        size_t n = min_field_width - (str_arg_l + number_of_zeros_to_pad);
                        if (n > 0) number_of_zeros_to_pad += n;
                    }
                    break;
                }
                default: /* unrecognized conversion specifier, keep format string as-is*/
                    zero_padding = 0;  /* turn zero padding off for non-numeric convers. */
#ifndef DIGITAL_UNIX_COMPATIBLE
                    justify_left = 1;
                    min_field_width = 0;                /* reset flags */
#endif
#if defined(PERL_COMPATIBLE) || defined(LINUX_COMPATIBLE)
                    /* keep the entire format string unchanged */
                    str_arg = starting_p;
                    str_arg_l = p - starting_p;
                    /* well, not exactly so for Linux, which does something inbetween,
                     * and I don't feel an urge to imitate it: "%+++++hy" -> "%+y"  */
#else
                    /* discard the unrecognized conversion, just keep *
                     * the unrecognized conversion character          */
                    str_arg = p;
                    str_arg_l = 0;
#endif
                    if (*p) str_arg_l++;  /* include invalid conversion specifier unchanged
                                 if not at end-of-string */
                    break;
            }
            if (*p) p++;      /* step over the just processed conversion specifier */
            /* insert padding to the left as requested by min_field_width;
               this does not include the zero padding in case of numerical conversions*/
            if (!justify_left)                  /* left padding with blank or zero */
            {
                int n = min_field_width - (str_arg_l + number_of_zeros_to_pad);
                if (n > 0)
                {
                    if (str_l < str_m)
                    {
                        size_t avail = str_m - str_l;
                        oscl_strset(str + str_l, (zero_padding ? '0' : ' '), (n > (int)avail ? avail : n));
                    }
                    str_l += n;
                }
            }
            /* zero padding as requested by the precision or by the minimal field width
             * for numeric conversions required? */
            if (number_of_zeros_to_pad <= 0)
            {
                /* will not copy first part of numeric right now, *
                 * force it to be copied later in its entirety    */
                zero_padding_insertion_ind = 0;
            }
            else
            {
                /* insert first part of numerics (sign or '0x') before zero padding */
                int n = zero_padding_insertion_ind;
                if (n > 0)
                {
                    if (str_l < str_m)
                    {
                        size_t avail = str_m - str_l;
                        oscl_strncpy(str + str_l, str_arg, (n > (int)avail ? avail : n));
                    }
                    str_l += n;
                }
                /* insert zero padding as requested by the precision or min field width */
                n = number_of_zeros_to_pad;
                if (n > 0)
                {
                    if (str_l < str_m)
                    {
                        size_t avail = str_m - str_l;
                        oscl_strset(str + str_l, '0', (n > (int)avail ? avail : n));
                    }
                    str_l += n;
                }
            }
            /* insert formatted string
             * (or as-is conversion specifier for unknown conversions) */
            {
                int n = str_arg_l - zero_padding_insertion_ind;
                if (n > 0)
                {
                    if (str_l < str_m)
                    {
                        size_t avail = str_m - str_l;
                        oscl_strncpy(str + str_l, str_arg + zero_padding_insertion_ind,
                                     (n > (int)avail ? avail : n));
                    }
                    str_l += n;
                }
            }
            /* insert right padding */
            if (justify_left)            /* right blank padding to the field width */
            {
                int n = min_field_width - (str_arg_l + number_of_zeros_to_pad);
                if (n > 0)
                {
                    if (str_l < str_m)
                    {
                        size_t avail = str_m - str_l;
                        oscl_strset(str + str_l, ' ', (n > (int)avail ? avail : n));
                    }
                    str_l += n;
                }
            }
        }
    }
#if defined(NEED_SNPRINTF_ONLY)
    //va_end(ap); jg
#endif
    if (str_m > 0)
    { /* make sure the string is null-terminated
                      even at the expense of overwriting the last character
                      (shouldn't happen, but just in case) */
        str[str_l <= str_m-1 ? str_l : str_m-1] = '\0';
    }
    /* Return the number of characters formatted (excluding trailing null
     * character), that is, the number of characters that would have been
     * written to the buffer if it were large enough.
     *
     * The value of str_l should be returned, but str_l is of unsigned type
     * size_t, and snprintf is int, possibly leading to an undetected
     * integer overflow, resulting in a negative return value, which is illegal.
     * Both XSH5 and ISO C99 (at least the draft) are silent on this issue.
     * Should errno be set to EOVERFLOW and EOF returned in this case???
     */
    return (int) str_l;
}
#endif

OSCL_EXPORT_REF int32 oscl_snprintf(char *str, uint32 count, const char *fmt, /*args*/ ...)
{
    int str_l;
    va_list args;
    va_start(args, fmt);
    str_l = portable_snprintf(str, count, fmt, args);
    return str_l;
}

OSCL_EXPORT_REF int32 oscl_vsnprintf(char *str, uint32 count, const char *fmt, va_list args)
{
    int str_l;
    str_l = portable_snprintf(str, count, fmt, args);
    return str_l;
}

#if !defined(HAVE_SNPRINTF) || defined(PREFER_PORTABLE_SNPRINTF)

#if defined(NEED_SNPRINTF_ONLY)
int32 portable_swprintf(oscl_wchar *str, size_t str_m, const oscl_wchar *fmt, va_list ap)   // changed elipsis to va_list-jg
{
#else
int portable_vswprintf(oscl_wchar *str, size_t str_m, const oscl_wchar *fmt, va_list ap)
{
#endif

#if defined(NEED_SNPRINTF_ONLY)
    //va_list ap; jg
#endif
    size_t str_l = 0;
    const oscl_wchar *p = fmt;

    /* In contrast with POSIX, the ISO C99 now says
     * that str can be NULL and str_m can be 0.
     * This is more useful than the old:  if (str_m < 1) return -1; */

#if defined(NEED_SNPRINTF_ONLY)
    //va_start(ap, fmt); jg
#endif
    oscl_wchar nullstr[1];
    nullstr[0] = 0;
    if (!p)
        p = &nullstr[0];
    while (*p)
    {
        if (*p != '%')
        {
            /* if (str_l < str_m) str[str_l++] = *p++;    -- this would be sufficient */
            /* but the following code achieves better performance for cases
             * where format string is long and contains few conversions */
            const oscl_wchar *q = oscl_strchr(p + 1, '%');
            size_t n = !q ? oscl_strlen(p) : (q - p);
            if (str_l < str_m)
            {
                size_t avail = str_m - str_l;
                oscl_strncpy(str + str_l, p, (n > avail ? avail : n));
            }
            p += n;
            str_l += n;
        }
        else
        {
#if defined(PERL_COMPATIBLE) || defined(LINUX_COMPATIBLE)
            const oscl_wchar *starting_p;
#endif
            size_t min_field_width = 0, precision = 0;
            int zero_padding = 0, precision_specified = 0, justify_left = 0;
            int alternate_form = 0, force_sign = 0;
            int space_for_positive = 1; /* If both the ' ' and '+' flags appear,
                                     the ' ' flag should be ignored. */
            oscl_wchar length_modifier = '\0';            /* allowed values: \0, h, l, L */
            oscl_wchar tmp[NUMERIC_CONV_BUF_SIZE];/* temporary buffer for simple numeric->string conversion */

            const oscl_wchar *str_arg;      /* string address in case of string argument */
            size_t str_arg_l;         /* natural field width of arg without padding
                                   and sign */
            oscl_wchar uchar_arg;
            /* unsigned char argument value - only defined for c conversion.
               N.B. standard explicitly states the char argument for
               the c conversion is unsigned */

            size_t number_of_zeros_to_pad = 0;
            /* number of zeros to be inserted for numeric conversions
               as required by the precision or minimal field width */

            size_t zero_padding_insertion_ind = 0;
            /* index into tmp where zero padding is to be inserted */

            oscl_wchar fmt_spec = '\0';
            /* current conversion specifier character */

            str_arg = NULL;
#if defined(PERL_COMPATIBLE) || defined(LINUX_COMPATIBLE)
            starting_p = p;
#endif
            p++;  /* skip '%' */

            /* parse flags */
            while (*p == '0' || *p == '-' || *p == '+' ||
                    *p == ' ' || *p == '#' || *p == '\'')
            {
                switch (*p)
                {
                    case '0':
                        zero_padding = 1;
                        break;
                    case '-':
                        justify_left = 1;
                        break;
                    case '+':
                        force_sign = 1;
                        space_for_positive = 0;
                        break;
                    case ' ':
                        force_sign = 1;
                        /* If both the ' ' and '+' flags appear, the ' ' flag should be ignored */
#ifdef PERL_COMPATIBLE
                        /* ... but in Perl the last of ' ' and '+' applies */
                        space_for_positive = 1;
#endif
                        break;
                    case '#':
                        alternate_form = 1;
                        break;
                    case '\'':
                        break;
                }
                p++;
            }
            /* If the '0' and '-' flags both appear, the '0' flag should be ignored. */

            /* parse field width */
            if (*p == '*')
            {
                int j;
                p++;
                j = va_arg(ap, int);
                if (j >= 0) min_field_width = j;
                else
                {
                    min_field_width = -j;
                    justify_left = 1;
                }
            }
            else if (oscl_isdigit((int)(*p)))
            {
                /* size_t could be wider than unsigned int;
                   make sure we treat argument like common implementations do */
                unsigned int uj = *p++ - '0';
                while (oscl_isdigit((int)(*p))) uj = 10 * uj + (unsigned int)(*p++ - '0');
                min_field_width = uj;
            }
            /* parse precision */
            if (*p == '.')
            {
                p++;
                precision_specified = 1;
                if (*p == '*')
                {
                    int j = va_arg(ap, int);
                    p++;
                    if (j >= 0) precision = j;
                    else
                    {
                        precision_specified = 0;
                        precision = 0;
                        /* NOTE:
                         *   Solaris 2.6 man page claims that in this case the precision
                         *   should be set to 0.  Digital Unix 4.0, HPUX 10 and BSD man page
                         *   claim that this case should be treated as unspecified precision,
                         *   which is what we do here.
                         */
                    }
                }
                else if (oscl_isdigit((int)(*p)))
                {
                    /* size_t could be wider than unsigned int;
                       make sure we treat argument like common implementations do */
                    unsigned int uj = *p++ - '0';
                    while (oscl_isdigit((int)(*p))) uj = 10 * uj + (unsigned int)(*p++ - '0');
                    precision = uj;
                }
            }
            /* parse 'h', 'l' and 'll' length modifiers */
            if (*p == 'h' || *p == 'l')
            {
                length_modifier = *p;
                p++;
                if (length_modifier == 'l' && *p == 'l')     /* double l = long long */
                {
#ifdef SNPRINTF_LONGLONG_SUPPORT
                    length_modifier = '2';                  /* double l encoded as '2' */
#else
                    length_modifier = 'l';                 /* treat it as a single 'l' */
#endif
                    p++;
                }
            }
            fmt_spec = *p;
            /* common synonyms: */
            switch (fmt_spec)
            {
                case 'i':
                    fmt_spec = 'd';
                    break;
                case 'D':
                    fmt_spec = 'd';
                    length_modifier = 'l';
                    break;
                case 'U':
                    fmt_spec = 'u';
                    length_modifier = 'l';
                    break;
                case 'O':
                    fmt_spec = 'o';
                    length_modifier = 'l';
                    break;
                default:
                    break;
            }
            /* get parameter value, do initial processing */
            switch (fmt_spec)
            {
                case '%': /* % behaves similar to 's' regarding flags and field widths */
                case 'c': /* c behaves similar to 's' regarding flags and field widths */
                case 's':
                    length_modifier = '\0';          /* wint_t and wchar_t not supported */
                    /* the result of zero padding flag with non-numeric conversion specifier*/
                    /* is undefined. Solaris and HPUX 10 does zero padding in this case,    */
                    /* Digital Unix and Linux does not. */
#if !defined(SOLARIS_COMPATIBLE) && !defined(HPUX_COMPATIBLE)
                    zero_padding = 0;    /* turn zero padding off for string conversions */
#endif
                    str_arg_l = 1;
                    switch (fmt_spec)
                    {
                        case '%':
                            str_arg = p;
                            break;
                        case 'c':
                        {
                            int j = va_arg(ap, int);
                            uchar_arg = (oscl_wchar) j;   /* standard demands unsigned char */
                            str_arg = (const oscl_wchar *) & uchar_arg;
                            break;
                        }
                        case 's':
                            str_arg = va_arg(ap, const oscl_wchar *);
                            if (!str_arg) str_arg_l = 0;
                            /* make sure not to address string beyond the specified precision !!! */
                            else if (!precision_specified) str_arg_l = oscl_strlen(str_arg);
                            /* truncate string if necessary as requested by precision */
                            else if (precision == 0) str_arg_l = 0;
                            else
                            {
                                /* oscl_memchr on HP does not like n > 2^31  !!! */ // cast on memchar - jg
                                const oscl_wchar *q = (const oscl_wchar*)oscl_memchr(str_arg, '\0',
                                                      precision <= 0x7fffffff ? precision : 0x7fffffff);
                                str_arg_l = !q ? precision : (q - str_arg);
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case 'd':
                case 'u':
                case 'o':
                case 'x':
                case 'X':
                case 'p':
                {
                    /* NOTE: the u, o, x, X and p conversion specifiers imply
                             the value is unsigned;  d implies a signed value */

                    int arg_sign = 0;
                    /* 0 if numeric argument is zero (or if pointer is NULL for 'p'),
                      +1 if greater than zero (or nonzero for unsigned arguments),
                      -1 if negative (unsigned argument is never negative) */

                    int int_arg = 0;
                    unsigned int uint_arg = 0;
                    /* only defined for length modifier h, or for no length modifiers */

                    long int long_arg = 0;
                    unsigned long int ulong_arg = 0;
                    /* only defined for length modifier l */

                    void *ptr_arg = NULL;
                    /* pointer argument value -only defined for p conversion */

#ifdef SNPRINTF_LONGLONG_SUPPORT
                    long long int long_long_arg = 0;
                    unsigned long long int ulong_long_arg = 0;
                    /* only defined for length modifier ll */
#endif
                    if (fmt_spec == 'p')
                    {
                        /* HPUX 10: An l, h, ll or L before any other conversion character
                         *   (other than d, i, u, o, x, or X) is ignored.
                         * Digital Unix:
                         *   not specified, but seems to behave as HPUX does.
                         * Solaris: If an h, l, or L appears before any other conversion
                         *   specifier (other than d, i, u, o, x, or X), the behavior
                         *   is undefined. (Actually %hp converts only 16-bits of address
                         *   and %llp treats address as 64-bit data which is incompatible
                         *   with (void *) argument on a 32-bit system).
                         */
#ifdef SOLARIS_COMPATIBLE
#  ifdef SOLARIS_BUG_COMPATIBLE
                        /* keep length modifiers even if it represents 'll' */
#  else
                        if (length_modifier == '2') length_modifier = '\0';
#  endif
#else
                        length_modifier = '\0';
#endif
                        ptr_arg = va_arg(ap, void *);
                        if (ptr_arg != NULL) arg_sign = 1;
                    }
                    else if (fmt_spec == 'd')    /* signed */
                    {
                        switch (length_modifier)
                        {
                            case '\0':
                            case 'h':
                                /* It is non-portable to specify a second argument of char or short
                                 * to va_arg, because arguments seen by the called function
                                 * are not char or short.  C converts char and short arguments
                                 * to int before passing them to a function.
                                 */
                                int_arg = va_arg(ap, int);
                                if (int_arg > 0) arg_sign =  1;
                                else if (int_arg < 0) arg_sign = -1;
                                break;
                            case 'l':
                                long_arg = va_arg(ap, long int);
                                if (long_arg > 0) arg_sign =  1;
                                else if (long_arg < 0) arg_sign = -1;
                                break;
#ifdef SNPRINTF_LONGLONG_SUPPORT
                            case '2':
                                long_long_arg = va_arg(ap, long long int);
                                if (long_long_arg > 0) arg_sign =  1;
                                else if (long_long_arg < 0) arg_sign = -1;
                                break;
#endif
                        }
                    }
                    else    /* unsigned */
                    {
                        switch (length_modifier)
                        {
                            case '\0':
                            case 'h':
                                uint_arg = va_arg(ap, unsigned int);
                                if (uint_arg) arg_sign = 1;
                                break;
                            case 'l':
                                ulong_arg = va_arg(ap, unsigned long int);
                                if (ulong_arg) arg_sign = 1;
                                break;
#ifdef SNPRINTF_LONGLONG_SUPPORT
                            case '2':
                                ulong_long_arg = va_arg(ap, unsigned long long int);
                                if (ulong_long_arg) arg_sign = 1;
                                break;
#endif
                        }
                    }
                    str_arg = tmp;
                    str_arg_l = 0;
                    /* NOTE:
                     *   For d, i, u, o, x, and X conversions, if precision is specified,
                     *   the '0' flag should be ignored. This is so with Solaris 2.6,
                     *   Digital UNIX 4.0, HPUX 10, Linux, FreeBSD, NetBSD; but not with Perl.
                     */
#ifndef PERL_COMPATIBLE
                    if (precision_specified) zero_padding = 0;
#endif
                    if (fmt_spec == 'd')
                    {
                        if (force_sign && arg_sign >= 0)
                            tmp[str_arg_l++] = space_for_positive ? ' ' : '+';
                        /* leave negative numbers for sprintf to handle,
                           to avoid handling tricky cases like (short int)(-32768) */
#ifdef LINUX_COMPATIBLE
                    }
                    else if (fmt_spec == 'p' && force_sign && arg_sign > 0)
                    {
                        tmp[str_arg_l++] = space_for_positive ? ' ' : '+';
#endif
                    }
                    else if (alternate_form)
                    {
                        if (arg_sign != 0 && (fmt_spec == 'x' || fmt_spec == 'X'))
                        {
                            tmp[str_arg_l++] = '0';
                            tmp[str_arg_l++] = fmt_spec;
                        }
                        /* alternate form should have no effect for p conversion, but ... */
#ifdef HPUX_COMPATIBLE
                        else if (fmt_spec == 'p'
                                 /* HPUX 10: for an alternate form of p conversion,
                                  *          a nonzero result is prefixed by 0x. */
#ifndef HPUX_BUG_COMPATIBLE
                                 /* Actually it uses 0x prefix even for a zero value. */
                                 && arg_sign != 0
#endif
                                )
                        {
                            tmp[str_arg_l++] = '0';
                            tmp[str_arg_l++] = 'x';
                        }
#endif
                    }
                    zero_padding_insertion_ind = str_arg_l;
                    if (!precision_specified) precision = 1;   /* default precision is 1 */
                    if (precision == 0 && arg_sign == 0
#if defined(HPUX_BUG_COMPATIBLE) || defined(LINUX_COMPATIBLE)
                            && fmt_spec != 'p'
                            /* HPUX 10 man page claims: With conversion character p the result of
                             * converting a zero value with a precision of zero is a null string.
                             * Actually HP returns all zeroes, and Linux returns "(nil)". */
#endif
                       )
                    {
                        /* converted to null string */
                        /* When zero value is formatted with an explicit precision 0,
                           the resulting formatted string is empty (d, i, u, o, x, X, p).   */
                    }
                    else
                    {
                        oscl_wchar f[5];
                        int f_l = 0;
                        f[f_l++] = '%';    /* construct a simple format string for sprintf */
                        if (!length_modifier) { }
                        else if (length_modifier == '2')
                        {
                            f[f_l++] = 'l';
                            f[f_l++] = 'l';
                        }
                        else f[f_l++] = length_modifier;
                        f[f_l++] = fmt_spec;
                        f[f_l++] = '\0';
                        if (fmt_spec == 'p')
                        {
                            str_arg_l += oscl_default_swprintf(tmp + str_arg_l, NUMERIC_CONV_BUF_SIZE - str_arg_l, f, ptr_arg);
                        }
                        else if (fmt_spec == 'd')    /* signed */
                        {
                            switch (length_modifier)
                            {
                                case '\0':
                                case 'h':
                                    str_arg_l += oscl_default_swprintf(tmp + str_arg_l, NUMERIC_CONV_BUF_SIZE - str_arg_l, f, int_arg);
                                    break;

                                case 'l':
                                    str_arg_l += oscl_default_swprintf(tmp + str_arg_l, NUMERIC_CONV_BUF_SIZE - str_arg_l, f, long_arg);
                                    break;

#ifdef SNPRINTF_LONGLONG_SUPPORT
                                case '2':
                                    str_arg_l += oscl_default_swprintf(tmp + str_arg_l, NUMERIC_CONV_BUF_SIZE - str_arg_l, f, long_long_arg);
                                    break;
#endif //snprintf_longlong_support

                            }
                        }
                        else    /* unsigned */
                        {
                            switch (length_modifier)
                            {
                                case '\0':
                                case 'h':
                                    str_arg_l += oscl_default_swprintf(tmp + str_arg_l, NUMERIC_CONV_BUF_SIZE - str_arg_l, f, uint_arg);
                                    break;

                                case 'l':
                                    str_arg_l += oscl_default_swprintf(tmp + str_arg_l, NUMERIC_CONV_BUF_SIZE - str_arg_l, f, ulong_arg);
                                    break;

#ifdef SNPRINTF_LONGLONG_SUPPORT
                                case '2':
                                    str_arg_l += oscl_default_swprintf(tmp + str_arg_l, NUMERIC_CONV_BUF_SIZE - str_arg_l, f, ulong_long_arg);
                                    break;
#endif //snprintf_longlong_support

                            }
                        }
                        /* include the optional minus sign and possible "0x"
                           in the region before the zero padding insertion point */
                        if (zero_padding_insertion_ind < str_arg_l &&
                                tmp[zero_padding_insertion_ind] == '-')
                        {
                            zero_padding_insertion_ind++;
                        }
                        if (zero_padding_insertion_ind + 1 < str_arg_l &&
                                tmp[zero_padding_insertion_ind]   == '0' &&
                                (tmp[zero_padding_insertion_ind+1] == 'x' ||
                                 tmp[zero_padding_insertion_ind+1] == 'X'))
                        {
                            zero_padding_insertion_ind += 2;
                        }
                    }
                    {
                        size_t num_of_digits = str_arg_l - zero_padding_insertion_ind;
                        if (alternate_form && fmt_spec == 'o'
#ifdef HPUX_COMPATIBLE                                  /* ("%#.o",0) -> ""  */
                                && (str_arg_l > 0)
#endif
#ifdef DIGITAL_UNIX_BUG_COMPATIBLE                      /* ("%#o",0) -> "00" */
#else
                                /* unless zero is already the first character */
                                && !(zero_padding_insertion_ind < str_arg_l
                                     && tmp[zero_padding_insertion_ind] == '0')
#endif
                           )          /* assure leading zero for alternate-form octal numbers */
                        {
                            if (!precision_specified || precision < num_of_digits + 1)
                            {
                                /* precision is increased to force the first character to be zero,
                                   except if a zero value is formatted with an explicit precision
                                   of zero */
                                precision = num_of_digits + 1;
                                precision_specified = 1;
                            }
                        }
                        /* zero padding to specified precision? */
                        if (num_of_digits < precision)
                            number_of_zeros_to_pad = precision - num_of_digits;
                    }
                    /* zero padding to specified minimal field width? */
                    if (!justify_left && zero_padding)
                    {
                        size_t n = min_field_width - (str_arg_l + number_of_zeros_to_pad);
                        if (n > 0) number_of_zeros_to_pad += n;
                    }
                    break;
                }
                default: /* unrecognized conversion specifier, keep format string as-is*/
                    zero_padding = 0;  /* turn zero padding off for non-numeric convers. */
#ifndef DIGITAL_UNIX_COMPATIBLE
                    justify_left = 1;
                    min_field_width = 0;                /* reset flags */
#endif
#if defined(PERL_COMPATIBLE) || defined(LINUX_COMPATIBLE)
                    /* keep the entire format string unchanged */
                    str_arg = starting_p;
                    str_arg_l = p - starting_p;
                    /* well, not exactly so for Linux, which does something inbetween,
                     * and I don't feel an urge to imitate it: "%+++++hy" -> "%+y"  */
#else
                    /* discard the unrecognized conversion, just keep *
                     * the unrecognized conversion character          */
                    str_arg = p;
                    str_arg_l = 0;
#endif
                    if (*p) str_arg_l++;  /* include invalid conversion specifier unchanged
                                 if not at end-of-string */
                    break;
            }
            if (*p) p++;      /* step over the just processed conversion specifier */
            /* insert padding to the left as requested by min_field_width;
               this does not include the zero padding in case of numerical conversions*/
            if (!justify_left)                  /* left padding with blank or zero */
            {
                int n = min_field_width - (str_arg_l + number_of_zeros_to_pad);
                if (n > 0)
                {
                    if (str_l < str_m)
                    {
                        size_t avail = str_m - str_l;
                        oscl_strset(str + str_l, (zero_padding ? '0' : ' '), (n > (int)avail ? avail : n));
                    }
                    str_l += n;
                }
            }
            /* zero padding as requested by the precision or by the minimal field width
             * for numeric conversions required? */
            if (number_of_zeros_to_pad <= 0)
            {
                /* will not copy first part of numeric right now, *
                 * force it to be copied later in its entirety    */
                zero_padding_insertion_ind = 0;
            }
            else
            {
                /* insert first part of numerics (sign or '0x') before zero padding */
                int n = zero_padding_insertion_ind;
                if (n > 0)
                {
                    if (str_l < str_m)
                    {
                        size_t avail = str_m - str_l;
                        oscl_strncpy(str + str_l, str_arg, (n > (int)avail ? avail : n));
                    }
                    str_l += n;
                }
                /* insert zero padding as requested by the precision or min field width */
                n = number_of_zeros_to_pad;
                if (n > 0)
                {
                    if (str_l < str_m)
                    {
                        size_t avail = str_m - str_l;
                        oscl_strset(str + str_l, '0', (n > (int)avail ? avail : n));
                    }
                    str_l += n;
                }
            }
            /* insert formatted string
             * (or as-is conversion specifier for unknown conversions) */
            {
                int n = str_arg_l - zero_padding_insertion_ind;
                if (n > 0)
                {
                    if (str_l < str_m)
                    {
                        size_t avail = str_m - str_l;
                        oscl_strncpy(str + str_l, str_arg + zero_padding_insertion_ind,
                                     (n > (int)avail ? avail : n));
                    }
                    str_l += n;
                }
            }
            /* insert right padding */
            if (justify_left)            /* right blank padding to the field width */
            {
                int n = min_field_width - (str_arg_l + number_of_zeros_to_pad);
                if (n > 0)
                {
                    if (str_l < str_m)
                    {
                        size_t avail = str_m - str_l;
                        oscl_strset(str + str_l, ' ', (n > (int)avail ? avail : n));
                    }
                    str_l += n;
                }
            }
        }
    }
#if defined(NEED_SNPRINTF_ONLY)
    //va_end(ap); jg
#endif
    if (str_m > 0)
    { /* make sure the string is null-terminated
                      even at the expense of overwriting the last character
                      (shouldn't happen, but just in case) */
        str[str_l <= str_m-1 ? str_l : str_m-1] = '\0';
    }
    /* Return the number of characters formatted (excluding trailing null
     * character), that is, the number of characters that would have been
     * written to the buffer if it were large enough.
     *
     * The value of str_l should be returned, but str_l is of unsigned type
     * size_t, and snprintf is int, possibly leading to an undetected
     * integer overflow, resulting in a negative return value, which is illegal.
     * Both XSH5 and ISO C99 (at least the draft) are silent on this issue.
     * Should errno be set to EOVERFLOW and EOF returned in this case???
     */
    return (int) str_l;
}

#endif

OSCL_EXPORT_REF int32 oscl_snprintf(oscl_wchar *str, uint32 count, const oscl_wchar *fmt, /*args*/ ...)
{
    int str_l;
    va_list args;
    va_start(args, fmt);
    str_l = portable_swprintf(str, count, fmt, args);
    return str_l;
}

OSCL_EXPORT_REF int32 oscl_vsnprintf(oscl_wchar *str, uint32 count, const oscl_wchar *fmt, va_list args)
{
    int str_l;
    str_l = portable_swprintf(str, count, fmt, args);
    return str_l;
}

