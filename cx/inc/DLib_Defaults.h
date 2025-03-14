/***************************************************
 *
 * DLib_Defaults.h is the library configuration manager.
 *
 * Copyright 2003-2010 IAR Systems AB.  
 *
 * This configuration header file performs the following tasks:
 *
 * 1. Includes the configuration header file, defined by _DLIB_CONFIG_FILE,
 *    that sets up a particular runtime environment.
 *
 * 2. Includes the product configuration header file, DLib_Product.h, that
 *    specifies default values for the product and makes sure that the
 *    configuration is valid.
 *
 * 3. Sets up default values for all remaining configuration symbols.
 *
 * This configuration header file, the one defined by _DLIB_CONFIG_FILE, and
 * DLib_Product.h configures how the runtime environment should behave. This
 * includes all system headers and the library itself, i.e. all system headers
 * includes this configuration header file, and the library has been built
 * using this configuration header file.
 *
 ***************************************************
 *
 * DO NOT MODIFY THIS FILE!
 *
 ***************************************************/

#ifndef _DLIB_DEFAULTS_H
#define _DLIB_DEFAULTS_H

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

/* Include the main configuration header file. */
#if defined(_DLIB_CONFIG_FILE_HEADER_NAME)
  #include _DLIB_CONFIG_FILE_HEADER_NAME
  /* _DLIB_CONFIG_FILE_STRING is the quoted variant of above */
#elif defined(_DLIB_CONFIG_FILE)
  #include _STRINGIFY(_DLIB_CONFIG_FILE)
#else
  #pragma message("Library configuration file is not specified. Use\
  --dlib_config, please see the compiler reference guide for details.")
#endif

/* Include the product specific header file. */
#ifndef __NO_DLIB_PRODUCT_FILE
  #include <DLib_Product.h>
#endif


/*
 * The remainder of the file sets up defaults for a number of
 * configuration symbols, each corresponds to a feature in the
 * libary.
 *
 * The value of the symbols should either be 1, if the feature should
 * be supported, or 0 if it shouldn't. (Except where otherwise
 * noted.)
 */


/*
 * Small or Large target
 *
 * This define determines whether the target is large or small. It must be 
 * setup in the DLib_Product header or in the compiler itself.
 *
 * For a small target some functionality in the library will not deliver 
 * the best available results. For instance the _accurate variants will not use
 * the extra precision packet for large arguments.
 * 
 */
#ifndef _DLIB_SMALL_TARGET
  #error "Must set _DLIB_SMALL_TARGET to 0 or 1"
#endif

#define _DLIB_SMALL_TARGET_INTERNAL _DLIB_SMALL_TARGET


/*
 * File handling
 *
 * Determines whether FILE descriptors and related functions exists or not.
 * When this feature is selected, i.e. set to 1, then FILE descriptors and
 * related functions (e.g. fprintf, fopen) exist. All files, even stdin,
 * stdout, and stderr will then be handled with a file system mechanism that
 * buffers files before accessing the lowlevel I/O interface (__open, __read,
 * __write, etc).
 *
 * If not selected, i.e. set to 0, then FILE descriptors and related functions
 * (e.g. fprintf, fopen) does not exist. All functions that normally uses
 * stderr will use stdout instead. Functions that uses stdout and stdin (like
 * printf and scanf) will access the lowlevel I/O interface directly (__open,
 * __read, __write, etc), i.e. there will not be any buffering.
 *
 * The default is not to have support for FILE descriptors.
 */

#ifndef _DLIB_FILE_DESCRIPTOR
#define _DLIB_FILE_DESCRIPTOR 0
#endif

/*
 * Use static buffers for stdout
 *
 * This setting controls whether the stream stdout uses a static 80 bytes
 * buffer or uses a one byte buffer allocated in the file descriptor. This
 * setting is only applicable if the FILE descriptors are enabled above.
 *
 * Default is to use a static 80 byte buffer.
 */

#ifndef _DLIB_STDOUT_USES_STATIC_BUFFER
#define _DLIB_STDOUT_USES_STATIC_BUFFER 1
#endif

/*
 * Support of locale interface
 *
 * "Locale" is the system in C that support language- and
 * contry-specific settings for a number of areas, including currency
 * symbols, date and time, and multibyte encodings.
 *
 * This setting determines whether the locale interface exist or not.
 * When this feature is selected, i.e. set to 1, the locale interface exist
 * (setlocale, etc). A number of preselected locales can be activated during
 * runtime. The preselected locales and encodings is choosen by defining any
 * number of _LOCALE_USE_xxx and _ENCODING_USE_xxx symbols. The application
 * will start with the "C" locale choosen. (Single byte encoding is always
 * supported in this mode.)
 *
 *
 * If not selected, i.e. set to 0, the locale interface (setlocale, etc) does
 * not exist. One preselected locale and one preselected encoding is then used
 * directly. That locale can not be changed during runtime. The preselected
 * locale and encoding is choosen by defining at most one of _LOCALE_USE_xxx
 * and at most one of _ENCODING_USE_xxx. The default is to use the "C" locale
 * and the single byte encoding, respectively.
 *
 * The default is not to have support for the locale interface with the "C"
 * locale and the single byte encoding.
 *
 * Supported locales
 * -----------------
 * _LOCALE_USE_C                  C standard locale (the default)
 * _LOCALE_USE_POSIX ISO-8859-1   Posix locale
 * _LOCALE_USE_CS_CZ ISO-8859-2   Czech language locale for Czech Republic
 * _LOCALE_USE_DA_DK ISO-8859-1   Danish language locale for Denmark
 * _LOCALE_USE_DA_EU ISO-8859-15  Danish language locale for Europe
 * _LOCALE_USE_DE_AT ISO-8859-1   German language locale for Austria
 * _LOCALE_USE_DE_BE ISO-8859-1   German language locale for Belgium
 * _LOCALE_USE_DE_CH ISO-8859-1   German language locale for Switzerland
 * _LOCALE_USE_DE_DE ISO-8859-1   German language locale for Germany
 * _LOCALE_USE_DE_EU ISO-8859-15  German language locale for Europe
 * _LOCALE_USE_DE_LU ISO-8859-1   German language locale for Luxemburg
 * _LOCALE_USE_EL_EU ISO-8859-7x  Greek language locale for Europe
 *                                (Euro symbol added)
 * _LOCALE_USE_EL_GR ISO-8859-7   Greek language locale for Greece
 * _LOCALE_USE_EN_AU ISO-8859-1   English language locale for Australia
 * _LOCALE_USE_EN_CA ISO-8859-1   English language locale for Canada
 * _LOCALE_USE_EN_DK ISO_8859-1   English language locale for Denmark
 * _LOCALE_USE_EN_EU ISO-8859-15  English language locale for Europe
 * _LOCALE_USE_EN_GB ISO-8859-1   English language locale for United Kingdom
 * _LOCALE_USE_EN_IE ISO-8859-1   English language locale for Ireland
 * _LOCALE_USE_EN_NZ ISO-8859-1   English language locale for New Zealand
 * _LOCALE_USE_EN_US ISO-8859-1   English language locale for USA
 * _LOCALE_USE_ES_AR ISO-8859-1   Spanish language locale for Argentina
 * _LOCALE_USE_ES_BO ISO-8859-1   Spanish language locale for Bolivia
 * _LOCALE_USE_ES_CL ISO-8859-1   Spanish language locale for Chile
 * _LOCALE_USE_ES_CO ISO-8859-1   Spanish language locale for Colombia
 * _LOCALE_USE_ES_DO ISO-8859-1   Spanish language locale for Dominican Republic
 * _LOCALE_USE_ES_EC ISO-8859-1   Spanish language locale for Equador
 * _LOCALE_USE_ES_ES ISO-8859-1   Spanish language locale for Spain
 * _LOCALE_USE_ES_EU ISO-8859-15  Spanish language locale for Europe
 * _LOCALE_USE_ES_GT ISO-8859-1   Spanish language locale for Guatemala
 * _LOCALE_USE_ES_HN ISO-8859-1   Spanish language locale for Honduras
 * _LOCALE_USE_ES_MX ISO-8859-1   Spanish language locale for Mexico
 * _LOCALE_USE_ES_PA ISO-8859-1   Spanish language locale for Panama
 * _LOCALE_USE_ES_PE ISO-8859-1   Spanish language locale for Peru
 * _LOCALE_USE_ES_PY ISO-8859-1   Spanish language locale for Paraguay
 * _LOCALE_USE_ES_SV ISO-8859-1   Spanish language locale for Salvador
 * _LOCALE_USE_ES_US ISO-8859-1   Spanish language locale for USA
 * _LOCALE_USE_ES_UY ISO-8859-1   Spanish language locale for Uruguay
 * _LOCALE_USE_ES_VE ISO-8859-1   Spanish language locale for Venezuela
 * _LOCALE_USE_ET_EE ISO-8859-1   Estonian language for Estonia
 * _LOCALE_USE_EU_ES ISO-8859-1   Basque language locale for Spain
 * _LOCALE_USE_FI_EU ISO-8859-15  Finnish language locale for Europe
 * _LOCALE_USE_FI_FI ISO-8859-1   Finnish language locale for Finland
 * _LOCALE_USE_FO_FO ISO-8859-1   Faroese language locale for Faroe Islands
 * _LOCALE_USE_FR_BE ISO-8859-1   French language locale for Belgium
 * _LOCALE_USE_FR_CA ISO-8859-1   French language locale for Canada
 * _LOCALE_USE_FR_CH ISO-8859-1   French language locale for Switzerland
 * _LOCALE_USE_FR_EU ISO-8859-15  French language locale for Europe
 * _LOCALE_USE_FR_FR ISO-8859-1   French language locale for France
 * _LOCALE_USE_FR_LU ISO-8859-1   French language locale for Luxemburg
 * _LOCALE_USE_GA_EU ISO-8859-15  Irish language locale for Europe
 * _LOCALE_USE_GA_IE ISO-8859-1   Irish language locale for Ireland
 * _LOCALE_USE_GL_ES ISO-8859-1   Galician language locale for Spain
 * _LOCALE_USE_HR_HR ISO-8859-2   Croatian language locale for Croatia
 * _LOCALE_USE_HU_HU ISO-8859-2   Hungarian language locale for Hungary
 * _LOCALE_USE_ID_ID ISO-8859-1   Indonesian language locale for Indonesia
 * _LOCALE_USE_IS_EU ISO-8859-15  Icelandic language locale for Europe
 * _LOCALE_USE_IS_IS ISO-8859-1   Icelandic language locale for Iceland
 * _LOCALE_USE_IT_EU ISO-8859-15  Italian language locale for Europe
 * _LOCALE_USE_IT_IT ISO-8859-1   Italian language locale for Italy
 * _LOCALE_USE_IW_IL ISO-8859-8   Hebrew language locale for Israel
 * _LOCALE_USE_KL_GL ISO-8859-1   Greenlandic language locale for Greenland
 * _LOCALE_USE_LT_LT   BALTIC     Lithuanian languagelocale for Lithuania
 * _LOCALE_USE_LV_LV   BALTIC     Latvian languagelocale for Latvia
 * _LOCALE_USE_NL_BE ISO-8859-1   Dutch language locale for Belgium
 * _LOCALE_USE_NL_EU ISO-8859-15  Dutch language locale for Europe
 * _LOCALE_USE_NL_NL ISO-8859-9   Dutch language locale for Netherlands
 * _LOCALE_USE_NO_EU ISO-8859-15  Norwegian language locale for Europe
 * _LOCALE_USE_NO_NO ISO-8859-1   Norwegian language locale for Norway
 * _LOCALE_USE_PL_PL ISO-8859-2   Polish language locale for Poland
 * _LOCALE_USE_PT_BR ISO-8859-1   Portugese language locale for Brazil
 * _LOCALE_USE_PT_EU ISO-8859-15  Portugese language locale for Europe
 * _LOCALE_USE_PT_PT ISO-8859-1   Portugese language locale for Portugal
 * _LOCALE_USE_RO_RO ISO-8859-2   Romanian language locale for Romania
 * _LOCALE_USE_RU_RU ISO-8859-5   Russian language locale for Russia
 * _LOCALE_USE_SL_SI ISO-8859-2   Slovenian language locale for Slovenia
 * _LOCALE_USE_SV_EU ISO-8859-15  Swedish language locale for Europe
 * _LOCALE_USE_SV_FI ISO-8859-1   Swedish language locale for Finland
 * _LOCALE_USE_SV_SE ISO-8859-1   Swedish language locale for Sweden
 * _LOCALE_USE_TR_TR ISO-8859-9   Turkish language locale for Turkey
 *
 *  Supported encodings
 *  -------------------
 * n/a                            Single byte (used if no other is defined).
 * _ENCODING_USE_UTF8             UTF8 encoding.
 */

#ifndef _DLIB_FULL_LOCALE_SUPPORT
#define _DLIB_FULL_LOCALE_SUPPORT 0
#endif

#ifndef _DLIB_CONFIG_AGNOSTIC
/* We need to have the "C" locale if we have full locale support. */
#if _DLIB_FULL_LOCALE_SUPPORT && !defined(_LOCALE_USE_C)
#define _LOCALE_USE_C
#endif
#endif


/*
 * Support of multibytes in printf- and scanf-like functions
 *
 * This is the default value for _DLIB_PRINTF_MULTIBYTE and
 * _DLIB_SCANF_MULTIBYTE. See them for a description.
 *
 * Default is to not have support for multibytes in printf- and scanf-like
 * functions.
 */

#ifndef _DLIB_FORMATTED_MULTIBYTE
#define _DLIB_FORMATTED_MULTIBYTE 0
#endif


/*
 * Throw handling in the EC++ library
 *
 * This setting determines what happens when the EC++ part of the library
 * fails (where a normal C++ library 'throws').
 *
 * The following alternatives exists (setting of the symbol):
 * 0                - The application does nothing, i.e. continues with the
 *                    next statement.
 * 1                - The application terminates by calling the 'abort'
 *                    function directly.
 * <anything else>  - An object of class "exception" is created.  This
 *                    object contains a string describing the problem.
 *                    This string is later emitted on "stderr" before
 *                    the application terminates by calling the 'abort'
 *                    function directly.
 *
 * Default is to do nothing.
 */

#ifndef _DLIB_THROW_HANDLING
#define _DLIB_THROW_HANDLING 0
#endif


/*
 * Hexadecimal floating-point numbers in strtod
 *
 * If selected, i.e. set to 1, strtod supports C99 hexadecimal floating-point
 * numbers. This also enables hexadecimal floating-points in internal functions
 * used for converting strings and wide strings to float, double, and long
 * double.
 *
 * If not selected, i.e. set to 0, C99 hexadecimal floating-point numbers
 * aren't supported.
 *
 * Default is not to support hexadecimal floating-point numbers.
 */

#ifndef _DLIB_STRTOD_HEX_FLOAT
#define _DLIB_STRTOD_HEX_FLOAT 0
#endif


/*
 * Printf configuration symbols.
 *
 * All the configuration symbols described further on controls the behaviour
 * of printf, sprintf, and the other printf variants.
 *
 * The library proves four formatters for printf: 'tiny', 'small',
 * 'large', and 'default'.  The setup in this file controls all except
 * 'tiny'.  Note that both small' and 'large' explicitly removes
 * some features.
 */

/*
 * Handle multibytes in printf
 *
 * This setting controls whether multibytes and wchar_ts are supported in
 * printf. Set to 1 to support them, otherwise set to 0.
 *
 * See _DLIB_FORMATTED_MULTIBYTE for the default setting.
 */

#ifndef _DLIB_PRINTF_MULTIBYTE
#define _DLIB_PRINTF_MULTIBYTE _DLIB_FORMATTED_MULTIBYTE
#endif

/*
 * Long long formatting in printf
 *
 * This setting controls long long support (%lld) in printf. Set to 1 to
 * support it, otherwise set to 0.

 * Note, if long long should not be supported and 'intmax_t' is larger than
 * an ordinary 'long', then %jd and %jn will not be supported.
 *
 * Default is to support long long formatting.
 */

#ifndef _DLIB_PRINTF_LONG_LONG
  #ifdef __LONG_LONG_SIZE__
    #define _DLIB_PRINTF_LONG_LONG 1
  #else
    #define _DLIB_PRINTF_LONG_LONG 0
  #endif
#endif

#if _DLIB_PRINTF_LONG_LONG && !defined(__LONG_LONG_SIZE__)
#error "Long long support turned on for printf, the compiler doesn't support long long though"
#endif


/*
 * Floating-point formatting in printf
 *
 * This setting controls whether printf supports floating-point formatting.
 * Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support floating-point formatting.
 */

#ifndef _DLIB_PRINTF_SPECIFIER_FLOAT
#define _DLIB_PRINTF_SPECIFIER_FLOAT 1
#endif

/*
 * Hexadecimal floating-point formatting in printf
 *
 * This setting controls whether the %a format, i.e. the output of
 * floating-point numbers in the C99 hexadecimal format. Set to 1 to support
 * it, otherwise set to 0.
 *
 * Default is to support %a in printf.
 */

#ifndef _DLIB_PRINTF_SPECIFIER_A
#define _DLIB_PRINTF_SPECIFIER_A 1
#endif

/*
 * Output count formatting in printf
 *
 * This setting controls whether the output count specifier (%n) is supported
 * or not in printf. Set to 1 to support it, otherwise set to 0.
 *
 * Default is to support %n in printf.
 */

#ifndef _DLIB_PRINTF_SPECIFIER_N
#define _DLIB_PRINTF_SPECIFIER_N 1
#endif

/*
 * Support of qualifiers in printf
 *
 * This setting controls whether qualifiers that enlarges the input value
 * [hlLjtz] is supported in printf or not. Set to 1 to support them, otherwise
 * set to 0. See also _DLIB_PRINTF_INT_TYPE_IS_INT and
 * _DLIB_PRINTF_INT_TYPE_IS_LONG.
 *
 * Default is to support [hlLjtz] qualifiers in printf.
 */

#ifndef _DLIB_PRINTF_QUALIFIERS
#define _DLIB_PRINTF_QUALIFIERS 1
#endif

/*
 * Support of flags in printf
 *
 * This setting controls whether flags (-+ #0) is supported in printf or not.
 * Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support flags in printf.
 */

#ifndef _DLIB_PRINTF_FLAGS
#define _DLIB_PRINTF_FLAGS 1
#endif

/*
 * Support widths and precisions in printf
 *
 * This setting controls whether widths and precisions are supported in printf.
 * Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support widths and precisions in printf.
 */

#ifndef _DLIB_PRINTF_WIDTH_AND_PRECISION
#define _DLIB_PRINTF_WIDTH_AND_PRECISION 1
#endif

/*
 * Support of unsigned integer formatting in printf
 *
 * This setting controls whether unsigned integer formatting is supported in
 * printf. Set to 1 to support it, otherwise set to 0.
 *
 * Default is to support unsigned integer formatting in printf.
 */

#ifndef _DLIB_PRINTF_SPECIFIER_UNSIGNED
#define _DLIB_PRINTF_SPECIFIER_UNSIGNED 1
#endif

/*
 * Support of signed integer formatting in printf
 *
 * This setting controls whether signed integer formatting is supported in
 * printf. Set to 1 to support it, otherwise set to 0.
 *
 * Default is to support signed integer formatting in printf.
 */

#ifndef _DLIB_PRINTF_SPECIFIER_SIGNED
#define _DLIB_PRINTF_SPECIFIER_SIGNED 1
#endif

/*
 * Support of formatting anything larger than int in printf
 *
 * This setting controls if 'int' should be used internally in printf, rather
 * than the largest existing integer type. If 'int' is used, any integer or
 * pointer type formatting use 'int' as internal type even though the
 * formatted type is larger. Set to 1 to use 'int' as internal type, otherwise
 * set to 0.
 *
 * See also next configuration.
 *
 * Default is to internally use largest existing internally type.
 */

#ifndef _DLIB_PRINTF_INT_TYPE_IS_INT
#define _DLIB_PRINTF_INT_TYPE_IS_INT 0
#endif

/*
 * Support of formatting anything larger than long in printf
 *
 * This setting controls if 'long' should be used internally in printf, rather
 * than the largest existing integer type. If 'long' is used, any integer or
 * pointer type formatting use 'long' as internal type even though the
 * formatted type is larger. Set to 1 to use 'long' as internal type,
 * otherwise set to 0.
 *
 * See also previous configuration.
 *
 * Default is to internally use largest existing internally type.
 */

#ifndef _DLIB_PRINTF_INT_TYPE_IS_LONG
#define _DLIB_PRINTF_INT_TYPE_IS_LONG 0
#endif

#if _DLIB_PRINTF_INT_TYPE_IS_INT && _DLIB_PRINTF_INT_TYPE_IS_LONG
#error "At most one of _DLIB_PRINTF_INT_TYPE_IS_INT and _DLIB_PRINTF_INT_TYPE_IS_LONG can be defined."
#endif

/*
 * Emit a char a time in printf
 *
 * This setting controls internal output handling. If selected, i.e. set to 1,
 * then printf emits one character at a time, which requires less code but
 * can be slightly slower for some types of output.
 *
 * If not selected, i.e. set to 0, then printf buffers some outputs.
 *
 * Note that it is recommended to either use full file support (see
 * _DLIB_FILE_DESCRIPTOR) or -- for debug output -- use the linker
 * option "-e__write_buffered=__write" to enable buffered I/O rather
 * than deselecting this feature.
 */

#ifndef _DLIB_PRINTF_CHAR_BY_CHAR
#define _DLIB_PRINTF_CHAR_BY_CHAR 1
#endif


/*
 * Scanf configuration symbols.
 *
 * All the configuration symbols described here controls the
 * behaviour of scanf, sscanf, and the other scanf variants.
 *
 * The library proves three formatters for scanf: 'small', 'large',
 * and 'default'.  The setup in this file controls all, however both
 * 'small' and 'large' explicitly removes some features.
 */

/*
 * Handle multibytes in scanf
 *
 * This setting controls whether multibytes and wchar_t:s are supported in
 * scanf. Set to 1 to support them, otherwise set to 0.
 *
 * See _DLIB_FORMATTED_MULTIBYTE for the default.
 */

#ifndef _DLIB_SCANF_MULTIBYTE
#define _DLIB_SCANF_MULTIBYTE _DLIB_FORMATTED_MULTIBYTE
#endif

/*
 * Long long formatting in scanf
 *
 * This setting controls whether scanf supports long long support (%lld). It
 * also controls, if 'intmax_t' is larger than an ordinary 'long', i.e. how
 * the %jd and %jn specifiers behaves. Set to 1 to support them, otherwise set
 * to 0.
 *
 * Default is to support long long formatting in scanf.
 */

#ifndef _DLIB_SCANF_LONG_LONG
  #ifdef __LONG_LONG_SIZE__
    #define _DLIB_SCANF_LONG_LONG 1
  #else
    #define _DLIB_SCANF_LONG_LONG 0
  #endif
#endif

#if _DLIB_SCANF_LONG_LONG && !defined(__LONG_LONG_SIZE__)
#error "Long long support turned on for scanf, the compiler doesn't support long long though"
#endif

/*
 * Support widths in scanf
 *
 * This controls whether scanf supports widths. Set to 1 to support them,
 * otherwise set to 0.
 *
 * Default is to support widths in scanf.
 */

#ifndef _DLIB_SCANF_WIDTH
#define _DLIB_SCANF_WIDTH 1
#endif

/*
 * Support qualifiers [hjltzL] in scanf
 *
 * This setting controls whether scanf supports qualifiers [hjltzL] or not. Set
 * to 1 to support them, otherwise set to 0.
 *
 * Default is to support qualifiers in scanf.
 */

#ifndef _DLIB_SCANF_QUALIFIERS
#define _DLIB_SCANF_QUALIFIERS 1
#endif

/*
 * Support floating-point formatting in scanf
 *
 * This setting controls whether scanf supports floating-point formatting. Set
 * to 1 to support them, otherwise set to 0.
 *
 * Default is to support floating-point formatting in scanf.
 */

#ifndef _DLIB_SCANF_SPECIFIER_FLOAT
#define _DLIB_SCANF_SPECIFIER_FLOAT 1
#endif

/*
 * Support output count formatting (%n)
 *
 * This setting controls whether scanf supports output count formatting (%n).
 * Set to 1 to support it, otherwise set to 0.
 *
 * Default is to support output count formatting in scanf.
 */

#ifndef _DLIB_SCANF_SPECIFIER_N
#define _DLIB_SCANF_SPECIFIER_N 1
#endif

/*
 * Support scansets ([]) in scanf
 *
 * This setting controls whether scanf supports scansets ([]) or not. Set to 1
 * to support them, otherwise set to 0.
 *
 * Default is to support scansets in scanf.
 */

#ifndef _DLIB_SCANF_SPECIFIER_SCANSET
#define _DLIB_SCANF_SPECIFIER_SCANSET 1
#endif

/*
 * Support signed integer formatting in scanf
 *
 * This setting controls whether scanf supports signed integer formatting or
 * not. Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support signed integer formatting in scanf.
 */

#ifndef _DLIB_SCANF_SPECIFIER_SIGNED
#define _DLIB_SCANF_SPECIFIER_SIGNED 1
#endif

/*
 * Support unsigned integer formatting in scanf
 *
 * This setting controls whether scanf supports unsigned integer formatting or
 * not. Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support unsigned integer formatting in scanf.
 */

#ifndef _DLIB_SCANF_SPECIFIER_UNSIGNED
#define _DLIB_SCANF_SPECIFIER_UNSIGNED 1
#endif

/*
 * Support assignment suppressing [*] in scanf
 *
 * This setting controls whether scanf supports assignment suppressing [*] or
 * not. Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support assignment suppressing in scanf.
 */

#ifndef _DLIB_SCANF_ASSIGNMENT_SUPPRESSING
#define _DLIB_SCANF_ASSIGNMENT_SUPPRESSING 1
#endif

/*
 * Handle multibytes in asctime and strftime.
 *
 * This setting controls whether multibytes and wchar_ts are
 * supported.Set to 1 to support them, otherwise set to 0.
 *
 * See _DLIB_FORMATTED_MULTIBYTE for the default setting.
 */

#ifndef _DLIB_STRFTIME_MULTIBYTE
#define _DLIB_STRFTIME_MULTIBYTE _DLIB_FORMATTED_MULTIBYTE
#endif

/*
 * True if "qsort" should be implemented using bubble sort.
 *
 * Bubble sort is less efficient than quick sort but requires less RAM
 * and ROM resources.
 */

#ifndef _DLIB_QSORT_USE_BUBBLE_SORT
#define _DLIB_QSORT_USE_BUBBLE_SORT 0
#endif

/*
 * Set Buffert size used in qsort
 */

#ifndef _DLIB_QSORT_BUF_SIZE
#define _DLIB_QSORT_BUF_SIZE 256
#endif

/*
 * The default "rand" function uses an array of 32 long:s of memory to
 * store the current state.
 *
 * The simple "rand" function uses only a single long. However, the
 * quality of the generated psuedo-random numbers are not as good as
 * the default implementation.
 */

#ifndef _DLIB_RAND_USE_SIMPLE
#define _DLIB_RAND_USE_SIMPLE 0
#endif

/*
 * Wide character and multi byte character support in library.
 */

#ifndef _DLIB_WIDE_CHARACTERS
#define _DLIB_WIDE_CHARACTERS 1
#endif

/*
 * Set attributes on the function used by the C-SPY debug interface to set a
 * breakpoint in.
 */

#ifndef _DLIB_DEBUG_BREAK_FUNCTION_ATTRIBUTE
#define _DLIB_DEBUG_BREAK_FUNCTION_ATTRIBUTE
#endif

/*
 * Support threading in the library
 *
 * 0    No thread support
 * 1    Thread support with a, b, and d.
 * 2    Thread support with a, b, and e.
 * 3    Thread support with all thread-local storage in a dynamically allocated
 *        memory area and a, and b.
 *      a. Lock on heap accesses
 *      b. Optional lock on file accesses (see _DLIB_FILE_OP_LOCKS below)
 *      d. Use an external thread-local storage interface for all the
 *         libraries static and global variables.
 *      e. Static and global variables aren't safe for access from several
 *         threads.
 *
 * Note that if locks are used the following symbols must be defined:
 *
 *   _DLIB_THREAD_LOCK_ONCE_TYPE
 *   _DLIB_THREAD_LOCK_ONCE_MACRO(control_variable, init_function)
 *   _DLIB_THREAD_LOCK_ONCE_TYPE_INIT
 *
 * They will be used to initialize the needed locks only once. TYPE is the
 * type for the static control variable, MACRO is the expression that will be
 * evaluated at each usage of a lock, and INIT is the initializer for the
 * control variable.
 *
 * Note that if thread model 3 is used the symbol _DLIB_TLS_POINTER must be
 * defined. It is a thread local pointer to a dynamic memory area that
 * contains all used TLS variables for the library. Optionally the following
 * symbols can be defined as well (default is to use the default const data
 * and data memories):
 *
 *   _DLIB_TLS_INITIALIZER_MEMORY The memory to place the initializers for the
 *                                TLS memory area
 *   _DLIB_TLS_MEMORY             The memory to use for the TLS memory area. A
 *                                pointer to this memory must be castable to a
 *                                default pointer and back.
 *   _DLIB_TLS_REQUIRE_INIT       Set to 1 to require __cstart_init_tls
 *                                when needed to initialize the TLS data
 *                                segment for the main thread.
 *   _DLIB_TLS_SEGMENT_DATA       The name of the TLS RAM data segment
 *   _DLIB_TLS_SEGMENT_INIT       The name of the used to initialize the
 *                                TLS data segment.
 *
 * See DLib_Threads.h for a description of what interfaces needs to be
 * defined for thread support.
 */

#ifndef _DLIB_THREAD_SUPPORT
#define _DLIB_THREAD_SUPPORT 0
#endif

/*
 * Used by products where one runtime library can be used by applications
 * with different data models, in order to reduce the total number of
 * libraries required. Typically, this is used when the pointer types does
 * not change over the data models used, but the placement of data variables
 * or/and constant variables do.
 *
 * If defined, this symbol is typically defined to the memory attribute that
 * is used by the runtime library. The actual define must use a
 * _Pragma("type_attribute = xxx") construct. In the header files, it is used
 * on all statically linked data objects seen by the application.
 */
#ifndef _DLIB_DATA_ATTR
#define _DLIB_DATA_ATTR
#endif

#ifndef _DLIB_CONST_ATTR
  #ifdef _DLIB_DATA_ATTR
    #define _DLIB_CONST_ATTR _DLIB_DATA_ATTR
  #else
    #define _DLIB_CONST_ATTR
  #endif
#endif


/*
 * Turn on support for the Target-specific ABI. The ABI is based on the
 * ARM AEABI. A target, except ARM, may deviate from it.
 */

#ifndef _DLIB_SUPPORT_FOR_AEABI
  #ifdef __EDG_IA64_ABI
    #define _DLIB_SUPPORT_FOR_AEABI 1
  #else
    #define _DLIB_SUPPORT_FOR_AEABI 0
  #endif
#endif

#if _DLIB_SUPPORT_FOR_AEABI == 1 
  /* Possible AEABI deviations */
  #ifndef _DLIB_MBSTATET_IS_UNSIGNED_LONG
    #if __INT_SIZE__ != 4
      #define _DLIB_MBSTATET_USES_UNSIGNED_LONG 1
    #else
      #define _DLIB_MBSTATET_USES_UNSIGNED_LONG 0
    #endif
  #endif

  #ifndef _DLIB_TIME_USES_LONG
    #if __INT_SIZE__ != 4
      #define _DLIB_TIME_USES_LONG 1
    #else
      #define _DLIB_TIME_USES_LONG 0
    #endif
  #endif
  /*
   * The "difunc" table contains information about C++ objects that
   * should be dynamically initialized, where each entry in the table
   * represents an initialization function that should be called. When
   * the symbol _DLIB_AEABI_DIFUNC_CONTAINS_OFFSETS is true, each
   * entry in the table is encoded as an offset from the entry
   * location. When false, the entries contain the actual addresses to
   * call.
   */
  #ifndef _DLIB_AEABI_DIFUNC_CONTAINS_OFFSETS
    #define _DLIB_AEABI_DIFUNC_CONTAINS_OFFSETS 1
  #endif
#endif /* _DLIB_SUPPORT_FOR_AEABI == 1  */


/*
 * Turn on usage of a pragma to tell the linker the number of elements used
 * in a setjmp jmp_buf.
 */

#ifndef _DLIB_USE_RTMODEL_JMP_BUF_NUM_ELEMENTS
#define _DLIB_USE_RTMODEL_JMP_BUF_NUM_ELEMENTS 0
#endif

/*
 * If true, the product supplies a "DLib_Product_string.h" file that
 * is included from "string.h".
 */

#ifndef _DLIB_PRODUCT_STRING
#define _DLIB_PRODUCT_STRING 0
#endif

/*
 * Determine whether the math fma routines are fast or not.
 */
#ifndef _DLIB_FAST_FMA
#define _DLIB_FAST_FMA 0
#endif

/*
 * Rtti support.
 */

#ifndef _DLIB_SUPPORT_RTTI
  #ifdef __cplusplus
    #ifdef __embedded_cplusplus
      #define _DLIB_SUPPORT_RTTI 0
    #else
      #define _DLIB_SUPPORT_RTTI 1
    #endif
  #else
    #define _DLIB_SUPPORT_RTTI 0
  #endif
#endif

/*
 * Use the "pointers to short" or "pointers to long" implementation of 
 * the basic floating point routines (like Dnorm, Dtest, Dscale, and Dunscale).
 */
#ifndef _DLIB_SMALL_FP_HELPERS
  #define _DLIB_SMALL_FP_HELPERS _DLIB_SMALL_TARGET_INTERNAL
#endif

/*
 * Use 64-bit long long as intermediary type in Dtest, and fabs.
 * Default is to do this if long long is 64-bits.
 */
#ifndef _DLIB_USE_LONGLONG_IN_DOUBLE_FP_HELPERS
  #define _DLIB_USE_LONGLONG_IN_DOUBLE_FP_HELPERS (__LONG_LONG_SIZE__ == 8)
#endif

/*
 * Favor speed versus some size enlargements in floating point functions.
 */
#ifndef _DLIB_SPEEDY_FLOATS
  #define _DLIB_SPEEDY_FLOATS 0
#endif

/*
 * Include dlmalloc as an alternative heap manager in product.
 *
 * Typically, an application will use a "malloc" heap manager that is
 * relatively small but not that efficient. An application can
 * optionally use the "dlmalloc" package, which provides a more
 * effective "malloc" heap manager, if it is included in the product
 * and supported by the settings.
 *
 * See the product documentation on how to use it, and whether or not
 * it is included in the product.
 */
#ifndef _DLIB_INCLUDE_DLMALLOC_ALTERNATIVE
  /* size_t/ptrdiff_t must be a 4 bytes unsigned integer. */
  #if _DLIB_SMALL_TARGET_INTERNAL || __SIZE_T_MAX__ < 0xfffffffful
    #define _DLIB_INCLUDE_DLMALLOC_ALTERNATIVE 0
  #else
    #define _DLIB_INCLUDE_DLMALLOC_ALTERNATIVE 1
  #endif
#endif

#ifndef __SOFTFP
  #define __SOFTFP
#endif

/*
 * Allow the 64-bit time_t interface?
 *
 * Default is yes if long long is 64 bits.
 */
#ifndef _DLIB_TIME_ALLOW_64
  #pragma language = save 
  #pragma language = extended
  #if defined(_LONGLONG) && __SIGNED_LONG_LONG_MAX__ > __SIGNED_LONG_MAX__
    #define _DLIB_TIME_ALLOW_64 1
  #else
    #define _DLIB_TIME_ALLOW_64 0
  #endif
  #pragma language = restore
#endif

#if _DLIB_TIME_ALLOW_64 && !defined(_LONGLONG)
  #error("_DLIB_TIME_ALLOW_64 requires _LONGLONG")
#endif

/*
 * Is time_t 64 or 32 bits?
 *
 * Default is 32 bits.
 */
#ifndef _DLIB_TIME_USES_64_DEFAULT
  #define _DLIB_TIME_USES_64_DEFAULT 0
#endif

/*
 * Do we include math functions that demands lots of constant bytes?
 * (like erf, erfc, expm1, fma, lgamma, tgamma, and *_accurate)
 *
 */
#ifndef _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
  #define _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH 1
#endif

/*
 * Set this to __weak, if supported.
 *
 */
#ifndef __WEAK
#if defined(__HAS_WEAK__) && __HAS_WEAK__
    #define __WEAK _Pragma("object_attribute = __weak")
  #else
    #define __WEAK
  #endif
#endif


/*
 * Deleted options
 *
 */
#ifdef _DLIB_FLOAT_ENVIRONMENT
  #error "Option has been removed"
#endif




#endif /* _DLIB_DEFAULTS_H */
