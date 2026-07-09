<?php
	$param = $_GET['param'];
	
	echo "the param is " . $param . "<br>";
   	if($param == "hello"){
	    echo "hola <br>" ;
	}else{
		$num = $param * 2+3;
		echo "number:" . $num . "<br>";
	}
	echo "<br>";
	echo "<img src='lab4_DFD.gif'/>" ;

?>
