--TEST--
Output a greeting for the name provided as the constructor argument
--SKIPIF--
<?php if (!extension_loaded("sample")) print "skip"; ?>
--FILE--
<?php

$node = sample_node_append(Null, "1");
var_dump($node);

?>
--EXPECT--
object(Node)#1 (5) {
  ["data"]=>
  string(8) "1"
  ["child"]=>
  NULL
  ["parent"]=>
  NULL
}
