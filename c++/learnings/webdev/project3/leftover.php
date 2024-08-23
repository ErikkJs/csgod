    echo'<div class="col-sm-4 col-lg-4 col-md-4">
                        <div class="thumbnail">
                            <img src="/~esoto/phpmidterm2/'.IMG_DIR.$row['prod_img'].'"'; echo'alt="'; echo $row['prod_name'].'"'; echo'>
                            <div class="caption">
                                <h4 class="pull-right">'.$row['prod_price']. '</h4>
                                <h5><a href="products.php?item='. $row['prod_sku'].'">'. $row['prod_name'].'</a>
                                </h5>
                                <p>'. $row['prod_description']. '</p>';
                                 if ($row['prod_stock']>0) {  echo '<div id="stock"> <p class ="pullright"> Stock: In Stock';
                                  }
                                else{echo '<div id="nstock"> <p class ="pullright"> Stock: Not in stock ';} 
                                echo " <br> SKU:". $row['prod_sku']." </p> 
                                </div> 
                                </div>";
                                echo '<div class="ratings">'."Rating: <p>";for ($i=0; $i <$row['prod_rating'] ; $i++) {echo "<span class='glyphicon glyphicon-star' aria-hidden='true'></span>";}echo "</p> </div>
                        ";
                        echo  '<div class="separator clear-left">
                        <form action ="shoppingcart.php" method= "post">
                       <input type="hidden" name="item_id" value="'.$item.'">
                       <input class="btn btn-sm btn-success btn-block" type="submit" value="Add To Cart">
                        </form>
                                    </div> <a href="products.php" class="btn btn-primary btn-default">Back To Store</a>
                  </div>
                  </div>
                
                ';



                if (isset($_POST['newpass']) && $_POST['newpass']!= NULL)
  { 
    $old= trim(htmlspecialchars($_POST['oldpass']));
    $new= trim(htmlspecialchars($_POST['newpass']));

    $_SESSION['newpass']= $new;
    $_SESSION['old']=$old;

    $getemail = "SELECT user_email, user_password, user_uname, user_fname 
    FROM Users 
    WHERE user_email = '$_SESSION[email]' or user_uname = '$_SESSION[email]' AND user_password = $_SESSION[old]";

    // check if password is right 
  if($res = $db->query($getemail)){

    $getpass = "SELECT user_email, user_password, user_uname, user_fname FROM Users 
    WHERE user_password = '$_SESSION[old]'";
    if($rep = $db->query($getpass)){
        $row= $res->fetch_assoc();    
            // check if passwords entered are the same 
           if($_SESSION['old']==$_SESSION['new']){
            $_SESSION['err']="Password is the same!";
            }
            // check if the password is diffrent than the accounts old 
            if ($_SESSION['old']==$row['user_password']) {
              $_SESSION['err']="New Password is The Same As Old Password";
            }
//$getpass = "SELECT user_email, user_password, user_uname, user_fname FROM Users WHERE user_password = '$_SESSION[newpass]' AND user_email = '$_SESSION[email]'";
                // update the password
                else{
                  $uppass = "UPDATE Users
                  SET user_password='$_SESSION[newpass]' 
                  WHERE user_email = '$_SESSION[email]'";
                  echo $uppass;
                  if($res = $db->query($uppass)){
                    $_SESSION['newpass']==NULL;
                    header("Location: manage.php");
                }
              } 
            }   
        }
      }
      else{
        $_SESSION['err']="Old password is incorrect";
      }