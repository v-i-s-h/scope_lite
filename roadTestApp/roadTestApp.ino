// Launchpad control panel

#include "Ethernet.h"

// defines for digital pins
const unsigned int D0 = PF_1;
const unsigned int D1 = PF_2;
const unsigned int D2 = PF_3;
const unsigned int D3 = PG_0;
const unsigned int D4 = PL_4;
const unsigned int D5 = PL_5;
const unsigned int D6 = PL_0;
const unsigned int D7 = PL_1;
const unsigned int D8 = PL_2;
const unsigned int D9 = PL_3;

// Prototypes
void printConfig( void );
void printEthernetData( void );
void page_home( void );
void getDeviceStatus( void );
void sendAnalogData( void );


// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x00, 0x1A, 0xB6, 0x02, 0xAE, 0xA0 };

EthernetServer server( 80 );
EthernetClient client;
int statusConfig = 0;

unsigned char led1Status = HIGH;
unsigned char led2Status = HIGH;
unsigned char led3Status = HIGH;
unsigned char led4Status = HIGH;

unsigned char out0Status = LOW;
unsigned char out1Status = LOW;
unsigned char out2Status = LOW;
unsigned char out3Status = LOW;
unsigned char out4Status = LOW;
unsigned char out5Status = LOW;
unsigned char out6Status = LOW;
unsigned char out7Status = LOW;
unsigned char out8Status = LOW;
unsigned char out9Status = LOW;

int errFlag = false;
int servePage = false;
int analogDataFlag = false;

void setup() {
  // initialize LEDs 
  pinMode( D1_LED, OUTPUT );
  pinMode( D2_LED, OUTPUT );
  pinMode( D3_LED, OUTPUT );
  pinMode( D4_LED, OUTPUT );
  
  //initialize output pins
  pinMode( D0, OUTPUT );
  pinMode( D1, OUTPUT );
  pinMode( D2, OUTPUT );
  pinMode( D3, OUTPUT );
  pinMode( D4, OUTPUT );
  pinMode( D5, OUTPUT );
  pinMode( D6, OUTPUT );
  pinMode( D7, OUTPUT );
  pinMode( D8, OUTPUT );
  pinMode( D9, OUTPUT );
  
  digitalWrite( D1_LED, led1Status );
  digitalWrite( D2_LED, led2Status );
  digitalWrite( D3_LED, led3Status );
  digitalWrite( D4_LED, led4Status );
  
  // start the serial library:
  Serial.begin( 115200 );
  Serial.print("Accquiring IP address...........");
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do blinking forever forevermore:
    led1Status = LOW;
    led2Status = HIGH;
    led3Status = LOW;
    led4Status = HIGH;
    for(;;) {
      digitalWrite( D1_LED, led1Status );
      digitalWrite( D2_LED, led2Status );
      digitalWrite( D3_LED, led3Status );
      digitalWrite( D4_LED, led4Status );
      
      led1Status = ~led1Status;
      led2Status = ~led2Status;
      led3Status = ~led3Status;
      led4Status = ~led4Status;
      delay( 1000 );
    }     
  }
  Serial.println( "[OK]" );
  printEthernetData();
    
  led1Status = LOW;
  led2Status = LOW;
  led3Status = LOW;
  led4Status = LOW;
  
  digitalWrite( D1_LED, led1Status );
  digitalWrite( D2_LED, led2Status );
  digitalWrite( D3_LED, led3Status );
  digitalWrite( D4_LED, led4Status );

  server.begin();
  
}

void loop() {
  client = server.available();
  
  if (client) {                             // if you get a client,
    Serial.print("new client on port ");           // print a message out the serial port
    Serial.println(client.port());
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected

      if (client.available()) {             // if there's bytes to read from the client,    
        char c = client.read();             // read a byte, then
                                            // This lockup is because the recv function is blocking.
        // Serial.print(c);
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {  
            break;    
          } 
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }     
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        errFlag = false;
        servePage = false;
        analogDataFlag = false;
        if (currentLine.endsWith("GET / ")) {
          statusConfig = 0;
          servePage = true;
          page_home();
        }
        else if( currentLine.endsWith( "GET /oscilloscope.html" ) ) {
          servePage = true;
          page_oscilloscope();
        }
        // Check the AJAX request
        else if (currentLine.endsWith("GET /led_0_ON.php"))   { led1Status = HIGH;    }
        else if (currentLine.endsWith("GET /led_1_ON.php"))   { led2Status = HIGH;    }
        else if (currentLine.endsWith("GET /led_2_ON.php"))   { led3Status = HIGH;    }
        else if (currentLine.endsWith("GET /led_3_ON.php"))   { led4Status = HIGH;    }
        else if (currentLine.endsWith("GET /out_0_ON.php"))   { out0Status = HIGH;    }
        else if (currentLine.endsWith("GET /out_1_ON.php"))   { out1Status = HIGH;    }
        else if (currentLine.endsWith("GET /out_2_ON.php"))   { out2Status = HIGH;    }
        else if (currentLine.endsWith("GET /out_3_ON.php"))   { out3Status = HIGH;    }
        else if (currentLine.endsWith("GET /out_4_ON.php"))   { out4Status = HIGH;    }
        else if (currentLine.endsWith("GET /out_5_ON.php"))   { out5Status = HIGH;    }
        else if (currentLine.endsWith("GET /out_6_ON.php"))   { out6Status = HIGH;    }
        else if (currentLine.endsWith("GET /out_7_ON.php"))   { out7Status = HIGH;    }
        else if (currentLine.endsWith("GET /out_8_ON.php"))   { out8Status = HIGH;    }
        else if (currentLine.endsWith("GET /out_9_ON.php"))   { out9Status = HIGH;    }        
        
        else if (currentLine.endsWith("GET /led_0_OFF.php"))  { led1Status = LOW;     }
        else if (currentLine.endsWith("GET /led_1_OFF.php"))  { led2Status = LOW;     }
        else if (currentLine.endsWith("GET /led_2_OFF.php"))  { led3Status = LOW;     }
        else if (currentLine.endsWith("GET /led_3_OFF.php"))  { led4Status = LOW;     }
        else if (currentLine.endsWith("GET /out_0_OFF.php"))  { out0Status = LOW;     }
        else if (currentLine.endsWith("GET /out_1_OFF.php"))  { out1Status = LOW;     }
        else if (currentLine.endsWith("GET /out_2_OFF.php"))  { out2Status = LOW;     }
        else if (currentLine.endsWith("GET /out_3_OFF.php"))  { out3Status = LOW;     }
        else if (currentLine.endsWith("GET /out_4_OFF.php"))  { out4Status = LOW;     }
        else if (currentLine.endsWith("GET /out_5_OFF.php"))  { out5Status = LOW;     }
        else if (currentLine.endsWith("GET /out_6_OFF.php"))  { out6Status = LOW;     }
        else if (currentLine.endsWith("GET /out_7_OFF.php"))  { out7Status = LOW;     }
        else if (currentLine.endsWith("GET /out_8_OFF.php"))  { out8Status = LOW;     }
        else if (currentLine.endsWith("GET /out_9_OFF.php"))  { out9Status = LOW;     }


        else if (currentLine.endsWith("GET /getDeviceStatus.php")) {          
          //Setting errFlag and servePage to false will automatically trigger an AJAX response
          errFlag = false;
          servePage = false;
          analogDataFlag = false;
        }
        else if (currentLine.endsWith("GET /getAnalogData.php")) {          
          errFlag = false;
          servePage = false;
          analogDataFlag= true;
        }
        else {
          errFlag = true;
        }
        if( !errFlag && !servePage && !analogDataFlag ) {    // this will serve for getDeviceStatus
          //Print current request
          Serial.print( currentLine );
          Serial.print( "\t\tSending AJAX response..." );
          getDeviceStatus();
          Serial.println( "[OK]" ); 
        }
        else if( !errFlag && !servePage && analogDataFlag ) {    /// this will serve for getAnalogData
          Serial.print( "Sending analog data..." );
          sendAnalogData();
          Serial.println( "[OK]" );
        }
      }
    }
    // close the connection:
    client.stop();
    //Serial.println("client disonnected");
  }
}

void page_oscilloscope( void ) {
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:    
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
 
  client.println("<html><head><title>TI Connected Launchpad Roadtest -- developed by vish</title><script type=\"text/javascript\" src=\"http://dygraphs.com/dygraph-combined.js\"></script><script>var boardStatus=[false,false,false,false,false,false,false,false,false,false,false,false,false,false];var xmlhttp;var index=0;xmlhttp=new XMLHttpRequest();xmlhttp.onreadystatechange=function(){if((xmlhttp.readyState==4)&&(xmlhttp.status==200)){" );
  client.println("buffer=xmlhttp.responseText.split(\",\");for(index=0;index<buffer.length;++index){boardStatus[index]=(buffer[index]==\"1\")?true:false;}");
  client.println("if(boardStatus[0]==true){document.getElementById(\"led_0\").className=\"ledStatusOn\";}else{document.getElementById(\"led_0\").className=\"ledStatusOff\";}");
  client.println("if(boardStatus[1]==true){document.getElementById(\"led_1\").className=\"ledStatusOn\";}else{document.getElementById(\"led_1\").className=\"ledStatusOff\";}");
  client.println("if(boardStatus[2]==true){document.getElementById(\"led_2\").className=\"ledStatusOn\";}else{document.getElementById(\"led_2\").className=\"ledStatusOff\";}");
  client.println("if(boardStatus[3]==true){document.getElementById(\"led_3\").className=\"ledStatusOn\";}else{document.getElementById(\"led_3\").className=\"ledStatusOff\";}");
  client.println("if(boardStatus[4]==true){document.getElementById(\"out_0\").className=\"outButtonActive\";}else{document.getElementById(\"out_0\").className=\"outButtonInactive\";}");
  client.println("if(boardStatus[5]==true){document.getElementById(\"out_1\").className=\"outButtonActive\";}else{document.getElementById(\"out_1\").className=\"outButtonInactive\";}");
  client.println("if(boardStatus[6]==true){document.getElementById(\"out_2\").className=\"outButtonActive\";}else{document.getElementById(\"out_2\").className=\"outButtonInactive\";}");
  client.println("if(boardStatus[7]==true){document.getElementById(\"out_3\").className=\"outButtonActive\";}else{document.getElementById(\"out_3\").className=\"outButtonInactive\";}");
  client.println("if(boardStatus[8]==true){document.getElementById(\"out_4\").className=\"outButtonActive\";}else{document.getElementById(\"out_4\").className=\"outButtonInactive\";}");
  client.println("if(boardStatus[9]==true){document.getElementById(\"out_5\").className=\"outButtonActive\";}else{document.getElementById(\"out_5\").className=\"outButtonInactive\";}");
  client.println("if(boardStatus[10]==true){document.getElementById(\"out_6\").className=\"outButtonActive\";}else{document.getElementById(\"out_6\").className=\"outButtonInactive\";}");
  client.println("if(boardStatus[11]==true){document.getElementById(\"out_7\").className=\"outButtonActive\";}else{document.getElementById(\"out_7\").className=\"outButtonInactive\";}");
  client.println("if(boardStatus[12]==true){document.getElementById(\"out_8\").className=\"outButtonActive\";}else{document.getElementById(\"out_8\").className=\"outButtonInactive\";}");
  client.println("if(boardStatus[13]==true){document.getElementById(\"out_9\").className=\"outButtonActive\";}else{document.getElementById(\"out_9\").className=\"outButtonInactive\";}");
  client.println("}");
  client.println("}");
  delay( 100 );    // wait for sending data from ethernet buffer
  client.println("function getDeviceStatus(){xmlhttp.open(\"GET\",\"getDeviceStatus.php\",true);xmlhttp.send();}function triggerInput(outButton){if(boardStatus[outButton.name]==false){boardStatus[outButton.name]=true;xmlhttp.open(\"GET\",outButton.id+\"_ON.php\",true);xmlhttp.send();}else{boardStatus[outButton.name]=false;xmlhttp.open(\"GET\",outButton.id+\"_OFF.php\",true);xmlhttp.send();}}</script>");
  client.println("<script>var dataTrace=[[0,0,0,0,0,0,]];var dataDisp=[[,,,,,,]];var dataIndex=0.00;var buffer=\"\";var scope;var channelVisibility=[false,false,false,false,false];");
  client.println("function initGraph(hCanvas){scope=new Dygraph(hCanvas,dataDisp,{visibility:channelVisibility,valueRange:[-0.4,4.1],drawAxesAtZero:true,labels:[\"time\",\"channel0\",\"channel1\",\"channel2\",\"channel3\",\"channel4\"]})}");
  client.println("function plotGraph(el){if(channelVisibility[el.id]==false){channelVisibility[el.id]=true;el.className=\"channelButtonActive\";}else{channelVisibility[el.id]=false;el.className=\"channelButtonInactive\";}scope.setVisibility(el.id,channelVisibility[el.id]);}");
  client.println("function getDataFromDevice(){var sampleTime=dataIndex;var analogVals=[0,0,0,0,0];var channelIdx=0;var xmlhttp;xmlhttp=new XMLHttpRequest();xmlhttp.onreadystatechange=function(){");
  client.println("if((xmlhttp.readyState==4)&&(xmlhttp.status==200)){");
  client.println("buffer=xmlhttp.responseText.split(\",\");");
  client.println("for(channelIdx=0;channelIdx<buffer.length;++channelIdx){analogVals[channelIdx]=buffer[channelIdx];}");
  client.println("var sampleCh0Val=analogVals[0]*3.3/4095;var sampleCh1Val=analogVals[1]*3.3/4095;var sampleCh2Val=analogVals[2]*3.3/4095;var sampleCh3Val=analogVals[3]*3.3/4095;var sampleCh4Val = analogVals[4] * 3.3 /4095;dataTrace.push([sampleTime,sampleCh0Val,sampleCh1Val,sampleCh2Val,sampleCh3Val,sampleCh4Val]);");
  client.println("var siz=dataDisp.push([sampleTime,sampleCh0Val,sampleCh1Val,sampleCh2Val,sampleCh3Val,sampleCh4Val]);");
  client.println("if(siz>50){dataDisp.shift();}dataIndex += 0.1;}};xmlhttp.open(\"GET\",\"getAnalogData.php\",true );xmlhttp.send();}");
  client.println("function updatePlot(){getDataFromDevice();scope.updateOptions({file:dataDisp});}var measure=0;var hAuto;function startStop(buttonId){if(measure==0){measure=1;dataTrace=[];dataDisp=[];scope.updateOptions({showRangeSelector:false});scope.resetZoom();hAuto=self.setInterval(function(){updatePlot()},100);buttonId.className=\"startStopButtonOn\";buttonId.value=\"STOP\";}");
  delay( 100 );    // wait for sending data from ethernet buffer
  client.println("else{measure=0;hAuto=self.clearInterval(hAuto);scope.updateOptions({showRangeSelector:true});scope.updateOptions({file:dataTrace});buttonId.className=\"startStopButtonOff\";buttonId.value=\"START\";dataIndex=0.00;}}</script>");
  client.println("<style>");
  client.println("input.ledStatusOff{border-radius:10px 10px 10px 10px;height:40px;width:100px;text-align:center;background-image: -moz-linear-gradient(center top,#FFFFFF,#C0C0C0);background-color:#F0F0F0;border:1px solid rgba(0,0,0,0.1);}");
  client.println("input.ledStatusOn{border-radius:10px 10px 10px 10px;height:40px;width:100px;text-align:center;background-image: -moz-linear-gradient(center top,#F0F0F0,#4FFF4F);background-color:#7F007F;border:1px solid rgba(0,0,0,0.1);}");
  client.println("input.outButtonActive{height:45px;width:48px;text-align:center;background-image:-moz-linear-gradient(center top,#EFEFEF,#FFFFFF);background-color:#F0F0F0;border:1px solid rgba(0,0,0,0.1);}");
  client.println("input.outButtonInactive{height:45px;width:48px;text-align:center;background-image:-moz-linear-gradient(center top,#FFFFFF,#C0C0C0);background-color:#7F007F;border:1px solid rgba(0,0,0,0.1);}");
  client.println("input.channelButtonActive{height:32px;width:96px;margin:0px;text-align:center;background-image:-moz-linear-gradient(center top,#EFEFEF,#FFFFFF);border:1px solid rgba(0,0,0,0.1);}");
  client.println("input.channelButtonInactive{height:32px;width:96px;margin:0px;text-align:center;background-image: -moz-linear-gradient(center top,#FFFFFF,#C0C0C0);border:2px solid rgba(0,0,0,0.1);}");
  client.println("input.startStopButtonOn{border-radius:20px 20px 20px 20px;height:96px;width:96px;text-align:center;background-image:-moz-linear-gradient(center top,#EFEFEF,#FFFFFF);border:3px solid rgba(0,0,0,0.1);}");
  client.println("input.startStopButtonOff{border-radius:15px 15px 15px 15px;height:96px;width:96px;text-align:center;background-image:-moz-linear-gradient(center top,#FFFFFF,#C0C0C0);border:1px solid rgba(0,0,0,0.1);}");
  client.println("</style>");
  delay( 100 );    // wait for sending data from ethernet buffer
  client.println("</head><body style='background: #D0D0D0' onload=\"getDeviceStatus()\"><table style=\"position:relative;margin-left:auto;margin-right:auto;padding-left:10px;padding-right:10px;padding-top:10px;padding-bottom:10px;\"><tr><td style=\"vertical-align:top;\"><div id='ledDivision' style=\"width:150px;height:563px;background:#EFEFEF;border-radius: 20px 0px 0px 20px;\"><table id='ledControls' style=\"margin-left:auto;margin-right:auto;padding-left:10px;padding-right:10px;padding-top:10px;padding-bottom:10px;\">");
  client.println("<tr><td colspan=2><input type=\"button\" class=\"ledStatusOff\" id=\"led_0\" name=\"0\" value=\"LED1\" onClick=\"triggerInput(this)\"></input></td></tr>");
  client.println("<tr><td colspan=2><input type=\"button\" class=\"ledStatusOff\" id=\"led_1\" name=\"1\" value=\"LED2\" onClick=\"triggerInput(this)\"></input></td></tr>");
  client.println("<tr><td colspan=2><input type=\"button\" class=\"ledStatusOff\" id=\"led_2\" name=\"2\" value=\"LED3\" onClick=\"triggerInput(this)\"></input></td></tr>");
  client.println("<tr><td colspan=2><input type=\"button\" class=\"ledStatusOff\" id=\"led_3\" name=\"3\" value=\"LED4\" onClick=\"triggerInput(this)\"></input></td></tr>");
  client.println("<tr><td><input type=\"button\" class=\"outButtonInactive\" style=\"border-radius:10px 0px 0px 0px\" id=\"out_0\" name=\"4\" value=\"D0\" onClick=\"triggerInput(this)\"></input></td><td><input type=\"button\" class=\"outButtonInactive\" style=\"border-radius:0px 10px 0px 0px\" id=\"out_1\" name=\"5\" value=\"D1\" onClick=\"triggerInput(this)\"></input></td></tr>");
  client.println("<tr><td><input type=\"button\" class=\"outButtonInactive\" style=\"border-radius:0px 0px 0px 0px\" id=\"out_2\" name=\"6\" value=\"D2\" onClick=\"triggerInput(this)\"></input></td><td><input type=\"button\" class=\"outButtonInactive\" style=\"border-radius:0px 0px 0px 0px\" id=\"out_3\" name=\"7\" value=\"D3\" onClick=\"triggerInput(this)\"></input></td></tr>");
  client.println("<tr><td><input type=\"button\" class=\"outButtonInactive\" style=\"border-radius:0px 0px 0px 0px\" id=\"out_4\" name=\"8\" value=\"D4\" onClick=\"triggerInput(this)\"></input></td><td><input type=\"button\" class=\"outButtonInactive\" style=\"border-radius:0px 0px 0px 0px\" id=\"out_5\" name=\"9\" value=\"D5\" onClick=\"triggerInput(this)\"></input></td></tr>");
  client.println("<tr><td><input type=\"button\" class=\"outButtonInactive\" style=\"border-radius:0px 0px 0px 0px\" id=\"out_6\" name=\"10\" value=\"D6\" onClick=\"triggerInput(this)\"></input></td><td><input type=\"button\" class=\"outButtonInactive\" style=\"border-radius:0px 0px 0px 0px\" id=\"out_7\" name=\"11\" value=\"D7\" onClick=\"triggerInput( this )\"></input></td></tr>");
  client.println("<tr><td><input type=\"button\" class=\"outButtonInactive\" style=\"border-radius:0px 0px 0px 10px\" id=\"out_8\" name=\"12\" value=\"D8\" onClick=\"triggerInput( this )\"></input></td><td><input type=\"button\" class=\"outButtonInactive\" style=\"border-radius:0px 0px 10px 0px\" id=\"out_9\" name=\"13\" value=\"D9\" onClick=\"triggerInput( this )\"></input></td></tr></table>");
  delay( 100 );    // wait for sending data from ethernet buffer
  client.println("<table style=\"margin-left:auto;margin-right:auto;padding-left:10px;padding-right:10px;padding-top:10px;padding-bottom:10px;\"><tr><td><input type=\"button\" name=\"button_SS\" id=\"button_StartStop\" value=\"START\" onclick=\"startStop(this)\" class=\"startStopButtonOff\"></td></tr></table></div></td><td>");
  client.println("<table id=\"mainFrame\" style=\"position:relative;margin-left:auto;margin-right:auto;padding-left:10px;padding-right:10px;padding-top:10px;padding-bottom:10px;background:#EFEFEF;border-radius: 0px 20px 20px 0px;\"><tr><td>");
  client.println("<table id='canvasTable' class='dot_selection'><tr><td colspan=6><div id=\"oscilloscope\" style=\"width:850px;height:475px;margin-right:50px;margin-top:10px;margin-bottom:10px;\"><script type=\"text/javascript\">initGraph(document.getElementById(\"oscilloscope\"));</script></div></td><td style='vertical-align:top;'></td></tr><tr>");
  client.println("<td align='center' ><input type='button' class='channelButtonInactive' name='chnl00' id='0' onclick='plotGraph(this)' value=\"0\" style=\"border-radius:10px 0px 0px 10px;\"></input>");
  client.println("<input type='button' class='channelButtonInactive' name='chnl01' id='1' onclick='plotGraph(this)' value=\"1\" ></input><input type='button' class='channelButtonInactive' name='chnl02' id='2' onclick='plotGraph(this)' value=\"2\" ></input><input type='button' class='channelButtonInactive' name='chnl03' id='3' onclick='plotGraph(this)' value=\"3\" ></input><input type='button' class='channelButtonInactive' name='chnl04' id='4' onclick='plotGraph(this)' value=\"4\"  style=\"border-radius: 0px 10px 10px 0px;\"></input>");
  client.println("</td></tr></table></td></tr></table></td></tr></table></body></html>");
  
  client.println( "" );
}

void page_home( void ) {
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:    
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  
  client.println( "<html> <head><title>TI Connected Launchpad Home</title>" );
  client.println( "<script type = \"text/javascript\" src = \"http://dygraphs.com/dygraph-combined.js\"></script>" );
  client.println( "<script> var ledStatus = [ false, false, false, false ]; var xmlhttp; xmlhttp = new XMLHttpRequest(); xmlhttp.onreadystatechange = function() { if( (xmlhttp.readyState == 4) && (xmlhttp.status == 200) ) { buffer = xmlhttp.responseText.split(\",\"); ledStatus[0] = (buffer[0]==\"1\")?true:false; ledStatus[1] = (buffer[1]==\"1\")?true:false; ledStatus[2] = (buffer[2]==\"1\")?true:false; ledStatus[3] = (buffer[3]==\"1\")?true:false; if( ledStatus[0] == true ) { document.getElementById( \"led_0\" ).className = \"ledStatusOn\"; } else { document.getElementById( \"led_0\" ).className = \"ledStatusOff\"; } if( ledStatus[1] == true ) { document.getElementById( \"led_1\" ).className = \"ledStatusOn\"; } else { document.getElementById( \"led_1\" ).className = \"ledStatusOff\"; } if( ledStatus[2] == true ) { document.getElementById( \"led_2\" ).className = \"ledStatusOn\"; } else { document.getElementById( \"led_2\" ).className = \"ledStatusOff\"; } if( ledStatus[3] == true ) { document.getElementById( \"led_3\" ).className = \"ledStatusOn\"; } else { document.getElementById( \"led_3\" ).className = \"ledStatusOff\"; } } }" );  
  client.println( "function getDeviceStatus() { xmlhttp.open( \"GET\", \"getDeviceStatus.php\", true );xmlhttp.send();}function triggerInput( ledButton ) { if( ledStatus[ledButton.name] == false ) { ledStatus[ledButton.name] = true; xmlhttp.open( \"GET\", ledButton.id+\"_ON.php\", true ); xmlhttp.send(); } else { ledStatus[ledButton.name] = false; xmlhttp.open( \"GET\", ledButton.id+\"_OFF.php\", true ); xmlhttp.send(); } }" );
  client.println( " </script>" );  
  client.println( "<style> input.ledStatusOff{ border-radius:10px 10px 10px 10px;height:40px;width:120px;text-align:center;background-color:#F0F0F0;border:1px solid rgba(0, 0, 0, 0.1); } input.ledStatusOn{ border-radius:10px 10px 10px 10px;height:40px;width:120px;text-align:center;background-color:#7FFF7F;border:1px solid rgba(0, 0, 0, 0.1);}</style></head>" );
  client.println( "<body style='background: #D0D0D0;' onload=\"getDeviceStatus()\"> <table id=\"mainFrame\" style=\"position:relative;margin-left:auto;margin-right:auto;padding-left:10px;padding-right:10px;padding-top:10px;background:#FFFFFF;border-radius: 20px 20px 20px 20px;\" > <tr><td align='center'><h1 align=center><font color=\"red\">TI Connected Launchpad Home</font></h1></td></tr> <tr><td align='center'><input type=\"button\" class=\"ledStatusOff\" id=\"led_0\" name=\"0\" value=\"LED1\" onClick=\"triggerInput( this )\"></input></td></tr> <tr><td align='center'><input type=\"button\" class=\"ledStatusOff\" id=\"led_1\" name=\"1\" value=\"LED2\" onClick=\"triggerInput( this )\"></input></td></tr> <tr><td align='center'><input type=\"button\" class=\"ledStatusOff\" id=\"led_2\" name=\"2\" value=\"LED3\" onClick=\"triggerInput( this )\"></input></td></tr> <tr><td align='center'><input type=\"button\" class=\"ledStatusOff\" id=\"led_3\" name=\"3\" value=\"LED4\" onClick=\"triggerInput( this )\"></input></td></tr> <tr><td align='center'><a href=\"oscilloscope.html\"<h1 align=center><font color=\"0x0F0F0F\">&#9654 Oscilloscope</font></h1></a></td></tr></table> </body></html>" );
  
  client.println( "" );  
}

void getDeviceStatus( void ) {
  digitalWrite( D1_LED, led1Status );
  digitalWrite( D2_LED, led2Status );
  digitalWrite( D3_LED, led3Status );
  digitalWrite( D4_LED, led4Status );
  
  digitalWrite( D0, out0Status );
  digitalWrite( D1, out1Status );
  digitalWrite( D2, out2Status );
  digitalWrite( D3, out3Status );
  digitalWrite( D4, out4Status );
  digitalWrite( D5, out5Status );
  digitalWrite( D6, out6Status );
  digitalWrite( D7, out7Status );
  digitalWrite( D8, out8Status );
  digitalWrite( D9, out9Status );
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  
  if( led1Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( led2Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( led3Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( led4Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  
  if( out0Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( out1Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( out2Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( out3Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( out4Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( out5Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( out6Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( out7Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( out8Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } client.print( "," );
  if( out9Status == HIGH ) { client.print( "1" ); } else { client.print( "0" ); } 

  client.println( "" );
}

void sendAnalogData( void ) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  
  client.print( analogRead(  A0 ) );  client.print( "," );
  client.print( analogRead(  A1 ) );  client.print( "," );
  client.print( analogRead(  A2 ) );  client.print( "," );
  client.print( analogRead(  A3 ) );  client.print( "," );
  client.print( analogRead(  A4 ) );
  
  client.println( "" );
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    switch(inChar) {
      case 'h':
        printHelp();
        break;
      case 'i':
        printEthernetData();
        break;
      default:
        Serial.println();
        Serial.println("Invalid menu option");
    }
  } 
}

void printHelp( void ) {
  Serial.println();
  Serial.println("+++++++++++++++++++++++++++++++++++++");
  Serial.println("Help menu:");
  Serial.println("\th: This help menu");
  Serial.println("\ti: IP address information");
  Serial.println("+++++++++++++++++++++++++++++++++++++");
}

void printEthernetData( void ) {
  // print your IP address:
  Serial.println();
  Serial.println("IP Address Information:");  
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address:\t");
  Serial.println(ip);
  
  // print your MAC address:  
  IPAddress subnet = Ethernet.subnetMask();
  Serial.print("NetMask:\t");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = Ethernet.gatewayIP();
  Serial.print("Gateway:\t");
  Serial.println(gateway);

  // print your gateway address:
  IPAddress dns = Ethernet.dnsServerIP();
  Serial.print("DNS:\t\t");
  Serial.println(dns);

}
