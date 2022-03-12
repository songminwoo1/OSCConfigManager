#pragma once
#define HTML "<!DOCTYPE html><head><title>mallangOSC</title></head><body>hihi <a href='http://localhost:80'>mallang button</a><br><form action='http://localhost:80'><input type='text' name='portnum'></form>	</body></html>"
#define HTML1 "<!DOCTYPE html><head><title>mallangOSC</title></head><body><h1><a href = 'http://localhost:80/' style = 'color: black;'>MallangOSC Control Panel V1.4.0 </a></h1><h2><a href = 'https://mallang.booth.pm/' style = 'color: green;'>Created by. Mallang</a></h2><p><a href = 'https://mallang.booth.pm/items/3687929' style = 'color: blue;'>update check</a></p><h5><a href = 'http://localhost:80/parmList' style = 'color: blue;'>mallangOSC parameter command list</a></h5><hr><hr>"
#define HTML2 "<hr><h4>Add new OSC receive port</h4><form action = 'http://localhost:80/portforwarding'><input type = 'text' name = 'NewPortnum'></form><hr><h4>Remove OSC receive port</h4><form action = 'http://localhost:80/portforwarding'><input type = 'text' name = 'OldPortnum'></form><hr><h4>Send OSC Command(currently, only avatar parameter int is supported)<br>ex) mallangOSC 102</h4><form action = 'http://localhost:80/portforwarding'><input type = 'text' name = 'OSCCommand'></form><hr><a href='http://localhost:80/quit' style='color: red;'>(DANGER) Terminate OSC Controller (DANGER)</a></body></html>"
#define HTML_Mallang "<hr><h2><a href = 'https://mallang.booth.pm/' style = 'color: green;'>Created by. Mallang</a></h2><p><a href = 'https://mallang.booth.pm/items/3687929' style = 'color: blue;'>update check</a></p>"

#define Portforwarding_htmlA "<div style='font-family:Times New Roman; background-color:rgb(15, 15, 15); padding:40px; padding-top:5px; padding-bottom:20px;margin: 5px'> \
<h4 style='color:white'>Current OSC Portforwarding Port<br>\
<span style='color:rgb(50,255,50)'>> "

#define Portforwarding_htmlB "</span><hr>New OSC receive port<br>\
<form action = 'http://localhost:80/portforwarding'><span style='color:rgb(50,255,50)'>> </span>\
<input type = 'text' name = 'NewPortnum' style='background-color:black; color:rgb(50,255,50); border: 1.5px solid rgb(50,255,50);' autocomplete=\"off\" \
onmousedown=\"style.backgroundColor='red'\" onmouseup=\"style.backgroundColor='black'\" \
onmouseleave=\"style.backgroundColor='black'\"></form><hr>\
Remove OSC receive port<br>\
<form action = 'http://localhost:80/portforwarding'><span style='color:rgb(50,255,50)'>> </span>\
<input type = 'text' name = 'OldPortnum' style='background-color:black; color:rgb(50,255,50); border: 1.5px solid rgb(50,255,50);' autocomplete=\"off\" \
onmousedown=\"style.backgroundColor='red'\" onmouseup=\"style.backgroundColor='black'\" \
onmouseleave=\"style.backgroundColor='black'\"></form><hr></h4></div>"

#define UserOSCSend_html "<div style='background-color:rgb(15, 15, 15); padding:40px; padding-top:5px; padding-bottom:20px;margin: 5px'> \
<h4 style = 'font-family:Times New Roman; color:white;'>Send OSC Command<br>\
<span style='font-size:90%'>(currently, only avatar parameter int is supported)<br>ex) mallangOSC 102</span>\
<form action = 'http://localhost:80/SendOSC'><span style='color:rgb(50,255,50)'>> </span>\
<input type = 'text' name = 'OSCCommand'style='background-color:black; color:rgb(50,255,50); border: 1.5px solid rgb(50,255,50);' autocomplete=\"off\" \
onmousedown=\"this.style.backgroundColor='red';\" onmouseup=\"style.backgroundColor='black'\" \
onmouseleave=\"style.backgroundColor='black'\"></form><hr></h4></div>"


#define MallangOSC_commandList "<div style='background-color:rgb(15, 15, 15); padding:40px; padding-top:5px; padding-bottom:5px;margin: 5px'> \
<h3 style = 'font-family:American Typewriter; color:white;'>mallangOSC Commands</h3>\
<h4 style = 'font-family:Times New Roman; color:white; line-height:180%'><hr>\
<span style='color:lightgreen'>0 :</span> none<br>\
<span style='color:lightgreen'>1~10 :</span> used for secondary parameter input(mostly for numbers)<br>\
<span style='color:lightgreen'>11~99 :</span> preserved for mallangOSC UI<br>\
<span style='color:lightgreen'>100 :</span> Turn off local computer(5 seconds delay)<br>\
<div style = 'font-family:Times New Roman; color:white; line-height:100%; margin-top:5px; margin-bottom:5px;'>\
<span style='color:lightgreen'>101 :</span> local computer shutdown timer set(uses next mallangOSC parameter input as timer value)<br>\
-> next parameter input>? 1 : 1min, 2 : 5min, 3 : 15min, 4 : 30min<br></div>\
<span style='color:lightgreen'>102 :</span> Cancel shutdown<br>\
<hr></h4></div>"

#define MallangBanner "<!DOCTYPE html><head><title>mallangOSC</title></head><body style='background-color:black'>\
<div style='background-color:rgb(15, 15, 15); padding:40px; padding-top:20px; padding-bottom:20px; margin: 5px'>\
<h1><a href = 'http://localhost:80/' style = 'text-decoration:none; font-family:American Typewriter; color: white;' \
OnMouseOver=\"this.style.color = 'lime'\" OnMouseOut=\"this.style.color = 'white'\"> MallangOSC Control Panel V1.4.0 </a></h1>\
<h2><a href = 'https://mallang.booth.pm/' target=\"_blank\" style = 'text-decoration:none; color: skyblue;' OnMouseOver=\"this.style.color = 'lime'; this.style.textDecoration='underline'\" \
OnMouseOut=\"this.style.color = 'skyblue'; this.style.textDecoration='none'\">Created by. Mallang</a></h2>\
<p><a href = 'https://mallang.booth.pm/items/3687929' target=\"_blank\" style = 'text-decoration:none; color: blue;' \
OnMouseOver=\"this.style.color = 'lightgreen'\" OnMouseOut=\"this.style.color = 'blue'\"> - (booth) update check</a><br>\
<a href = 'https://discord.gg/atpnHfYfn5' target=\"_blank\" style = 'text-decoration:none; color: blue;' \
OnMouseOver=\"this.style.color = 'lightgreen'\" OnMouseOut=\"this.style.color = 'blue'\"> - mallangLabs Discord</a><br><hr><hr>\
<a href = 'http://localhost:80/portforwarding' style = 'text-decoration:none; color: blue;' \
OnMouseOver=\"this.style.color = 'lightgreen'\" OnMouseOut=\"this.style.color = 'blue'\"> - multi portforwarding setup</a><br>\
<a href = 'http://localhost:80/SendOSC' style = 'text-decoration:none; color: blue;' \
OnMouseOver = \"this.style.color = 'lightgreen'\" OnMouseOut=\"this.style.color = 'blue'\"> - Custom OSC Command Sender</a><br>\
<a href = 'http://localhost:80/parmList' style = 'text-decoration:none; color: blue;' \
OnMouseOver=\"this.style.color = 'lightgreen'\" OnMouseOut=\"this.style.color = 'blue'\"> - mallangOSC parameter command list</a></p><hr><hr></div>"

#define MallangDiv "<div width=80% height= 10% style='background-color:lightblue; margin:5px;'><p> Mallang Here! </p></div>"

#define MallangVideo "<iframe style= 'background-color:lightblue; margin:5px; padding:2%; width:calc(96% - 10px); height:690px;' src='https://www.youtube.com/embed/6vCX3WScg3M' title='YouTube video player' frameborder='0' allow='accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture' allowfullscreen></iframe>"

#define MallangEnder "<h4 style = 'background-color:rgb(15, 15, 15); padding:15px; \
padding-bottom:18px; padding-left:40px; margin:0; margin-left:5px; margin-right:5px'>\
<a href='http://localhost:80/quit' style='text-decoration:none; color: darkred;'\
OnMouseOver = \"this.style.color = 'red'\" OnMouseOut=\"this.style.color = 'darkred'\">(DANGER) Terminate OSC Controller (DANGER)</a></h4>\
</body></html>"