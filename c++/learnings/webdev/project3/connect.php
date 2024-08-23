<?php
// $link, $mysqli, $db    ---- resource variable
define(HOST, 'localhost');
define(USERNAME, 'esoto');
define(PW, 'Dtut4xmid');
define(DB, 'esoto');
define("IMG_DIR", "/images/");
//$db = mysqli_connect(HOST, USERNAME, PW, DB); // Procedural
$db = new mysqli(HOST,USERNAME, PW, DB); // OOP


// ~$mysqli -u username -p 
// mysql> use DB;
//
// mysqli_foo($db, params) $db->foo(params)

if($db) {
}
else {
    echo "connection unsuccessful\n";
    exit();
}
?>
