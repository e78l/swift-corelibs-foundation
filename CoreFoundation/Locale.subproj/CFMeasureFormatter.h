/*  CFMeasureFormatter.h
    Copyright (c) 1998-2019, Apple Inc. and the Swift project authors

    Portions Copyright (c) 2019, Apple Inc. and the Swift project authors
    Licensed under Apache License v2.0 with Runtime Library Exception
    See http://swift.org/LICENSE.txt for license information
    See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
*/

//
// Measure[ment] formatter for SwiftFoundation
//
// As of June 2019, the ICU-C project
// (http://icu-project.org) did not have a
// C measure API, though the MeasureFormat API
// was ported from Java to C++ around 2014.
//
// Swift MeasurementFormatter and DateComponentsFormatter
// call this formatter for object-to-string
// conversions. Future changes may use the updated NumberFormatter.
//
// String-to-object convesions seem unlikely to be supported
// It may make sense to ask for a unitless value and
// convert string->number using NumberFormatter
//

#if !defined(__COREFOUNDATION_CFMEASUREFORMATTER__)
#define __COREFOUNDATION_CFMEASUREFORMATTER__ 1

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFLocale.h>
#include <CoreFoundation/CFNumberFormatter.h>

CF_ASSUME_NONNULL_BEGIN
CF_IMPLICIT_BRIDGING_ENABLED
CF_EXTERN_C_BEGIN

typedef CF_ENUM(CFIndex, CFMeasureFormatterStyle) {
    CFMeasureFormatterStyleDefault = 0,
    CFMeasureFormatterStyleNumeric = 1,
    CFMeasureFormatterStyleShort   = 10,
    CFMeasureFormatterStyleMedium  = 20,
    CFMeasureFormatterStyleLong    = 30,
};

// CFMeasureUnit helps express MeasureUnit ICU objects
//
// Constants are not meant to correspond with ICU-unit type values
// Purpose of grouping is to keep related units close to one another
#define ICU_UNIT(group, subgroup) (group * 64 + subgroup)

// The enum was generated with a script
typedef CF_ENUM(CFIndex, CFMeasureUnit) {
    CFMeasureUnitGForce = ICU_UNIT(0, 0),
    CFMeasureUnitMeterPerSecondSquared = ICU_UNIT(0, 1),
    CFMeasureUnitArcMinute = ICU_UNIT(1, 0),
    CFMeasureUnitArcSecond = ICU_UNIT(1, 1),
    CFMeasureUnitDegree = ICU_UNIT(1, 2),
    CFMeasureUnitRadian = ICU_UNIT(1, 3),
    CFMeasureUnitRevolution = ICU_UNIT(1, 4),
    CFMeasureUnitAcre = ICU_UNIT(2, 0),
    CFMeasureUnitHectare = ICU_UNIT(2, 1),
    CFMeasureUnitSquareCentimeter = ICU_UNIT(2, 2),
    CFMeasureUnitSquareFoot = ICU_UNIT(2, 3),
    CFMeasureUnitSquareInch = ICU_UNIT(2, 4),
    CFMeasureUnitSquareKilometer = ICU_UNIT(2, 5),
    CFMeasureUnitSquareMeter = ICU_UNIT(2, 6),
    CFMeasureUnitSquareMile = ICU_UNIT(2, 7),
    CFMeasureUnitSquareYard = ICU_UNIT(2, 8),
    CFMeasureUnitKarat = ICU_UNIT(3, 0),
    CFMeasureUnitMilligramPerDeciliter = ICU_UNIT(3, 1),
    CFMeasureUnitMillimolePerLiter = ICU_UNIT(3, 2),
    CFMeasureUnitPartPerMillion = ICU_UNIT(3, 3),
    CFMeasureUnitLiterPer100kilometers = ICU_UNIT(4, 0),
    CFMeasureUnitLiterPerKilometer = ICU_UNIT(4, 1),
    CFMeasureUnitMilePerGallon = ICU_UNIT(4, 2),
    CFMeasureUnitMilePerGallonImperial = ICU_UNIT(4, 3),
    CFMeasureUnitBit = ICU_UNIT(5, 0),
    CFMeasureUnitByte = ICU_UNIT(5, 1),
    CFMeasureUnitGigabit = ICU_UNIT(5, 2),
    CFMeasureUnitGigabyte = ICU_UNIT(5, 3),
    CFMeasureUnitKilobit = ICU_UNIT(5, 4),
    CFMeasureUnitKilobyte = ICU_UNIT(5, 5),
    CFMeasureUnitMegabit = ICU_UNIT(5, 6),
    CFMeasureUnitMegabyte = ICU_UNIT(5, 7),
    CFMeasureUnitTerabit = ICU_UNIT(5, 8),
    CFMeasureUnitTerabyte = ICU_UNIT(5, 9),
    CFMeasureUnitCentury = ICU_UNIT(6, 0),
    CFMeasureUnitDay = ICU_UNIT(6, 1),
    CFMeasureUnitHour = ICU_UNIT(6, 2),
    CFMeasureUnitMicrosecond = ICU_UNIT(6, 3),
    CFMeasureUnitMillisecond = ICU_UNIT(6, 4),
    CFMeasureUnitMinute = ICU_UNIT(6, 5),
    CFMeasureUnitMonth = ICU_UNIT(6, 6),
    CFMeasureUnitNanosecond = ICU_UNIT(6, 7),
    CFMeasureUnitSecond = ICU_UNIT(6, 8),
    CFMeasureUnitWeek = ICU_UNIT(6, 9),
    CFMeasureUnitYear = ICU_UNIT(6, 10),
    CFMeasureUnitAmpere = ICU_UNIT(7, 0),
    CFMeasureUnitMilliampere = ICU_UNIT(7, 1),
    CFMeasureUnitOhm = ICU_UNIT(7, 2),
    CFMeasureUnitVolt = ICU_UNIT(7, 3),
    CFMeasureUnitCalorie = ICU_UNIT(8, 0),
    CFMeasureUnitFoodcalorie = ICU_UNIT(8, 1),
    CFMeasureUnitJoule = ICU_UNIT(8, 2),
    CFMeasureUnitKilocalorie = ICU_UNIT(8, 3),
    CFMeasureUnitKilojoule = ICU_UNIT(8, 4),
    CFMeasureUnitKilowattHour = ICU_UNIT(8, 5),
    CFMeasureUnitGigahertz = ICU_UNIT(9, 0),
    CFMeasureUnitHertz = ICU_UNIT(9, 1),
    CFMeasureUnitKilohertz = ICU_UNIT(9, 2),
    CFMeasureUnitMegahertz = ICU_UNIT(9, 3),
    CFMeasureUnitAstronomicalUnit = ICU_UNIT(10, 0),
    CFMeasureUnitCentimeter = ICU_UNIT(10, 1),
    CFMeasureUnitDecimeter = ICU_UNIT(10, 2),
    CFMeasureUnitFathom = ICU_UNIT(10, 3),
    CFMeasureUnitFoot = ICU_UNIT(10, 4),
    CFMeasureUnitFurlong = ICU_UNIT(10, 5),
    CFMeasureUnitInch = ICU_UNIT(10, 6),
    CFMeasureUnitKilometer = ICU_UNIT(10, 7),
    CFMeasureUnitLightYear = ICU_UNIT(10, 8),
    CFMeasureUnitMeter = ICU_UNIT(10, 9),
    CFMeasureUnitMicrometer = ICU_UNIT(10, 10),
    CFMeasureUnitMile = ICU_UNIT(10, 11),
    CFMeasureUnitMileScandinavian = ICU_UNIT(10, 12),
    CFMeasureUnitMillimeter = ICU_UNIT(10, 13),
    CFMeasureUnitNanometer = ICU_UNIT(10, 14),
    CFMeasureUnitNauticalMile = ICU_UNIT(10, 15),
    CFMeasureUnitParsec = ICU_UNIT(10, 16),
    CFMeasureUnitPicometer = ICU_UNIT(10, 17),
    CFMeasureUnitPoint = ICU_UNIT(10, 18),
    CFMeasureUnitYard = ICU_UNIT(10, 19),
    CFMeasureUnitLux = ICU_UNIT(11, 0),
    CFMeasureUnitCarat = ICU_UNIT(12, 0),
    CFMeasureUnitGram = ICU_UNIT(12, 1),
    CFMeasureUnitKilogram = ICU_UNIT(12, 2),
    CFMeasureUnitMetricTon = ICU_UNIT(12, 3),
    CFMeasureUnitMicrogram = ICU_UNIT(12, 4),
    CFMeasureUnitMilligram = ICU_UNIT(12, 5),
    CFMeasureUnitOunce = ICU_UNIT(12, 6),
    CFMeasureUnitOunceTroy = ICU_UNIT(12, 7),
    CFMeasureUnitPound = ICU_UNIT(12, 8),
    CFMeasureUnitStone = ICU_UNIT(12, 9),
    CFMeasureUnitTon = ICU_UNIT(12, 10),
    CFMeasureUnitBase = ICU_UNIT(13, 0),
    CFMeasureUnitPercent = ICU_UNIT(13, 1),
    CFMeasureUnitPermille = ICU_UNIT(13, 2),
    CFMeasureUnitGigawatt = ICU_UNIT(14, 0),
    CFMeasureUnitHorsepower = ICU_UNIT(14, 1),
    CFMeasureUnitKilowatt = ICU_UNIT(14, 2),
    CFMeasureUnitMegawatt = ICU_UNIT(14, 3),
    CFMeasureUnitMilliwatt = ICU_UNIT(14, 4),
    CFMeasureUnitWatt = ICU_UNIT(14, 5),
    CFMeasureUnitHectopascal = ICU_UNIT(15, 0),
    CFMeasureUnitInchHg = ICU_UNIT(15, 1),
    CFMeasureUnitMillibar = ICU_UNIT(15, 2),
    CFMeasureUnitMillimeterOfMercury = ICU_UNIT(15, 3),
    CFMeasureUnitPoundPerSquareInch = ICU_UNIT(15, 4),
    CFMeasureUnitKilometerPerHour = ICU_UNIT(16, 0),
    CFMeasureUnitKnot = ICU_UNIT(16, 1),
    CFMeasureUnitMeterPerSecond = ICU_UNIT(16, 2),
    CFMeasureUnitMilePerHour = ICU_UNIT(16, 3),
    CFMeasureUnitCelsius = ICU_UNIT(17, 0),
    CFMeasureUnitFahrenheit = ICU_UNIT(17, 1),
    CFMeasureUnitGeneric = ICU_UNIT(17, 2),
    CFMeasureUnitKelvin = ICU_UNIT(17, 3),
    CFMeasureUnitAcreFoot = ICU_UNIT(18, 0),
    CFMeasureUnitBushel = ICU_UNIT(18, 1),
    CFMeasureUnitCentiliter = ICU_UNIT(18, 2),
    CFMeasureUnitCubicCentimeter = ICU_UNIT(18, 3),
    CFMeasureUnitCubicFoot = ICU_UNIT(18, 4),
    CFMeasureUnitCubicInch = ICU_UNIT(18, 5),
    CFMeasureUnitCubicKilometer = ICU_UNIT(18, 6),
    CFMeasureUnitCubicMeter = ICU_UNIT(18, 7),
    CFMeasureUnitCubicMile = ICU_UNIT(18, 8),
    CFMeasureUnitCubicYard = ICU_UNIT(18, 9),
    CFMeasureUnitCup = ICU_UNIT(18, 10),
    CFMeasureUnitCupMetric = ICU_UNIT(18, 11),
    CFMeasureUnitDeciliter = ICU_UNIT(18, 12),
    CFMeasureUnitFluidOunce = ICU_UNIT(18, 13),
    CFMeasureUnitGallon = ICU_UNIT(18, 14),
    CFMeasureUnitGallonImperial = ICU_UNIT(18, 15),
    CFMeasureUnitHectoliter = ICU_UNIT(18, 16),
    CFMeasureUnitLiter = ICU_UNIT(18, 17),
    CFMeasureUnitMegaliter = ICU_UNIT(18, 18),
    CFMeasureUnitMilliliter = ICU_UNIT(18, 19),
    CFMeasureUnitPint = ICU_UNIT(18, 20),
    CFMeasureUnitPintMetric = ICU_UNIT(18, 21),
    CFMeasureUnitQuart = ICU_UNIT(18, 22),
    CFMeasureUnitTablespoon = ICU_UNIT(18, 23),
    CFMeasureUnitTeaspoon = ICU_UNIT(18, 24),
};

// MARK: Functions

typedef struct __CFMeasureFormatter * CFMeasureFormatterRef;

CF_EXPORT CFMeasureFormatterRef _Nullable
CFMeasureFormatterCreate(CFAllocatorRef allocator, CFLocaleRef locale, CFMeasureFormatterStyle style);

CF_EXPORT CFLocaleRef CFMeasureFormatterGetLocale(CFMeasureFormatterRef formatter);

CF_EXPORT CFStringRef _Nullable
CFMeasureFormatterCreateNumberUnit(CFMeasureFormatterRef formatter, double value, CFMeasureUnit unit);

CF_EXPORT CFStringRef _Nullable
CFMeasureFormatterCreateUnitName(CFMeasureFormatterRef formatter, CFMeasureUnit unit);

CF_EXTERN_C_END
CF_IMPLICIT_BRIDGING_DISABLED
CF_ASSUME_NONNULL_END


#endif /* !defined(__COREFOUNDATION_CFMEASUREFORMATTER__) */
