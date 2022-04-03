t <html><head><title>RTC </title>
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
# Define URL and refresh timeout
t var formUpdate = new periodicObj("RTC.cgx", 500);
t function plotRTCGraph() {
t  RTCVal = document.getElementById("RTC_value").value;
t }
t function periodicUpdateRTC() {
t  if(document.getElementById("RTCChkBox").checked == true) {
t   updateMultiple(formUpdate,plotRTCGraph);
t   RTC_elTime = setTimeout(periodicUpdateRTC, formUpdate.period);
t  }
t  else
t   clearTimeout(RTC_elTime);
t }
t </script></head>
i pg_header.inc
t <h2 align="center"><br>RTC</h2>
t <p><font size="2">this page shows you the internal  RTC time which is syncronize with a SNTP server </font></p>
t <form action="RTC.cgi" method="post" name="RTC">
t <input type="hidden" value="RTC" name="pg">
t <table border=0 width=99%><font size="3">
t <tr style="background-color: #aaccff">
t <th width=70%>RTC Time</th></tr>
t <td align="center">
t <input type="text" readonly style="background-color: transparent; border: 0px"
c j size="10" id="RTC_value" value="%s"></td>
t <tr><td><img src=pabb.gif>Choose IP adress for SNTP server'</td>
t <td><select name="ctrl" onchange="submit();">
c m c <option %s>150,214,94,10</option><option %s>150,214,94,5</option></select></td></tr>
t </font></table>
t <p align=center>
t <input type=button value="Refresh" onclick="updateMultiple(formUpdate,plotRTCGraph)">
t  Periodic:<input type="checkbox" id="RTCChkBox" onclick="periodicUpdateRTC()">
t </p></form>
i pg_footer.inc
. End of script must be closed with period