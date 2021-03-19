#ifndef PHP_SAMPLE_H
#define PHP_SAMPLE_H


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include "php.h"

extern zend_module_entry sample_module_entry;
#define phpext_sample_ptr &sample_module_entry


#ifdef ZTS
#include "TSRM.h"
#endif

#if defined(ZTS) && defined(COMPILE_DL_SAMPLE)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif /* PHP_SAMPLE_H */
