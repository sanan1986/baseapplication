<?php
$_GET["ip"]
?>
<meta http-equiv="Content-type" content="text/html;charset=UTF-8">
<html>
<head>
<title>ABC AND YOU</title>

<script src="/socket.io/socket.io.js"></script>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.6.4/jquery.min.js"></script>
<script src="http://192.168.2.77/mootools/mootools-core-1.4.5-full-compat.js"></script>
<script src="http://192.168.2.77/breslin/clientside/application/application_breslin.php"></script>
<script src="http://192.168.2.77/breslin/clientside/network/network.php"></script>
<script src="http://192.168.2.77/breslin/breslin/statemachine/state.php"></script>
<script src="http://192.168.2.77/breslin/statemachine/state_machine.php"></script>
<script src="http://192.168.2.77/breslin/clientside/application/states/application_global.php"></script>
<script src="http://192.168.2.77/breslin/clientside/application/states/application_initialize.php"></script>
<script src="http://192.168.2.77/breslin/clientside/application/states/application_main.php"></script>
<script src="http://192.168.2.77/breslin/clientside/application/states/application_play.php"></script>
<script src="http://192.168.2.77/breslin/clientside/bytebuffer/byte_buffer.php"></script>
<script src="http://192.168.2.77/breslin/clientside/game/game.php"></script>
<script src="http://192.168.2.77/breslin/clientside/game/states/game_global.php"></script>
<script src="http://192.168.2.77/breslin/clientside/game/states/game_initialize.php"></script>
<script src="http://192.168.2.77/breslin/clientside/game/states/game_pause.php"></script>
<script src="http://192.168.2.77/breslin/clientside/game/states/game_play.php"></script>
<script src="http://192.168.2.77/breslin/clientside/shape/shape.php"></script>
<script src="http://192.168.2.77/breslin/math/vector_3d.php"></script>
<script src="http://192.168.2.77/breslin/clientside/command/command.php"></script>
<script src="http://192.168.2.77/breslin/clientside/div/div.php"></script>
<script src="http://192.168.2.77/breslin/clientside/ability/ability.php"></script>
<script src="http://192.168.2.77/breslin/clientside/ability/move/ability_move.php"></script>
<script src="http://192.168.2.77/breslin/clientside/ability/move/ability_move_state.php"></script>
<script src="http://192.168.2.77/breslin/clientside/ability/move/global_process_tick_move.php"></script>
<script src="http://192.168.2.77/breslin/clientside/ability/move/catchup_process_tick_move.php"></script>
<script src="http://192.168.2.77/breslin/clientside/ability/move/normal_process_tick_move.php"></script>
<script src="http://192.168.2.77/breslin/clientside/ability/move/normal_interpolate_tick_move.php"></script>
<script src="http://192.168.2.77/breslin/clientside/ability/move/ability_move_state_machine.php"></script>

</head>

<body bgcolor="grey">

<script>

var mApplication;

window.addEvent('domready', function()
{
        mApplication = new ApplicationBreslin('192.168.2.77', 30004);

        //input
        document.addEvent("keydown", mApplication.keyDown);
        document.addEvent("keyup", mApplication.keyUp);

        //main loop
        var t=setInterval("mApplication.processUpdate()",32)
       
	mApplication.mNetwork.mSocket.on('news', function (byteBuffer)
        {
		var a = byteBuffer.split(','); 
		if (a[0] == -103)
		{
			mApplication.log('t:' + a[0] + 'c:' + a[1] + 'i:' + a[2] + 'x:' + a[3] + 'y:' + a[4] + 'z:' + a[5] + 'x:' + a[6] + 'z:' + a[7] + 'm:' + a[8] + 'a:' + a[9]);
			mApplication.mGame.addShape(byteBuffer);
		}
		if (a[0] == 1)
		{
			mApplication.log('1111');
			mApplication.mGame.readServerTick(byteBuffer);
		}
        });
}

);

</script>
</body>
</html>

