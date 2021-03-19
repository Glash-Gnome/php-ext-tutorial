<?php

/*
//$root = new Sample('Universe');
//$last = $root->add('Hello')->add('World')->add('!!!');
//$root->add('Hello')->add('World');

//$root = $root->add('Hello');
//$root = $root->add('World');
//$root = $root->add('!');

var_dump($root);
sample_dump($root);
*/

/*
$node = sample_node_append(Null, "1");
sample_node_append($node, "2");
sample_node_append($node, "3");
sample_node_append($node, "4");
sample_node_append($node, "5");
sample_node_dump($node);
*/

//sample_node_dump(sample_node_append(sample_node_append(Null, "1"), "2"));

$node = sample_node_append(Null, "1");
sample_node_append($node, "2");
sample_node_append($node, "3");
sample_node_append($node, "4");
sample_node_dump($node);
//var_dump($node);

//sample_node_append(Null, "1");
//sample_node_dump(sample_node_append(Null, "1"));
//sample_node_dump(sample_node_dump(sample_node_dump($node)));


//if($baz==Null);
//if($bar==Null) var_dump($bar);
