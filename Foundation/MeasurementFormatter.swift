// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2016 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//

import CoreFoundation

open class MeasurementFormatter : Formatter, NSSecureCoding {
    public struct UnitOptions : OptionSet {
        public private(set) var rawValue: UInt
        public init(rawValue: UInt) { self.rawValue = rawValue }
        
        
        public static let providedUnit = UnitOptions(rawValue: 1 << 0)
        
        public static let naturalScale = UnitOptions(rawValue: 1 << 1)
        
        public static let temperatureWithoutUnit = UnitOptions(rawValue: 1 << 2)
    }
    
    /*
     This property can be set to ensure that the formatter behaves in a way the developer expects, even if it is not standard according to the preferences of the user's locale. If not specified, unitOptions defaults to localizing according to the preferences of the locale.
     
     Ex:
     
     By default, if unitOptions is set to the empty set, the formatter will do the following:
     - kilocalories may be formatted as "C" instead of "kcal" depending on the locale.
     - kilometersPerHour may be formatted as "miles per hour" for US and UK locales but "kilometers per hour" for other locales.
     
     However, if MeasurementFormatter.UnitOptions.providedUnit is set, the formatter will do the following:
     - kilocalories would be formatted as "kcal" in the language of the locale, even if the locale prefers "C".
     - kilometersPerHour would be formatted as "kilometers per hour" for US and UK locales even though the preference is for "miles per hour."
     
     Note that MeasurementFormatter will handle converting measurement objects to the preferred units in a particular locale.  For instance, if provided a measurement object in kilometers and the set locale is en_US, the formatter will implicitly convert the measurement object to miles and return the formatted string as the equivalent measurement in miles.
     
     */
    typealias CFType = CFMeasureFormatterRef
    private var __cfObject: CFType?
    private var _cfObject: CFType {
        get {
            if let formatter = __cfObject {
                return formatter
            }
            var cfStyle: CFMeasureFormatterStyle
            switch unitStyle {
            case .short:
#if os(macOS) || os(iOS)
                cfStyle = .short
#else
                cfStyle = CFMeasureFormatterStyleShort
#endif
            case .medium:
#if os(macOS) || os(iOS)
                cfStyle = .medium
#else
                cfStyle = CFMeasureFormatterStyleMedium
#endif
            case .long:
#if os(macOS) || os(iOS)
                cfStyle = .long
#else
                cfStyle = CFMeasureFormatterStyleLong
#endif
            }
            let formatter = CFMeasureFormatterCreate(kCFAllocatorSystemDefault, locale._cfObject, cfStyle)!
            __cfObject = formatter
            return formatter
        }
    }

    func reset() {
        __cfObject = nil
    }

    open var unitOptions: UnitOptions = [] {
        didSet {
            reset()
        }
    }
    
    
    /*
     If not specified, unitStyle is set to NSFormattingUnitStyleMedium.
     */
    open var unitStyle: Formatter.UnitStyle = .medium {
        didSet {
            reset()
        }
    }


    /*
     If not specified, locale is set to the user's current locale.
     */
    private var _locale: Locale?
    /*@NSCopying*/ open var locale: Locale!  {
        get {
            guard let locale = _locale else { return .current }
            return locale
        }
        set {
            _locale = newValue
            reset()
        }
    }
    
    
    /*
     If not specified, the number formatter is set up with NumberFormatter.Decimal.style.
     */
    open var numberFormatter: NumberFormatter!

    open func string(from measurement: Measurement<Unit>) -> String {
        return _string(from: measurement)
    }
    
    
    /*
     @param An NSUnit
     @return A formatted string representing the localized form of the unit without a value attached to it.  This method will return [unit symbol] if the provided unit cannot be localized.
     */
    open func string(from unit: Unit) -> String {
        // If unit is associated with a CFMeasureUnit
        // (which can be converted to a ICU::MeasureUnit),
        // ask CFMeasureFormatter/ICU for unit name
        // If unit is nil (unknown to ICU)...
        var result: CFString?
        if let cfUnit = unit.cfUnit {
            result = CFMeasureFormatterCreateUnitName(_cfObject, cfUnit)
        }
        // ...return symbol as fallback
        return result?._swiftObject ?? unit.symbol
    }

    public override init() {
        super.init()
    }

    public required init?(coder aDecoder: NSCoder) {
        // Implementation differs from Darwin
        guard aDecoder.allowsKeyedCoding else {
            fatalError("MeasurementFormatter cannot be decoded by non-keyed archivers")
        }
        super.init(coder: aDecoder)
        self.unitOptions = UnitOptions(rawValue: UInt(aDecoder.decodeInteger(forKey: "NS.unitOptions")))
        self.unitStyle = UnitStyle(rawValue: aDecoder.decodeInteger(forKey: "NS.unitStyle"))!

        // Decode Locale, number formatter
        print("Decoding MeasurementFormatter is not fully supported")
    }
    
    open override func encode(with aCoder: NSCoder) {
        // Implementation differs from Darwin
        NSUnsupported()
    }

    public static var supportsSecureCoding: Bool { return true }

    public func string<UnitType>(from measurement: Measurement<UnitType>) -> String {
        return _string(from: measurement)
    }

    public func _string<UnitType>(from measurement: Measurement<UnitType>) -> String {
        var result: String
        if let cfUnit = measurement.unit.cfUnit {
            result = CFMeasureFormatterCreateNumberUnit(_cfObject, measurement.value, cfUnit)?._swiftObject ?? ""
        } else {
            // "# Symbol" as fallback
            let numberPart = numberFormatter.string(from: NSNumber(value: measurement.value)) ?? ""
            result = "\(numberPart) \(measurement.unit.symbol)"
        }
        return result
    }
}
