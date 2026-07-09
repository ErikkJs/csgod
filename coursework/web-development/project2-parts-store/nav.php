
  <!-- Navigation -->
    <nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
        <div class="container">
            <!-- Brand and toggle get grouped for better mobile display -->
            <div class="navbar-header">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
                    <span class="sr-only">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="home.php">Goat Squad </a>
            </div>
            <!-- Collect the nav links, forms, and other content for toggling -->
            <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
                <ul class="nav navbar-nav">
                     <li>
                        <a href="home.php">Home</a>
                    </li>
                    <li>
                        <a href="products.php">Products</a>
                    </li>
                    <li>
                        <a href="contactus.php">Contact Us</a>
                    </li>
                    <li>
                      <a href="finalprop.php">Final Proposition thing</a>
                    </li>
                   
                </ul> 
            <ul class="nav navbar-nav navbar-right">
             
             <li><a href="shoppingcart.php"><span class="glyphicon glyphicon-shopping-cart"></span> Cart</a></li>
             <?php if(isset($_SESSION['active'])){ // if active user -> display user home page
                    echo " <li> <a href='./logout.php'>Logout</a></li>";
                }else{?>
             <li> <a href="login.php">Login</a></li>
             <li> <a href="">Register</a></li>
             </ul> 
             <?php } ?>
          </div>
            <!-- /.navbar-collapse -->
        </div>
        <!-- /.container -->
    </nav>
    <!--END NAVIGATION -->
