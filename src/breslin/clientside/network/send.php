<?php
$bytebuffer = $_POST["bytebuffer"];

//is this where you want to send back server data from game? or should we use seperate ajax calls? 
//scratch that for a minute i think you need to interject here and..
//ask if this is a mMessageConnect and if so
//then ask the db for a client_id
//then send back the client_id...
//and tell db that you got it.

if ($bytebuffer == -101)
{
	echo 'mMessageConnect';
	
	//get current client id then update record 
	$conn = pg_connect("host=localhost dbname=abcandyou user=postgres password=mibesfat")
        or die('Could not connect: ' . pg_last_error());

	$query = "select client_id from client where id = 1;";

	$result = pg_query($conn,$query);
 	$row = pg_fetch_array($result, 0);
	
	echo $row["client_id"];

	

}
else
{
	echo 'somethingElse';
}



//have to create this every time because it cannot be save in a session variable as it's a resource
$sock = socket_create(AF_INET, SOCK_DGRAM,0);

$packed = pack("i",$bytebuffer);
$len = strlen($packed);

socket_sendto($sock, $packed, $len, 0, '192.168.1.100', 30004);
socket_close($sock);


?>

