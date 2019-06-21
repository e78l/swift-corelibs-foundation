/*  CFMeasureFormatterICU.cpp
    Copyright (c) 1998-2018, Apple Inc. and the Swift project authors

    Portions Copyright (c) 2019, Apple Inc. and the Swift project authors
    Licensed under Apache License v2.0 with Runtime Library Exception
    See http://swift.org/LICENSE.txt for license information
    See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
*/

#include <CoreFoundation/CFMeasureFormatter.h>
#include "CFMeasureFormatterICU.h"

#include <unicode/utypes.h>
#include <unicode/measunit.h>
#include <unicode/measfmt.h>
#include <unicode/measure.h>

U_NAMESPACE_USE

#if DEBUG && 0
#include <iostream>
#include <sstream>

// This function can be used to generate
// CFMeasureUnit enum definition
extern "C"
void CFMeasureFormatterDumpUnits(void) {
    // Retrieve all units
    MeasureUnit *units = new MeasureUnit[512];
    UErrorCode code = U_ZERO_ERROR;
    int count = MeasureUnit::getAvailable(units, 512, code);
    // Define enum value as CFMeasureUnit[getSubtype() value] = ICU_UNIT([typeID], [subtypeID])
    int typeID = 0;
    int subtypeID = -1;
    const char *prefix = "CFMeasureUnit";
    // Switch code
    std::ostringstream switchCode;
    // Will increment subtypeID, keep track when type changes
    const char *lastType = units[0].getType();
    for (int i = 0; i < count; i++) {
        if (strcmp(units[i].getType(), "currency") == 0)
            continue; // ignore currencies
        char *subtype = (char *)units[i].getSubtype();
        char name[128];
        strcpy(name, prefix);

        // Loop through subtype string, copying it to `name`
        bool capitalizeNext = true;
        int jlen = (int)strlen(subtype);
        char *c = name + strlen(name);
        for (int j = 0; j < jlen; j++) {
            if (subtype[j] == 0x0) {
                break; // end of string
            } else if (subtype[j] == '-') {
                // skip over -, capitalize next word
                capitalizeNext = true;
                j++;
            }
            if (capitalizeNext) {
                *c = toupper(subtype[j]);
                capitalizeNext = false;
            } else {
                *c = subtype[j];
            }
            c++;
        }
        *c = 0x0;
        if (strcmp(lastType, units[i].getType()) != 0) {
            lastType = units[i].getType();
            typeID++;
            subtypeID = 0;
        } else {
            subtypeID++;
        }
        char lines[256];
        snprintf(lines, sizeof(lines), "case %s:\nunit = UNIT_FROM(create%s);\nbreak;\n", name, name+strlen(prefix));
        switchCode << lines;
        printf("%s = ICU_UNIT(%d, %d),\n", name, typeID, subtypeID);
    }
    std::cout << switchCode.str();
}

#endif /* DEBUG */

// MARK: -

// Macro to call MeasureUnit creator
#define UNIT_FROM(__create) MeasureUnit::__create(status)

static MeasureUnit * createObjectForMeasureUnit(CFMeasureUnit cfunit) {
    MeasureUnit *unit = NULL;
    UErrorCode status = U_ZERO_ERROR;
    switch (cfunit) {
        case CFMeasureUnitGForce:
            unit = UNIT_FROM(createGForce);
            break;
        case CFMeasureUnitMeterPerSecondSquared:
            unit = UNIT_FROM(createMeterPerSecondSquared);
            break;
        case CFMeasureUnitArcMinute:
            unit = UNIT_FROM(createArcMinute);
            break;
        case CFMeasureUnitArcSecond:
            unit = UNIT_FROM(createArcSecond);
            break;
        case CFMeasureUnitDegree:
            unit = UNIT_FROM(createDegree);
            break;
        case CFMeasureUnitRadian:
            unit = UNIT_FROM(createRadian);
            break;
        case CFMeasureUnitRevolution:
#if U_ICU_VERSION_MAJOR_NUM > 55
            unit = UNIT_FROM(createRevolutionAngle);
#endif
            break;
        case CFMeasureUnitAcre:
            unit = UNIT_FROM(createAcre);
            break;
        case CFMeasureUnitHectare:
            unit = UNIT_FROM(createHectare);
            break;
        case CFMeasureUnitSquareCentimeter:
            unit = UNIT_FROM(createSquareCentimeter);
            break;
        case CFMeasureUnitSquareFoot:
            unit = UNIT_FROM(createSquareFoot);
            break;
        case CFMeasureUnitSquareInch:
            unit = UNIT_FROM(createSquareInch);
            break;
        case CFMeasureUnitSquareKilometer:
            unit = UNIT_FROM(createSquareKilometer);
            break;
        case CFMeasureUnitSquareMeter:
            unit = UNIT_FROM(createSquareMeter);
            break;
        case CFMeasureUnitSquareMile:
            unit = UNIT_FROM(createSquareMile);
            break;
        case CFMeasureUnitSquareYard:
            unit = UNIT_FROM(createSquareYard);
            break;
        case CFMeasureUnitKarat:
            unit = UNIT_FROM(createKarat);
            break;
        case CFMeasureUnitMilligramPerDeciliter:
#if U_ICU_VERSION_MAJOR_NUM > 56
            unit = UNIT_FROM(createMilligramPerDeciliter);
#endif
            break;
        case CFMeasureUnitMillimolePerLiter:
#if U_ICU_VERSION_MAJOR_NUM > 56
            unit = UNIT_FROM(createMillimolePerLiter);
#endif
            break;
        case CFMeasureUnitPartPerMillion:
#if U_ICU_VERSION_MAJOR_NUM > 56
            unit = UNIT_FROM(createPartPerMillion);
#endif
            break;
        case CFMeasureUnitLiterPer100kilometers:
#if U_ICU_VERSION_MAJOR_NUM > 55
            unit = UNIT_FROM(createLiterPer100Kilometers);
#endif
            break;
        case CFMeasureUnitLiterPerKilometer:
            unit = UNIT_FROM(createLiterPerKilometer);
            break;
        case CFMeasureUnitMilePerGallon:
            unit = UNIT_FROM(createMilePerGallon);
            break;
        case CFMeasureUnitMilePerGallonImperial:
#if U_ICU_VERSION_MAJOR_NUM > 56
            unit = UNIT_FROM(createMilePerGallonImperial);
#endif
            break;
        case CFMeasureUnitBit:
            unit = UNIT_FROM(createBit);
            break;
        case CFMeasureUnitByte:
            unit = UNIT_FROM(createByte);
            break;
        case CFMeasureUnitGigabit:
            unit = UNIT_FROM(createGigabit);
            break;
        case CFMeasureUnitGigabyte:
            unit = UNIT_FROM(createGigabyte);
            break;
        case CFMeasureUnitKilobit:
            unit = UNIT_FROM(createKilobit);
            break;
        case CFMeasureUnitKilobyte:
            unit = UNIT_FROM(createKilobyte);
            break;
        case CFMeasureUnitMegabit:
            unit = UNIT_FROM(createMegabit);
            break;
        case CFMeasureUnitMegabyte:
            unit = UNIT_FROM(createMegabyte);
            break;
        case CFMeasureUnitTerabit:
            unit = UNIT_FROM(createTerabit);
            break;
        case CFMeasureUnitTerabyte:
            unit = UNIT_FROM(createTerabyte);
            break;
        case CFMeasureUnitCentury:
#if U_ICU_VERSION_MAJOR_NUM > 55
            unit = UNIT_FROM(createCentury);
#endif
            break;
        case CFMeasureUnitDay:
            unit = UNIT_FROM(createDay);
            break;
        case CFMeasureUnitHour:
            unit = UNIT_FROM(createHour);
            break;
        case CFMeasureUnitMicrosecond:
            unit = UNIT_FROM(createMicrosecond);
            break;
        case CFMeasureUnitMillisecond:
            unit = UNIT_FROM(createMillisecond);
            break;
        case CFMeasureUnitMinute:
            unit = UNIT_FROM(createMinute);
            break;
        case CFMeasureUnitMonth:
            unit = UNIT_FROM(createMonth);
            break;
        case CFMeasureUnitNanosecond:
            unit = UNIT_FROM(createNanosecond);
            break;
        case CFMeasureUnitSecond:
            unit = UNIT_FROM(createSecond);
            break;
        case CFMeasureUnitWeek:
            unit = UNIT_FROM(createWeek);
            break;
        case CFMeasureUnitYear:
            unit = UNIT_FROM(createYear);
            break;
        case CFMeasureUnitAmpere:
            unit = UNIT_FROM(createAmpere);
            break;
        case CFMeasureUnitMilliampere:
            unit = UNIT_FROM(createMilliampere);
            break;
        case CFMeasureUnitOhm:
            unit = UNIT_FROM(createOhm);
            break;
        case CFMeasureUnitVolt:
            unit = UNIT_FROM(createVolt);
            break;
        case CFMeasureUnitCalorie:
            unit = UNIT_FROM(createCalorie);
            break;
        case CFMeasureUnitFoodcalorie:
            unit = UNIT_FROM(createFoodcalorie);
            break;
        case CFMeasureUnitJoule:
            unit = UNIT_FROM(createJoule);
            break;
        case CFMeasureUnitKilocalorie:
            unit = UNIT_FROM(createKilocalorie);
            break;
        case CFMeasureUnitKilojoule:
            unit = UNIT_FROM(createKilojoule);
            break;
        case CFMeasureUnitKilowattHour:
            unit = UNIT_FROM(createKilowattHour);
            break;
        case CFMeasureUnitGigahertz:
            unit = UNIT_FROM(createGigahertz);
            break;
        case CFMeasureUnitHertz:
            unit = UNIT_FROM(createHertz);
            break;
        case CFMeasureUnitKilohertz:
            unit = UNIT_FROM(createKilohertz);
            break;
        case CFMeasureUnitMegahertz:
            unit = UNIT_FROM(createMegahertz);
            break;
        case CFMeasureUnitAstronomicalUnit:
            unit = UNIT_FROM(createAstronomicalUnit);
            break;
        case CFMeasureUnitCentimeter:
            unit = UNIT_FROM(createCentimeter);
            break;
        case CFMeasureUnitDecimeter:
            unit = UNIT_FROM(createDecimeter);
            break;
        case CFMeasureUnitFathom:
            unit = UNIT_FROM(createFathom);
            break;
        case CFMeasureUnitFoot:
            unit = UNIT_FROM(createFoot);
            break;
        case CFMeasureUnitFurlong:
            unit = UNIT_FROM(createFurlong);
            break;
        case CFMeasureUnitInch:
            unit = UNIT_FROM(createInch);
            break;
        case CFMeasureUnitKilometer:
            unit = UNIT_FROM(createKilometer);
            break;
        case CFMeasureUnitLightYear:
            unit = UNIT_FROM(createLightYear);
            break;
        case CFMeasureUnitMeter:
            unit = UNIT_FROM(createMeter);
            break;
        case CFMeasureUnitMicrometer:
            unit = UNIT_FROM(createMicrometer);
            break;
        case CFMeasureUnitMile:
            unit = UNIT_FROM(createMile);
            break;
        case CFMeasureUnitMileScandinavian:
#if U_ICU_VERSION_MAJOR_NUM > 55
            unit = UNIT_FROM(createMileScandinavian);
#endif
            break;
        case CFMeasureUnitMillimeter:
            unit = UNIT_FROM(createMillimeter);
            break;
        case CFMeasureUnitNanometer:
            unit = UNIT_FROM(createNanometer);
            break;
        case CFMeasureUnitNauticalMile:
            unit = UNIT_FROM(createNauticalMile);
            break;
        case CFMeasureUnitParsec:
            unit = UNIT_FROM(createParsec);
            break;
        case CFMeasureUnitPicometer:
            unit = UNIT_FROM(createPicometer);
            break;
        case CFMeasureUnitPoint:
#if U_ICU_VERSION_MAJOR_NUM > 58
            unit = UNIT_FROM(createPoint);
#endif
            break;
        case CFMeasureUnitYard:
            unit = UNIT_FROM(createYard);
            break;
        case CFMeasureUnitLux:
            unit = UNIT_FROM(createLux);
            break;
        case CFMeasureUnitCarat:
            unit = UNIT_FROM(createCarat);
            break;
        case CFMeasureUnitGram:
            unit = UNIT_FROM(createGram);
            break;
        case CFMeasureUnitKilogram:
            unit = UNIT_FROM(createKilogram);
            break;
        case CFMeasureUnitMetricTon:
            unit = UNIT_FROM(createMetricTon);
            break;
        case CFMeasureUnitMicrogram:
            unit = UNIT_FROM(createMicrogram);
            break;
        case CFMeasureUnitMilligram:
            unit = UNIT_FROM(createMilligram);
            break;
        case CFMeasureUnitOunce:
            unit = UNIT_FROM(createOunce);
            break;
        case CFMeasureUnitOunceTroy:
            unit = UNIT_FROM(createOunceTroy);
            break;
        case CFMeasureUnitPound:
            unit = UNIT_FROM(createPound);
            break;
        case CFMeasureUnitStone:
            unit = UNIT_FROM(createStone);
            break;
        case CFMeasureUnitTon:
            unit = UNIT_FROM(createTon);
            break;
        case CFMeasureUnitBase:
            // unsupported
            break;
        case CFMeasureUnitPercent:
#if U_ICU_VERSION_MAJOR_NUM > 62
            unit = UNIT_FROM(createPercent);
#endif
            break;
        case CFMeasureUnitPermille:
#if U_ICU_VERSION_MAJOR_NUM > 62
            unit = UNIT_FROM(createPermille);
#endif
            break;
        case CFMeasureUnitGigawatt:
            unit = UNIT_FROM(createGigawatt);
            break;
        case CFMeasureUnitHorsepower:
            unit = UNIT_FROM(createHorsepower);
            break;
        case CFMeasureUnitKilowatt:
            unit = UNIT_FROM(createKilowatt);
            break;
        case CFMeasureUnitMegawatt:
            unit = UNIT_FROM(createMegawatt);
            break;
        case CFMeasureUnitMilliwatt:
            unit = UNIT_FROM(createMilliwatt);
            break;
        case CFMeasureUnitWatt:
            unit = UNIT_FROM(createWatt);
            break;
        case CFMeasureUnitHectopascal:
            unit = UNIT_FROM(createHectopascal);
            break;
        case CFMeasureUnitInchHg:
            unit = UNIT_FROM(createInchHg);
            break;
        case CFMeasureUnitMillibar:
            unit = UNIT_FROM(createMillibar);
            break;
        case CFMeasureUnitMillimeterOfMercury:
            unit = UNIT_FROM(createMillimeterOfMercury);
            break;
        case CFMeasureUnitPoundPerSquareInch:
            unit = UNIT_FROM(createPoundPerSquareInch);
            break;
        case CFMeasureUnitKilometerPerHour:
            unit = UNIT_FROM(createKilometerPerHour);
            break;
        case CFMeasureUnitKnot:
#if U_ICU_VERSION_MAJOR_NUM > 55
            unit = UNIT_FROM(createKnot);
#endif
            break;
        case CFMeasureUnitMeterPerSecond:
            unit = UNIT_FROM(createMeterPerSecond);
            break;
        case CFMeasureUnitMilePerHour:
            unit = UNIT_FROM(createMilePerHour);
            break;
        case CFMeasureUnitCelsius:
            unit = UNIT_FROM(createCelsius);
            break;
        case CFMeasureUnitFahrenheit:
            unit = UNIT_FROM(createFahrenheit);
            break;
        case CFMeasureUnitGeneric:
            unit = UNIT_FROM(createGenericTemperature);
            break;
        case CFMeasureUnitKelvin:
            unit = UNIT_FROM(createKelvin);
            break;
        case CFMeasureUnitAcreFoot:
            unit = UNIT_FROM(createAcreFoot);
            break;
        case CFMeasureUnitBushel:
            unit = UNIT_FROM(createBushel);
            break;
        case CFMeasureUnitCentiliter:
            unit = UNIT_FROM(createCentiliter);
            break;
        case CFMeasureUnitCubicCentimeter:
            unit = UNIT_FROM(createCubicCentimeter);
            break;
        case CFMeasureUnitCubicFoot:
            unit = UNIT_FROM(createCubicFoot);
            break;
        case CFMeasureUnitCubicInch:
            unit = UNIT_FROM(createCubicInch);
            break;
        case CFMeasureUnitCubicKilometer:
            unit = UNIT_FROM(createCubicKilometer);
            break;
        case CFMeasureUnitCubicMeter:
            unit = UNIT_FROM(createCubicMeter);
            break;
        case CFMeasureUnitCubicMile:
            unit = UNIT_FROM(createCubicMile);
            break;
        case CFMeasureUnitCubicYard:
            unit = UNIT_FROM(createCubicYard);
            break;
        case CFMeasureUnitCup:
            unit = UNIT_FROM(createCup);
            break;
        case CFMeasureUnitCupMetric:
#if U_ICU_VERSION_MAJOR_NUM > 55
            unit = UNIT_FROM(createCupMetric);
#endif
            break;
        case CFMeasureUnitDeciliter:
            unit = UNIT_FROM(createDeciliter);
            break;
        case CFMeasureUnitFluidOunce:
            unit = UNIT_FROM(createFluidOunce);
            break;
        case CFMeasureUnitGallon:
            unit = UNIT_FROM(createGallon);
            break;
        case CFMeasureUnitGallonImperial:
#if U_ICU_VERSION_MAJOR_NUM > 56
            unit = UNIT_FROM(createGallonImperial);
#endif
            break;
        case CFMeasureUnitHectoliter:
            unit = UNIT_FROM(createHectoliter);
            break;
        case CFMeasureUnitLiter:
            unit = UNIT_FROM(createLiter);
            break;
        case CFMeasureUnitMegaliter:
            unit = UNIT_FROM(createMegaliter);
            break;
        case CFMeasureUnitMilliliter:
            unit = UNIT_FROM(createMilliliter);
            break;
        case CFMeasureUnitPint:
            unit = UNIT_FROM(createPint);
            break;
        case CFMeasureUnitPintMetric:
#if U_ICU_VERSION_MAJOR_NUM > 55
            unit = UNIT_FROM(createPintMetric);
#endif
            break;
        case CFMeasureUnitQuart:
            unit = UNIT_FROM(createQuart);
            break;
        case CFMeasureUnitTablespoon:
            unit = UNIT_FROM(createTablespoon);
            break;
        case CFMeasureUnitTeaspoon:
            unit = UNIT_FROM(createTeaspoon);
            break;

    }
    return unit;

}

int c_measfmt_open(void **measfmt, void **mu, const char *locale, CFMeasureFormatterStyle cwidth, CFMeasureUnit unit) {
    UErrorCode status = U_ZERO_ERROR;

    UMeasureFormatWidth width;
    switch (cwidth) {
        case CFMeasureFormatterStyleLong:
            width = UMEASFMT_WIDTH_WIDE;
            break;
        case CFMeasureFormatterStyleMedium:
        case CFMeasureFormatterStyleDefault:
            width = UMEASFMT_WIDTH_SHORT;
            break;
        case CFMeasureFormatterStyleShort:
            width = UMEASFMT_WIDTH_NARROW;
            break;
        case CFMeasureFormatterStyleNumeric:
            width = UMEASFMT_WIDTH_NUMERIC;
            break;
        default:
            status = U_ILLEGAL_ARGUMENT_ERROR;
    }

    *measfmt = new MeasureFormat(Locale(locale), width, status);
    *mu = createObjectForMeasureUnit(unit);

    return status;
}

int c_measfmt_close(void **measfmt, void **mu) {
    printf("Closing");
 return 0;
    auto fmt = reinterpret_cast<MeasureFormat *>(measfmt);
    delete fmt;

    auto unit = reinterpret_cast<MeasureUnit *>(mu);
    delete unit;

    return 0;
}

int c_measfmt_get_unitname(void *measfmtc, void *unitc, char *cstr, int maxlen) {
    UErrorCode status = U_ZERO_ERROR;

#if U_ICU_VERSION_MAJOR_NUM >= 58
    MeasureFormat *measfmt = reinterpret_cast<MeasureFormat *>(measfmtc);
    MeasureUnit *unit = reinterpret_cast<MeasureUnit *>(unitc);
    UnicodeString str = measfmt->getUnitDisplayName(*unit, status);
    str.extract(cstr, maxlen, NULL, status);
#else
    // fail with null string
    cstr[0] = 0x0;
#endif
    return status;
}

int c_measfmt_get_numunit(void *measfmtc, void *unitc, double value, char *cstr, int maxlen) {
    UErrorCode status = U_ZERO_ERROR;

#if U_ICU_VERSION_MAJOR_NUM >= 53
    MeasureFormat *measfmt = reinterpret_cast<MeasureFormat *>(measfmtc);
    MeasureUnit *unit = reinterpret_cast<MeasureUnit *>(unitc);
    auto measure = new Measure(value, new MeasureUnit(*unit), status);

    UnicodeString str;
    measfmt->format(measure, str, status);

    str.extract(cstr, maxlen, NULL, status);
#else
    // fail with null string
    cstr[0] = 0x0;
#endif
    return status;
}
