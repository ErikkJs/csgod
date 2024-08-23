<h3>Notes 13</h3>
<h4>Project II In-Class Development</h4>
<hr>
<h4>PHP</h4>
<p>PHP: Hypertext Preprocessor - a server side scripting language.  
File that contains HTML,CSS,JS, and PHP code.  Ability to generate 
dynamic page content, access to server files, process GET POST requests, 
access to relational database, cookie managment, data encryption, 
session managment, and more. Can also be used for command line scripting.
Open to all major operating systems and types of webservers. Also, has
the choice of procedural programming or object oriented programming.
Read more at <a href='http://www.php.net/'>php.net</a>. </p>
<br>
<div class='btn-group'>
<button class='btn btn-primary' type='button' data-toggle='modal'
data-target='#http' data-keyboard="true">Request Response</button>
<button class='btn btn-primary' type='button' data-toggle='modal'
data-target='#php' data-keyboard="true">HelloWorld.php</button>
<button class='btn btn-primary' type='button' data-toggle='modal'
data-target='#phpDev' data-keyboard="true">Developer Mode</button>

</div>
<br>
<br>
<hr>

<div id='http' class='modal fade' 
data-backdrop='false' role='dialog'>
    <div class='modal-dialog'>
        <div class='modal-content'>
            <div class='modal-header'>
                <button type='button' class='close' 
                  data-dismiss='modal' style='color:black;'> &times;</button>
                <h4 class='modal-title'>Request Response</h4>
            </div>
            <div class='modal-body text-left'>
                <img src='./images/phpHTTP.jpg' alt='phphttp' 
                class='image-responsive' style='width:100%' >
            </div>
            <div class="modal-footer">
              <button type="button" class="btn btn-default" 
              data-dismiss="modal">Close</button>
            </div>
        </div>
    </div>
</div>

<div id='php' class='modal fade' 
data-backdrop='false' role='dialog'>
    <div class='modal-dialog'>
        <div class='modal-content'>
            <div class='modal-header'>
                <button type='button' class='close' 
                  data-dismiss='modal' style='color:black;'> &times;</button>
                <h4 class='modal-title'>HelloWorld.php</h4>
            </div>
            <div class='modal-body text-left'>
                <pre>
&lt;!DOCTYPE html&gt;
&lt;html lang="en"&gt;
&lt;head&gt;
    &lt;meta charset="UTF-8"&gt;
    &lt;title&gt;Document&lt;/title&gt;
&lt;/head&gt;
&lt;body&gt;
    &lt;?php echo "Hello World"; ?&gt;
&lt;/body&gt;
&lt;/html&gt;
                </pre>
            </div>
            <div class="modal-footer">
              <button type="button" class="btn btn-default" 
              data-dismiss="modal">Close</button>
            </div>
        </div>
    </div>
</div>

<div id='phpDev' class='modal fade' 
data-backdrop='false' role='dialog'>
    <div class='modal-dialog'>
        <div class='modal-content'>
            <div class='modal-header'>
                <button type='button' class='close' 
                  data-dismiss='modal' style='color:black;'> &times;</button>
                <h4 class='modal-title'>Enable Error</h4>
            </div>
            <div class='modal-body text-left'>
<pre>
&lt;?php
/* *DISCLAIMER*
    This will only display non-fatal (runtime)
    errors. Fatal errors (syntax errors) will 
    cause a blank page to display regardless of 
    the level of error reporting.  */

// Overrides server configuration for error reporting
// (All error types are reported)
error_reporting(E_ALL);   

// Overrides server configuration for displaying errors
// (Error messages are turned on)
ini_set('display_errors', 1);
?&gt;
&lt;!DOCTYPE html&gt;
&lt;html lang="en"&gt;
&lt;head&gt;
    &lt;meta charset="UTF-8"&gt;
    &lt;title&gt;Document&lt;/title&gt;
&lt;/head&gt;
&lt;body&gt;
    &lt;?php echo "Hello World"; ?&gt;
&lt;/body&gt;
&lt;/html&gt;
</pre>
            </div>
            <div class="modal-footer">
              <button type="button" class="btn btn-default" 
              data-dismiss="modal">Close</button>
            </div>
        </div>
    </div>
</div>

