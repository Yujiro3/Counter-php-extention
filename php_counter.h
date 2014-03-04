
#ifndef PHP_COUNTER_H
#define PHP_COUNTER_H

extern zend_module_entry counter_module_entry;
#define phpext_counter_ptr &counter_module_entry

#define PHP_COUNTER_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#    define PHP_COUNTER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define PHP_COUNTER_API __attribute__ ((visibility("default")))
#else
#    define PHP_COUNTER_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(counter);
PHP_MSHUTDOWN_FUNCTION(counter);
PHP_RINIT_FUNCTION(counter);
PHP_RSHUTDOWN_FUNCTION(counter);
PHP_MINFO_FUNCTION(counter);

PHP_FUNCTION(confirm_counter_compiled);    /* For testing, remove later. */

ZEND_BEGIN_MODULE_GLOBALS(counter)
    long  reset_time;
    char *save_path;
    long  initial_value;
ZEND_END_MODULE_GLOBALS(counter)

/**
 * Counterクラス::メンバー変数定義
 */
typedef struct {
    char *name;
    int value;
    int time;
    int flags;
    char *path;
} counter_t;

/**
 * Counterクラス::メンバー関数定義
 */
PHP_METHOD(Counter, __construct);
PHP_METHOD(Counter, __destruct);

PHP_METHOD(Counter, getValue);
PHP_METHOD(Counter, bumpValue);
PHP_METHOD(Counter, voidresetValue);
PHP_METHOD(Counter, getMeta);
PHP_METHOD(Counter, getNamed);
PHP_METHOD(Counter, voidsetCounterClass);

#define REGISTER_CLASS_CONST_LONG(pce, const_name, value) zend_declare_class_constant_long(pce, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC);
#define REGISTER_CLASS_CONST_STRING(pce, const_name, value) zend_declare_class_constant_stringl(pce, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC);

#ifdef ZTS
#define COUNTER_G(v) TSRMG(counter_globals_id, zend_counter_globals *, v)
#else
#define COUNTER_G(v) (counter_globals.v)
#endif

#endif    /* PHP_COUNTER_H */

