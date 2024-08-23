<?php 
if(!isset($_SESSION['active'])){
	header("Location: home.php");
	exit();
}

elseif (isset($_POST['item_id'])) {
	$item = $_POST['item_id'];
    $sql = 
    "SELECT prod_name, prod_id, prod_img, prod_description, 
                    prod_price, prod_rating, prod_sku, prod_stock
            FROM Products WHERE prod_sku = '$item'";
    if($res=$db->query($sql)){
     $row= $res->fetch_assoc();
     $item_id= $row['prod_id'];
     if(!isset($_SESSION['cart'])){
     	$_SESSION['cart']=[];
	}     
     	$_SESSION['cart'][$item_id]= isset($_SESSION['cart'][$item_id]) ? $_SESSION['cart'][$item_id]+1 : 1; 
    }
}
elseif (isset($_POST['yeet']) || $_SESSION['cart']==NULL ){
	$_SESSION['cart']= NULL;
	echo 'Shopping cart is empty!';
}elseif (isset($_POST['item_id']) && $_SESSION['cart']!=NULL ){
	var_dump($_POST['item_id']);
	$item=$_POST['item_id'];
	$quantityposted=$_POST['quantity'];
	foreach ($_POST as $key_prod => $qnty) {
	
	$qnty = htmlspecialchars($quantityposted);
  if(array_key_exists($key_prod, $_SESSION['cart'])) {
    $_SESSION['cart'][$key_prod] = $qnty;
		}
	}
}
if(isset($_SESSION['active'])){
	if (isset($_SESSION['cart'])){
		$_SESSION['cart_total']=0.0;
		$cartset=false;
		echo '<div class="container">
    			<div class="row">
    			 <div class="col-sm-12 col-md-10 col-md-offset-1">
           			 <table class="table table-hover"><thead>
                    <tr>
                        <th>Product</th>
                        <th>Quantity</th>
                        <th class="text-center">Price</th>
                        <th class="text-center">Total</th>
                        <th> </th>
                    </tr>
                </thead>
                <tbody>
                    ';
		
		foreach ($_SESSION['cart'] as $item_id => $cartQnty) {
			$item = $item_id;
			$cartset=true;
   			 $sql = 
    		"SELECT prod_name, prod_img, prod_description, 
    		prod_price, prod_sku, prod_stock, prod_id
            FROM Products WHERE prod_id = '$item'";

			if($res=$db->query($sql)){
     			$row= $res->fetch_assoc();
     			$itemqunt = $_SESSION['cart'][$item_id];
     			$itemtotal = $cartQnty*$row['prod_price'];
     			$cart_total = $itemqunt+$cart_total;
     			$_SESSION['cart_total']+=$itemtotal;

     		 echo '<td class="col-sm-8 col-md-6">
                        <div class="media">
                            <a class="thumbnail pull-left" href="products.php?item='. $row['prod_sku'].'"</a>'.'<img class="media-object" src="/~esoto/phpmidterm2/'.IMG_DIR.$row['prod_img']. '"'. 'style="width: 72px; height: 72px;">';
              echo '<div class="media-body">
                                <h4 class="media-heading"><a href="products.php?item='. $row['prod_sku'].'">'.$row['prod_name'].'</a></h4>
                                <h5 class="media-heading"> by <a href="#">'. $row['prod_description'].' </a></h5>
                                <span>Status: </span><span class=';
                                if ($row['prod_stock']!=0) {echo '"text-success"><strong>In Stock</strong></span>';}else {echo '"text-danger"><strong>Not In Stock</strong></span>';}
             echo ' </div>
                        </div></td>';
             echo '<form action= "shoppingcart.php" method = "post">';         
             echo '<td class="col-sm-1 col-md-1" style="text-align: center">
                        <input type="number" class="form-control" id="Quantity" name="quantity" value="'. $itemqunt.'">
                        </td>';
             echo '<td class="col-md-1 text-center"><strong>$'. $row['prod_price'].'</strong></td>
                        <td class="col-md-1 text-center"><strong>$'. $itemtotal.' </strong></td>
                        <td class="col-md-1">
                      
                        <input type="hidden" name="item_id" value="'.$row['prod_id'].'"> </button><button type="submit" class="btn btn-success" value= "updatecart">
                            <span class="glyphicon glyphicon-check"></span> Update
                        </button></form>';

              echo '<button type="submit" class="btn btn-danger" value= "removeitem">
                            <span class="glyphicon glyphicon-remove"></span> Remove
                        </td>';
              echo "</tr>
                    ";

			}
		}
	
	if ($cartset == true){
		echo '<tr> 
				<td>   </td>
                        <td>   </td>
                        <td>   </td>
                        <td><h5>Subtotal</h5></td>
                        <td class="text-right"><h5><strong>';
                        echo $_SESSION['cart_total']; 
                        echo '</strong></h5></td>
                    </tr>
                    <tr>
                        <td>   </td>
                        <td>   </td>
                        <td>   </td>
                        <td><h5>Estimated shipping</h5></td>
                        <td class="text-right"><h5><strong>$6.94</strong></h5></td>
                    </tr>
                    <tr>
                        <td>   </td>
                        <td>   </td>
                        <td>   </td>
                        <td><h3>Total</h3></td>
                        <td class="text-right"><h3><strong>$';
                        echo $_SESSION['cart_total']+6.94;
                        echo '</strong></h3></td>
                    </tr>
                    <tr>
                        <td>   </td>
                        <td>   </td>
                       <td>
                       <form action ="shoppingcart.php" method= "post">
                       <input type="hidden" name="yeet" value="yeet">
                        <button type="submit" class="btn btn-danger">
                            Empty Cart <span class="glyphicon glyphicon-fire"></span>
                        </button>
                        </form>
                        </td>
                         
                        <td> 
                        <a href="products.php" <button type="button" class="btn btn-default">
                            <span class="glyphicon glyphicon-shopping-cart"></span> Continue Shopping
                        </button> </a></td>
                        <td>
                        <button type="button" class="btn btn-success">
                            Checkout <span class="glyphicon glyphicon-play"></span>
                        </button></td>

                    </tr>
                </tbody>
            </table>
        </div>
    </div>
</div>';}
	}


}
	


?>