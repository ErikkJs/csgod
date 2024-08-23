<?php
require('connect.php'); // $db : database object
/* Persons
 * +--------+------------------+------+-----+---------+----------------+
 * | Field  | Type             | Null | Key | Default | Extra          |
 * +--------+------------------+------+-----+---------+----------------+
 * | p_id   | int(10) unsigned | NO   | PRI | NULL    | auto_increment |
 * | p_name | varchar(31)      | YES  |     | NULL    |                |
 * | p_age  | int(10) unsigned | YES  |     | NULL    |                |
 * +--------+------------------+------+-----+---------+----------------+
 *
 * example code to insert, update, delete, prev and next
 */
?>

<?php
// q is the variable that we pass GET hiddenPOST, 
// q : controls which row offset we are "looking" at
$q = 0;
$counter=0;
$name='';
$age = '';
if(isset($_GET['q']) || isset($_POST['q']) ){
    if(!empty($_GET['q']))
        $q=htmlspecialchars($_GET['q']);
    else if(!empty($_POST['q']))
        $q=htmlspecialchars($_POST['q']);
    else
        $q=0;

//    echo $q."<br>";
}
?>
<?php 
$sql='';
if(isset($_POST['action'])){
    $action = htmlentities(trim($_POST['action']));
    echo $action."<br>";
    // 1)validate that no missing form fields
    // 2)extract and sanitize data
    $name= htmlspecialchars($_POST['Name']);
    $IMG=htmlspecialchars($_POST['IMG']);
    $SKU=htmlspecialchars($_POST['SKU']);
    $description= htmlspecialchars($_POST['Description']);
    $rating= htmlspecialchars($_POST['Rating']);
    $stock = htmlspecialchars($_POST['Stock']);
    $price =htmlspecialchars($_POST['Price']);
    switch($action) {
    case "Insert": 
       $sql = "INSERT INTO Products (prod_name, prod_img, prod_description, prod_price, prod_rating, prod_sku, prod_stock) VALUES ('{$name}','{$IMG}','{$description}','{$price}','{$rating}','{$SKU}','{$stock}')";
        break;
    case "Update":
        $sql = "UPDATE Products SET prod_name='{$name}', prod_img='{$IMG}',prod_description= '{$description}', prod_price= '{$price}', prod_rating='{$rating}', prod_sku= '{$SKU}', prod_stock='{$stock}'  WHERE prod_sku={$SKU}";
        break;
    case "Delete":
        $sql = "DELETE FROM Products WHERE prod_sku={$SKU}";
        break;
    default:
        $sql='';
    }

    if($res = $db->query($sql)){
        echo "$action successful<br>";
        $name='';
        $age ='';
    }else {
        echo "$action unsuccessful<br>";
    }
}// end of isset post action



if($q>0 && !isset($_POST['action']) ) { // and also less than num_rows
    $sql="SELECT * FROM Products";
    if($res = $db->query($sql)) {
        // need to check for $q >=num_rows  if so :( set $q to 0 break
        if($q>= 0 && $q <= $res->num_rows) {
            $res->data_seek($q - 1);
            $row = $res->fetch_assoc();
            
            $name=$row['prod_name']; // to populate the form
            $IMG=$row['prod_img'];
            $SKU=$row['prod_sku'];
            $description= $row['prod_description'];
            $rating= $row['prod_rating'];
            $stock = $row['prod_stock'];
            $price =$row['prod_price'];
              // to populate the form
        } 
        else { $q=0; }
    }
}
echo $q ."<br>";
?>

<div class="panel panel-default">
  <div class="panel-body" id="">
<form action='lab11admin.php' method='POST'>
Name: <input type='text' name='Name' required value='<?php echo $name;?>'><br>
IMG: <input type='text' name='IMG' value='<?php echo $IMG;?>' required><br>
SKU: <input type='text' name='SKU' value='<?php echo $SKU;?>' required><br>
Description: <input type='text' textarea  name='Description' value='<?php echo $description;?>' required><br>
Rating: <input type='number' name='Rating' min= '0' max="5" value='<?php echo $rating;?>' required><br>
Stock: <input type='number' name='Stock' min='0'  value='<?php echo $stock;?>' required><br>
Price: <input type="number" name="Price" min='0' step = '.01' value='<?php echo $price;?>' required><br>
<input type='submit' name='action' value='Insert'>
<input type='submit' name='action' value='Update'>
<input type='submit' name='action' value='Delete'>
<input type='hidden' name='q' value='<?php echo $q; ?>'>
</form>

<?php
echo "<a href='http://www.cs.csubak.edu/~esoto/lab11/lab11admin.php?q=".($q-1)."'><button>&larr;Prev</button></a>";
?>
<?php
echo "<a href='http://www.cs.csubak.edu/~esoto/lab11/lab11admin.php?q=".($q+1)."'><button>Next&rarr;</button></a>";
?>

</div>
</div>


<?php
echo "<hr>";
?>

<div class="panel panel-default">
  <div class="panel-heading" id="titl">
    <h3 class="panel-title" id="titl">
  </div>
  <div class="panel-body" id="con">
<?php
$sql = "SELECT * FROM Products";
if($res=$db->query($sql)){
    while($row = $res->fetch_assoc()){
        echo "<div class='container'>";
        echo "<div class='row'>";
      //foreach($items as $item) {
      
        echo "<div class='col-sm-4'>";
        //starts it 
        echo "<div class='panel panel-primary'>";
        echo "<div class='panel-heading'>". $row['prod_name']. "</div>";
        //image
        echo "<div class= 'panel-body'>";
        echo "<img src='".".".IMG_DIR.$row['prod_img']."'"."class= 'img-responsive'style='width:100%, height:100%;'>";
        // end image
        
        //price
        echo "<div id='stuff'>";echo 'Price:$'.$row['prod_price']. "<br>";
        //end price

          //rating
        echo "Rating:";for ($i=0; $i <$row['prod_rating'] ; $i++) {echo "<span class='glyphicon glyphicon-star' aria-hidden='true'></span>";}echo "<br>";
          //end rating

         //stock
        if ($row['prod_stock']>0) {  echo "<div id='stock'>Stock: In Stock </div>";}else{echo "<div id='nstock'>Stock: Not in stock </div>";}echo "SKU:". $row['prod_sku']; echo "</div>";
        

        echo "</div>";
        //description
        echo "<div class='panel-footer'>".$row['prod_description']. '</div>';
        //end des
        //starts new one
        echo "</div>";
        echo "</div>";
      
       
    }
}
?>
  </div>
</div>
</div>
</div>
</div>
</div>
</div>
</div>

<?php
$db->close();
?>
  </div>

</div>
</div>






