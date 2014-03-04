
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_counter.h"

ZEND_DECLARE_MODULE_GLOBALS(counter)

/* True global resources - no need for thread safety here */
static int le_counter;

/* クラス構造体 */
zend_class_entry *counter_ce;

/* メンバー関数 */
const zend_function_entry counter_class_methods[] = {
    PHP_ME(Counter, __construct,         NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
    PHP_ME(Counter, __destruct,          NULL, ZEND_ACC_DTOR | ZEND_ACC_PUBLIC)
    PHP_ME(Counter, getValue,            NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Counter, bumpValue,           NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Counter, voidresetValue,      NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Counter, getMeta,             NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Counter, getNamed,            NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Counter, voidsetCounterClass, NULL, ZEND_ACC_PUBLIC)

    PHP_FE_END /* Must be the last line in counter_functions[] */
};


/* モジュールの登録用構造体 */
zend_module_entry counter_module_entry = {
    STANDARD_MODULE_HEADER,
    "Counter",
    NULL,
    PHP_MINIT(counter),
    PHP_MSHUTDOWN(counter),
    PHP_RINIT(counter),        /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(counter),    /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(counter),
    PHP_COUNTER_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_COUNTER
ZEND_GET_MODULE(counter)
#endif

/* PHP_INIの設定 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("counter.reset_time",    COUNTER_RESET_PER_REQUEST, PHP_INI_ALL, OnUpdateLong,   global_value,  zend_counter_globals, counter_globals)
    STD_PHP_INI_ENTRY("counter.save_path",     "",                        PHP_INI_ALL, OnUpdateString, global_value,  zend_counter_globals, counter_globals)
    STD_PHP_INI_ENTRY("counter.initial_value", 0,                         PHP_INI_ALL, OnUpdateLong,   global_string, zend_counter_globals, counter_globals)
PHP_INI_END()

/* php_counter_init_globals */
static void php_counter_init_globals(zend_counter_globals *counter_globals)
{
    counter_globals->reset_time = COUNTER_RESET_PER_REQUEST;
    counter_globals->save_path = NULL;
    counter_globals->initial_value = 0;
}


/* {{{ php_counter_free_storage
 */
static void php_counter_free_storage(counter_t *object TSRMLS_DC)
{
    zend_object_std_dtor(&object->std TSRMLS_CC);
    efree(object->path);
    efree(object->name);
    efree(object);
}
/* }}} */


/* {{{ php_counter_new
 */
zend_object_value php_counter_new(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    counter_t *object;
    zval *tmp;

    object = ecalloc(1, sizeof(*object));

    zend_object_std_init(&object->std, ce TSRMLS_CC);
    object_properties_init(&object->std, ce);
    rebuild_object_properties(&object->std);

    object->name = (char *)emalloc(sizeof(char) * 128);
    object->path = (char *)ecalloc(128, sizeof(char));

    retval.handle = zend_objects_store_put(
        object, 
        (zend_objects_store_dtor_t)zend_objects_destroy_object, 
        (zend_objects_free_object_storage_t)php_counter_free_storage, 
        NULL TSRMLS_CC
    );
    retval.handlers = zend_get_std_object_handlers();

    return retval;
}
/* }}} */


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(counter)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "Counter", counter_class_methods);
    counter_ce = zend_register_internal_class(&ce TSRMLS_CC);
    counter_ce->create_object = php_counter_new;

    counter_table_init(TSRMLS_C);

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(counter)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(counter)
{
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(counter)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(counter)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "counter support", "enabled");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
    */
}
/* }}} */


/* __construct ( string $name [, integer $initial_value [, integer $flags ]] ) */
PHP_METHOD(Counter, __construct)
{
    counter_t *self;
    char *name = NULL;
    unsigned int name_len;
    int initial_value, flags;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl|ll", &name, &name_len, &initial_value, &flags) == FAILURE) {
        RETURN_FALSE;
    }
    self = (counter_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
    
    strcpy(self->name, name);
    self->value = initial_value;
    self->flags = flags;
}

/* __destruct */
PHP_METHOD(Counter, __destruct)
{
}

/* integer getValue ( void ) */
PHP_METHOD(Counter, getValue)
{
    RETURN_LONG(self->value);
}

/* bumpValue ( integer $offset ) */
PHP_METHOD(Counter, bumpValue)
{
}

/* voidresetValue ( void ) */
PHP_METHOD(Counter, voidresetValue)
{
}

/* mixed getMeta ( integer $attribute ) */
PHP_METHOD(Counter, getMeta)
{
}

/* static Counter getNamed ( string $name ) */
PHP_METHOD(Counter, getNamed)
{
    Z_TYPE_P(return_value) = IS_OBJECT;
    object_init_ex(return_value, example_ce);
    Z_SET_REFCOUNT_P(return_value, 1);
    Z_SET_ISREF_P(return_value);
}

/* static voidsetCounterClass ( string $name ) */
PHP_METHOD(Counter, voidsetCounterClass)
{
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
