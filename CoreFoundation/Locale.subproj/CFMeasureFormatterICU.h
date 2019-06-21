/*  CFMeasureFormatter.h
    Copyright (c) 1998-2019, Apple Inc. and the Swift project authors

    Portions Copyright (c) 2019, Apple Inc. and the Swift project authors
    Licensed under Apache License v2.0 with Runtime Library Exception
    See http://swift.org/LICENSE.txt for license information
    See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
*/

#if !defined(__COREFOUNDATION_CFMEASUREFORMATTERICU__)
#define __COREFOUNDATION_CFMEASUREFORMATTERICU__ 1

#include <CoreFoundation/CFBase.h>

CF_EXTERN_C_BEGIN

// Specific
int c_measfmt_get_unitname(void *measfmtc, void *unitc, char *unitname, int maxlen);
int c_measfmt_get_numunit(void *measfmtc, void *unitc, double value, char *cstr, int maxlen);

// Get a measure format for use with _open
int c_measfmt_open(void **measfmt, void **mu, const char *locale, CFMeasureFormatterStyle cwidth, CFMeasureUnit unit);
// Free measure format and unit with close
int c_measfmt_close(void **measfmt, void **mu);

CF_EXTERN_C_END

#endif /* __COREFOUNDATION_CFMEASUREFORMATTERICU__ */
