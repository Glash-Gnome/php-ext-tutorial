
//#include "php_sample.h"
#define PHP_SAMPLE_EXT_NAME  "sample"
#define PHP_SAMPLE_EXT_VERSION  "1.0"
#define PHP_SAMPLE_EXT_NS  "sample"


#include "php.h"
#include "ext/standard/info.h"

#include "node.h"


extern zend_class_entry *zend_standard_class_def;

static HashTable classes;




/* {{{ sample_functions[]
 *
 * Every user visible function must have an entry in gtk_functions[].
 */
const zend_function_entry sample_functions[] = {
    PHP_SAMPLE_NODE_FE()
    PHP_FE_END	/* Must be the last line in gtk_functions[] */
};
/* }}} */


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(sample)
{
    //int zm_startup_gtk(int type, int module_number)

    /* If you have INI entries, uncomment these lines
    REGISTER_INI_ENTRIES();
    */

    zend_class_entry ce;

    PHP_SAMPLE_NODE_MINIT_FUNCTION(&ce);

    return SUCCESS;
}
/* }}} */

PHP_MSHUTDOWN_FUNCTION(sample)
{
    zend_hash_destroy(&classes);

    PHP_SAMPLE_NODE_MSHUTDOWN_FUNCTION();
}

PHP_RSHUTDOWN_FUNCTION(sample)
{
    PHP_SAMPLE_NODE_RSHUTDOWN_FUNCTION();
}

zend_module_entry sample_module_entry = {
	STANDARD_MODULE_HEADER,
    PHP_SAMPLE_EXT_NAME,
    sample_functions, /* Functions */
	PHP_MINIT(sample), /* MINIT */
    PHP_MSHUTDOWN(sample), /* MSHUTDOWN */
	NULL, /* RINIT */
    PHP_RSHUTDOWN(sample), /* RSHUTDOWN */
	NULL, /* MINFO */
	PHP_SAMPLE_EXT_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SAMPLE
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(sample)
#endif
