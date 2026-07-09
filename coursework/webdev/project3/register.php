<!doctype html>
<html>
<head>
<?php
			require_once("connect.php");
			require("lib.php");
			require("header.php");  ?>
</head>
<body>
<div class="wrapper">
<?php 
$sentusername;
$sentpassword;
$err;
$res;
$row;

if(isset($_SESSION['active'])){
	header("Location: home.php");
	exit();
}
if (isset($_POST['submit'])){
	Signup();
}
function newuser(){
	global $db;
	if(isset($_POST['username'], $_POST['password'], $_POST['email'],$_POST['passwordcheck'],$_POST['userfirstname'], $_POST['userlastname']) &&!empty($_POST['userfirstname']) && !empty($_POST['userlastname']) &&!empty($_POST['username']) && !empty($_POST['password']) && !empty($_POST['passwordcheck'])){

	//sanatize form
	$sentemail =trim(htmlspecialchars($_POST['email']));
	$sentusername =trim(htmlspecialchars($_POST['username']));
	$sentpassword =trim(htmlspecialchars($_POST['password']));
	$sentpasswordcheck =trim(htmlspecialchars($_POST['passwordcheck']));
	$sentfirstname =trim(htmlspecialchars($_POST['userfirstname']));
	$sentlastname =trim(htmlspecialchars($_POST['userlastname']));
	

	$getemail = "SELECT user_email, user_password, user_uname, user_fname 
	FROM Users 
	WHERE user_email = '$sentusername' or user_uname = '$sentusername'";		
	
		$adduser = "INSERT INTO Users 
		(user_email, user_password, user_uname, user_fname, user_lastname)
		Values( '{$sentemail}', '{$sentpassword}', '{$sentusername}', '{$sentfirstname}', '{$sentlastname}')";
		if($res = $db->query($adduser)){
			echo "Seccessfully Registered";
			header("Location: login.php");
		}		
	}
	else{
		$_SESSION['err']= "All Fields Must Be filled";
	}
} 

function Signup(){
	global $db;
	if ($_POST['passwordcheck'] != $_POST['password']) {
		$_POST['err']= "Passwords Do not Match!";
	}
	if (!empty($_POST['username']) && $_POST['passwordcheck']== $_POST['password']) {
		$testingu =trim(htmlspecialchars($_POST['username']));
		$testinge =trim(htmlspecialchars($_POST['email']));
		$testingp=trim(htmlspecialchars($_POST['password']));

		$check= "SELECT * FROM Users WHERE user_uname = '{$testingu}'";
		if ($res= $db->query($check)){
			$row = $res->fetch_assoc();
			
			if($row['user_uname'] != $testingu && $row['user_email']!= $testinge){
			newuser();
			}
		}
		else{$_POST['err'] = "User Already exists!";}

	}
}
// check if a form has been submitted

?>


<?php include('nav.php');?>
<div class="container">
    <div class="row">
    	<div class="col-md-4 col-md-offset-4">
    		<div class="panel panel-default">
			  	<div class="panel-heading">
			    	<h3 class="panel-title">Register</h3>
			 	</div>
			  	<div class="panel-body">
			    	<form action="register.php" method="POST">
                    <fieldset>
    
                    	<div class="form-group">
			    		    <input class="form-control" placeholder="First Name" name="userfirstname" type="text" required="true">
			    		</div>

			    		<div class="form-group">
			    		    <input class="form-control" placeholder="UserName" name="username" type="text" required="true">
			    		</div>
			    		<div class="form-group">
			    		    <input class="form-control" placeholder="Last Name" name="userlastname" type="text" required="true">
			    		</div>
			    	  	
			    	  	<div class="form-group">
			    		    <input class="form-control" placeholder="yourmail@example.com" name="email" type="email" required="true">
			    		</div>

			    		<div class="form-group">
			    			<input class="form-control" placeholder="Password" name="password" type="password" value="" required="true">
			    		</div>
			    		<div class="form-group">
			    			<input class="form-control" placeholder="Re-Password" name="passwordcheck" type="password" value="" required="true">
			    		</div>
			    		<div class="checkbox">
			    	    	<label>
			    	    		<input name="remember" type="checkbox" value="Remember Me"> Remember Me
			    	    	</label>
			    	    </div>
			    		<input class="btn btn-lg btn-success btn-block" type="submit" name ="submit" value="Login">
			    	</fieldset>
			      	</form>
                      <hr/>
                    <center><h4>OR</h4></center>
                    <input class="btn btn-lg btn-facebook btn-block" type="submit" value="Login via facebook">
			    </div>
			</div>
			<?php
				echo $_POST['err'];
		
			
			?>
		</div>
	</div>
</div> 
<?php include ('footer.php'); ?>
</body>
</html>
