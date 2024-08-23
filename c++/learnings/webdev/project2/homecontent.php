<div class="panel panel-default">
  <div class="panel-heading" id="titl">
    <h3 class="panel-title" id="titl"><?php if (isset($_SESSION['active'])) {
      echo "Welcome ". $_SESSION['fname'];
    }else{
      echo "Welcome Guest";
      } ?></h3>
  </div>
</div>

<div class="row">
    <div class="col-sm-4">
      <div class="panel panel-primary" id="item">
      <div class="panel-heading" id="item">Exhaust Systems</div>
        <div class="panel-body"> <img src="exhuast.jpg" class="img-responsive" style="width:100%" alt="Ex Systems"></div>
        
      </div>
    </div>
    <div class="col-sm-4">  
      <div class="panel panel-primary" id="item">
        <div class="panel-heading" id="item">Intercoolers</div>
        <div class="panel-body">  <img src="intercooler.jpg" class="img-responsive" style="width:100%" alt="intercooler"></div>
        
      </div>
    </div>
    <div class="col-sm-4"> 
      <div class="panel panel-primary" id="item">
        <div class="panel-heading" id="item">Strut Bars</div>
        <div class="panel-body" id=""><img src="strutbars.jpg" class="img-responsive" style="width:100% " alt="strutbars"></div>
        
      </div>
    </div>
  </div>
