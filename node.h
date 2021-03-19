/*
+----------------------------------------------------------------------+
| PHP Version 7                                                        |
+----------------------------------------------------------------------+
| Copyright (c) 1997-2018 The PHP Group                                |
+----------------------------------------------------------------------+
| This source file is subject to version 3.01 of the PHP license,      |
| that is bundled with this package in the file LICENSE, and is        |
| available through the world-wide-web at the following url:           |
| http://www.php.net/license/3_01.txt                                  |
| If you did not receive a copy of the PHP license and are unable to   |
| obtain it through the world-wide-web, please send a note to          |
| license@php.net so we can mail you a copy immediately.               |
+----------------------------------------------------------------------+
| Author: No Name                                                      |
+----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_SAMPLE_NODE_H
#define PHP_SAMPLE_NODE_H


#define PHP_SAMPLE_NODE(ptr)           ((php_sample_node*)(ptr))
#define PHP_SAMPLE_NODE_CLASS          php_sample_node_class_entry
#define PHP_SAMPLE_NODE_OFFSET         (sizeof(php_sample_node) - sizeof(zend_object))

#define ZOBJ_IS_PHP_SAMPLE_NODE(obj)   ((obj)!=NULL && obj->ce == php_sample_node_class_entry)
#define ZOBJ_TO_PHP_SAMPLE_NODE(std)   PHP_SAMPLE_NODE( ((void*)(std))-PHP_SAMPLE_NODE_OFFSET )

#define ZVAL_IS_PHP_SAMPLE_NODE(val)   ((val)!=NULL && Z_TYPE_P(val)==IS_OBJECT && ZOBJ_IS_PHP_SAMPLE_NODE((val)->value.obj))
#define ZVAL_GET_PHP_SAMPLE_NODE(val)  (((val)==NULL || ZVAL_IS_NULL(val)) ? NULL : ZOBJ_TO_PHP_SAMPLE_NODE((val)->value.obj))

#define ZVAL_SET_PHP_SAMPLE_NODE(z, o) do {         \
        if (o==NULL) {                              \
            ZVAL_NULL(z);                           \
        } else {                                    \
            zval *__z = (z);						\
            Z_OBJ_P(__z) = &(o)->std;    			\
            Z_TYPE_INFO_P(__z) = IS_OBJECT_EX;		\
            GC_REFCOUNT(&o->std)++;                 \
        }                                           \
    } while (0)


/*extern HashTable php_sample_node_properties;*/

#define PHP_SAMPLE_NODE_FE() \
    PHP_FE(sample_node_append,  arginfo_sample_node_append) \
    PHP_FE(sample_node_last,  arginfo_sample_node_last) \
    PHP_FE(sample_node_dump,  arginfo_sample_node_dump)

#define PHP_SAMPLE_NODE_MINIT_FUNCTION(parent_ce) \
    php_sample_node_class_init(parent_ce)

#define PHP_SAMPLE_NODE_MSHUTDOWN_FUNCTION() { \
    /*php_printf("    PHP_SAMPLE_NODE_MSHUTDOWN_FUNCTION\n");*/ \
}

#define PHP_SAMPLE_NODE_RSHUTDOWN_FUNCTION() {\
    /*zend_hash_destroy(&php_sample_node_properties);*/ \
}



typedef struct _php_sample_node php_sample_node;

struct _php_sample_node {
    php_sample_node *child;
    php_sample_node *parent;
    zval data;

    /* private:internal */
    char *ptr;

    /* get_properties[_for] */
    HashTable *properties;

    /* zend_object */
    zend_object std;
};

php_sample_node *php_sample_node_append(php_sample_node *node, zval *data);
php_sample_node *php_sample_node_last(php_sample_node *list);

zend_class_entry *php_sample_node_class_init(zend_class_entry *ce);



ZEND_BEGIN_ARG_INFO_EX(arginfo_sample_node_append, 0, 0, 0)
    ZEND_ARG_INFO(0, node)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
PHP_FUNCTION(sample_node_append);

ZEND_BEGIN_ARG_INFO_EX(arginfo_sample_node_last, 0, 0, 0)
    ZEND_ARG_INFO(0, node)
ZEND_END_ARG_INFO()
PHP_FUNCTION(sample_node_last);

ZEND_BEGIN_ARG_INFO_EX(arginfo_sample_node_dump, 0, 0, 0)
    ZEND_ARG_INFO(0, node)
ZEND_END_ARG_INFO()
PHP_FUNCTION(sample_node_dump);


ZEND_BEGIN_ARG_INFO_EX(arginfo_sample_node___construct, 0, 0, 0)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
PHP_METHOD(sample_node, __construct);


#endif	/* PHP_SAMPLE_NODE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
