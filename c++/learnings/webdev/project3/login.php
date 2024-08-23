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
if(isset($_POST['username'], $_POST['password']) &&!empty($_POST['username']) && !empty($_POST['password'])){

	//sanatize form
	$sentusername =trim(htmlspecialchars($_POST['username']));
	$sentpassword =trim(htmlspecialchars($_POST['password']));
	$getemail = "SELECT user_email, user_password, user_uname, user_fname 
	FROM Users 
	WHERE user_email = '$sentusername' or user_uname = '$sentusername'";
		
	if($res = $db->query($getemail)){
		//get credentials from database 
		$row = $res->fetch_assoc();
		if($sentpassword == $row['user_password']){
	
			//regenerate session id for a logged in user
			session_regenerate_id();
			//set session globals
			$_SESSION['active'] = true; // for isset verrification
			$_SESSION['username']= $row['user_uname']; // user global ref name 
			$_SESSION['email']= $row['user_email']; //global ref email
			$_SESSION['fname'] = $row['user_fname']; // global ref ses first name

			// rediirect user after suuccession
			header("Location: home.php");
			exit();
		}
		else{
			// set error message if passwords did not match
			$_SESSION['err'] = "Username/Password Did Not Match!";
		}
	}

}
?>


<?php include('nav.php');?>
<div class="container">
    <div class="row">
    	<div class="col-md-4 col-md-offset-4">
    		<div class="panel panel-default">
			  	<div class="panel-heading">
			    	<h3 class="panel-title">Login via site</h3>
			 	</div>
			  	<div class="panel-body">
			    	<form action="login.php" method="POST">
                    <fieldset>
			    	  	<div class="form-group">
			    		    <input class="form-control" placeholder="yourmail@example.com" name="username" type="text" required="true">
			    		</div>
			    		<div class="form-group">
			    			<input class="form-control" placeholder="Password" name="password" type="password" value="" required="true">
			    		</div>
			    		<div class="checkbox">
			    	    	<label>
			    	    		<input name="remember" type="checkbox" value="Remember Me"> Remember Me
			    	    	</label>
			    	    </div>
			    		<input class="btn btn-lg btn-success btn-block" type="submit" value="Login">
			    	</fieldset>
			      	</form>
                      <hr/>
                    <center><h4>OR</h4></center>
                    <input class="btn btn-lg btn-facebook btn-block" type="submit" value="Login via facebook">
			    </div>
			</div>
			<?php
				echo $_SESSION['err'];
				$_SESSION['err']= NULL;
			?>
		</div>
	</div>
</div> 
<?php include ('footer.php'); ?>
</body>
</html>
