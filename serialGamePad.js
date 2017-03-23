var buffer = "";
var splitSring = "\n";
var joyButtons = []
var joyStickX,joyStickY;
var games = [
            {url:"http://janekx21.lima-city.de/phaser/jumpandrun/",name:"Jump and Run"},
            {url:"http://janekx21.lima-city.de/phaser/jumpandrun/",name:"Phaser 3"}
        ];
//Called when application is started.
function OnStart()
{
	//Create a layout with objects vertically centered.
	app.SetDebugEnabled(false);
	app.SetOrientation("landscape");
	game = app.CreateWebView(1,1,"NoScrollBars");
	//game.LoadUrl("http://janekx21.lima-city.de/phaser/jumpandrun/");
	//game.LoadUrl("http://janekx21.lima-city.de/phaser/jumpandrun/");
	game.Execute("theBridge = {}");
	
	lay = app.CreateLayout( "linear", "VCenter,FillXY" );	

    btn = app.CreateButton("shake");
    btn.SetOnTouch(shake);
    
    lay.AddChild(game);
	//lay.AddChild(btn);
	
	//Add layout to app.	
	app.AddLayout( lay );
	
	
	usb = app.CreateUSBSerial(9600);
	if( !usb ){
		app.ShowPopup( "Please connect your Arduino and restart" );
		return;
	}
	usb.SetOnReceive( usb_OnReceive );
	app.ShowPopup( "Connected" );
	
	selectGame();
}

function usb_OnReceive( data ){
    buffer+=data;
    var p = buffer.search(splitSring);
    if(p != -1){
        //console.log("++found newline in:"+p + "\t buffer ="+ buffer + "\t data="+data);
        var ex = buffer.split(splitSring);
        ex.forEach(handle);
        buffer = "";
    }else{
        
        //console.log("--buffer update" + "\t data="+data + "\t buffer ="+ buffer);
    }
    var x = data.charCodeAt(0);
    //console.log(data);
    //console.log(x);
    if(x>75){
        //shake();
    }
}

function shake(){
    game.Execute("customKeys.up = true");
    setTimeout(function(){game.Execute("customKeys.up = false");},100);
}

function handle(str,index){
    if(str == ""){
        //console.log("..nothing index=" + index);
    }else{
        //console.log("..got execute:"+str + "\tchar1:"+str.charCodeAt(0) +"\tchar2:"+str.charCodeAt(1) +"\tchar3:"+str.charCodeAt(2).toString(2));
        var x=str.charCodeAt(0);
        var y=str.charCodeAt(1);
        var z = reverse(str.charCodeAt(2).toString(2));
        
        var buttons = [false,false,false,false,false,false,false,false];
        for(var i=0;i<8;i++){
            if(z[i] == "1"){
                buttons[i] = true;
            }else{
                buttons[i] = false;
            }
            if(joyButtons[i] != buttons[i]){
                console.log("buttons Change I:"+i+ "buttons:" + z);
            }
        }
        //console.log("X:"+x+"\tY:"+y+"\tButtons:"+buttons);
        game.Execute("theBridge.x = "+x+ ";theBridge.y = "+y+";theBridge.buttons = ["+buttons+"];");
        joyStickX = x;
        joyStickY = y;
        joyButtons = buttons;
    }
}

function reverse(s){
    return s.split("").reverse().join("");
}

function selectGame(){
    var data ="";
    for(x in games){
        data += games[x].name + ",";
    }
    var list = app.CreateListDialog("Game Change",data);
    list.Show();
    list.SetOnTouch(changeGame);
}
function changeGame(name){
    console.log(name);
    var toLoad = games.find(function(obj){
        if(obj.name == name){
            return obj;
        }
        return false;
    });
    game.LoadUrl(toLoad.url);
}