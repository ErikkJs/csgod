<div class="panel panel-default">
  <div class="panel-heading" id="titl">
    <h3 class="panel-title" id="titl"> General Account Settings</h3>
  </div>
</div>

<?php

if(!isset($_SESSION['active'])){
	header("Location: home.php");
	exit();
}

//CHANGE UserName
else if(isset($_GET['action']) && $_GET['action']=='updateusername') {
	if (isset($_POST['newusername']))
	{
		$old= trim(htmlspecialchars($_POST['oldusername']));
		$new= trim(htmlspecialchars($_POST['newusername']));
		$_SESSION['newusername']= $new;
		if ($old==$new){
			$_SESSION['err']="username is the same!";
		}
		
		else{
		$getu_uname = "SELECT user_uname, user_password, user_fname FROM Users 
		WHERE user_uname = '$_SESSION[newusername]'";
				if($res = $db->query($getu_uname) && $res['num_rows']==1 ) {
				 	$_SESSION['err']="username is already taken";		
				}
				else{
					$upuser = "UPDATE Users
					SET user_uname='$_SESSION[newusername]' 
					WHERE user_uname = '$_SESSION[username]'";
					if($res = $db->query($upuser)){
						$_SESSION['username'] = $_SESSION['newusername'];
						$_SESSION['newusername']=NULL;
						header("Location: manage.php");
								}
				}
			}
	}

echo '<form class="form-horizontal" method="POST" action="manage.php?action=updateusername">
<fieldset>

<!-- Form Name -->
<legend>Update Username</legend>

<!-- Text input-->
<div class="form-group">
	<fieldset disabled>
  <label class="col-md-4 control-label">Old Username</label>  
  <div class="col-md-4">
  <input name="oldusername" class="form-control input-md" id="textinput" type="text" placeholder="'.$_SESSION['username'].'"> 
  </div>
  	</fieldset>
</div>


<!-- Text input-->
<div class="form-group">
  <label class="col-md-4 control-label" for="textinput">New Username</label>  
  <div class="col-md-4">
  <input name="newusername" class="form-control input-md" id="textinput" type="u_uname" placeholder="New Username">  
  </div>
</div>

<!-- Button -->
<div class="form-group">
  <label class="col-md-4 control-label">Update</label>
  <div class="col-md-4">
    <button type="submit" class="btn btn-primary" id="singlebutton">Update</button>
  </div>
</div>

</fieldset>
</form>';
echo $_SESSION['err'];
$_SESSION['err']==" ";
}
// END Username CHANGE

//CHANGE EMAIL
else if(isset($_GET['action']) && $_GET['action']=='updatemail') {
	if (isset($_POST['newemail']))
	{
		$old= trim(htmlspecialchars($_POST['oldemail']));
		$new= trim(htmlspecialchars($_POST['newemail']));
		$_SESSION['newemail']= $new;
		if ($old==$new){
			$_SESSION['err']="email is the same!";
		}
		
		else{
		$getemail = "SELECT user_email, user_password, user_uname, user_fname FROM Users 
		WHERE user_email = '$_SESSION[newemail]' or user_uname = '$_SESSION[newemail]'";
			echo $getemail;
				if($res = $db->query($getemail) && $res['num_rows']==1 ) {
					var_dump($res);
				 	$_SESSION['err']="email already exists";		
				}
				else{
					$upuser = "UPDATE Users
					SET user_email='$_SESSION[newemail]' 
					WHERE user_email = '$_SESSION[email]'";
					echo $upuser;
					if($res = $db->query($upuser)){
						$_SESSION['email'] = $_SESSION['newemail'];
						$_SESSION['newemail']==NULL;
						header("Location: manage.php");
								}
				}
			}
	}

echo '<form class="form-horizontal" method="POST" action="manage.php?action=updatemail">
<fieldset>

<!-- Form Name -->
<legend>Update Email</legend>

<!-- Text input-->
<div class="form-group">
	<fieldset disabled>
  <label class="col-md-4 control-label">Old Email</label>  
  <div class="col-md-4">
  <input name="oldemail" class="form-control input-md" id="textinput" type="email" placeholder="'.$_SESSION['email'].'"> 
  </div>
  	</fieldset>
</div>


<!-- Text input-->
<div class="form-group">
  <label class="col-md-4 control-label" for="textinput">New Email</label>  
  <div class="col-md-4">
  <input name="newemail" class="form-control input-md" id="textinput" type="email" placeholder="New Email">  
  </div>
</div>

<!-- Button -->
<div class="form-group">
  <label class="col-md-4 control-label">Update</label>
  <div class="col-md-4">
    <button type="submit" class="btn btn-primary" id="singlebutton">Update</button>
  </div>
</div>

</fieldset>
</form>';
echo $_SESSION['err'];
$_SESSION['err']==" ";
}
// END EMAIL CHANGE

// PASSWORD CHANGE
else if(isset($_GET['action']) && $_GET['action']=='updatepassword') {
	if (isset($_POST['newpass']))
	{
		$old= trim(htmlspecialchars($_POST['oldpass']));
		$new= trim(htmlspecialchars($_POST['newpass']));
		$_SESSION['newpass']= $new;
		$_SESSION['old'] = $old;
		if ($old==$new){
			$_SESSION['err']="Password is the same!";
		}
		else{
		$getemail = "SELECT user_email, user_password, user_uname, user_fname FROM Users 
		WHERE user_email = '$_SESSION[email]' or user_uname = '$_SESSION[email]' AND user_password = '$_SESSION[old]'";
		echo $getemail;
				if($res = $db->query($getemail)){
					$row= $res->fetch_assoc();
					if($_SESSION['old'] == $row['user_password']){
				$getpass = "SELECT user_email, user_password, user_uname, user_fname FROM Users 
					WHERE user_email = '$_SESSION[email]' or user_uname = '$_SESSION[email]'";
					echo $getpass;
					if($res=$db->query($getpass)){
						$upuser = "UPDATE Users
						SET user_password='$_SESSION[newpass]' 
						WHERE user_email = '$_SESSION[email]'";
						echo $upuser;
						if($res = $db->query($upuser)){
							$_SESSION['newpass']==NULL;
							header("Location: manage.php");
									}
						}
					}else {
					$_SESSION['err']= "Old password did not match";
					}
				}
			}
	}
echo '<form class="form-horizontal" method="POST" action="manage.php?action=updatepassword">
<fieldset>

<!-- Form Name -->
<legend>Update Password</legend>

<!-- Text input-->
<div class="form-group">
  <label class="col-md-4 control-label">Old Password</label>  
  <div class="col-md-4">
  <input name="oldpass" class="form-control input-md" id="textinput" type="password" placeholder="Old Password"> 
  </div>
</div>

<!-- Text input-->
<div class="form-group">
  <label class="col-md-4 control-label" for="textinput">New Password</label>  
  <div class="col-md-4">
  <input name="newpass" class="form-control input-md" id="textinput" type="password" placeholder="New Password">  
  </div>
</div>

<!-- Button -->
<div class="form-group">
  <label class="col-md-4 control-label">Update</label>
  <div class="col-md-4">
    <button type="submit" class="btn btn-primary" id="singlebutton">Update</button>
  </div>
</div>

</fieldset>
</form>';
echo $_SESSION['err'];
$_SESSION['err']=" ";
}
else if ($_GET['action']==NULL){
	$getemail = "SELECT user_email, user_password, user_uname, user_fname 
			FROM Users 
			WHERE user_email = '$_SESSION[email]' or user_uname = '$_SESSION[email]'";
			if($res = $db->query($getemail)){
		//get credentials from database 
		$row = $res->fetch_assoc();		
echo '
<div class="row">
        <div class="col-xs-12 col-sm-12 col-md-6 col-lg-6 col-xs-offset-0 col-sm-offset-0 col-md-offset-3 col-lg-offset-3 toppad" id = "w">
   
   
          <div class="panel panel-info">
            <div class="panel-heading">
              <h3 class="panel-title">'.$row["user_fname"].'</h3>
            </div>
            <div class="panel-body">
              <div class="row">
                <div class="col-md-3 col-lg-3 " align="center"> <img alt="User Pic" src="/~esoto/Finaltest'.IMG_DIR."goat.jpg".'" class="img-circle img-responsive"> </div>
                  <div class=" col-md-9 col-lg-9 "> 
                  <table class="table table-user-information">
                    <tbody>
                      <tr>
                        <td>Email:</td>
                        <td>'. $row['user_email'].'</td>
                        <td> <a href="manage.php?action=updatemail" class="btn btn-primary">Update Email</a><td>
                      </tr>
                      <tr>
                        <td>UserName:</td>
                        <td>'. $row['user_uname'].'</td>
                        <td><a href="manage.php?action=updateusername" class="btn btn-primary">Update Username</a> </td>
                      </tr>
                      <tr>
                        <td>Password</td>
                        <td>'. $row['user_password'].'</td>
                        <td> <a href="manage.php?action=updatepassword" class="btn btn-primary">Update Password</a></td>
                      </tr>      
                    </tbody>
                  </table>
        
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>';
  }
  }
 ?>