  <?php 
  $sql='';
  if(isset($_GET['sortby'])) {
    switch($_GET['sortby']) {

      case 'rating': $sql= "SELECT prod_name, prod_img, prod_description, 
                    prod_price, prod_rating, prod_sku, prod_stock
            FROM Products 
            ORDER BY prod_rating DESC";
        break;

      case 'priceHighToLow': $sql= "SELECT prod_name, prod_img, prod_description, 
                    prod_price, prod_rating, prod_sku, prod_stock
            FROM Products
            ORDER BY prod_price DESC";
        break;

      case 'priceLowToHigh': $sql=  "SELECT prod_name, prod_img, prod_description, 
                    prod_price, prod_rating, prod_sku, prod_stock
            FROM Products
            ORDER BY prod_price";
        break;
    case '': // fall through to default case
    default:
        $sql = 
            "SELECT prod_name, prod_img, prod_description, 
                    prod_price, prod_rating, prod_sku, prod_stock
            FROM Products";
        break;
}
}
?>
<?php 
if($_GET['item']==NULL){
  echo '<div class="row">
          <div class="col-md-3">
               <p class="lead"></p>
              <div class="list-group">
                 <form method="GET" action="products.php" id="form">
                   <label for="sortby">Sort By</label>
                   <select name="sortby" id="sortby" onchange="this.form.submit()"" id="form">
                     <option value=""> </option>
                      <option value="rating">Rating</option>
                      <option value="priceHighToLow">Price High to Low </option>
                      <option value="priceLowToHigh">Price Low to High </option>
                    </select>
                 </form>
              </div>
           </div> 
    <div class="col-md-9">
   <div class="row">
   ' ;
 } 
   else{
    echo '<div class="row">
        <div class = "col-md-8">';
   }
   ?>
  <?php  
    
  if(isset($_GET['item']) && $_GET['item'] > 1){

    $item = $_GET['item'];
    $sql = 
    "SELECT prod_name, prod_img, prod_description, 
                    prod_price, prod_rating, prod_sku, prod_stock
            FROM Products WHERE prod_sku = '$item'";
      $res=$db->query($sql);
      while( $row=$res->fetch_assoc()){

/*  echo'<div class="col-sm-4 col-lg-4 col-md-4">
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
                                    </div> <a href="products.php" class="btn btn-sm btn-default btn-block">Back To Store</a>
                  </div>
                  </div>
                
                ';*/
    echo'<img class ="img-responsive" src="/~esoto/phpmidterm2/'.IMG_DIR.$row['prod_img'].'"'; echo'alt="'; echo $row['prod_name'].'"'; echo'></div>
                            <div class="col-md-4">
                                <h1>'.$row['prod_name']. '</h1>
                                <p>'.$row['prod_description'].'</p>
                                <p>'.$row['prod_sku'].'</p>';
                                 if ($row['prod_stock']>0) {  echo '<div id="stock"> <p class ="pullright"> Stock: In Stock';
                                  }
                                else{echo '<div id="nstock"> <p class ="pullright"> Stock: Not in stock ';} 
                                echo " <br> SKU:". $row['prod_sku']." </p> 
                                </div>";
                                echo '<div class="ratings">'."Rating: <p>";for ($i=0; $i <$row['prod_rating'] ; $i++) {echo "<span class='glyphicon glyphicon-star' aria-hidden='true'></span>";}echo "</p> </div>
                        ";
                        echo  '<div class="separator clear-left">
                        <form action ="shoppingcart.php" method= "post">
                       <input type="hidden" name="item_id" value="'.$item.'">
                       <input class="btn btn-sm btn-success btn-block" type="submit" value="Add To Cart">
                        </form>
                                    </div> <a href="products.php" class="btn btn-sm btn-default btn-block">Back To Store</a>
                  </div>
                  </div>';
      }
  } 
  else if(isset($_GET['sortby'])) {
  $res=$db->query($sql);
  while($row = $res->fetch_assoc()){
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
                        </div>
                  </div>
                
                ";
                global $counter;
                $counter++;
                if($counter %3 == 0)
                {
                  echo "</div>";
                  echo '<div class="row">';
                  
                }

    
  }
}
else{  
   $sql = "SELECT prod_name, prod_img, prod_description, 
                    prod_price, prod_rating, prod_sku, prod_stock
            FROM Products";
  $res=$db->query($sql);
  
  while($row = $res->fetch_assoc()){
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
                        </div>
                  </div>
                
                ";
                global $counter;
                $counter++;
                if($counter %3 == 0)
                {
                  echo "</div>";
                  echo '<div class="row">';
                  
                }

}
};
?>
<?php
$db->close();
?>
</div>    