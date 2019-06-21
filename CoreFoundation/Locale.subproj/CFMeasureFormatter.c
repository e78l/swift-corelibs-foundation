/*  CFMeasureFormatter.m
    Copyright (c) 1998-2018, Apple Inc. and the Swift project authors

    Portions Copyright (c) 2019, Apple Inc. and the Swift project authors
    Licensed under Apache License v2.0 with Runtime Library Exception
    See http://swift.org/LICENSE.txt for license information
    See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
 */

#include <CoreFoundation/CFMeasureFormatter.h>
#include "CFMeasureFormatterICU.h"

#include "CFPriv.h"
#include "CFInternal.h"
#include "CFICULogging.h"

void CFMeasureFormatterOpen(CFMeasureFormatterRef formatter, CFMeasureUnit unit);
void CFMeasureFormatterClose(CFMeasureFormatterRef formatter);

struct __CFMeasureFormatter {
    CFRuntimeBase _base;
    CFLocaleRef _locale;
    CFNumberFormatterRef _numberFormatter;
    CFMeasureFormatterStyle _style;
    CFMeasureUnit _unit;
    void *_icu_measfmt;
    void *_icu_unit;
    struct {
    } _property;
};

static void __CFMeasureFormatterClassDeallocate(CFTypeRef cf) {
    CFMeasureFormatterRef formatter = (CFMeasureFormatterRef)cf;
    CFRelease(formatter->_locale);
}

static CFStringRef __CFMeasureFormatterClassCopyDescription(CFTypeRef cf) {
    CFMeasureFormatterRef formatter = (CFMeasureFormatterRef)cf;
    CFAllocatorRef allocator = CFGetAllocator(formatter);
    return CFStringCreateWithFormat(allocator, NULL, CFSTR("<CFDateComponentsFormatter %p [%p]>"), cf, allocator);
}

static const CFRuntimeClass __CFMeasureFormatterClass = {
    0,
    "CFMeasureFormatter",
    NULL,        // init
    NULL,        // copy
    __CFMeasureFormatterClassDeallocate,
    NULL,    // equals
    NULL,    // hash
    NULL,    // copyFormattingDescription
    __CFMeasureFormatterClassCopyDescription
};

static CFTypeID __kCFMeasureFormatterTypeID = _kCFRuntimeNotATypeID;

CFTypeID CFMeasureFormatterGetTypeID(void) {
    static dispatch_once_t initOnce = 0;
    dispatch_once(&initOnce, ^{
        __kCFMeasureFormatterTypeID = _CFRuntimeRegisterClass(&__CFMeasureFormatterClass);
    });
    return __kCFMeasureFormatterTypeID;
}

CFMeasureFormatterRef _Nullable
CFMeasureFormatterCreate(CFAllocatorRef allocator, CFLocaleRef locale, CFMeasureFormatterStyle style) {

    // Pointer `dcf` to new date component formatter
    struct __CFMeasureFormatter *dcf;

    // Use default allocator if `NULL`
    if (allocator == NULL) {
        allocator = __CFGetDefaultAllocator();
    }
    // Check locale
    __CFGenericValidateType(allocator, CFAllocatorGetTypeID());
    if (locale == NULL) {
        locale = CFLocaleGetSystem();
    }
    if (locale) __CFGenericValidateType(locale, CFLocaleGetTypeID());

    uint32_t size = sizeof(struct __CFMeasureFormatter) - sizeof(CFRuntimeBase);
    dcf = (CFMeasureFormatterRef)_CFRuntimeCreateInstance(allocator, CFMeasureFormatterGetTypeID(), size, NULL);

    if (dcf != NULL) {
        dcf->_locale = CFRetain(locale);
        dcf->_style = style;
    }

    return dcf;
}

CFLocaleRef CFMeasureFormatterGetLocale(CFMeasureFormatterRef formatter) {
    return formatter->_locale;
}

CFNumberFormatterRef _Nullable
CFMeasureFormatterGetNumberFormatter(CFMeasureFormatterRef formatter) {
    return formatter->_numberFormatter;
}

void CFMeasureFormatterOpen(CFMeasureFormatterRef formatter, CFMeasureUnit unit) {
    // Is formatter already open?
    bool open = false;
    if (formatter->_icu_measfmt == NULL) {
        open = true;
    } else if (formatter->_unit != unit) {
        // Close then open
        CFMeasureFormatterClose(formatter);
        open = true;
    }
    if (open) {
        const char *localeID = CFStringGetCStringPtr(CFLocaleGetIdentifier(formatter->_locale), kCFStringEncodingASCII);
        formatter->_unit = unit;
        c_measfmt_open(&formatter->_icu_measfmt, &formatter->_icu_unit, localeID, formatter->_style, formatter->_unit);
    }
}

void CFMeasureFormatterClose(CFMeasureFormatterRef formatter) {
    if (formatter->_icu_measfmt) {
        c_measfmt_close(formatter->_icu_measfmt, formatter->_icu_unit);
        formatter->_icu_measfmt = NULL;
    }
}

CFStringRef _Nullable
CFMeasureFormatterCreateNumberUnit(CFMeasureFormatterRef formatter, double value, CFMeasureUnit unit) {

    CFMeasureFormatterOpen(formatter, unit);

    char cstr[64];
    c_measfmt_get_numunit(formatter->_icu_measfmt, formatter->_icu_unit, value, cstr, 64);
    CFStringRef result = CFStringCreateWithCString(kCFAllocatorSystemDefault, cstr, kCFStringEncodingUTF8);
    return result;
}

CFStringRef _Nullable
CFMeasureFormatterCreateUnitName(CFMeasureFormatterRef formatter, CFMeasureUnit unit) {

    CFMeasureFormatterOpen(formatter, unit);

    char cstr[64];
    c_measfmt_get_unitname(formatter->_icu_measfmt, formatter->_icu_unit, cstr, 64);
    CFStringRef result = CFStringCreateWithCString(kCFAllocatorSystemDefault, cstr, kCFStringEncodingUTF8);
    return result;
}
