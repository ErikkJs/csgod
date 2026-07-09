<?php
session_start(); 
// sesssion has started in script that 
// is including this file
function redir_if_active() {
    if(isset($_SESSION['active'])){
        redir_home();
    }
}
function redir_if_NOT_active() {
    if(!isset($_SESSION['active'])){
        redir_home();
    }
}
function redir_home() {
    header("Location: home.php");
    exit();
}
?>
