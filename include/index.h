const char PAGE_MAIN[] PROGMEM = R"##(

<!DOCTYPE html>

<html lang="en">
<head>
  <style>
    body    {background-color: #2D2D52; text-align: center; }
    h1      {color: #ffffff;}
    p       {color: #ffffff;}
    table   {
      border-collapse: collapse;
      margin-left: auto;
      margin-right: auto;
      color: #ffffff;
    }
    td, th {
      border: 1px solid #ffffff;
      padding: 0.5rem;
      text-align: left;
    }
  </style>
  <meta charset="UTF-8">
  <title>Esp 32</title>
</head>

<body onload="OLProcess()">
  <h1>Esp32</h1>
  <table>
    <caption id="caption">Aktuelle Sensordaten</caption>
    <tr>
      <th>Temperatur</th>
      <th id="tempElement"></th>
    </tr>
    <tr>
      <th>Luftfeuchtigkeit</th>
      <th id="humiElement"></th>
    </tr>
    <tr>
      <th>co2</th>
      <th id="co2Element"></th>
    </tr>
  </table>
  <footer>
    (c) 2048 Great Code inc
  </footer>
</body>
<script type = "text/javascript">

  var xmlHttp=createXmlHttpObject();

    function createXmlHttpObject(){
      if(window.XMLHttpRequest){
        xmlHttp=new XMLHttpRequest();
      }
      else{
        xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
      }
      return xmlHttp;
    }

  function OLProcess(){
    if(xmlHttp.readyState==0) {
      xmlHttp.open("PUT","xml",true);
      xmlHttp.send("XML");
      XMLProcess();
    }
    setTimeout(OLProcess, 30000);
  }
  function XMLProcess(){
    if(xmlHttp.readyState==4){
      var XML_web;
      var temp_web;
      var humi_web;
      var sensio_web;
      var XML_Text_web;
      var temp_text_web;
      var co2_web;

      XML_web = xmlHttp.responseXML;
      XML_Text_web = xmlHttp.responseText;
      temp_web = XML_web.getElementsByTagName("temp");

      temp_web = XML_web.getElementsByTagName("temp")[0].innerHTML + "°";
      humi_web = XML_web.getElementsByTagName("humi")[0].innerHTML + "%";
      sensio_web = XML_web.getElementsByTagName("sensio")[0].innerHTML;
      co2_web = XML_web.getElementsByTagName("co2")[0].innerHTML + " ppm";

      if (sensio_web == 1) {
        document.getElementById("humiElement").innerHTML=humi_web;
        document.getElementById("tempElement").innerHTML=temp_web;
        document.getElementById("co2Element").innerHTML = co2_web;
        return;
      }
      else if (sensio_web == 0) {
        document.getElementById("humiElement").innerHTML="sensor broken";
        document.getElementById("tempElement").innerHTML="kaboom";
        return;
      }

    }
    else{
      setTimeout(XMLProcess, 100);
    }
  }
  </script>
</html>

)##";